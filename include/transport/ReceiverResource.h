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

#ifndef TRANSPORT_RECEIVER_RESOURCE_H_
#define TRANSPORT_RECEIVER_RESOURCE_H_

#include <functional>


namespace transport
{
/**
 * RAII object that encapsulates the Receive operation over one channel in an unknown transport.
 * A Receiver resource is always univocally associated to a transport channel; the
 * act of constructing a Receiver Resource opens the channel and its destruction
 * closes it.
 * @ingroup NETWORK_MODULE
 */
class ReceiverResource
{
    //! Only TransportFactory is ever allowed to construct a ReceiverResource from scratch.
    //! In doing so, it guarantees the transport and channel are in a valid state for
    //! this resource to exist.
    friend class TransportFactory;

public:
    ReceiverResource(const Locator &locator,
                        uint32_t max_recv_buffer_size)
        : locator_(locator)
        , max_message_size_(max_recv_buffer_size)
        , recv_callback_(nullptr)
        , locator_check_callback_(nullptr)

    {
    }

    virtual ~ReceiverResource()
    {
        locator_check_callback_ = nullptr;
        recv_callback_ = nullptr;
    }

public:
    /**
     * Reports whether this resource supports the given local locator (i.e., said locator
     * maps to the transport channel managed by this resource).
     */
    bool support_locator(
        const Locator &localLocator)
    {
        if (locator_check_callback_)
        {
            return locator_check_callback_(localLocator);
        }
        return false;
    }

    /**
     * Register a MessageReceiver object to be called upon reception of data.
     * @param receiver The message receiver to register.
     */
    virtual void register_receiver(
        const std::function<void(const unsigned char* data,
                                    const uint32_t size,
                                    const Locator& local_locator,
                                    const Locator& remote_locator)> &callback) = 0;

    inline uint32_t max_message_size() const
    {
        return max_message_size_;
    }

    inline Locator locator() const
    {
        return locator_;
    }

protected:
    ReceiverResource() = delete;
    ReceiverResource(
        const ReceiverResource &) = delete;
    ReceiverResource &operator=(
        const ReceiverResource &) = delete;

    Locator locator_;
    uint32_t max_message_size_;
    std::function<void(const unsigned char* data,
                            const uint32_t size,
                            const Locator& local_locator,
                            const Locator& remote_locator)> recv_callback_;
    std::function<bool(const Locator &)> locator_check_callback_;
};

} // namespace transport

#endif /* TRANSPORT_RECEIVER_RESOURCE_H_ */
