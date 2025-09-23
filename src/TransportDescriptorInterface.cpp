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

#include <transport/TransportDescriptorInterface.h>
#include "udp/UDPv4Transport.h"
#include "udp/UDPv6Transport.h"


namespace transport
{

TransportInterface *TransportDescriptor<UDPv4Descriptor>::create_transport() const
{
    return new UDPv4Transport(
        std::make_shared<TransportDescriptor<UDPv4Descriptor>>(*this));
}

TransportInterface *TransportDescriptor<UDPv6Descriptor>::create_transport() const
{
    return new UDPv6Transport(
        std::make_shared<TransportDescriptor<UDPv6Descriptor>>(*this));
}


} // namespace transport