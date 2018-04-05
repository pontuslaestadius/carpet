/*
  Author: Sebastian Fransson
  Created: 4/4 - 2018
  Last Updated: 5/4 - 2018

*/
#include "commander.hpp"

int main(){

	std::shared_ptr<commander> commanderService = std::make_shared<commander>();

	std::cout << "Starting service.." << std::endl;
	//Enter the chosen set of movements. TODO: Should loop through these values.
	while(1){


	}
	
}

//Control car through OD4 messages received etc.
commander::commander(){	

    receivedMessage =
        std::make_shared<cluon::OD4Session>(CID,
          [this](cluon::data::Envelope &&envelope) noexcept {
		std::cout << "Joined OD4 Session... " << std::endl;
              switch (envelope.dataType()) {
                  case TURN_DIRECTION: { //Remember to check at what angle it wants to turn.
                      Turn trn = cluon::extractMessage<Turn>(std::move(envelope));
                      std::cout << "received 'TURN' " << trn.steeringAngle() << " from controller'" << std::endl; 
		      msgSteering.steeringAngle(trn.steeringAngle()); //Turn appropriately...
		      std::cout << "'TURN' message sent to vehicle" << std::endl;
		       
                      break;
		}

		   case MOVE_FORWARD: {
			Move mo = cluon::extractMessage<Move>(std::move(envelope));
			std::cout << "received 'MOVE' from controller" << std::endl;
			msgPedal.percent(mo.percent()); //Move forward
		      break;
                  }

                  default: std::cout << "No matching case, wrong message type!" << std::endl;
              }
  });

}

