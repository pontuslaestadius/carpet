#ifndef RC_IMU
#define RC_IMU

#include <unistd.h>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "Messages.hpp"
#include <iostream>

static const int CARPET_CHANNEL = 60;

static const int TRAVELED_DISTANCE = 2002;
static const int ACCELERATION_READING = 2003;
static const int GYRO_READING = 2004;

class Rc_imu {
private:

    Rc_imu();

    void sendAccelReading(float accelX, float accelY, float accelZ;
    void sendGyroReading(float gyroX, float gyroY, float gyroZ);
    float calculateTraveledDistance(float accelX, float accelY, float accelZ);
    void sendTraveledDistance(float traveledDistance);

    std::shared_ptr<cluon::OD4Session>  carpet;

};

#endif //RC_IMU