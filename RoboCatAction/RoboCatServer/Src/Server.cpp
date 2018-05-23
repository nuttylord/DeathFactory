
#include <RoboCatServerPCH.h>
#include <set>
#include <vector>
#include <iostream>

using std::vector;

struct Score {
	string playerID;
	int playerScore;
};

vector<Score> scoreList; // will contain all of the high scores

bool Server::StaticInit()
{
	sInstance.reset( new Server() );

	return true;
}

Server::Server()
{

	GameObjectRegistry::sInstance->RegisterCreationFunction( 'RCAT', RoboCatServer::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'MOUS', MouseServer::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'YARN', YarnServer::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'ENVT', EnviromentServer::StaticCreate);
	
	InitNetworkManager();
	//SDL_Window
	//::sInstance->
	
	mQuadtree.reset( new Quadtree(-3.6f, -6.4f, 10.8f, 19.2f, 1, 6) );
	
	//NetworkManagerServer::sInstance->SetDropPacketChance( 0.8f );
	//NetworkManagerServer::sInstance->SetSimulatedLatency( 0.25f );
	//NetworkManagerServer::sInstance->SetSimulatedLatency( 0.5f );
	//NetworkManagerServer::sInstance->SetSimulatedLatency( 0.1f );

}


int Server::Run()
{
	SetupWorld();
	ReadHighScore();

	return Engine::Run();
}

bool Server::InitNetworkManager()
{
	string portString = StringUtils::GetCommandLineArg( 1 );
	uint16_t port = stoi( portString );

	return NetworkManagerServer::StaticInit( port );
}


namespace
{
	// creates mice !
	void CreateRandomMice( int inMouseCount )
	{
		Vector3 mouseMin( -5.f, -3.f, 0.f );
		Vector3 mouseMax( 5.f, 3.f, 0.f );
		GameObjectPtr go;

		//make a mouse somewhere- where will these come from?
		for( int i = 0; i < inMouseCount; ++i )
		{
			go = GameObjectRegistry::sInstance->CreateGameObject( 'MOUS' );
			Vector3 mouseLocation = RoboMath::GetRandomVector( mouseMin, mouseMax );
			go->SetLocation( mouseLocation );
		}
	}
}

namespace
{
	// creates the environment
	void CreateEnviroment()
	{
		Vector3 loc(0, 0, 0);

		GameObjectPtr make;

		/*loc.mX = 0;
		make = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
		make->SetLocation(loc);
		make->setType(GameObject::Type::Background);*/
		//make->SetIsDirty(true);
		
		loc.mY = 1;
		for (float i = -2.5f; i < 3; i++)
		{
			make = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
			loc.mX = i * 4.f;
			make->setType(GameObject::Type::ShortPlatform); // extended function of GameObject for environments.
			make->SetLocation(loc);
		}
		loc.mY = 1.5f;
		for (float i = -2; i < 3; i++)
		{
			make = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
			loc.mX = i * 2.f;
			make->setType(GameObject::Type::ShortPlatform); // extended function of GameObject for environments.
			make->SetLocation(loc);
		}
		loc.mY = 2.25f;
		for (float i = -2; i < 3; i++)
		{
			make = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
			loc.mX = i * 4.f;
			make->setType(GameObject::Type::SpinningSaw); // extended function of GameObject for environments.
			make->SetLocation(loc);
		}
		loc.mY = 3.f;
		for (float i = -3; i < 4; i++)
		{
			loc.mX = i * 2.5f;
			make = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
			make->setType(GameObject::Type::LongPlatform);
			make->SetLocation(loc);
		}
	}
}

void Server::SetupWorld()
{
	//spawn more random mice!
	CreateRandomMice( 15 );

	CreateEnviroment();
}

void Server::DoFrame()
{
	
	if (ReadyManager::sInstance->IsPlaying()) {
		//allowPlayers to move
	}
	else if (ReadyManager::sInstance->IsEveryoneReady())
	{
		//game not playing but ready to start
		ReadyManager::sInstance->StartGame();
	}
	else {
		// game not player, not ready to start
	}

	NetworkManagerServer::sInstance->ProcessIncomingPackets();
	
	NetworkManagerServer::sInstance->CheckForDisconnects();
	
	NetworkManagerServer::sInstance->RespawnCats();

	Engine::DoFrame();

	DoObjectCollision();

	NetworkManagerServer::sInstance->SendOutgoingPackets();
}

void Server::HandleNewClient( ClientProxyPtr inClientProxy )
{
	
	int playerId = inClientProxy->GetPlayerId();
	
	//get score
	int score = getPlayerScore(inClientProxy->GetName());

	ScoreBoardManager::sInstance->AddEntry(playerId, inClientProxy->GetName());

	//if score != 0 then we add their old score to their new score (0)
	if(score > 0)
		ScoreBoardManager::sInstance->IncScore(playerId, score);

	SpawnCatForPlayer( playerId );
	ReadyManager::sInstance->AddEntry(playerId, inClientProxy->GetName());
}

void Server::SpawnCatForPlayer( int inPlayerId )
{
	RoboCatPtr cat = std::static_pointer_cast< RoboCat >( GameObjectRegistry::sInstance->CreateGameObject( 'RCAT' ) );
	cat->SetColor( ScoreBoardManager::sInstance->GetEntry( inPlayerId )->GetColor() );
	cat->SetPlayerId( inPlayerId );
	//gotta pick a better spawn location than this...
	cat->SetLocation( Vector3( 1.f - static_cast< float >( inPlayerId / 2 ), 0.f, 0.f ) );

}

void Server::ReadHighScore()
{
	//declare variables 
	std::string inLine;
	std::ifstream file;

	file.open("../Assets/persistance.txt");
	
	//Loop to end of file, reading in each line
	while (getline(file, inLine, (char)'\n')) {

		//Temp Variables
		Score score;
		std::stringstream stream(inLine);
		std::string input;

		//read in player name
		getline(stream, input, (char)',');
		score.playerID = input;

		//read in score, convert from string to int.......
		getline(stream, input, (char)',');
		score.playerScore = stoi(input); // string to int
		scoreList.push_back(score);
	}

	file.close();


}

void Server::UpdateHighScore()
{
	bool updated;
	for (ScoreBoardManager::Entry instance : ScoreBoardManager::sInstance->GetEntries()) {

		updated = UpdateExistingHighScore(instance);

		//if we didn't find a matching score, make a new one
		if (!updated) {

			Score newScore;

			newScore.playerID = instance.GetPlayerName();

			newScore.playerScore = instance.GetScore();

		}
	}
}

//update existing high score, returns false if no player was updated
bool Server::UpdateExistingHighScore(ScoreBoardManager::Entry inScore)
{
	//Loop though all of our existing scores
	for (Score scoreIterator : scoreList) {

		//if we find a matching player name
		if (inScore.GetPlayerName() == scoreIterator.playerID)
		{
			scoreIterator.playerScore = inScore.GetScore(); // set the score
			return true; //updated
		}
	}

	//we didn't find a maching name
	return false;
}

void Server::WriteHighScores()
{
	//make sure our scores are up to date before saving
	UpdateHighScore();

	//openfile
	std::ofstream save("../Assets/persistance.txt");

	for (Score scoreIterator : scoreList) {

		save << scoreIterator.playerID << ',' << scoreIterator.playerScore;
		save << '\n';

	}

	save.close();
}

//pass in player name, returns their corrosponding score
int Server::getPlayerScore(std::string name)
{
	//loop through every score we have
	for (Score scoreIterator : scoreList) {

		//if names match, return that score
		if (name == scoreIterator.playerID)
			return scoreIterator.playerScore;
	}

	return 0;
}

void Server::HandleLostClient( ClientProxyPtr inClientProxy )
{
	//kill client's cat
	//remove client from scoreboard
	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->RemoveEntry( playerId );
	RoboCatPtr cat = GetCatForPlayer( playerId );
	if( cat )
	{
		cat->SetDoesWantToDie( true );
	}

	UpdateHighScore();
	WriteHighScores();
}

RoboCatPtr Server::GetCatForPlayer( int inPlayerId )
{
	//run through the objects till we find the cat...
	//it would be nice if we kept a pointer to the cat on the clientproxy
	//but then we'd have to clean it up when the cat died, etc.
	//this will work for now until it's a perf issue
	const auto& gameObjects = World::sInstance->GetGameObjects();
	for( int i = 0, c = gameObjects.size(); i < c; ++i )
	{
		GameObjectPtr go = gameObjects[ i ];
		RoboCat* cat = go->GetAsCat();
		if( cat && cat->GetPlayerId() == inPlayerId )
		{
			return std::static_pointer_cast< RoboCat >( go );
		}
	}

	return nullptr;

}

void Server::DoObjectCollision()
{
	// This will be passed by reference into the quadtree.
	vector<vector<GameObjectPtr>> QuadVector; // coolest name in this game.

	// TL - so the first aim is to make a set of all the collidable objects in the scene in a set. 
	const auto& gameObjects = World::sInstance->GetGameObjects();
	objectSet.clear();
	GameObjectPtr object;
	Vector3 output;
	uint32_t mask;
	for (GameObjectPtr obj : gameObjects)
	{
		mask = obj->GetClassId();
		if (mask == 'ENVT' || mask == 'RCAT' || mask == 'MOUS' || mask == 'YARN')
		{
			//if (mask == 'RCAT')
			//{
			//	Vector3 loc = obj->GetLocation();
			//	//LOG("player is at  x: %3.4f  y: %3.4f",loc.mX, loc.mY);
			//}
			if (mask == 'ENVT' && (obj->getType() == GameObject::Type::Background || obj->getType() == GameObject::Type::TitleScreen))
			{
				//LOG("yo objects exist at X: %d, %d", mask, obj->getType());
			}
			else
			{
				objectSet.insert(obj);
			}
		}
	}
	World::sInstance->setCollisionSetNum(objectSet.size());
	// Recursive Function that returns nearby Quads of collisions. 
	mQuadtree->CheckObjectCollision(objectSet, QuadVector);
	//for (vector<GameObjectPtr> quad : QuadVector)
	RoboCat* obj;
	GameObjectPtr target;
	for (vector<GameObjectPtr> quad : QuadVector) 
	{
		// iterate through current Quad
		for (GameObjectPtr object : quad) 
		{
			if (object != nullptr && object->GetClassId() == 'RCAT')
			{
				obj = object->GetAsCat();
				for (GameObjectPtr target : quad)
				{
					if (target != object && !target->DoesWantToDie())
					{
						//simple collision test for spheres- are the radii summed less than the distance?
						Vector3 targetLocation = target->GetLocation();
						float targetRadius = target->GetCollisionRadius();

						Vector3 delta = targetLocation - obj->GetLocation();
						float distSq = delta.LengthSq2D();
						float collisionDist = (obj->GetCollisionRadius() + targetRadius);

						// SDL_Rect usage. since its a C struct it is hard to dynamically allocate so 
						// i am creating it here.
						SDL_Rect targetRect;
						SDL_Rect objRect;
						GameObject::Type type = target->getType();
						Vector3 loc = obj->GetLocation();

						// doing your rect. it is important to note that these values are for 
						// collision checking and are not valid to the world. The reason i have 
						// * 1000 each of them is because of the integer structure of the Rect's.
						objRect.x = (loc.mX - obj->GetWidth() / 2) * 1000; 
						objRect.y = (loc.mY - obj->GetHeight() / 2) * 1000;
						objRect.w = obj->GetWidth() * 1000; 
						objRect.h = obj->GetHeight() * 1000;
						
						loc = target->GetLocation();

						//doing the target's rect.
						if (type == GameObject::Type::ShortPlatform)
						{
							targetRect.x = (loc.mX - obj->GetWidth() / 2) * 1000;
							targetRect.y = (loc.mY - obj->GetHeight() / 2) * 1000;
							targetRect.w = obj->GetWidth() * 1000;
							targetRect.h = obj->GetHeight() * 1000;
						}
						else if (type == GameObject::Type::LongPlatform)
						{
							targetRect.x = (loc.mX - obj->GetWidth() / 2) * 1000;
							targetRect.y = (loc.mY - obj->GetHeight() / 2) * 1000;
							targetRect.w = obj->GetWidth() * 1000;
							targetRect.h = obj->GetHeight() * 1000;
						}
						if (target->HandleCollisionWithPlayer(obj))
						{
							// complicated collision part. 
							// either works off of SDL_Rect for squares or it works off of circular collision.
							if (distSq < (collisionDist * collisionDist) || ((type == GameObject::Type::ShortPlatform || type == GameObject::Type::LongPlatform) && SDL_bool::SDL_TRUE == SDL_HasIntersection(&targetRect, &objRect)))
							{
								//okay, you hit something!
								//so, project your location far enough that you're not colliding
								Vector3 dirToTarget = delta;
								dirToTarget.Normalize2D();
								Vector3 acceptableDeltaFromSourceToTarget = dirToTarget * collisionDist;
								//important note- we only move this cat. the other cat can take care of moving itself



								Vector3 relVel = obj->GetVelocity();
								Vector3 enemyRelVel;

								if (type == GameObject::Type::SpinningSaw)
								{
									obj->SetHealth(obj->GetHealth() - 1);
									relVel.mX * -1.1f;
									relVel.mY * -1.1f;
									obj->SetVelocity(relVel);
								}

								//if other object is a cat, it might have velocity, so there might be relative velocity...
								if (target->GetAsCat())
								{
									bool targetPunching = target->GetIsPunching();
									bool playerPunching = obj->GetIsPunching();
									Vector3 enemyVel = target->GetVelocity();
									enemyRelVel = target->GetVelocity();


									if (std::abs(enemyVel.mX) > std::abs(relVel.mX) && targetPunching && !playerPunching)
									{
										enemyRelVel -= relVel * 1.4f;
										relVel -= enemyVel * .7f;
									}
									else if (std::abs(enemyVel.mX) > std::abs(relVel.mX) && !targetPunching && playerPunching)
									{
										enemyRelVel -= relVel * .7f;
										relVel -= enemyVel * 1.4f;
									}
									else
									{
										enemyRelVel -= relVel * .6f;
										relVel -= enemyVel * .6f;
									}

								}

								//got vel with dir between objects to figure out if they're moving towards each other
								//and if so, the magnitude of the impulse ( since they're both just balls )
								float relVelDotDir = Dot2D(relVel, dirToTarget);

								if (SDL_bool::SDL_TRUE == SDL_HasIntersection(&targetRect, &objRect) && (type == GameObject::Type::ShortPlatform || type == GameObject::Type::LongPlatform))
								{
									Vector3 loc = obj->GetLocation();
									Vector3 tarLoc = target->GetLocation();

									//important note- we only move this cat. the other cat can take care of moving itself
									//obj->SetLocation(targetLocation - acceptableDeltaFromSourceToTarget);

									Vector3 velocity = obj->GetVelocity();
									// player is to side of the platform
									if (objRect.y > targetRect.y && objRect.y + objRect.h < targetRect.y)
									{
										Vector3 move = loc - tarLoc;
										move.Normalize2D();
										move *= (obj->GetWidth() / 2.f + target->GetWidth() / 2.f);
										obj->SetLocation(move);
									}
									else if (objRect.y > targetRect.y)
									{ // if you bump your head on the top, nullify your upwards velocity.
										if (objRect.x > targetRect.x
											&& objRect.x < targetRect.x + targetRect.w
											|| objRect.x + objRect.w > targetRect.x
											&& objRect.x + objRect.w < targetRect.x + targetRect.w)
										{
											Vector3 move = Vector3(0, 1, 0);//loc - tarLoc;
																			//move.Normalize2D();
											move *= (obj->GetHeight() / 2.f + target->GetHeight() / 2.f);
											obj->SetLocation(move);
										}
									}
									else if (objRect.y + objRect.h < targetRect.y)
									{ // bottom of player is above the centrepoint of platform and colliding. 
										if (objRect.x / 2.f > targetRect.x
											&& objRect.x / 2.f < targetRect.x + targetRect.w
											|| objRect.x + objRect.w > targetRect.x
											&& objRect.x + objRect.w < targetRect.x + targetRect.w)
										{
											Vector3 move = Vector3(0, -1, 0);//loc - tarLoc;
																			 //move.Normalize2D();
											move *= (obj->GetHeight() / 2.f + target->GetHeight() / 2.f);
											obj->SetLocation(move);

											//player.setIsOnGround(true);
											//player.setFallSpeed(0.f);
										}
									}
								}
								else if (relVelDotDir > 0.f && !(type == GameObject::Type::ShortPlatform || type == GameObject::Type::LongPlatform))
								{
									obj->SetLocation(targetLocation - acceptableDeltaFromSourceToTarget);
									Vector3 impulse = relVelDotDir * dirToTarget;
									Vector3 velocity = obj->GetVelocity();

									if (target->GetAsCat())
									{
										RoboCat* tar = target->GetAsCat();

										float targetRelVelDotDir = Dot2D(enemyRelVel, dirToTarget);
										Vector3 targetImpulse = targetRelVelDotDir * dirToTarget;
										Vector3 velocityTarget = target->GetVelocity();

										velocity -= impulse;
										//velocity *= obj->GetCatRestitution() * .8f;

										// do the enemy player bounce back 
										velocityTarget -= targetImpulse;
										//velocityTarget *= target->GetCatRestitution() * .5f;

										// cast to RoboCat* for setting velocity
										tar->SetVelocity(velocityTarget);
										tar->SetIsDirty(true);
									}
									else
									{
										velocity -= impulse * 2.f;
										velocity *= obj->GetWallRestitution();
									}

									obj->SetVelocity(velocity);
									obj->SetIsDirty(true);
								}

							}
						}
					}
				}
			}
		}
	}
}
