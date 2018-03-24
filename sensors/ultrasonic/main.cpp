// Sourced from: https://stackoverflow.com/questions/22611675/i-am-trying-to-use-i2c-on-the-beagle-bone-black-with-c-but-i-keep-getting-0x00

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>

#include <cstdint>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

using namespace std;

int main(int /*argc*/, char** /*argv*/){
    int X_orientation=0;
    char buffer1[256];
    string i2cDeviceDriver="/dev/i2c-1";
    int fileHandler;

    if((fileHandler=open(i2cDeviceDriver.c_str(),O_RDWR))<0){
        perror("Failed To Open i2c-1 Bus");
        exit(1);
    }
    if(ioctl(fileHandler,I2C_SLAVE,0x71)<0){ // This should reflect the front ultra sonic sensor.
        perror("Failed to acquire i2c bus access and talk to slave");
        exit(1);
    }

    char buffer[1]={0x0D};
    if(write(fileHandler,buffer,1)!=1){                 
        perror("Failed to write byte to sensor");
        exit(1);
    }

    if(read(fileHandler,buffer1,1)!=1){                 
        perror("Failed to read byte from sensor");
        exit(1);
    }
    printf("Contents of sensor is 0x%02X\n",buffer1[0]);
}
