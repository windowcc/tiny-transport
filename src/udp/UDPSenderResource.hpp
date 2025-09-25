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

#ifndef TRANSPORT_TRANSPORT_UDPSENDERRESOURCE_HPP__
#define TRANSPORT_TRANSPORT_UDPSENDERRESOURCE_HPP__

#include <uvw.hpp>
#include <transport/type.h>
#include <transport/SenderResource.h>
#include "UDPTransportInterface.h"

namespace transport
{

class UDPSenderResource : public SenderResource
{
public:
    UDPSenderResource(
        UDPTransportInterface &transport,
        std::shared_ptr<uvw::udp_handle> socket,
        bool only_multicast_purpose = false,
        bool whitelisted = false)
    : SenderResource()
    , socket_(socket)
    , only_multicast_purpose_(only_multicast_purpose)
    , whitelisted_(whitelisted)
    , transport_(transport)
    {
        send_lambda_ = [this, &transport](
                            const octet *data,
                            uint32_t dataSize,
                            const LocatorList &locators,
                            const std::chrono::steady_clock::time_point &max_blocking_time_point) -> bool
        {
            return transport.send(data, dataSize, socket_, locators, only_multicast_purpose_, whitelisted_,
                                    max_blocking_time_point);
        };
    }

    virtual ~UDPSenderResource()
    {
        // socket_->close();
    }

private:
    UDPSenderResource() = delete;

    UDPSenderResource(
        const SenderResource &) = delete;

    UDPSenderResource &operator=(
        const SenderResource &) = delete;

    std::shared_ptr<uvw::udp_handle> socket_;
    bool only_multicast_purpose_;
    bool whitelisted_;
    UDPTransportInterface &transport_;
};

} // namespace transport

#endif // TRANSPORT_TRANSPORT_UDPSENDERRESOURCE_HPP__
