
#include <RoboCatServerPCH.h>



void EnviromentServer::HandleDying()
{
}

bool EnviromentServer::HandleCollisionsWithCat(RoboCat * inCat)
{
	//collision will change based on what the player is doing


	return false;
}

EnviromentServer::EnviromentServer()
{

	//NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), EERS_Pose);
}


