#include <chrono>
#include <iostream> 

#include "cluon/UDPSender.hpp"
#include "cluon/UDPReceiver.hpp"
int main(int argc, char **argv) {
	std::cout << "Let's test this" << std::endl;

    
    cluon::UDPSender sender{"225.0.0.111", 1236};
    sender.send("Sending!");
    
    
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(5s);

    cluon::UDPReceiver receiver("225.0.0.111", 1236,
        [](std::string &&data, std::string &&/*from*/,
           std::chrono::system_clock::time_point &&/*timepoint*/) noexcept {
        std::cout << "Received " << data.size() << " bytes." << std::endl;
    });

    using namespace std::literals::chrono_literals;
    while (receiver.isRunning()) {
        std::this_thread::sleep_for(1s);
    }    
    
  	return 0;
}

