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
#include "PortParameters.hpp"

namespace transport
{

UDPTransportInterface::UDPTransportInterface(
    int32_t transport_kind)
    : TransportInterface(transport_kind)
    , mSendBufferSize(0)
    , mReceiveBufferSize(0)
{
}

UDPTransportInterface::~UDPTransportInterface()
{
}

void UDPTransportInterface::clean()
{
}

bool UDPTransportInterface::DoInputLocatorsMatch(
    const Locator &left,
    const Locator &right) const
{
    return IPLocator::getPhysicalPort(left) == IPLocator::getPhysicalPort(right);
}

bool UDPTransportInterface::init()
{
    int sockfd;
    int send_buf_size, recv_buf_size;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // 创建 UDP 套接字

    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    // if (descriptor_.min_send_buffer_size() == 0)
    // {
    //     socklen_t optlen = sizeof(send_buf_size);
    //     getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &send_buf_size, &optlen);
    //     set_send_buffer_size(send_buf_size);
    //     if (descriptor_.min_send_buffer_size() < s_minimumSocketBuffer)
    //     {
    //         set_send_buffer_size(s_minimumSocketBuffer);
    //         send_buf_size = s_minimumSocketBuffer;
    //     }

    //     setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &send_buf_size, sizeof(send_buf_size));
    //     mSendBufferSize = send_buf_size;
    // }

    // if (descriptor_.min_recv_buffer_size() == 0)
    // {
    //     socklen_t optlen = sizeof(recv_buf_size);
    //     getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &recv_buf_size, &optlen);
    //     set_receive_buffer_size(recv_buf_size);
    //     if (descriptor_.min_recv_buffer_size() < s_minimumSocketBuffer)
    //     {
    //         recv_buf_size = s_minimumSocketBuffer;
    //         set_receive_buffer_size(s_minimumSocketBuffer);
    //     }

    //     setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &recv_buf_size, sizeof(recv_buf_size));
    //     mReceiveBufferSize = recv_buf_size;
    // }
    close(sockfd);

    // if (descriptor_.max_message_size_ > s_maximumMessageSize)
    // {
    //     // LOG_ERROR(TRANSPORT_MSG_OUT, "max_message_size_ cannot be greater than 65000");
    //     return false;
    // }

    // if (descriptor_.max_message_size_ > descriptor_.min_send_buffer_size())
    // {
    //     // LOG_ERROR(TRANSPORT_MSG_OUT, "max_message_size_ cannot be greater than send_buffer_size");
    //     return false;
    // }

    // if (descriptor_.max_message_size_ > descriptor_.min_recv_buffer_size())
    // {
    //     // LOG_ERROR(TRANSPORT_MSG_OUT, "max_message_size_ cannot be greater than receive_buffer_size");
    //     return false;
    // }

    // TODO(Ricardo) Create an event that update this list.
    get_ips(currentInterfaces);

    return true;
}

bool UDPTransportInterface::IsLocatorSupported(
    const Locator &locator) const
{
    return locator.kind == transport_kind_;
}

bool UDPTransportInterface::OpenOutputChannel(
    SendResourceList &sender_resource_list,
    const Locator &locator)
{
    if (!IsLocatorSupported(locator))
    {
        return false;
    }

    auto send_socket = uvw::loop::get_default()->resource<uvw::udp_handle>();
    if(send_socket)
    {
        send_socket->bind(IPLocator::toIPv4string(locator), locator.port);
        sender_resource_list.emplace_back(
            static_cast<SenderResource *>(new UDPSenderResource(*this, send_socket, false, true))
        );
    }

    return false;
}

Locator UDPTransportInterface::RemoteToMainLocal(
    const Locator &remote) const
{
    if (!IsLocatorSupported(remote))
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
        if (IsLocatorSupported(locator))
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
    // if (send_buffer_size > descriptor_.min_send_buffer_size())
    // {
    //     return false;
    // }

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

bool UDPTransportInterface::fillMetatrafficMulticastLocator(
    Locator &locator,
    uint32_t metatraffic_multicast_port) const
{
    if (locator.port == 0)
    {
        locator.port = metatraffic_multicast_port;
    }
    return true;
}

bool UDPTransportInterface::fillMetatrafficUnicastLocator(
    Locator &locator,
    uint32_t metatraffic_unicast_port) const
{
    if (locator.port == 0)
    {
        locator.port = metatraffic_unicast_port;
    }
    return true;
}


bool UDPTransportInterface::fillUnicastLocator(
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

std::vector<std::string> UDPTransportInterface::PrintNetworkInterfaces()
{
    struct ifaddrs *interfaces = nullptr;
    struct ifaddrs *interface = nullptr;

    std::vector<std::string> vec;

    if (getifaddrs(&interfaces) == -1)
    {
        std::cerr << "Failed to get network interfaces" << std::endl;
        return vec;
    }

    for (interface = interfaces; interface != nullptr; interface = interface->ifa_next)
    {
        if (interface->ifa_addr->sa_family == AF_INET)
        {
            void *tmpAddrPtr = &((struct sockaddr_in *)interface->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            std::string str(addressBuffer);
            vec.push_back(str);
        }
    }
    freeifaddrs(interfaces);
    return vec;
}

} // namespace transport
