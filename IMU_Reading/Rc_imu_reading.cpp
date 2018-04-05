/*
* MIT License
*
* Copyright (c) 2016 Strawson Design
*
* Modified by Elaine Qvarnström.
*
*/


#include <iostream>

int main() {
    rc_imu_data_t data; // struct to hold new data

    // initialize hardware
    if(rc_initialize()){
        fprintf(stderr, "ERROR: failed to run rc_initialize(), are you root?\n");
        return -1;
    }

    rc_imu_config_t conf = rc_default_imu_config();

    if(rc_initialize_imu(&data, conf)){
        fprintf(stderr, "rc_initialize_imu_failed\n");
        return -1;
    }

    printf("Accel XYZ(m/s²) |");
    printf("Gyro XYZ(deg/s) |");
    printf("\n");

    while(rc_get_state() != EXITING){
        printf("\r");

        // print accel data
        if(rc_read_accel_data(&data) < 0){
            printf("read accel data failed\n");

        }
        printf("%6.2f %6.2f %6.2f |", data.accel[0], data.accel[1], data.accel[2]);

        // print gyro data
        if(rc_read_gyro_data(&data) < 0){
            printf("read gyro data failed\n");
        }

        printf("%6.1f %6.1f %6.1f |", data.gyro[0], data.gyro[1], data.gyro[2]);

        fflush(stdout);
        rc_usleep(100000);

    }

    rc_power_off_imu();
    rc_cleanup();

    return 0;
}
