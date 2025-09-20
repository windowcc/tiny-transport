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

#include <transport/TransportFactory.h>
#include <transport/TransportDescriptorInterface.h>
#include "IPLocator.h"

namespace transport
{

TransportFactory::TransportFactory()
    : max_message_size_between_transports_(std::numeric_limits<uint32_t>::max()),
        min_send_buffer_size_(std::numeric_limits<uint32_t>::max())
{
}

bool TransportFactory::build_send_resources(
    SendResourceList &sender_resource_list,
    const Locator &locator)
{
    bool returned_value = false;

    for (auto &transport : registered_transports_)
    {
        returned_value |= transport->OpenOutputChannel(sender_resource_list, locator);
    }

    return returned_value;
}

bool TransportFactory::build_receiver_resources(
    Locator &locator,
    std::vector<std::shared_ptr<ReceiverResource>> &receiver_resources_list,
    uint32_t receiver_max_message_size)
{
    bool returnedValue = false;
    for (auto &transport : registered_transports_)
    {
        uint32_t max_recv_buffer_size = (std::min)(
            transport->max_recv_buffer_size(), receiver_max_message_size);
        returnedValue |= transport->OpenInputChannel(receiver_resources_list, locator, max_recv_buffer_size);
    }
    return returnedValue;
}

bool TransportFactory::register_transport(
    const TransportDescriptorInterface *descriptor,
    std::shared_ptr<Worker> work_manager)
{
    bool wasRegistered = false;

    uint32_t minSendBufferSize = std::numeric_limits<uint32_t>::max();

    std::unique_ptr<TransportInterface> transport(descriptor->create_transport());

    if (transport)
    {
        if (transport->init())
        {
            minSendBufferSize = transport->get_configuration()->min_send_buffer_size();
            registered_transports_.emplace_back(std::move(transport));
            wasRegistered = true;
        }

        if (wasRegistered)
        {
            if (descriptor->max_message_size() < max_message_size_between_transports_)
            {
                max_message_size_between_transports_ = descriptor->max_message_size();
            }

            if (minSendBufferSize < min_send_buffer_size_)
            {
                min_send_buffer_size_ = minSendBufferSize;
            }
        }
    }

    return wasRegistered;
}

void TransportFactory::normalize_locators(
    LocatorList &locators)
{
    LocatorList normalizedLocators;

    std::for_each(locators.begin(), locators.end(), [&](Locator &loc)
                    {
            bool normalized = false;
            for (auto& transport : registered_transports_)
            {
                // Check if the locator is supported and filter unicast locators.
                if (transport->IsLocatorSupported(loc))
                {
                    // First found transport that supports it, this will normalize the locator.
                    normalizedLocators.push_back(transport->NormalizeLocator(loc));
                    normalized = true;
                }
            }

            if (!normalized)
            {
                normalizedLocators.push_back(loc);
            } });

    locators.swap(normalizedLocators);
}

size_t TransportFactory::register_transport_szie() const
{
    return registered_transports_.size();
}

bool TransportFactory::getDefaultMetatrafficMulticastLocators(
    LocatorList &locators,
    uint32_t metatraffic_multicast_port) const
{
    bool result = false;

    TransportInterface *shm_transport = nullptr;

    for (auto &transport : registered_transports_)
    {
        // For better fault-tolerance reasons, SHM multicast metatraffic is avoided if it is already provided
        // by another transport
        if (transport->kind() != LOCATOR_KIND_SHM)
        {
            result |= transport->getDefaultMetatrafficMulticastLocators(locators, metatraffic_multicast_port);
        }
        else
        {
            shm_transport = transport.get();
        }
    }

    if (locators.size() == 0 && shm_transport)
    {
        result |= shm_transport->getDefaultMetatrafficMulticastLocators(locators, metatraffic_multicast_port);
    }

    return result;
}

bool TransportFactory::fillMetatrafficMulticastLocator(
    Locator &locator,
    uint32_t metatraffic_multicast_port) const
{
    bool result = false;
    for (auto &transport : registered_transports_)
    {
        if (transport->IsLocatorSupported(locator))
        {
            result |= transport->fillMetatrafficMulticastLocator(locator, metatraffic_multicast_port);
        }
    }
    return result;
}

bool TransportFactory::getDefaultMetatrafficUnicastLocators(
    LocatorList &locators,
    uint32_t metatraffic_unicast_port) const
{
    bool result = false;
    for (auto &transport : registered_transports_)
    {
        result |= transport->getDefaultMetatrafficUnicastLocators(locators, metatraffic_unicast_port);
    }
    return result;
}

bool TransportFactory::fillMetatrafficUnicastLocator(
    Locator &locator,
    uint32_t metatraffic_unicast_port) const
{
    bool result = false;
    for (auto &transport : registered_transports_)
    {
        if (transport->IsLocatorSupported(locator))
        {
            result |= transport->fillMetatrafficUnicastLocator(locator, metatraffic_unicast_port);
        }
    }
    return result;
}

void TransportFactory::shutdown()
{
    for (auto &transport : registered_transports_)
    {
        transport->shutdown();
    }
}


void TransportFactory::update_network_interfaces()
{
    for (auto &transport : registered_transports_)
    {
        transport->update_network_interfaces();
    }
}

} // namespace transport
