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
// 

char filename[11] = "/dev/i2c-1";


int main () {
    read_ultrasonic(0x71); // Front sensor.
    read_ultrasonic(0x70); // Back sensor. (hypothetically.)
    return 0; // Front sensor.
}

int read_ultrasonic(unsigned long device_addr) {
    // Command register, command.
    uint8_t buffer[2] = [0x00, 0x51];
    int file = open(filename, O_RDWR);

    // Verify access to i2c bus.
    if (open(file, O_RDWR) < 0) {
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

    // Sleep while the driver reads from the ultrasonic. Takes about 65mS.
    std::this_thread::sleep_for(std::chrono::milliseconds(80));

    // Read from the sensor buffer here.

    return 0;
}
