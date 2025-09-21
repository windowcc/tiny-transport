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

#ifndef TRANSPORT_UDPV6_TRANSPORT_H_
#define TRANSPORT_UDPV6_TRANSPORT_H_

#include "UDPTransportInterface.h"

namespace transport
{

/**
 * This is a default UDPv6 implementation.
 *    - Opening an output channel by passing a locator will open a socket per interface on the given port.
 *       This collection of sockets constitute the "outbound channel". In other words, a channel corresponds
 *       to a port + a direction.
 *
 *    - It is possible to provide a white list at construction, which limits the interfaces the transport
 *       will ever be able to interact with. If left empty, all interfaces are allowed.
 *
 *    - Opening an input channel by passing a locator will open a socket listening on the given port on every
 *       whitelisted interface, and join the multicast channel specified by the locator address. Hence, any locator
 *       that does not correspond to the multicast range will simply open the port without a subsequent join. Joining
 *       multicast groups late is supported by attempting to open the channel again with the same port + a
 *       multicast address (the open_input_channel function will fail, however, because no new channel has been
 *       opened in a strict sense).
 * @ingroup TRANSPORT_MODULE
 */
class UDPv6Transport : public UDPTransportInterface
{
public:
    UDPv6Transport();

    virtual ~UDPv6Transport() override;

    // const UDPTransportDescriptor *configuration() const override;

    /**
     * Starts listening on the specified port, and if the specified address is in the
     * multicast range, it joins the specified multicast group,
     */
    bool open_input_channel(
        ReceiverResourceList &receiver_resource_list,
        const Locator &,
        uint32_t) override;

    LocatorList normalize_locator(
        const Locator &locator) override;

    bool default_metatraffic_multicast_locators(
        LocatorList &locators,
        uint32_t metatraffic_multicast_port) const override;

    bool default_metatraffic_unicast_locators(
        LocatorList &locators,
        uint32_t metatraffic_unicast_port) const override;

    void update_network_interfaces() override;

protected:
    //! Constructor with no descriptor is necessary for implementations derived from this class.
    // UDPv6Transport();

    bool compare_locator_ip(
        const Locator &lh,
        const Locator &rh) const override;
    bool compare_locator_ip_and_port(
        const Locator &lh,
        const Locator &rh) const override;

    void get_ips(
        std::vector<IPFinder::info_IP> &locNames,
        bool return_loopback = false) override;
};

} // namespace transport

#endif // TRANSPORT_UDPV6_TRANSPORT_H_
