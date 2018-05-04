/**
 * @file rc_test_dmp.c
 * @example    rc_test_dmp
 *
 * @brief      serves as an example of how to use the MPU in DMP mode
 *
 *
 *
 * @author     James Strawson
 * @date       1/29/2018
 */


#include <stdio.h>
#include <getopt.h>
#include <signal.h>
#include <stdlib.h> // for atoi() and exit()
#include "rc/mpu.h"
#include "rc/time.h"

#include "cluon-complete.hpp"
#include "messages.hpp"

// bus for Robotics Cape and BeagleboneBlue is 2, gpio int pin  is 117
// change these for your platform
#define I2C_BUS 2
#define GPIO_INT_PIN 117
#define ACCEL_READING 2003

// Global Variables
int running;
int silent_mode = 0;
int show_accel = 0;
int show_gyro  = 0;
int enable_mag = 0;
int show_compass = 0;
int show_temp  = 0;
int show_quat  = 0;
int show_tb = 0;
int orientation_menu = 0;
rc_mpu_data_t data;

// local functions
//rc_mpu_orientation_t orientation_prompt();
void print_usage();
void print_data();
void print_header();


/**
 * Printed if some invalid argument was given, or -h option given.
 */
void print_usage()
{
	printf("\n Options\n");
	printf("-r {rate}	Set sample rate in HZ (default 100)\n");
	printf("		Sample rate must be a divisor of 200\n");
	printf("-m		Enable Magnetometer\n");
	printf("-b		Enable Reading Magnetometer before ISR (default after)\n");
	printf("-c		Show raw compass angle\n");
	printf("-a		Print Accelerometer Data\n");
	printf("-g		Print Gyro Data\n");
	printf("-p {prio}	Set Interrupt Priority and FIFO scheduling policy (requires root)\n");

	return;
}

/**
 * This is the IMU interrupt function.
 */
void print_data()
{

		printf("\r  %5.2f %5.2f %5.2f |",	data.accel[0],\
						data.accel[1],\
						data.accel[2]);
		/*printf(" %5.1f %5.1f %5.1f |",	data.gyro[0],\
						data.gyro[1],\
						data.gyro[2]);*/

	cluon::OD4Session od4(170,
																  [](cluon::data::Envelope &&envelope) noexcept {
																	  std::cout << "[OD4] ";
																	  switch (envelope.dataType()) {
																		  case ACCEL_READING: {
																			  AccelerationReading msg = cluon::extractMessage<AccelerationReading>(std::move(envelope));
																			  std::cout << "received 'AccelerationReading' XYZ (m/s2): "
																						<< msg.accelX() << ", "
																						<< msg.accelY() << ", "
																					    << msg.accelZ() << ". \n"<< std::endl;

																			  break;
																		  }
																		  default: std::cout << "¯\\_(ツ)_/¯" << std::endl;
																	  }
																  });
	if(od4.isRunning() == 0) {
		throw std::domain_error("no od4 running, are you connected to the interwebs?");
	}

	// Send a message over od4.
	AccelerationReading accelMsg;
	accelMsg.accelX((float)data.accel[0]);
	accelMsg.accelY((float)data.accel[1]);
	accelMsg.accelZ((float)data.accel[2]);
	cluon::data::TimeStamp sampleTime;
	od4.send(accelMsg, sampleTime, ACCEL_READING);

	fflush(stdout);
	return;
}

/**
 * Based on which data is marked to be printed, print the correct labels. this
 * is printed only once and the actual data is updated on the next line.
 */
void print_header(){
	printf(" ");

	printf(" Accel XYZ (m/s^2) |");
	/*printf("  Gyro XYZ (deg/s) |");*/

	printf("\n");
}

/**
 * @brief      interrupt handler to catch ctrl-c
 */
void signal_handler(__attribute__ ((unused)) int dummy)
{
	running=0;
	return;
}

/**
 * main() serves to parse user options, initialize the imu and interrupt
 * handler, and wait for the rc_get_state()==EXITING condition before exiting
 * cleanly. The imu_interrupt function print_data() is what actually prints new
 * imu data to the screen after being set with rc_mpu_set_dmp_callback().
 *
 * @param[in]  argc  The argc
 * @param      argv  The argv
 *
 * @return     0 on success -1 on failure
 */
int main(int argc, char *argv[])
{

	while(1) {

		int c, sample_rate, priority;

		// start with default config and modify based on options
		rc_mpu_config_t conf = rc_mpu_default_config();
		conf.i2c_bus = I2C_BUS;
		conf.gpio_interrupt_pin = GPIO_INT_PIN;

		// parse arguments
		/*opterr = 0;
        while ((c=getopt(argc, argv, "sr:mbagrqtcp:hwo"))!=-1 && argc>1){
            switch (c){
            case 'r': // sample rate option
                sample_rate = atoi(optarg);
                if(sample_rate>200 || sample_rate<4){
                    printf("sample_rate must be between 4 & 200");
                    return -1;
                }
                conf.dmp_sample_rate = sample_rate;
                break;
            case 'p': // priority option
                priority = atoi(optarg);
                conf.dmp_interrupt_priority = priority;
                conf.dmp_interrupt_sched_policy = SCHED_FIFO;
                break;
            case 'a': // show accelerometer option
                show_accel = 1;
                conf.dmp_fetch_accel_gyro=1;
                break;
            case 'g': // show gyro option
                show_gyro = 1;
                conf.dmp_fetch_accel_gyro=1;
                break;
            case 'w': // print warnings
                conf.show_warnings=1;
                break;
            default:
                return -1;
            }
        }*/

		conf.dmp_fetch_accel_gyro = 1;

		// set signal handler so the loop can exit cleanly
		signal(SIGINT, signal_handler);
		running = 1;

		// now set up the imu for dmp interrupt operation
		if (rc_mpu_initialize_dmp(&data, conf)) {
			printf("rc_mpu_initialize_failed\n");
			return -1;
		}
		// write labels for what data will be printed and associate the interrupt
		// function to print data immediately after the header.
		print_header();
		rc_mpu_set_dmp_callback(&print_data);
		//now just wait, print_data() will be called by the interrupt
		while (running) rc_usleep(100000);

		// shut things down
		rc_mpu_power_off();
		printf("\n");
		fflush(stdout);
	}
	return 0;
}

