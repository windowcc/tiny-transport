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


namespace transport
{
class Worker;
class TransportInterface;

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
struct TransportDescriptorInterface
{
    //! Constructor
    TransportDescriptorInterface(
        uint32_t maximumMessageSize,
        uint32_t maximumInitialPeersRange)
        : max_message_size_(maximumMessageSize),
        max_initial_peers_range_(maximumInitialPeersRange)
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
    virtual TransportInterface *create_transport() const = 0;

    //! Returns the minimum size required for a send operation.
    virtual uint32_t min_send_buffer_size() const = 0;
    virtual uint32_t min_recv_buffer_size() const = 0;

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
        return (this->max_message_size_ == t.max_message_size() &&
                this->max_initial_peers_range_ == t.max_initial_peers_range());
    }

    //! Maximum size of a single message in the transport
    uint32_t max_message_size_;

    //! Number of channels opened with each initial remote peer.
    uint32_t max_initial_peers_range_;
};

} // namespace transport

#endif // TRANSPORT_TRANSPORT_DESCRIPTOR_INTERFACE_H_
