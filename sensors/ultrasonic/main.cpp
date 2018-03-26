/*
	Author: Pontus Laestadius
	Since: 2018-03-23

	Synopsis:
	Reads and writes to the i2c bus to communicate with the connected sensors.
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

// This is derived from the original source:
// https://github.com/chalmers-revere/opendlv.scaledcars/blob/master/scripts/devantech_change_addr.cpp
// Resources used:
// http://www.robot-electronics.co.uk/i2c-tutorial

uint8_t read_ultrasonic(unsigned long device_addr) {
    char *end;
    uint8_t buffer[2];
    // Command register.
    buffer[0] = 0x00;
    // Command, read the SRF08 datasheet for specifications.
    buffer[1] = 0x51;
    //i2c bus.
    char filename[11] = "/dev/i2c-1";

    // Verify access to i2c bus.
    int file = open(filename, O_RDWR);
    if (file < 0) {
        std::cout << "Failed to open the i2c bus: " << filename << "." << std::endl;
        return -1;
    }

    // Verify communication over i2c bus.
    if (ioctl(file, I2C_SLAVE, device_addr) < 0) {
        std::cout << "Failed to acquire bus access or talk to device. Addr: " << device_addr << std::endl;
        return -1;
    }

    // Write to the command register specify the command.
    if (write(file, buffer, 2) != 2) {
        std::cout << "Failed to send " << buffer[1] << " to device." << std::endl;
        return -1;
    }

    // Sleep while the driver reads from the ultrasonic. Takes about 65mS with default gain.
    std::this_thread::sleep_for(std::chrono::milliseconds(80));
    uint8_t readbuffer[10];
    uint8_t length = 10;

    // If we are able to read amount of bytes we requested.
    if (read(file, readbuffer, length) != length) {
        printf("Failed to read the requested amount of bytes from the i2c bus.\n");
        //return -1; // add back in once this works.
    }

    // Example print.
    for (int i = 0; i < length; ++i){
        std::cout << buffer[i];
        if (length % 6 == 0) {
            std::cout << std::endl;
        }
        std::cout << ", ";
    }


    return readbuffer[0];
}

int main () {
    read_ultrasonic(0x71); // Front sensor.
    read_ultrasonic(0x70); // Back sensor. (hypothetically.)
    return 0; // Front sensor.
}