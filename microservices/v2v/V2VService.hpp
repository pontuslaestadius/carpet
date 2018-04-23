#ifndef V2V_PROTOCOL_DEMO_V2VSERVICE_H
#define V2V_PROTOCOL_DEMO_V2VSERVICE_H

#include <iomanip>
#include <unistd.h>
#include <sys/time.h>
#include <chrono>
#include <iostream>
#include <cstdint>
#include <queue>

#include "cluon/OD4Session.hpp"
#include "cluon/UDPSender.hpp"
#include "cluon/UDPReceiver.hpp"
#include "cluon/Envelope.hpp"

#include "Messages.hpp"

/** ADD YOUR CAR_IP AND GROUP_ID HERE:  *****************/

static const std::string YOUR_CAR_IP    = "10.132.91.239";
static const std::string YOUR_GROUP_ID  = "6";

/********************************************************/
/** DON'T CHANGE STUFF BELOW THIS LINE. *****************/
/********************************************************/

static const int BROADCAST_CHANNEL = 250;
static const int COMMANDER_LINK = 171; //V2V link to commander.
static const int DEFAULT_PORT = 50001;

static const int ANNOUNCE_PRESENCE = 1001;
static const int FOLLOW_REQUEST = 1002;
static const int FOLLOW_RESPONSE = 1003;
static const int STOP_FOLLOW = 1004;
static const int LEADER_STATUS = 2001;
static const int FOLLOWER_STATUS = 3001;
static const int IMU_READ = 0000; // Add real value.....
   
 //Keeps track of LeaderStatus values so that we can correctly send to follower.
    static float LDS_MOVE = 0;
    static float LDS_TURN = 0;
    static float LDS_DIST = 0;

//Follower Logic ....
static float LATEST_SPEED = 0;
static float LATEST_ANGLE = 0;
static float LATEST_DIST = 0;
static float LATEST_TIME = 0;

class V2VService {
public:
    std::map <std::string, std::string> presentCars;

    V2VService();
    
    int single_car;

    void announcePresence();
    void followRequest(std::string vehicleIp);
    void followResponse();
    void stopFollow(std::string vehicleIp);
    void leaderStatus(float speed, float steeringAngle, uint8_t distanceTraveled);
    void followerStatus();

private:
    std::string leaderIp;
    std::string followerIp;
  
    std::shared_ptr<cluon::OD4Session>  broadcast;
    std::shared_ptr<cluon::OD4Session>  fromCommander;
    std::shared_ptr<cluon::OD4Session>  toCommander;
    std::shared_ptr<cluon::UDPReceiver> incoming;
    std::shared_ptr<cluon::UDPSender>   toLeader;
    std::shared_ptr<cluon::UDPSender>   toFollower;

    static uint32_t getTime();
    static std::pair<int16_t, std::string> extract(std::string data);
    template <class T>
    static std::string encode(T msg);
    template <class T>
    static T decode(std::string data);
};

#endif //V2V_PROTOCOL_DEMO_V2VSERVICE_H
