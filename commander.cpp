
#include "commander.hpp"

int carController(char foo[]); // Should handle the commands received from Controller UI.
int openComm(); // Should open the com od4? should this be in main or seperated?



//Main starts here
int main(/*int argc*/, /*char** argv*/) {
	std::shared_ptr<commander> commanderService = std::make_shared<commander>();

	//Enter the chosen set of movements. TODO: Should loop through these values.
	//openComm();
	
}



int openComm(){
	//Well this was useless....... Update when hpp file is made.
/*
    cluon::OD4Session od4(211,[](cluon::data::Envelope &&envelope) noexcept {
        if (envelope.dataType() == opendlv::proxy::GroundSteeringReading::ID()) {
            opendlv::proxy::GroundSteeringReading receivedMsg = cluon::extractMessage<opendlv::proxy::GroundSteeringReading>(std::move(envelope));
            std::cout << "Sent a message for ground steering to " << receivedMsg.steeringAngle() << "." << std::endl;
        }
        else if (envelope.dataType() == opendlv::proxy::PedalPositionReading::ID()) {
            opendlv::proxy::PedalPositionReading receivedMsg = cluon::extractMessage<opendlv::proxy::PedalPositionReading>(std::move(envelope));
            std::cout << "Sent a message for pedal position to " << receivedMsg.percent() << "." << std::endl;
        }
    });

    if(od4.isRunning() == 0){
        std::cout << "ERROR: No od4 running!!!" << std::endl;
        return -1;
    }
*/
  return 0;
}


//Control car through an array of commands for moving and turning etc.
//Inspired by V2VProtocol - V2VService.cpp
commander::carController() {	

    receivedMessage=
        std::make_shared<cluon::OD4Session>(CID,
          [](cluon::data::Envelope &&envelope) noexcept {
              switch (envelope.dataType()) {
                  case TURN: { //Remember to check at what angle it wants to turn.
                      TURN turn = cluon::extractMessage<TURN>(std::move(envelope));
                      std::cout << "received 'TURN' "<< turn << " from controller'"; 
		      msgSteering.steeringAngle(turn);//Turn appropriately...
		      std::cout << "'TURN' message sent to vehicle";
		      
                      break;

		   case MOVE: {
			MOVE move = cluon::extractMessage<MOVE>(std::move(envelope));
			std::cout << "received 'MOVE' from controller"
			msgPedal.percent(move)//Move forward
		      break;
                  }

                  default: std::cout << "No matching case, wrong message type!" << std::endl;
              }
});

