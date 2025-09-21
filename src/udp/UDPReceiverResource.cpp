// Copyright 2018 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

#include "UDPReceiverResource.h"
#include "UDPTransportInterface.h"
#include "IPLocator.h"

namespace transport
{

UDPReceiverResource::UDPReceiverResource(
    UDPTransportInterface *transport,
    std::shared_ptr<uvw::udp_handle> socket,
    uint32_t maxMsgSize,
    const Locator &locator)
    : ReceiverResource(locator, maxMsgSize)
    , alive_(true)
    , socket_(socket)
    , callback_(nullptr)
    , transport_(transport)
{
    socket_->on<uvw::udp_data_event>([&](const uvw::udp_data_event &event, uvw::udp_handle &){
        if(callback_)
        {
            SocketLocator remote_locator({event.sender.ip, event.sender.port});
            callback_(reinterpret_cast<unsigned char*>(event.data.get()),
                    event.length,
                    {IPLocator::getIpByLocatorv4(locator), locator.port},
                    remote_locator);
        }
    });

    locator_check_callback_ = [&](const Locator &locatorToCheck) -> bool
    {
        return locator.kind == locatorToCheck.kind && transport_->do_input_locators_match(locator, locatorToCheck);
    };
}

UDPReceiverResource::~UDPReceiverResource()
{
    if(socket_)
    {
        socket_->close();
    }
}

void UDPReceiverResource::register_receiver(
    const Callback &callback)
{
    callback_ = callback;
}

} // namespace transport
