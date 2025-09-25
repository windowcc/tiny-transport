// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "UDPTransportInterface.h"
#include <utility>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "UDPSenderResource.hpp"

namespace transport
{

UDPTransportInterface::UDPTransportInterface(
    int32_t transport_kind,
    std::shared_ptr<uvw::loop> loop)
    : TransportInterface()
    , transport_kind_(transport_kind)
    , loop_(loop)
    , mSendBufferSize(0)
    , mReceiveBufferSize(0)
{
}

UDPTransportInterface::~UDPTransportInterface()
{
}

bool UDPTransportInterface::do_input_locators_match(
    const Locator &left,
    const Locator &right) const
{
    return IPLocator::getPhysicalPort(left) == IPLocator::getPhysicalPort(right);
}

bool UDPTransportInterface::init()
{

    get_ips(currentInterfaces);
    return true;
}

bool UDPTransportInterface::is_locator_supported(
    const Locator &locator) const
{
    return locator.kind == transport_kind_;
}

bool UDPTransportInterface::open_output_channel(
    SendResourceList &sender_resource_list,
    const Locator &locator)
{
    if (!is_locator_supported(locator))
    {
        return false;
    }

    auto send_socket = loop_->resource<uvw::udp_handle>();
    if(send_socket)
    {
        send_socket->bind(IPLocator::toIPv4string(locator), locator.port);

        sender_resource_list.emplace_back(
            static_cast<SenderResource *>(new UDPSenderResource(*this, send_socket, false, true))
        );
    }

    return false;
}

Locator UDPTransportInterface::remote_to_main_local(
    const Locator &remote) const
{
    if (!is_locator_supported(remote))
    {
        return false;
    }

    Locator mainLocal(remote);
    // memset(mainLocal.address, 0x00, sizeof(mainLocal.address));
    mainLocal.set_invalid_address();
    return mainLocal;
}

bool UDPTransportInterface::send(
    const octet *send_buffer,
    uint32_t send_buffer_size,
    // UDPSocket &socket,
    std::shared_ptr<uvw::udp_handle> socket,
    const LocatorList &locators,
    bool only_multicast_purpose,
    bool whitelisted,
    const std::chrono::steady_clock::time_point &max_blocking_time_point)
{
    bool ret = true;
    auto time_out = std::chrono::duration_cast<std::chrono::microseconds>(
        max_blocking_time_point - std::chrono::steady_clock::now());

    for (auto &locator : locators)
    {
        if (is_locator_supported(locator))
        {
            ret &= send(send_buffer,
                        send_buffer_size,
                        socket,
                        locator,
                        only_multicast_purpose,
                        whitelisted,
                        time_out);
        }
    }

    return ret;
}

bool UDPTransportInterface::send(
    const octet *send_buffer,
    uint32_t send_buffer_size,
    std::shared_ptr<uvw::udp_handle> socket,
    const Locator &remote_locator,
    bool only_multicast_purpose,
    bool whitelisted,
    const std::chrono::microseconds &timeout)
{
    (void)timeout;

    bool success = true;
    bool is_multicast_remote_address = IPLocator::isMulticast(remote_locator);
    if (is_multicast_remote_address == only_multicast_purpose || whitelisted)
    {
        socket->send(IPLocator::getIpByLocatorv4(remote_locator),
                       remote_locator.port,
                       const_cast<char*>(reinterpret_cast<const char*>(send_buffer)),
                       send_buffer_size);
    }

    return success;
}

bool UDPTransportInterface::fill_metatraffic_multicast_locator(
    Locator &locator,
    uint32_t metatraffic_multicast_port) const
{
    if (locator.port == 0)
    {
        locator.port = metatraffic_multicast_port;
    }
    return true;
}

bool UDPTransportInterface::fill_metatraffic_unicast_locator(
    Locator &locator,
    uint32_t metatraffic_unicast_port) const
{
    if (locator.port == 0)
    {
        locator.port = metatraffic_unicast_port;
    }
    return true;
}


bool UDPTransportInterface::fill_unicast_locator(
    Locator &locator,
    uint32_t well_known_port) const
{
    if (locator.port == 0)
    {
        locator.port = well_known_port;
    }
    return true;
}

void UDPTransportInterface::update_network_interfaces()
{
    rescan_interfaces_.store(true);
}

} // namespace transport
