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
#include "IPLocator.h"
#include "UDPv6Transport.h"
#include <transport/TransportInterface.h>
#include <transport/SenderResource.h>

namespace transport
{

static void get_ipv6s(
    std::vector<IPFinder::info_IP> &locNames,
    bool return_loopback = false)
{
    IPFinder::getIPs(&locNames, return_loopback);
    // Controller out IP4
    auto new_end = remove_if(locNames.begin(),
                                locNames.end(),
                                [](IPFinder::info_IP ip)
                                {
                                    return ip.type != IPFinder::IP6 && ip.type != IPFinder::IP6_LOCAL;
                                });
    locNames.erase(new_end, locNames.end());
    std::for_each(locNames.begin(), locNames.end(), [](IPFinder::info_IP &loc)
                    { loc.locator.kind = LOCATOR_KIND_UDPv6; });
}

UDPv6Transport::UDPv6Transport()
    : UDPTransportInterface(LOCATOR_KIND_UDPv6)
{
}

UDPv6Transport::~UDPv6Transport()
{
}

bool UDPv6Transport::default_metatraffic_multicast_locators(
    LocatorList &locators,
    uint32_t metatraffic_multicast_port) const
{
    Locator locator;
    locator.kind = LOCATOR_KIND_UDPv6;
    locator.port = static_cast<uint16_t>(metatraffic_multicast_port);
    IPLocator::setIPv6(locator, "ff1e::ffff:efff:1");
    locators.push_back(locator);
    return true;
}

bool UDPv6Transport::default_metatraffic_unicast_locators(
    LocatorList &locators,
    uint32_t metatraffic_unicast_port) const
{
    Locator locator;
    locator.kind = LOCATOR_KIND_UDPv6;
    locator.port = static_cast<uint16_t>(metatraffic_unicast_port);
    locator.set_invalid_address();
    locators.push_back(locator);

    return true;
}

bool UDPv6Transport::compare_locator_ip(
    const Locator &lh,
    const Locator &rh) const
{
    return IPLocator::compareAddress(lh, rh);
}

bool UDPv6Transport::compare_locator_ip_and_port(
    const Locator &lh,
    const Locator &rh) const
{
    return IPLocator::compareAddressAndPhysicalPort(lh, rh);
}

void UDPv6Transport::get_ips(
    std::vector<IPFinder::info_IP> &locNames,
    bool return_loopback)
{
    get_ipv6s(locNames, return_loopback);
}

bool UDPv6Transport::open_input_channel(
    ReceiverResourceList &receiver_resource_list,
    const Locator &locator,
    uint32_t maxMsgSize)
{
    (void)receiver_resource_list;
    (void)locator;
    (void)maxMsgSize;
    return true;
}

LocatorList UDPv6Transport::normalize_locator(
    const Locator &locator)
{
    LocatorList list;
    if (IPLocator::isAny(locator))
    {
        std::vector<IPFinder::info_IP> locNames;
        get_ipv6s(locNames);
        for (const auto &infoIP : locNames)
        {
            Locator newloc(locator);
            IPLocator::setIPv6(newloc, infoIP.locator);
            list.push_back(newloc);
        }
        if (list.empty())
        {
            Locator newloc(locator);
            IPLocator::setIPv6(newloc, "::1");
            list.push_back(newloc);
        }
    }
    else
    {
        list.push_back(locator);
    }

    return list;
}

void UDPv6Transport::update_network_interfaces()
{
    UDPTransportInterface::update_network_interfaces();
    // TODO(jlbueno)
}

} // namespace transport
