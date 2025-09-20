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

#ifndef TRANSPORT_SOCKET_TRANSPORT_DESCRIPTOR_H_
#define TRANSPORT_SOCKET_TRANSPORT_DESCRIPTOR_H_

#include <cstdint>
#include <vector>
#include <string>
#include <transport/TransportInterface.h>

namespace transport
{

//! Default time to live (TTL)
constexpr uint8_t s_defaultTTL = 1;

/**
 * Virtual base class for the data type used to define configuration of transports using sockets.
 *
 * - \c send_buffer_size_: size of the sending buffer of the socket (in octets).
 *
 * - \c recv_buffer_size_: size of the receiving buffer of the socket (in octets).
 *
 * - \c interfaceWhiteList: list of allowed interfaces.
 *
 * - \c TTL: time to live, in number of hops.
 *
 * @ingroup TRANSPORT_MODULE
 * */
struct SocketTransportDescriptor : public TransportDescriptorInterface
{
    //! Constructor
    SocketTransportDescriptor(
        uint32_t maximumMessageSize,
        uint32_t maximumInitialPeersRange)
        : TransportDescriptorInterface(maximumMessageSize, maximumInitialPeersRange)
        , send_buffer_size_(0)
        , recv_buffer_size_(0)
        , ttl_(s_defaultTTL)
    {
    }

    //! Copy constructor
    SocketTransportDescriptor(
        const SocketTransportDescriptor &t) = default;

    //! Copy assignment
    SocketTransportDescriptor &operator=(
        const SocketTransportDescriptor &t) = default;

    //! Destructor
    virtual ~SocketTransportDescriptor() = default;

    virtual uint32_t min_send_buffer_size() const override
    {
        return send_buffer_size_;
    }

    virtual uint32_t min_recv_buffer_size() const override
    {
        return recv_buffer_size_;
    }

    //! Comparison operator
    bool operator==(
        const SocketTransportDescriptor &t) const
    {
        return (this->send_buffer_size_ == t.min_send_buffer_size() &&
                this->recv_buffer_size_ == t.recv_buffer_size_ &&
                this->ttl_ == t.ttl_ &&
                TransportDescriptorInterface::operator==(t));
    }

    //! Length of the send buffer.
    uint32_t send_buffer_size_;
    //! Length of the receive buffer.
    uint32_t recv_buffer_size_;
    //! Specified time to live (8bit - 255 max TTL)
    uint8_t ttl_;
};

} // namespace transport

#endif //TRANSPORT_SOCKET_TRANSPORT_DESCRIPTOR_H_
