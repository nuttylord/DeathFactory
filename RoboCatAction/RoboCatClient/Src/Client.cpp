
#include <RoboCatClientPCH.h>
namespace
{
	// creates the environment
	void CreateEnviroment()
	{
		Vector3 loc(0, 0, 0);

		GameObjectPtr make;

		//loc.mX = 0;
		make = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
		make->SetLocation(loc);
		make->setType(GameObject::Type::Background);

		//loc.mY = 1.5f;
		//for (float i = -2.5f; i < 3; i++)
		//{
		//	make = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
		//	loc.mX = i * 4.f;
		//	make->setType(GameObject::Type::ShortPlatform); // extended function of GameObject for environments.
		//	make->SetLocation(loc);
		//}
		//loc.mY = 2.0f;
		//for (float i = -2; i < 3; i++)
		//{
		//	make = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
		//	loc.mX = i * 2.f;
		//	make->setType(GameObject::Type::ShortPlatform); // extended function of GameObject for environments.
		//	make->SetLocation(loc);
		//}
		//loc.mY = 2.5f;
		//for (float i = -2; i < 3; i++)
		//{
		//	make = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
		//	loc.mX = i * 4.f;
		//	make->setType(GameObject::Type::SpinningSaw); // extended function of GameObject for environments.
		//	make->SetLocation(loc);
		//}
		//loc.mY = 3.f;
		//for (float i = -3; i < 4; i++)
		//{
		//	loc.mX = i * 2.5f;
		//	make = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
		//	make->setType(GameObject::Type::LongPlatform);
		//	make->SetLocation(loc);
		//}
		//loc.mY = 1;
		//for (float i = -2.2f; i < 3; i++)
		//{
		//	make = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
		//	loc.mX = i * 4.f;

		//	make->setType(GameObject::Type::ShortPlatform); // extended function of GameObject for environments.
		//	make->SetLocation(loc);
		//}
		//loc.mY = 2.25f;
		//for (float i = -2.2f; i < 3; i++)
		//{
		//	make = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
		//	loc.mX = i * 2.f;

		//	make->setType(GameObject::Type::ShortPlatform); // extended function of GameObject for environments.
		//	make->SetLocation(loc);
		//}
		//loc.mY = 1.5f;
		//for (float i = -2.2f; i < 3; i++)
		//{
		//	make = GameObjectRegistry::sInstance->CreateGameObject('ENVT');
		//	loc.mX = i * 4.f;

		//	make->setType(GameObject::Type::SpinningSaw); // extended function of GameObject for environments.
		//	make->SetLocation(loc);
		//}
		//loc.mY = 3.f;
		//for (float i = -5.2f; i < 6; i++)
		//{
		//	loc.mX = i * 2.5f;
		//	make = GameObjectRegistry::sInstance->CreateGameObject('ENVT');

		//	make->setType(GameObject::Type::LongPlatform);

		//	make->SetLocation(loc);
		//}
	}
}
bool Client::StaticInit( )
{
	// Create the Client pointer first because it initializes SDL
	Client* client = new Client();

	if( WindowManager::StaticInit() == false )
	{
		return false;
	}
	
	if( GraphicsDriver::StaticInit( WindowManager::sInstance->GetMainWindow() ) == false )
	{
		return false;
	}

	TextureManager::StaticInit();
	RenderManager::StaticInit();
	InputManager::StaticInit();

	HUD::StaticInit();

	sInstance.reset( client );

	// spent too long trying to find ways to optimise this over network.
	// spawned it in the client instead. 
	CreateEnviroment();
	return true;
}

void Client::updateEnvironmentTextures()
{
	const auto& gameObjects = World::sInstance->GetGameObjects();
	for (GameObjectPtr obj : gameObjects)
	{
		if (obj->GetClassId() == 'ENVT' && obj->getIsNew())
		{
			//LOG("got one ! x: %3.4f, y: %3.4f", obj->GetLocation().mX, obj->GetLocation().mY);
			obj->setIsNew(false);
			obj->UpdateTextures();
		}
	}
}

Client::Client()
{
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'RCAT', RoboCatClient::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'MOUS', MouseClient::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'YARN', YarnClient::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'ENVT', EnviromentClient::StaticCreate );


	string destination = StringUtils::GetCommandLineArg( 1 );
	string name = StringUtils::GetCommandLineArg( 2 );

	SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString( destination );

	NetworkManagerClient::StaticInit( *serverAddress, name );

	//NetworkManagerClient::sInstance->SetDropPacketChance( 0.6f );
	//NetworkManagerClient::sInstance->SetSimulatedLatency( 0.25f );
	//NetworkManagerClient::sInstance->SetSimulatedLatency( 0.5f );
	//NetworkManagerClient::sInstance->SetSimulatedLatency( 0.1f );
}



void Client::DoFrame()
{
	InputManager::sInstance->Update();

	Engine::DoFrame();

	NetworkManagerClient::sInstance->ProcessIncomingPackets();

	//updateEnvironmentTextures();

	RenderManager::sInstance->Render();

	NetworkManagerClient::sInstance->SendOutgoingPackets();
}

void Client::HandleEvent( SDL_Event* inEvent )
{
	switch( inEvent->type )
	{
	case SDL_KEYDOWN:
		InputManager::sInstance->HandleInput( EIA_Pressed, inEvent->key.keysym.sym );
		break;
	case SDL_KEYUP:
		InputManager::sInstance->HandleInput( EIA_Released, inEvent->key.keysym.sym );
		break;
	default:
		break;
	}
}

