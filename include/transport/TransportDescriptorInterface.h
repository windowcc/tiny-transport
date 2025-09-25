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

#ifndef TRANSPORT_TRANSPORT_DESCRIPTOR_INTERFACE_H_
#define TRANSPORT_TRANSPORT_DESCRIPTOR_INTERFACE_H_

#ifdef _WIN32
#include <cstdint>
#endif // ifdef _WIN32
#include <vector>
#include <string>
#include <memory>

namespace uvw
{
    class loop;
} // namespace uvw

namespace transport
{
class TransportInterface;

//! Default time to live (TTL)
constexpr uint8_t s_defaultTTL = 1;

//! Default maximum message size
constexpr uint32_t s_maximumMessageSize = 65500;
//! Default maximum initial peers range
constexpr uint32_t s_maximumInitialPeersRange = 4;

/**
 * Virtual base class for the data type used to define transport configuration.
 * It acts as a builder for a given transport meaning that it allows to configure
 * the transport, and then a new Transport can be built according to this configuration
 * using its create_transport() factory member function.
 *
 * - max_message_size_: maximum size of a single message in the transport.
 *
 * - max_initial_peers_range_: number of channels opened with each initial remote peer.
 *
 * @ingroup TRANSPORT_MODULE
 * */
struct TransportDescriptorInterface : public std::enable_shared_from_this<TransportDescriptorInterface>
{
    //! Constructor
    TransportDescriptorInterface(
        uint32_t maximumMessageSize,
        uint32_t maximumInitialPeersRange)
        : send_buffer_size_(0)
        , recv_buffer_size_(0)
        , ttl_(s_defaultTTL)
        , max_message_size_(maximumMessageSize)
        , max_initial_peers_range_(maximumInitialPeersRange)
    {
    }

    //! Copy constructor
    TransportDescriptorInterface(
        const TransportDescriptorInterface &t) = default;

    //! Copy assignment
    TransportDescriptorInterface &operator=(
        const TransportDescriptorInterface &t) = default;

    //! Destructor
    virtual ~TransportDescriptorInterface() = default;

    /**
     * Factory method pattern. It will create and return a TransportInterface
     * corresponding to this descriptor. This provides an interface to the TransportFactory
     * to create the transports without the need to know about their type
     */
    virtual TransportInterface *create_transport(std::shared_ptr<uvw::loop> loop) const {return nullptr;};

    virtual int32_t transport_kind() const = 0;

    virtual uint32_t min_send_buffer_size() const
    {
        return send_buffer_size_;
    }

    virtual uint32_t min_recv_buffer_size() const
    {
        return recv_buffer_size_;
    }

    //! Returns the maximum size expected for received messages.
    virtual uint32_t max_message_size() const
    {
        return max_message_size_;
    }

    /** Returns the maximum number of opened channels for each initial remote peer
     * (maximum number of guessed initial peers to try to connect)
     */
    virtual uint32_t max_initial_peers_range() const
    {
        return max_initial_peers_range_;
    }

    //! Comparison operator
    bool operator==(
        const TransportDescriptorInterface &t) const
    {
        return (this->send_buffer_size_ == t.min_send_buffer_size() &&
                this->recv_buffer_size_ == t.recv_buffer_size_ &&
                this->ttl_ == t.ttl_ &&
                this->max_message_size_ == t.max_message_size() &&
                this->max_initial_peers_range_ == t.max_initial_peers_range());
    }

    //! Length of the send buffer.
    uint32_t send_buffer_size_;
    //! Length of the receive buffer.
    uint32_t recv_buffer_size_;
    //! Specified time to live (8bit - 255 max TTL)
    uint8_t ttl_;

    //! Maximum size of a single message in the transport
    uint32_t max_message_size_;

    //! Number of channels opened with each initial remote peer.
    uint32_t max_initial_peers_range_;
};



template<typename Descriptor>
class TransportDescriptor;

struct UDPv4Descriptor
{

};

template<>
class TransportDescriptor<UDPv4Descriptor> : public TransportDescriptorInterface
{
public:
    TransportDescriptor()
    : TransportDescriptorInterface(s_maximumMessageSize, s_maximumInitialPeersRange)
    {

    }

    virtual ~TransportDescriptor(){}
public:
    virtual TransportInterface *create_transport(std::shared_ptr<uvw::loop> loop) const override;

    virtual int32_t transport_kind() const override;
};

struct UDPv6Descriptor
{

};
template<>
class TransportDescriptor<UDPv6Descriptor> : public TransportDescriptorInterface
{
public:
    TransportDescriptor()
    : TransportDescriptorInterface(s_maximumMessageSize, s_maximumInitialPeersRange)
    {

    }

    virtual ~TransportDescriptor(){}
public:
    virtual TransportInterface *create_transport(std::shared_ptr<uvw::loop> loop) const override;

    virtual int32_t transport_kind() const override;
};

} // namespace transport

#endif // TRANSPORT_TRANSPORT_DESCRIPTOR_INTERFACE_H_
