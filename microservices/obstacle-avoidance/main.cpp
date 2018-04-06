/*
    Author: Pontus Laestadius
    Since: 2018-03-23
    Last updated: 2018-04-05
    Synopsis:
    Reads and writes to the i2c bus to communicate with the connected sensors.
    This is derived from the original source:
    https://github.com/chalmers-revere/opendlv.scaledcars/blob/master/scripts/devantech_change_addr.cpp
    Resources used:
    http://www.robot-electronics.co.uk/i2c-tutorial
    https://www.robot-electronics.co.uk/htm/srf08tech.html
    The od4 part is gather from:
    https://github.com/chalmers-revere/opendlv-device-ultrasonic-srf08/blob/master/src/opendlv-device-ultrasonic-srf08.cpp
*/


#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <assert.h>
#include <chrono>
#include <thread>
#include <vector>

#include "cluon-complete.hpp"
#include "messages.hpp"

#define MAXSPEED 0.30
#define FRONTSENSOR 0x71
#define LENGTH 3 // First usable echo reading is found at position 3.
#define MID 1039

const char filename[11] = "/dev/i2c-1";     //i2c bus.
// read the SRF08 datasheet for specifications.
const uint8_t buffer[2] = {0x00, 0x51};     // What to write to i2c, Command register, command.

void read_ultrasonic(unsigned long device_addr, uint16_t const CID, uint16_t const FREQ) {
    // Input validation, to make sure the addr is within the scope of the i2c.
    if (device_addr < 0x03 || device_addr > 0x77) {
        throw std::invalid_argument("Device address out of bounds.");
    }
    
    // Verify access to i2c bus.
    int file = open(filename, O_RDWR);
    if (file < 0) {
        throw std::domain_error("Failed to open i2c bus.");
    }

    // Verify communication over i2c bus.
    if (ioctl(file, I2C_SLAVE, device_addr) < 0) {
        throw std::domain_error("Failed to acquire bus access.");
    }

    cluon::OD4Session od4{CID};

    if(od4.isRunning() == 0) {
       throw std::domain_error("no od4 running, are you connected to the interwebs?");
    }

    // For each reading.
    while(1) {

        // Write to the command register specify the command.
        if (write(file, buffer, 2) != 2) {
            throw std::domain_error("Failed to send bytes to the the ultrasonic.");
        }

        // Sleep while the driver reads from the ultrasonic. Takes about 65mS with default gain.
        std::this_thread::sleep_for(std::chrono::milliseconds(68));
        uint8_t *readbuffer = new uint8_t[LENGTH];

        // If we are able to read amount of bytes we requested.
        int count = read(file, readbuffer, LENGTH);
        if (count != LENGTH) {
            std::cout << "Error OA: got: " << count << " expected: " << LENGTH << std::endl;
            throw std::ios_base::failure("Failed to read from the i2c bus.");
        }

        uint8_t reading = readbuffer[LENGTH -1];
        delete[] readbuffer; // Clear the memory pointer, otherwise memory leak.

        // Send a message over od4.
        opendlv::proxy::DistanceReading distanceReading;
        distanceReading.distance((float)reading);
        cluon::data::TimeStamp sampleTime;
        od4.send(distanceReading, sampleTime, MID);

        std::this_thread::sleep_for(std::chrono::milliseconds(FREQ));
    }
}


// TODO Replace current speed with acceleration from the IMU.
bool obstacle_check(uint8_t distance, uint8_t multiplier, float current_speed) {
    // Negative values are converted to their positive counterpart.
    current_speed = current_speed < 0 ? -current_speed:current_speed;
    // If the current speed exceeds the maximum speed throw exception.
    if (current_speed > MAXSPEED) {
        throw std::invalid_argument("Current speed is out of bounds.");
    }
    // Algorithm for evaluating distance 
    return distance < (multiplier * current_speed);
}


/*
    This is an example of how the methods should be called.
*/
int main (int32_t argc, char **argv) {
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    uint16_t const CID = std::stoi(commandlineArguments["cid"]);
    uint16_t const FREQ = std::stoi(commandlineArguments["freq"]);

    try {
        read_ultrasonic(FRONTSENSOR, CID, FREQ);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
