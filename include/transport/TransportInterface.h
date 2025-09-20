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

#ifndef TRANSPORT_TRANSPORT_INTERFACE_H_
#define TRANSPORT_TRANSPORT_INTERFACE_H_

#include <string>
#include <vector>
#include <memory>
#include <transport/SenderResource.h>
#include <transport/ReceiverResource.h>
#include <transport/TransportDescriptorInterface.h>

namespace transport
{

//! Default maximum message size
constexpr uint32_t s_maximumMessageSize = 65500;
//! Default maximum initial peers range
constexpr uint32_t s_maximumInitialPeersRange = 4;
//! Default minimum socket buffer
constexpr uint32_t s_minimumSocketBuffer = 65536;
//! Default IPv4 address
static const std::string s_IPv4AddressAny = "0.0.0.0";
//! Default IPv6 address
static const std::string s_IPv6AddressAny = "::";

using SendResourceList = std::vector<std::shared_ptr<SenderResource>>;

using ReceiverResourceList = std::vector<std::shared_ptr<ReceiverResource>>;


class PortParameters;
/**
 * Interface against which to implement a transport layer, decoupled from TRANSPORT internals.
 * TransportInterface expects the user to implement a logical equivalence between Locators and protocol-specific "channels".
 * This equivalence can be narrowing: For instance in UDP/IP, a port would take the role of channel, and several different
 * locators can map to the same port, and hence the same channel.
 * @ingroup TRANSPORT_MODULE
 * */
class TransportInterface
{
public:
    /**
     * Aside from the API defined here, an user-defined Transport must define a descriptor data type and a constructor that
     * expects a constant reference to such descriptor. e.g:
     *
     * class MyTransport:
     * public:
     *    typedef struct { ... } MyTransportDescriptor;
     *    MyTransport(const MyTransportDescriptor&);
     *    ...
     */
    virtual ~TransportInterface() = default;

    //! Copy constructor
    TransportInterface(
        const TransportInterface &t) = delete;

    //! Copy assignment
    TransportInterface &operator=(
        const TransportInterface &t) = delete;

    //! Move constructor
    TransportInterface(
        TransportInterface &&t) = delete;

    //! Move assignment
    TransportInterface &operator=(
        TransportInterface &&t) = delete;

    /**
     * Initialize this transport. This method will prepare all the internals of the transport.
     * @param properties Optional policy to specify additional parameters of the created transport.
     * @return True when the transport was correctly initialized.
     */
    virtual bool init() = 0;

    //! Must report whether the given locator is supported by this transport (typically inspecting its "kind" value).
    virtual bool IsLocatorSupported(
        const Locator &) const = 0;

    //! Returns the locator describing the main (most general) channel that can write to the provided remote locator.
    virtual Locator RemoteToMainLocal(
        const Locator &remote) const = 0;

    //! Must open the channel that maps to/from the given locator. This method must allocate, reserve and mark
    //! any resources that are needed for said channel.
    virtual bool OpenOutputChannel(
        SendResourceList &sender_resource_list,
        const Locator &) = 0;

    /** Opens an input channel to receive incoming connections.
     *   If there is an existing channel it registers the receiver interface.
     */
    virtual bool OpenInputChannel(
        ReceiverResourceList &,
        const Locator &,
        uint32_t) = 0;

    //! Must report whether two locators map to the same internal channel.
    virtual bool DoInputLocatorsMatch(
        const Locator &,
        const Locator &) const = 0;

    //! Performs locator normalization (assign valid IP if not defined by user)
    virtual LocatorList NormalizeLocator(
        const Locator &locator) = 0;

    //! Return the transport configuration (Transport Descriptor)
    virtual TransportDescriptorInterface *get_configuration() = 0;

    //! Add default output locator to the locator list
    virtual void AddDefaultOutputLocator(
        LocatorList &defaultList) = 0;

    //! Add metatraffic multicast locator with the given port
    virtual bool getDefaultMetatrafficMulticastLocators(
        LocatorList &locators,
        uint32_t metatraffic_multicast_port) const = 0;

    //! Add metatraffic unicast locator with the given port
    virtual bool getDefaultMetatrafficUnicastLocators(
        LocatorList &locators,
        uint32_t metatraffic_unicast_port) const = 0;

    //! Add unicast locator with the given port
    // virtual bool getDefaultUnicastLocators(
    //     LocatorList &locators,
    //     uint32_t unicast_port) const = 0;

    //! Assign port to the given metatraffic multicast locator if not already defined
    virtual bool fillMetatrafficMulticastLocator(
        Locator &locator,
        uint32_t metatraffic_multicast_port) const = 0;

    //! Assign port to the given metatraffic unicast locator if not already defined
    virtual bool fillMetatrafficUnicastLocator(
        Locator &locator,
        uint32_t metatraffic_unicast_port) const = 0;


    //! Assign port to the given unicast locator if not already defined
    virtual bool fillUnicastLocator(
        Locator &locator,
        uint32_t well_known_port) const = 0;

    /**
     * @return The maximum datagram size for reception supported by the transport
     */
    virtual uint32_t max_recv_buffer_size() const = 0;

    /**
     * shutdown method to close the connections of the transports.
     */
    virtual void shutdown()
    {
    }

    /**
     * @brief Update network interfaces.
     */
    virtual void update_network_interfaces()
    {
    }

    //! Return the transport kind
    int32_t kind() const
    {
        return transport_kind_;
    }

protected:
    TransportInterface(
        int32_t transport_kind)
        : transport_kind_(transport_kind)
    {
    }

    int32_t transport_kind_;
};

} // namespace transport

#endif // TRANSPORT_TRANSPORT_INTERFACE_H_
