#include <RoboCatServerPCH.h>
//#include <iostream>

RoboCatServer::RoboCatServer() :
	mCatControlType(ESCT_Human),
	mTimeOfNextShot(0.f),
	mTimeBetweenShots(0.2f)
{}

void RoboCatServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}

void RoboCatServer::Update()
{
	RoboCat::Update();

	Vector3 oldLocation = GetLocation();
	Vector3 oldVelocity = GetVelocity();
	float oldRotation = GetRotation();

	//are you controlled by a player?
	//if so, is there a move we haven't processed yet?
	if (GetPlayerId() != 0)
	{
		ClientProxyPtr client = NetworkManagerServer::sInstance->GetClientProxy(GetPlayerId());
		if (client)
		{
			MoveList& moveList = client->GetUnprocessedMoveList();
			for (const Move& unprocessedMove : moveList)
			{
				const InputState& currentState = unprocessedMove.GetInputState();

				float deltaTime = unprocessedMove.GetDeltaTime();

				ProcessInput(deltaTime, currentState);
				SimulateMovement(deltaTime);

				//LOG( "Server Move Time: %3.4f deltaTime: %3.4f left rot at %3.4f", unprocessedMove.GetTimestamp(), deltaTime, GetRotation() );

			}

			moveList.Clear();
		}
	}
	else
	{
		//AI stuff - DL

		//get all objects in game
		std::vector<GameObjectPtr> objects = World::sInstance->GetGameObjects();

		std::vector<GameObjectPtr> playerObjects; // will store player objects

												  //for every object
		for (GameObjectPtr obj : objects) {

			//is it a cat?
			if (obj->GetClassId() == 'RCAT')
			{
				//if it has a player
				if ((obj->GetAsCat())->GetPlayerId() != 0) {

					//add to the list
					playerObjects.push_back(obj);

				}

			}
		}

		GameObjectPtr closestPlayer; //holds the index of the nearest playerObject
		float closestDistance = 10000000; // stores the distance to target

										  //loops through and assigns points to the closest player
		for (GameObjectPtr const player : playerObjects) {

			if (GetDistance(player) < closestDistance && GetDistance(player) > 1)
			{
				closestDistance = GetDistance(player);
				closestPlayer = player;
			}

		}

		if (closestPlayer == NULL)
			return;

		float targetX = closestPlayer->GetLocation().mX;
		float myX = GetLocation().mX;

		//if closest player X is greater than our x, move right
		if (targetX > myX)
		{
			Vector3 newVelocity;

			//set rotation
			SetRotation(1.5708f);
			newVelocity = GetVelocity();
			newVelocity += Vector3(getAcceleration() * Timing::sInstance.GetDeltaTime(), 0, 0);

			SetVelocity(newVelocity);
		}
		else if (targetX < myX)// move left
		{
			Vector3 newVelocity;

			//set rotation
			SetRotation(1.5708f);
			newVelocity = GetVelocity();
			newVelocity -= Vector3(getAcceleration() * Timing::sInstance.GetDeltaTime(), 0, 0);

			SetVelocity(newVelocity);

		}


		//do some AI stuff
		SimulateMovement(Timing::sInstance.GetDeltaTime());
	}

	HandleShooting();

	if (!RoboMath::Is2DVectorEqual(oldLocation, GetLocation()) ||
		!RoboMath::Is2DVectorEqual(oldVelocity, GetVelocity()) ||
		oldRotation != GetRotation())
	{
		NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_Pose);
	}

}

//pass in a pointer to game object, returns the distance to it. -DL 
float RoboCatServer::GetDistance(GameObjectPtr target)
{
	// get points
	Vector3 pointA, pointB, pointC;

	pointA = target->GetLocation(); // a is target
	pointB = GetLocation(); // B is us
	pointC = pointA - pointB; // C is difference between points

							  //return length
	return pointC.Length();


}

void RoboCatServer::HandleShooting()
{
	float time = Timing::sInstance.GetFrameStartTime();
	if (mIsShooting && Timing::sInstance.GetFrameStartTime() > mTimeOfNextShot)
	{
		//not exact, but okay
		mTimeOfNextShot = time + mTimeBetweenShots;

		//fire!
		YarnPtr yarn = std::static_pointer_cast< Yarn >(GameObjectRegistry::sInstance->CreateGameObject('YARN'));
		yarn->InitFromShooter(this);
	}
}

void RoboCatServer::TakeDamage(int inDamagingPlayerId)
{
	mHealth--;
	if (mHealth <= 0.f)
	{
		//score one for damaging player...
		ScoreBoardManager::sInstance->IncScore(inDamagingPlayerId, 1);

		//and you want to die
		SetDoesWantToDie(true);

		//tell the client proxy to make you a new cat
		ClientProxyPtr clientProxy = NetworkManagerServer::sInstance->GetClientProxy(GetPlayerId());
		if (clientProxy)
		{
			clientProxy->HandleCatDied();
		}
	}

	//tell the world our health dropped
	NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_Health);
}
