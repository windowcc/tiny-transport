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

#ifndef TRANSPORT_SENDER_RESOURCE_H_
#define TRANSPORT_SENDER_RESOURCE_H_

#include <functional>
#include <vector>
#include <chrono>
#include <transport/type.h>

namespace transport
{

using SendCallback = std::function<bool(
    const octet *,
    uint32_t,
    const LocatorList &locators,
    const std::chrono::steady_clock::time_point &)>;

/**
 * RAII object that encapsulates the Send operation over one chanel in an unknown transport.
 * A Sender resource is always univocally associated to a transport channel; the
 * act of constructing a Sender Resource opens the channel and its destruction
 * closes it.
 * @ingroup NETWORK_MODULE
 */
class SenderResource
{
public:
    /**
     * Sends to a destination locator, through the channel managed by this resource.
     * @param data Raw data slice to be sent.
     * @param dataLength Length of the data to be sent. Will be used as a boundary for
     * the previous parameter.
     * @param destination_locators_begin destination endpoint Locators iterator begin.
     * @param destination_locators_end destination endpoint Locators iterator end.
     * @param max_blocking_time_point If transport supports it then it will use it as maximum blocking time.
     * @return Success of the send operation.
     */
    bool send(
        const octet *data,
        uint32_t dataLength,
        const LocatorList &locators,
        const std::chrono::steady_clock::time_point &max_blocking_time_point)
    {
        bool returned_value = true;

        if (send_lambda_)
        {
            returned_value = send_lambda_(data, dataLength, locators, max_blocking_time_point);
        }

        return returned_value;
    }

    virtual Locator locator() const = 0;

    virtual ~SenderResource() = default;

protected:
    SenderResource() = default;

    SendCallback send_lambda_;

private:
    SenderResource(
        const SenderResource &) = delete;
    SenderResource &operator=(
        const SenderResource &) = delete;
};

} // namespace transport

#endif /* TRANSPORT_SENDER_RESOURCE_H_ */
