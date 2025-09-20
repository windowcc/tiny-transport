// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

#ifndef TRANSPORT_UDP_CHANNEL_RESOURCE_INFO_
#define TRANSPORT_UDP_CHANNEL_RESOURCE_INFO_

#include <uvw.hpp>
#include "IPLocator.h"
#include <transport/type.h>
#include <transport/ReceiverResource.h>

namespace transport
{
class Socket;
    class SocketLocator;
class UDPTransportInterface;

typedef std::shared_ptr<Socket> UDPSocket;

struct SocketLocator
{
    std::string ip {};
    unsigned int port {0};
};

using Callback = std::function<void(const unsigned char* data,
                                    const uint32_t size,
                                    const SocketLocator& local_locator,
                                    const SocketLocator& remote_locator)>;

class UDPReceiverResource : public ReceiverResource
{
public:
    UDPReceiverResource(
        UDPTransportInterface *transport,
        std::shared_ptr<uvw::udp_handle> socket,
        uint32_t maxMsgSize,
        const Locator &locator);

    virtual ~UDPReceiverResource();

    void register_receiver(
        const Callback &callback);

private:
    bool alive_;
    std::shared_ptr<uvw::udp_handle> socket_;
    Callback callback_;
    UDPTransportInterface *transport_;

    UDPReceiverResource(
        const UDPReceiverResource &) = delete;
    UDPReceiverResource(
        const UDPReceiverResource &&) = delete;
    UDPReceiverResource &operator=(
        const UDPReceiverResource &) = delete;
    UDPReceiverResource &operator=(
        UDPReceiverResource &&) = delete;
};

} // namespace transport

#endif // TRANSPORT_UDP_CHANNEL_RESOURCE_INFO_
