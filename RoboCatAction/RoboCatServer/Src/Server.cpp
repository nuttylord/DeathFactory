
#include <RoboCatServerPCH.h>
//#include <Quadtree.h>
#include <set>
#include <vector>
#include <iostream>

using std::vector;

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
	
	mQuadtree.reset( new Quadtree(-3.6f, -4.6f, 7.2f, 9.2f, 1, 5) );
	
	//NetworkManagerServer::sInstance->SetDropPacketChance( 0.8f );
	//NetworkManagerServer::sInstance->SetSimulatedLatency( 0.25f );
	//NetworkManagerServer::sInstance->SetSimulatedLatency( 0.5f );
	//NetworkManagerServer::sInstance->SetSimulatedLatency( 0.1f );

}


int Server::Run()
{
	SetupWorld();

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

	void CreateEnviroment()
	{
		//Vector3 mouseMin(-5.f, -3.f, 0.f);
		//Vector3 mouseMax(5.f, 3.f, 0.f);
		//GameObjectPtr go;


		//make a mouse somewhere- where will these come from?
		for (float i = -6; i < 5; i+= 0.75f)
		{
			//go = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
			//Vector3 mouseLocation = RoboMath::GetRandomVector(mouseMin, mouseMax);

			EnviromentPtr go = std::static_pointer_cast< Enviroment >(GameObjectRegistry::sInstance->CreateGameObject('ENVT'));
			go->SetLocation(Vector3(i, 2, 0));
			go->setType(Enviroment::Type::GasPipe);
			go->SetScale(1);
			
			
		}

		go = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
		//Vector3 mouseLocation = RoboMath::GetRandomVector(mouseMin, mouseMax);

		go->SetLocation(Vector3(6, -3.5, 0));
		go->SetScale(5);

		go = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
		//Vector3 mouseLocation = RoboMath::GetRandomVector(mouseMin, mouseMax);

		go->SetLocation(Vector3(6, -3.8, 0));
		go->SetScale(1);

		go = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
		//Vector3 mouseLocation = RoboMath::GetRandomVector(mouseMin, mouseMax);

		go->SetLocation(Vector3(-6.5, 3, 0));
		go->SetScale(5);

		//make a mouse somewhere- where will these come from?
		//for (int i = -2; i < 3; ++i)
		//{
		//	go = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
		//	//Vector3 mouseLocation = RoboMath::GetRandomVector(mouseMin, mouseMax);

		//	go->SetLocation(Vector3(2,1,0));
		//	go->SetScale(5);
		//}

	}


}


void Server::SetupWorld()
{
	//spawn some random mice
	//CreateRandomMice( 10 );
	
	//spawn more random mice!
	CreateRandomMice( 10 );

	CreateEnviroment();
}

void Server::DoFrame()
{
	NetworkManagerServer::sInstance->ProcessIncomingPackets();

	NetworkManagerServer::sInstance->CheckForDisconnects();

	NetworkManagerServer::sInstance->RespawnCats();

	Engine::DoFrame();

	DoObjectCollision();
	//mQuadtree->CheckObjectCollision();

	NetworkManagerServer::sInstance->SendOutgoingPackets();

}

void Server::HandleNewClient( ClientProxyPtr inClientProxy )
{
	
	int playerId = inClientProxy->GetPlayerId();
	
	ScoreBoardManager::sInstance->AddEntry( playerId, inClientProxy->GetName() );
	SpawnCatForPlayer( playerId );
}

void Server::SpawnCatForPlayer( int inPlayerId )
{
	RoboCatPtr cat = std::static_pointer_cast< RoboCat >( GameObjectRegistry::sInstance->CreateGameObject( 'RCAT' ) );
	cat->SetColor( ScoreBoardManager::sInstance->GetEntry( inPlayerId )->GetColor() );
	cat->SetPlayerId( inPlayerId );
	//gotta pick a better spawn location than this...
	cat->SetLocation( Vector3( 1.f - static_cast< float >( inPlayerId ), 0.f, 0.f ) );

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
	for (GameObjectPtr object : gameObjects)
	{
		
		uint32_t mask = object->GetClassId();
		
		std::cout << mask << std::endl;
		if (mask == 'ENVT' || mask == 'RCAT' || mask == 'MOUS' || mask == 'YARN')
			objectSet.insert(object);
	}

	//std::copy(fooVec.begin(), fooVec.end(), std::inserter(fooSet, fooSet.end()));
	//mQuadtree->setCollisionSet(objectSet);

	// Recursive Function that returns nearby Quads of collisions. 
	mQuadtree->CheckObjectCollision(objectSet, QuadVector);
	
	for (vector<GameObjectPtr> quad : QuadVector)
	{
		// iterate through current Quad
		for (GameObjectPtr object : quad) {
			if (object && object->GetClassId() == 'RCAT')
			{
				RoboCat* obj = object->GetAsCat();
				for (auto goIt = quad.begin(), end = quad.end(); goIt != end; ++goIt)
				{
					GameObject* target = goIt->get();
					if (target != obj && !target->DoesWantToDie())
					{
						//simple collision test for spheres- are the radii summed less than the distance?
						Vector3 targetLocation = target->GetLocation();
						float targetRadius = target->GetCollisionRadius();

						Vector3 delta = targetLocation - obj->GetLocation();
						float distSq = delta.LengthSq2D();
						float collisionDist = (obj->GetCollisionRadius() + targetRadius);
						if (distSq < (collisionDist * collisionDist))
						{
							//first, tell the other guy there was a collision with a cat, so it can do something...

							if (target->HandleCollisionWithCat(obj))
							{
								//okay, you hit something!
								//so, project your location far enough that you're not colliding
								Vector3 dirToTarget = delta;
								dirToTarget.Normalize2D();
								Vector3 acceptableDeltaFromSourceToTarget = dirToTarget * collisionDist;
								//important note- we only move this cat. the other cat can take care of moving itself
								obj->SetLocation(targetLocation - acceptableDeltaFromSourceToTarget);


								Vector3 relVel = obj->GetVelocity();

								//if other object is a cat, it might have velocity, so there might be relative velocity...
								//RoboCat targetCat = target->GetAsCat();
								if (target->GetAsCat())
								{
									if (target->GetIsPunching())
										relVel -= target->GetVelocity() * .75f;
									else
										relVel -= target->GetVelocity();
								}


								//got vel with dir between objects to figure out if they're moving towards each other
								//and if so, the magnitude of the impulse ( since they're both just balls )
								float relVelDotDir = Dot2D(relVel, dirToTarget);

								if (relVelDotDir > 0.f)
								{
									Vector3 impulse = relVelDotDir * dirToTarget;
									Vector3 velocity = obj->GetVelocity();

									if (target->GetAsCat())
									{
										velocity -= impulse;
										velocity *= obj->GetCatRestitution();
									}
									else
									{
										velocity -= impulse * 2.f;
										velocity *= obj->GetWallRestitution();
									}

									obj->SetVelocity(velocity);
								}
							}
						}
					}
				}
			}
		}
	}
	
}
