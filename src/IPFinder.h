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
 * @file IPFinder.h
 *
 */

#ifndef TRANSPORT_IPFINDER_H_
#define TRANSPORT_IPFINDER_H_

#include <string>
#include <transport/type.h>

namespace transport
{

/**
 * Class IPFinder, to determine the IP of the NICs.
 * @ingroup UTILITIES_MODULE
 */
class IPFinder
{
public:
    /**
     * Enum IPTYPE, to define the type of IP obtained from the NICs.
     */
    enum IPTYPE
    {
        IP4,       //!< IP4
        IP6,       //!< IP6
        IP4_LOCAL, //!< IP4_LOCAL
        IP6_LOCAL  //!< IP6_LOCAL
    };
    /**
     * Structure info_IP with information about a specific IP obtained from a NIC.
     */
    typedef struct info_IP
    {
        IPTYPE type;
        std::string name;
        std::string dev;
        Locator locator;
    } info_IP;

    /**
     * Structure info_MAC with information about a specific MAC obtained from a NIC.
     */
    typedef struct info_MAC
    {
        unsigned char address[6];

        bool operator==(
            const info_MAC &other)
        {
            return memcmp(address, other.address, 6) == 0;
        }

    } info_MAC;

    IPFinder();
    virtual ~IPFinder();

    static bool getIPs(
        std::vector<info_IP> *vec_name,
        bool return_loopback = false);

    /**
     * Get the IP4Adresses in all interfaces.
     * @param[out] locators List of locators to be populated with the IP4 addresses.
     */
    static bool getIP4Address(
        LocatorList *locators);
    /**
     * Get the IP6Adresses in all interfaces.
     * @param[out] locators List of locators to be populated with the IP6 addresses.
     */
    static bool getIP6Address(
        LocatorList *locators);
    /**
     * Get all IP Adresses in all interfaces.
     * @param[out] locators List of locators to be populated with the addresses.
     */
    static bool getAllIPAddress(
        LocatorList *locators);
    /**
     * Parses an IP4 string, populating a info_IP with its value.
     * @param[out] info info_IP to populate.
     * */
    static bool parseIP4(
        info_IP &info);
    /**
     * Parses an IP6 string, populating a info_IP with its value.
     * @param[out] info info_IP to populate.
     * */
    static bool parseIP6(
        info_IP &info);

    static std::string getIPv4Address(
        const std::string &name);
    static std::string getIPv6Address(
        const std::string &name);

    /**
     * Get all MAC Adresses of all interfaces.
     * Will return all unique MAC addresses for eadh of the interfaces returned by getAllIPAddress
     * @param[out] macs List of MAC addresses.
     */
    static bool getAllMACAddress(
        std::vector<info_MAC> *macs);
};

} // namespace transport

#endif /* TRANSPORT_IPFINDER_H_ */
