/*
	Author: Pontus Laestadius
	Since: 2018-03-23
    Last updated: 2018-03-27

	Synopsis:
	Reads and writes to the i2c bus to communicate with the connected sensors.

    This is derived from the original source:
    https://github.com/chalmers-revere/opendlv.scaledcars/blob/master/scripts/devantech_change_addr.cpp
    Resources used:
    http://www.robot-electronics.co.uk/i2c-tutorial
    https://www.robot-electronics.co.uk/htm/srf08tech.html

    Compiled using:
    g++ -pedantic -Wextra -std=c++11 -I /usr/include -c ultrasonic.cpp 
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

//#include "cluon-complete.hpp"

#define MAXSPEED 0.30
#define DELAY 70
#define FRONTSENSOR 0x71

const char filename[11] = "/dev/i2c-1";     //i2c bus.
// read the SRF08 datasheet for specifications.
const uint8_t buffer[2] = {0x00, 0x51};     // What to write to i2c, Command register, command.

uint8_t read_ultrasonic(unsigned long device_addr) {
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

    // Write to the command register specify the command.
    if (write(file, buffer, 2) != 2) {
        throw std::domain_error("Failed to send bytes to the the ultrasonic.");
    }

    // Sleep while the driver reads from the ultrasonic. Takes about 65mS with default gain.
    std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    uint8_t length = 3; // First usable echo reading is found at position 3.
    uint8_t *readbuffer = new uint8_t[length];

    // If we are able to read amount of bytes we requested.
    if (read(file, readbuffer, length) != length) {
        throw std::ios_base::failure("Failed to read from the i2c bus.");
    }

    uint8_t reading = readbuffer[length -1];
    delete[] readbuffer; // Clear the memory pointer, otherwise memory leak.
    return reading;
}

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
int main () {
    const uint8_t multiplier = 80;
    float current_speed = 0.25; // Example speed the vehicle is moving. between -1 and 1.
    uint8_t front = 0;
    uint8_t previous = 0;

    try {
        while (1) {
            front = read_ultrasonic(FRONTSENSOR);

            // TODO Sent it to the od4 session. 
            if (obstacle_check(front, multiplier/2, current_speed)) {

                if (previous != 1) {
                    std::cout << "obstacle very close" << std::endl;
                    previous = 1;
                }
            } else if (obstacle_check(front, multiplier, current_speed)) {

                if (previous != 2) {
                    std::cout << "obstacle detected" << std::endl;
                    previous = 2;
                }
            } else {
                
                if (previous != 3) {
                    std::cout << "no obstacle detected" << std::endl;
                    previous = 3;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(DELAY/2));
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}
