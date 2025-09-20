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

/**
 * @file IPLocator.h
 *
 */

#ifndef RTOPS_IP_LOCATOR_H_
#define RTOPS_IP_LOCATOR_H_

#include <regex>
#include <set>
#include <transport/type.h>

namespace transport
{

/**
 * Class IPLocator, to provide helper functions to the IP based transports.
 * @ingroup UTILITIES_MODULE
 */
class IPLocator
{
public:
    /**
     * Fills locator with the given parameters.
     * @param kindin Kind of the locator.
     * @param address IP Address of the locator as string.
     * @param portin Port of the locator.
     * @param locator Locator to be filled.
     */
    static void createLocator(
        int32_t kindin,
        const std::string &address,
        uint32_t portin,
        Locator &locator);

    //! Sets locator's IPv4.
    static bool setIPv4(
        Locator &locator,
        const unsigned char *addr);

    //! Sets locator's IPv4.
    static bool setIPv4(
        Locator &locator,
        unsigned char o1,
        unsigned char o2,
        unsigned char o3,
        unsigned char o4);

    //! Sets locator's IPv4.
    static bool setIPv4(
        Locator &locator,
        const std::string &ipv4);

    //! Copies locator's IPv4.
    static bool setIPv4(
        Locator &destlocator,
        const Locator &origlocator);

    //! Copies locator's IPv4.
    static bool setIPv4address(
        Locator &destlocator,
        const std::string &lan,
        const std::string &wan,
        const std::string &ipv4);

    //! Retrieves locator's IPv4 as unsigned char array.
    static const unsigned char *getIPv4(
        const Locator &locator);

    //! Check if the locator has IPv4.
    static bool hasIPv4(
        const Locator &locator);

    //! Returns a string representation of the locator's IPv4.
    static std::string toIPv4string(
        const Locator &locator);

    //! Copies locator's IPv4.
    static bool copyIPv4(
        const Locator &locator,
        unsigned char *dest);

    // IPv6
    //! Sets locator's IPv6.
    static bool setIPv6(
        Locator &locator,
        const unsigned char *addr);

    //! Sets locator's IPv6.
    static bool setIPv6(
        Locator &locator,
        uint16_t group0,
        uint16_t group1,
        uint16_t group2,
        uint16_t group3,
        uint16_t group4,
        uint16_t group5,
        uint16_t group6,
        uint16_t group7);

    //! Sets locator's IPv6.
    static bool setIPv6(
        Locator &locator,
        const std::string &ipv6);

    //! Copies locator's IPv6.
    static bool setIPv6(
        Locator &destlocator,
        const Locator &origlocator);

    //! Retrieves locator's IPv6 as unsigned char array.
    static const unsigned char *getIPv6(
        const Locator &locator);

    //! Check if the locator has IPv6.
    static bool hasIPv6(
        const Locator &locator);

    //! Returns a string representation of the locator's IPv6 following RFC 5952 recommendation.
    static std::string toIPv6string(
        const Locator &locator);

    //! Copies locator's IPv6.
    static bool copyIPv6(
        const Locator &locator,
        unsigned char *dest);

    //! Sets locator's IP
    static bool ip(
        Locator &locator,
        const std::string &ip);

    // TCP
    //! Sets locator's logical port (as in RTCP protocol)
    static bool setLogicalPort(
        Locator &locator,
        uint16_t port);

    //! Gets locator's logical port (as in RTCP protocol)
    static uint16_t getLogicalPort(
        const Locator &locator);

    //! Sets locator's physical port (as in RTCP protocol)
    static bool setPhysicalPort(
        Locator &locator,
        uint16_t port);

    //! Gets locator's physical port (as in RTCP protocol)
    static uint16_t getPhysicalPort(
        const Locator &locator);

    // TCPv4
    //! Sets locator's WAN address (as in RTCP protocol)
    static bool setWan(
        Locator &locator,
        unsigned char o1,
        unsigned char o2,
        unsigned char o3,
        unsigned char o4);

    //! Sets locator's WAN address (as in RTCP protocol)
    static bool setWan(
        Locator &locator,
        const std::string &wan);

    //! Gets locator's WAN address (as in RTCP protocol)
    static const unsigned char *getWan(
        const Locator &locator);

    //! Checks if the locator has WAN address (as in RTCP protocol)
    static bool hasWan(
        const Locator &locator);

    //! Retrieves a string representation of the locator's WAN address (as in RTCP protocol)
    static std::string toWanstring(
        const Locator &locator);

    //! Sets locator's LAN ID (as in RTCP protocol)
    static bool setLanID(
        Locator &locator,
        const std::string &lanId);

    //! Gets locator's LAN ID (as in RTCP protocol)
    static const unsigned char *getLanID(
        const Locator &locator);

    //! Retrieves a string representation of the locator's LAN ID (as in RTCP protocol)
    static std::string toLanIDstring(
        const Locator &locator);

    //! Returns a new locator without logical port (as in RTCP protocol).
    static Locator toPhysicalLocator(
        const Locator &locator);

    //! Checks if a locator WAN address and IP address are the same (as in RTCP protocol).
    static bool ip_equals_wan(
        const Locator &locator);

    // Common
    //! Sets locator's RTCP port. Physical for UDP and logical for TCP (as in RTCP protocol)
    static bool setPortTRANSPORT(
        Locator &locator,
        uint16_t port);

    //! Gets locator's RTCP port. Physical for UDP and logical for TCP (as in RTCP protocol)
    static uint16_t getPortTRANSPORT(
        Locator &locator);

    //! Checks if a locator has local IP address.
    static bool isLocal(
        const Locator &locator);

    //! Checks if a locator has any IP address.
    static bool isAny(
        const Locator &locator);

    //! Checks if both locators has the same IP address.
    static bool compareAddress(
        const Locator &loc1,
        const Locator &loc2,
        bool fullAddress = false);

    //! Checks if a both locators has the same IP address and physical port  (as in RTCP protocol).
    static bool compareAddressAndPhysicalPort(
        const Locator &loc1,
        const Locator &loc2);

    // UDP
    //! Checks if the locator has a multicast IP address.
    static bool isMulticast(
        const Locator &locator);

    //! Resolve an address name by a DNS request and return the IP that this address references by a DNS server
    // static std::pair<std::set<std::string>, std::set<std::string>> resolveNameDNS(
    //         const std::string& address_name);

    //! Check whether a string contains an IPv4 format
    static bool isIPv4(
        const std::string &address);
    //! Check whether a string contains an IPv6 format
    static bool isIPv6(
        const std::string &address);
    static std::string getIpByLocatorv4(const Locator &locator);

protected:
    // Checks if the locator address is equal to 0
    // It checks the proper locator address depending on the locator kind
    static bool isEmpty(
        const Locator &locator);

    // Checks if the locator address from index till the end is equal to 0
    static bool isEmpty(
        const Locator &locator,
        uint16_t index);

    // Checks if a string matches an ipv6 address
    static bool IPv6isCorrect(
        const std::string &ipv6);

private:
    IPLocator() = delete;
    ~IPLocator() = delete;
};

} // namespace transport

#endif /* RTOPS_IP_LOCATOR_H_ */
