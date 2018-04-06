/*
* MIT License
*
* Copyright (c) 2016 Strawson Design
*
* Modified by Elaine Qvarnström.
*
*/

#include "Rc_imu_reading.hpp"
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

Rc_imu_reading::Rc_imu_reading(){

    carpet = std:: make_shared<cluon::OD4Session>(CARPET_CHANNEL,
    [this](cluon::data::Envelope &&envelope) noexcept{
        switch (envelope.dataType()){
            case TRAVELED_DISTANCE:
                // To be implemented
                break;
            case ACCELERATION_READING:
                // To be implemented
                break;
            case GYRO_READING:
                // To be implemented
                break;
            default:
                // To be implemented
                break;
        }
    });
}

/**
 * Sends the acceleration to the OD4Session used for internal communication.
 * Acceleration is in m/s².
 *
 * @param accelX
 * @param accelY
 * @param accelZ
 */

void Rc_imu_reading::sendAccelReading(float accelX, float accelY, float accelZ){
    AccelerationReading msg;
    msg.accelerationX(accelX);
    msg.accelerationY(accelY);
    msg.accelerationZ(accelZ);
    carpet->send(msg);
}

/**
 * Sends the gyro readings to the OD4Session used for internal communication.
 * Gyro readings are in deg/s.
 * @param gyroX
 * @param gyroY
 * @param gyroZ
 */
void Rc_imu_reading::sendGyroReading(float gyroX, float gyroY, float gyroZ){
    GyroReading msg;
    msg.gyroX(gyroX);
    msg.gyroY(gyroY);
    msg.gyroZ(gyroZ);
    carpet->send(msg);

}

float Rc_imu_reading::calculateTraveledDistance(float accelX, float accelY, float accelZ){
    float traveledDistance = 0;
    return traveledDistance;
}
/**
 * Sends the traveled distance to the OD4Session used for internal communication.
 * Unit is yet to be decided.
 * @param traveledDistance
 */
void Rc_imu_reading::sendTraveledDistance(float traveledDistance){
    TraveledDistance msg;
    msg.distanceTraveled(traveledDistance);
    carpet->send(msg);
}