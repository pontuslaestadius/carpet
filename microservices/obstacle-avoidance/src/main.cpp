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

#define MAXSPEED 0.40
#define DELAY 70

uint8_t read_ultrasonic(unsigned long device_addr) {

    // Input validation, to make sure the addr is within the scope of the i2c.
    if (device_addr < 0x03 || device_addr > 0x77) {
        throw std::invalid_argument("Device address out of bounds.");
    }

    uint8_t buffer[2];                          // What to write to i2c.
    buffer[0] = 0x00;                           // Command register.
    buffer[1] = 0x51;                           // Command, read the SRF08 datasheet for specifications.
    char filename[11] = "/dev/i2c-1";           //i2c bus.

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
    uint8_t length = 3;
    uint8_t *readbuffer = new uint8_t[length];

    // If we are able to read amount of bytes we requested.
    if (read(file, readbuffer, length) != length) {
        throw std::ios_base::failure("Failed to read from the i2c bus.");
    }

    return readbuffer[length -1];
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
    float current_speed = 0.25; // Example speed the vehicle is moving. between -1 and 1.
    uint8_t multiplier = 60;
    uint8_t front = 0;

    while (1) {
        front = read_ultrasonic(0x71); // Front sensor.
        printf("%d > %d \n", front, obstacle_check(front, multiplier, 0.25));

        // TODO Sent it to the od4 session. 
        if (obstacle_check(front, multiplier/2, current_speed)) {
            printf("obstacle very close!\n");
        } else if (obstacle_check(front, multiplier, current_speed)) {
            printf("obstacle detected.\n");
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY/2));
    }

    return 0;
}
