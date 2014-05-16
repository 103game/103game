#include "GameActions.h"

#include <memory>

#include "Client.h"
#include "World.h"
#include "WorldObject.h"

#include "ClientApp.h"


void GameActions::updateLogic(){
	// check if dead
	if(client->userCreature){
		if(client->userCreature->getLife() == 0){
			client->app->alertView->setAlert("You are dead!", "Sign in and try again");
			client->clientActions->signOut();			
		}
	}
	
}