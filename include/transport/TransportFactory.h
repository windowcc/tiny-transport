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

#ifndef TRANSPORT_NETWORK_FACTORY_H_
#define TRANSPORT_NETWORK_FACTORY_H_

#include <memory>
#include <vector>
#include <transport/SocketTransportDescriptor.h>



namespace transport
{

class ParticipantAttributes;

/**
 * Provides the TRANSPORT library with abstract resources, which
 * in turn manage the SEND and RECEIVE operations over some transport.
 * Once a transport is registered, it becomes invisible to the library
 * and is abstracted away for good.
 * @ingroup NETWORK_MODULE
 */
class TransportFactory
{
public:
    TransportFactory();

    /**
     * Allow registration of a transport dynamically.
     *
     * @param descriptor Structure that defines all initial configuration for a given transport.
     * @param properties Optional policy to specify additional parameters for the created transport.
     */
    bool register_transport(
        const TransportDescriptorInterface *descriptor,
        std::shared_ptr<Worker> work_manager = nullptr);

    /**
     * Walk over the list of transports, opening every possible channel that can send through
     * the given locator and returning a vector of Sender Resources associated with it.
     * @param locator Locator through which to send.
     */
    bool build_send_resources(
        SendResourceList &,
        const Locator &locator);

    /**
     * Walk over the list of transports, opening every possible channel that we can listen to
     * from the given locator, and returns a vector of Receiver Resources for this goal.
     * @param local Locator from which to listen.
     * @param receiver_resources_list List that will be filled with the created ReceiverResources.
     * @param receiver_max_message_size Max message size allowed by the message receiver.
     */
    bool build_receiver_resources(
        Locator &local,
        std::vector<std::shared_ptr<ReceiverResource>> &receiver_resources_list,
        uint32_t receiver_max_message_size);

    void normalize_locators(
        LocatorList &locators);

    size_t register_transport_szie() const;

    uint32_t get_max_message_size_between_transports() const
    {
        return max_message_size_between_transports_;
    }

    uint32_t get_min_send_buffer_size()
    {
        return min_send_buffer_size_;
    }

    /**
     * Add locators to the metatraffic multicast list.
     * */
    bool getDefaultMetatrafficMulticastLocators(
        LocatorList &locators,
        uint32_t metatraffic_multicast_port) const;

    /**
     * Add locators to the metatraffic unicast list.
     * */
    bool getDefaultMetatrafficUnicastLocators(
        LocatorList &locators,
        uint32_t metatraffic_unicast_port) const;

    /**
     * Fill the locator with the metatraffic multicast configuration.
     * */
    bool fillMetatrafficMulticastLocator(
        Locator &locator,
        uint32_t metatraffic_multicast_port) const;

    /**
     * Fill the locator with the metatraffic unicast configuration.
     * */
    bool fillMetatrafficUnicastLocator(
        Locator &locator,
        uint32_t metatraffic_unicast_port) const;
    /**
     * shutdown method to close the connections of the transports.
     */
    void shutdown();

    /**
     * Re-scan network interfaces
     */
    void update_network_interfaces();

private:
    std::vector<std::unique_ptr<TransportInterface>> registered_transports_;

    uint32_t max_message_size_between_transports_;

    uint32_t min_send_buffer_size_;
};

} // namespace transport

#endif /* TRANSPORT_NETWORK_FACTORY_H_ */
