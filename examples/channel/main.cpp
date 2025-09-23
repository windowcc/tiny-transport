
#include <signal.h>
#include <string.h>
#include <iostream>
#include <atomic>
#include <thread>
#include <uvw.hpp>
#include <IPLocator.h>
#include <transport.h>

using namespace transport;

namespace {

std::shared_ptr<TransportFactory> factory = nullptr;

transport::SendResourceList send_resource_list_;
transport::ReceiverResourceList recv_resource_list_;

void do_send()
{
    Locator local_locator,remote_locator;
    IPLocator::createLocator(LOCATOR_KIND_UDPv4,"0.0.0.0",30490,local_locator);
    LocatorList locators;
    factory->default_metatraffic_multicast_locators(locators,30490);

    IPLocator::createLocator(LOCATOR_KIND_UDPv4,"192.168.198.11",8888,remote_locator);
    factory->build_send_resources(send_resource_list_,*(locators.begin()));

    // LocatorList locators;
    // locators.push_back(remote_locator);
    octet *data = new octet[12];
    std::string str = "Hello,World!!!";
    memcpy(data,str.c_str(),str.length());
    for (auto it : send_resource_list_)
    {
        it->send(data,12,locators,std::chrono::steady_clock::time_point());
    }
}

void do_recv()
{
    // Locator local_locator;
    // IPLocator::createLocator(LOCATOR_KIND_UDPv4,"192.168.198.11",8888,local_locator);

    LocatorList locators;
    factory->default_metatraffic_multicast_locators(locators,30490);

    factory->build_receiver_resources(recv_resource_list_, *(locators.begin()) ,65535);

    for(auto it : recv_resource_list_)
    {
        it->register_receiver([](const unsigned char* data,
                                    const uint32_t size,
                                    const Locator& local_locator,
                                    const Locator& remote_locator)
        {
            std::cout << "receive message : " <<
                std::string(reinterpret_cast<const char*>(data),size) << std::endl;
        });
    }
}

} // namespace

int main(int argc, char ** argv)
{
    auto exit = [](int)
    {
        uvw::loop::get_default()->stop();
        uvw::loop::get_default()->close();
    };
    ::signal(SIGINT  , exit);
    ::signal(SIGABRT , exit);
    ::signal(SIGSEGV , exit);
    ::signal(SIGTERM , exit);
#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || \
    defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || \
    defined(WINCE) || defined(_WIN32_WCE)
    ::signal(SIGBREAK, exit);
#else
    ::signal(SIGHUP  , exit);
#endif

    TransportDescriptor<UDPv4Descriptor> desc;
    if(!factory)
    {
        factory = std::make_shared<TransportFactory>();
    }
    factory->register_transport(&desc);
    
    std::string mode {argv[1]};
    if (mode == "send")
    {
        do_send();
    }
    else if (mode == "recv")
    {
        do_recv();
    }
    uvw::loop::get_default()->run();
    return 0;
}

