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

#include <utility>
#include <cstring>
#include <algorithm>
#include <transport/SenderResource.h>
#include <transport/ReceiverResource.h>
#include <transport/TransportInterface.h>
#include "UDPv4Transport.h"
#include "IPLocator.h"

namespace transport
{

static void get_ipv4s(
    std::vector<IPFinder::info_IP> &locNames,
    bool return_loopback = false)
{
    IPFinder::getIPs(&locNames, return_loopback);
    auto new_end = remove_if(locNames.begin(),
                                locNames.end(),
                                [](IPFinder::info_IP ip)
                                {
                                    return ip.type != IPFinder::IP4 && ip.type != IPFinder::IP4_LOCAL;
                                });
    locNames.erase(new_end, locNames.end());
    std::for_each(locNames.begin(), locNames.end(), [](IPFinder::info_IP &loc)
                    { loc.locator.kind = LOCATOR_KIND_UDPv4; });
}

UDPv4Transport::UDPv4Transport(
    const UDPv4TransportDescriptor &descriptor)
    : UDPTransportInterface(LOCATOR_KIND_UDPv4)
    , descriptor_(descriptor)
{
    mSendBufferSize = descriptor.send_buffer_size_;
    mReceiveBufferSize = descriptor.recv_buffer_size_;
}

UDPv4Transport::UDPv4Transport()
    : UDPTransportInterface(LOCATOR_KIND_UDPv4)
{
}

UDPv4Transport::~UDPv4Transport()
{
    clean();
}

UDPv4TransportDescriptor::UDPv4TransportDescriptor()
    : UDPTransportDescriptor()
{
}

TransportInterface *UDPv4TransportDescriptor::create_transport() const
{
    return new UDPv4Transport(*this);
}

bool UDPv4TransportDescriptor::operator==(
    const UDPv4TransportDescriptor &t) const
{
    return (UDPTransportDescriptor::operator==(t));
}

bool UDPv4Transport::getDefaultMetatrafficMulticastLocators(
    LocatorList &locators,
    uint32_t metatraffic_multicast_port) const
{
    Locator locator;
    locator.kind = LOCATOR_KIND_UDPv4;
    locator.port = static_cast<uint16_t>(metatraffic_multicast_port);
    IPLocator::setIPv4(locator, DEFAULT_METATRAFFIC_MULTICAST_ADDRESS);
    locators.push_back(locator);
    return true;
}

bool UDPv4Transport::getDefaultMetatrafficUnicastLocators(
    LocatorList &locators,
    uint32_t metatraffic_unicast_port) const
{
    Locator locator;
    locator.kind = LOCATOR_KIND_UDPv4;
    locator.port = static_cast<uint16_t>(metatraffic_unicast_port);
    locator.set_invalid_address();
    locators.push_back(locator);

    return true;
}

// bool UDPv4Transport::getDefaultUnicastLocators(
//     LocatorList &locators,
//     uint32_t unicast_port) const
// {
//     Locator locator;
//     locator.kind = LOCATOR_KIND_UDPv4;
//     locator.set_invalid_address();
//     fillUnicastLocator(locator, unicast_port);
//     locators.push_back(locator);

//     return true;
// }

void UDPv4Transport::AddDefaultOutputLocator(
    LocatorList &defaultList)
{
    Locator locator;
    IPLocator::createLocator(LOCATOR_KIND_UDPv4, DEFAULT_METATRAFFIC_MULTICAST_ADDRESS,
                                descriptor_.m_output_udp_socket, locator);
    defaultList.push_back(locator);
}

bool UDPv4Transport::compare_locator_ip(
    const Locator &lh,
    const Locator &rh) const
{
    return IPLocator::compareAddress(lh, rh);
}

bool UDPv4Transport::compare_locator_ip_and_port(
    const Locator &lh,
    const Locator &rh) const
{
    return IPLocator::compareAddressAndPhysicalPort(lh, rh);
}

const UDPTransportDescriptor *UDPv4Transport::configuration() const
{
    return &descriptor_;
}

void UDPv4Transport::get_ips(
    std::vector<IPFinder::info_IP> &locNames,
    bool return_loopback)
{
    get_ipv4s(locNames, return_loopback);
}

const std::string &UDPv4Transport::localhost_name()
{
    static const std::string ip4_localhost = "127.0.0.1";
    return ip4_localhost;
}

bool UDPv4Transport::OpenInputChannel(
    ReceiverResourceList &receiver_resource_list,
    const Locator &locator,
    uint32_t maxMsgSize)
{

    if (IsLocatorSupported(locator))
    {

        // UDPReceiverResource *p_channel_resource;
        if (IPLocator::isMulticast(locator))
        {
            auto recv_socket = uvw::loop::get_default()->resource<uvw::udp_handle>();
            auto recv_resource = std::make_shared<UDPReceiverResource>(this, recv_socket, maxMsgSize, locator);
            if (recv_socket->bind("0.0.0.0", locator.port, uvw::details::uvw_udp_flags::REUSEADDR) < 0 ||
                    !recv_socket->multicast_membership(IPLocator::getIpByLocatorv4(locator), "0.0.0.0",uvw::udp_handle::membership::JOIN_GROUP))
            {
                return false;
            }

            recv_socket->recv();
            receiver_resource_list.push_back(recv_resource);
        }
        else
        {
            auto recv_socket = uvw::loop::get_default()->resource<uvw::udp_handle>();
            auto recv_resource = std::make_shared<UDPReceiverResource>(this, recv_socket, maxMsgSize, locator);
            if (recv_socket->bind(IPLocator::toIPv4string(locator), locator.port) < 0)
            {
                return false;
            }
            recv_socket->recv();
            receiver_resource_list.push_back(recv_resource);
        }
    }

    return true;
}

LocatorList UDPv4Transport::NormalizeLocator(
    const Locator &locator)
{
    LocatorList list;

    if (IPLocator::isAny(locator))
    {
        std::vector<IPFinder::info_IP> locNames;
        get_ipv4s(locNames);
        for (const auto &infoIP : locNames)
        {
            Locator newloc(locator);
            IPLocator::setIPv4(newloc, infoIP.locator);
            list.push_back(newloc);
        }
        if (list.empty())
        {
            Locator newloc(locator);
            IPLocator::setIPv4(newloc, "127.0.0.1");
            list.push_back(newloc);
        }
    }
    else
    {
        list.push_back(locator);
    }

    return list;
}

void UDPv4Transport::set_receive_buffer_size(
    uint32_t size)
{
    descriptor_.recv_buffer_size_ = size;
}

void UDPv4Transport::set_send_buffer_size(
    uint32_t size)
{
    descriptor_.send_buffer_size_ = size;
}

void UDPv4Transport::update_network_interfaces()
{
    UDPTransportInterface::update_network_interfaces();
}

} // namespace transport
