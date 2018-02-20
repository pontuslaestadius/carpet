#include <chrono>
#include <iostream> 

#include "cluon/UDPSender.hpp"
#include "cluon/UDPReceiver.hpp"
int main(int argc, char **argv) {
	std::cout << "Let's test this" << std::endl;

    
    cluon::UDPSender sender{"127.0.0.1", 1234};
    sender.send("Sending!");

    cluon::UDPReceiver receiver("0.0.0.0", 1235,
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

