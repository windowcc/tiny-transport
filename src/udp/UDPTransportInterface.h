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

#ifndef TRANSPORT_UDP_TRANSPORT_INTERFACE_H_
#define TRANSPORT_UDP_TRANSPORT_INTERFACE_H_

#include <vector>
#include <memory>
#include <atomic>
#include <uvw.hpp>
#include "IPFinder.h"
#include "UDPReceiverResource.h"
#include <transport/TransportInterface.h>

namespace transport
{

class UDPTransportInterface : public TransportInterface
{
    friend class UDPSenderResource;

public:
    virtual ~UDPTransportInterface() override;

    //! Reports whether Locators correspond to the same port.
    bool do_input_locators_match(
        const Locator &,
        const Locator &) const override;

    bool init() override;

    virtual void shutdown()
    {

    }

    virtual int32_t kind() const
    {
        return transport_kind_;
    }

    //! Checks for TCP kinds.
    bool is_locator_supported(
        const Locator &) const override;

    //! Opens a socket on the given address and port (as long as they are white listed).
    bool open_output_channel(
        SendResourceList &sender_resource_list,
        const Locator &) override;

    /**
     * Converts a given remote locator (that is, a locator referring to a remote
     * destination) to the main local locator whose channel can write to that
     * destination. In this case it will return a 0.0.0.0 address on that port.
     */
    Locator remote_to_main_local(
        const Locator &) const override;

    /**
     * Blocking Send through the specified channel. In both modes, using a localLocator of 0.0.0.0 will
     * send through all whitelisted interfaces provided the channel is open.
     * @param send_buffer Slice into the raw data to send.
     * @param send_buffer_size Size of the raw data. It will be used as a bounds check for the previous argument.
     * It must not exceed the send_buffer_size fed to this class during construction.
     * @param socket channel we're sending from.
     * @param destination_locators_begin pointer to destination locators iterator begin, the iterator can be advanced inside this fuction
     * so should not be reuse.
     * @param destination_locators_end pointer to destination locators iterator end, the iterator can be advanced inside this fuction
     * so should not be reuse.
     * @param only_multicast_purpose multicast network interface
     * @param whitelisted network interface included in the user whitelist
     * @param max_blocking_time_point maximum blocking time.
     */
    virtual bool send(
        const octet *send_buffer,
        uint32_t send_buffer_size,
        std::shared_ptr<uvw::udp_handle> socket,
        const LocatorList &locators,
        bool only_multicast_purpose,
        bool whitelisted,
        const std::chrono::steady_clock::time_point &max_blocking_time_point);

    /**
     * Performs the locator selection algorithm for this transport.
     *
     * It basically consists of the following steps
     *   - selector.transport_starts is called
     *   - transport handles the selection state of each locator
     *   - if a locator from an entry is selected, selector.select is called for that entry
     *
     * In the case of UDP, multicast locators are selected when present in more than one entry,
     * otherwise unicast locators are selected.
     *
     * @param [in, out] selector Locator selector.
     */

    bool fill_metatraffic_multicast_locator(
        Locator &locator,
        uint32_t metatraffic_multicast_port) const override;

    bool fill_metatraffic_unicast_locator(
        Locator &locator,
        uint32_t metatraffic_unicast_port) const override;

    bool fill_unicast_locator(
        Locator &locator,
        uint32_t well_known_port) const override;

    void update_network_interfaces() override;

    std::atomic_bool rescan_interfaces_ = {true};

protected:
    friend class UDPReceiverResource;

    int32_t transport_kind_;

    // For UDPv6, the notion of channel corresponds to a port + direction tuple.
    std::vector<IPFinder::info_IP> currentInterfaces;

    uint32_t mSendBufferSize;
    uint32_t mReceiveBufferSize;

    UDPTransportInterface(
        int32_t transport_kind);
    // CBaseWorker *worker
    virtual bool compare_locator_ip(
        const Locator &lh,
        const Locator &rh) const = 0;
    virtual bool compare_locator_ip_and_port(
        const Locator &lh,
        const Locator &rh) const = 0;

    virtual void get_ips(
        std::vector<IPFinder::info_IP> &locNames,
        bool return_loopback = false) = 0;

    /**
     * Send a buffer to a destination
     */
    bool send(
        const octet *send_buffer,
        uint32_t send_buffer_size,
        std::shared_ptr<uvw::udp_handle> socket,
        const Locator &remote_locator,
        bool only_multicast_purpose,
        bool whitelisted,
        const std::chrono::microseconds &timeout);
};

} // namespace transport

#endif // TRANSPORT_UDP_TRANSPORT_INTERFACE_H_
