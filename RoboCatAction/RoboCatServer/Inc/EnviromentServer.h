//#pragma once
//#include <RoboCatServerPCH.h>

class EnviromentServer : public Enviroment
{
public:
	static GameObjectPtr StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new EnviromentServer()); }

	void HandleDying() override;


	//bool HandleCollisionsWithCat(RoboCat* inCat);

	uint32_t GetClassId() { return 'ENVT'; }
protected:
	EnviromentServer();



};