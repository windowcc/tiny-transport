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
 * @file Types.h
 */

#ifndef DDS_CORE_CORE_HPP_
#define DDS_CORE_CORE_HPP_

#include <stddef.h>
#include <iostream>
#include <cstdint>
#include <array>
#include <chrono>
#include <string>
#include <vector>
#include <cstring>
#include <iomanip>
#include <cassert>
#include <transport/config.h>
#include <transport/export.h>

#ifdef _WIN32
#define MEMCCPY _memccpy
#else
#define MEMCCPY memccpy
#endif // ifdef _WIN32

namespace transport
{
using octet = unsigned char;

/// Initialize locator with invalid values
#define LOCATOR_INVALID(loc)                  \
{                                         \
    loc.kind = LOCATOR_KIND_INVALID;      \
    loc.port = LOCATOR_PORT_INVALID;      \
    LOCATOR_ADDRESS_INVALID(loc.address); \
}
/// Invalid locator kind
#define LOCATOR_KIND_INVALID -1

/// Set locator IP address to 0
#define LOCATOR_ADDRESS_INVALID(a)                \
{                                             \
    std::memset(a, 0x00, 16 * sizeof(octet)); \
}

/// Invalid locator port
#define LOCATOR_PORT_INVALID 0

/// Reserved locator kind
#define LOCATOR_KIND_RESERVED 0
/// UDP over IPv4 locator kind
#define LOCATOR_KIND_UDPv4 1
/// UDP over IPv6 locator kindin
#define LOCATOR_KIND_UDPv6 2
/// TCP over IPv4 kind
#define LOCATOR_KIND_TCPv4 4
/// TCP over IPv6 locator kind
#define LOCATOR_KIND_TCPv6 8
/// Shared memory locator kind
#define LOCATOR_KIND_SHM 16

/**
 * @brief Class Locator, uniquely identifies a communication channel for a particular transport.
 * For example, an address + port combination in the case of UDP.
 * @ingroup COMMON_MODULE
 */
struct Locator
{
public:
    /**
     * @brief Equal to operator.
     *
     * @param loc1 Left hand side locator being compared.
     * @param loc2 Right hand side locator being compared.
     * @return true if \c loc1 is equal to  \c loc2.
     * @return false otherwise.
     */
    inline bool operator==(
        const Locator &loc) const
    {
        if (kind != loc.kind)
        {
            return false;
        }
        if (port != loc.port)
        {
            return false;
        }
        if (!std::equal(address, address + 16, loc.address))
        {
            return false;
        }
        return true;
    }

    /**
     * @brief Not equal to operator.
     *
     * @param loc1 Left hand side locator being compared.
     * @param loc2 Right hand side locator being compared.
     * @return true if \c loc1 is not equal to \c loc2.
     * @return false otherwise.
     */
    inline bool operator!=(
        const Locator &loc) const
    {
        return !((*this) == loc);
    }

    /**
     * @brief Greater than operator.
     *
     * @param loc1 Left hand side locator being compared.
     * @param loc2 Right hand side locator being compared.
     * @return true if \c loc1 is less than \c loc2.
     * @return false otherwise.
     */
    inline bool operator>(
        const Locator &loc) const
    {
        return memcmp(this, &loc, sizeof(Locator)) > 0;
    }

    /**
     * @brief Less than operator.
     *
     * @param loc1 Left hand side locator being compared.
     * @param loc2 Right hand side locator being compared.
     * @return true if \c loc1 is less than \c loc2.
     * @return false otherwise.
     */
    inline bool operator<(
        const Locator &loc) const
    {
        return memcmp(this, &loc, sizeof(Locator)) < 0;
    }

    /**
     * @brief Specifies the locator type. Valid values are:
     *
     * LOCATOR_KIND_UDPv4
     *
     * LOCATOR_KIND_UDPv6
     *
     * LOCATOR_KIND_TCPv4
     *
     * LOCATOR_KIND_TCPv6
     *
     * LOCATOR_KIND_SHM
     */
    int32_t kind;
    /// Network port
    uint32_t port;
    /// IP address
    octet address[16];

    /// Default constructor
    Locator()
        : kind(LOCATOR_KIND_UDPv4)
    {
        port = 0;
        LOCATOR_ADDRESS_INVALID(address);
    }

    /// Move constructor
    Locator(
        Locator &&loc)
        : kind(loc.kind)
    {
        port = loc.port;
        std::memcpy(address, loc.address, 16 * sizeof(octet));
    }

    /// Copy constructor
    Locator(
        const Locator &loc)
        : kind(loc.kind)
    {
        port = loc.port;
        std::memcpy(address, loc.address, 16 * sizeof(octet));
    }

    /// Port constructor
    Locator(
        uint32_t portin)
        : kind(LOCATOR_KIND_UDPv4)
    {
        port = portin;
        LOCATOR_ADDRESS_INVALID(address);
    }

    /// Kind and port constructor
    Locator(
        int32_t kindin,
        uint32_t portin)
        : kind(kindin)
    {
        port = portin;
        LOCATOR_ADDRESS_INVALID(address);
    }

    /// Copy assignment
    Locator &operator=(
        const Locator &loc)
    {
        kind = loc.kind;
        port = loc.port;
        std::memcpy(address, loc.address, 16 * sizeof(octet));
        return *this;
    }

    /**
     * @brief Set the locator IP address using another locator.
     *
     * @param other Locator which IP address is used to set this locator IP address.
     * @return always true.
     */
    bool set_address(
        const Locator &other)
    {
        memcpy(address, other.address, sizeof(octet) * 16);
        return true;
    }

    /**
     * @brief Getter for the locator IP address.
     *
     * @return IP address as octet pointer.
     */
    octet *get_address()
    {
        return address;
    }

    /**
     * @brief Getter for a specific field of the locator IP address.
     *
     * @param field IP address element to be accessed.
     * @return Octet value for the specific IP address element.
     */
    octet get_address(
        uint16_t field) const
    {
        return address[field];
    }

    /**
     * @brief Automatic setter for setting locator IP address to invalid address (0).
     */
    void set_invalid_address()
    {
        LOCATOR_ADDRESS_INVALID(address);
    }

    /**
     * @brief Auxiliary method to check that locator kind is not LOCATOR_KIND_INVALID (-1).
     *
     * @param loc Locator to be checked.
     * @return true if the locator kind is not LOCATOR_KIND_INVALID.
     * @return false otherwise.
     */
    inline bool is_valid() const
    {
        return (0 <= kind);
    }
};

/**
 * Class LocatorList, a Locator vector that doesn't allow duplicates.
 * @ingroup COMMON_MODULE
 */
class LocatorList
{
    /// Iterator to iterate over a vector of locators.
    typedef std::vector<Locator>::iterator LocatorListIterator;
    /// Constant iterator to iterate over a vector of locators.
    typedef std::vector<Locator>::const_iterator LocatorListConstIterator;

public:
    /// Constructor
    LocatorList()
    {
    }

    /// Destructor
    ~LocatorList()
    {
        clear();
    }

    /// Copy constructor
    LocatorList(
        const LocatorList &list)
        : m_locators(list.m_locators)
    {
    }

    /// Move constructor
    LocatorList(
        LocatorList &&list)
        : m_locators(std::move(list.m_locators))
    {
    }

    /// Copy assignment
    LocatorList &operator=(
        const LocatorList &list)
    {
        m_locators = list.m_locators;
        return *this;
    }

    /// Move assignment
    LocatorList &operator=(
        LocatorList &&list)
    {
        m_locators = std::move(list.m_locators);
        return *this;
    }

    /// Equal to operator
    bool operator==(
        const LocatorList &locator_list) const
    {
        if (locator_list.m_locators.size() == m_locators.size())
        {
            bool returnedValue = true;

            for (auto it = locator_list.m_locators.begin(); returnedValue &&
                                                            it != locator_list.m_locators.end();
                    ++it)
            {
                returnedValue = false;

                for (auto it2 = m_locators.begin(); !returnedValue && it2 != m_locators.end(); ++it2)
                {
                    if (*it == *it2)
                    {
                        returnedValue = true;
                    }
                }
            }

            return returnedValue;
        }

        return false;
    }

    /**
     * @brief Return an iterator to the beginning.
     *
     * @return LocatorListIterator iterator to the first locator.
     */
    LocatorListIterator begin()
    {
        return m_locators.begin();
    }

    /**
     * @brief Return an iterator to the end.
     *
     * @return LocatorListIterator iterator to the element following the last element.
     */
    LocatorListIterator end()
    {
        return m_locators.end();
    }

    /**
     * @brief Return a constant iterator to the beginning.
     *
     * @return LocatorListConstIterator iterator to the first locator.
     */
    LocatorListConstIterator begin() const
    {
        return m_locators.begin();
    }

    /**
     * @brief Return a constant iterator to the end.
     *
     * @return LocatorListConstIterator iterator to the element following the last element.
     */
    LocatorListConstIterator end() const
    {
        return m_locators.end();
    }

    /**
     * @brief Return the number of locators.
     *
     * @return size_t The number of locators in the container.
     */
    size_t size() const
    {
        return m_locators.size();
    }

    /**
     * @brief Replace the contents of the container.
     *
     * @param list New content to be saved into the container.
     * @return LocatorList& reference to the container with the replaced content.
     */
    LocatorList &assign(
        const LocatorList &list)
    {
        if (!(*this == list))
        {
            m_locators = list.m_locators;
        }
        return *this;
    }

    /**
     * @brief Erase all locators from the container.
     */
    void clear()
    {
        return m_locators.clear();
    }

    void erase(LocatorListIterator itor)
    {
        m_locators.erase(itor);
    }

    /**
     * @brief Add locator to the end if not found within the list.
     *
     * @param loc locator to be appended.
     */
    void push_back(
        const Locator &loc)
    {
        bool already = false;
        for (LocatorListIterator it = this->begin(); it != this->end(); ++it)
        {
            if (loc == *it)
            {
                already = true;
                break;
            }
        }
        if (!already)
        {
            m_locators.push_back(loc);
        }
    }

    /**
     * @brief Add several locators to the end if not already present within the list.
     *
     * @param locList LocatorList with the locators to be appended.
     */
    void push_back(
        const LocatorList &locList)
    {
        for (auto it = locList.m_locators.begin(); it != locList.m_locators.end(); ++it)
        {
            this->push_back(*it);
        }
    }

    /**
     * @brief Check that the container has no locators.
     *
     * @return true if the container is empty. False otherwise.
     */
    bool empty() const
    {
        return m_locators.empty();
    }

    /**
     * @brief Check that every locator contained in the list is not LOCATOR_KIND_INVALID.
     *
     * @return true if all locators are valid. False otherwise.
     */
    bool isValid() const
    {
        for (LocatorListConstIterator it = this->begin(); it != this->end(); ++it)
        {
            if (!it->is_valid())
            {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief exchange the content of the container.
     *
     * @param locatorList container to exchange the contents with.
     */
    void swap(
        LocatorList &locatorList)
    {
        this->m_locators.swap(locatorList.m_locators);
    }

private:
    std::vector<Locator> m_locators;
};


} // namespace transport

#endif // ! DDS_CORE_CORE_HPP_
