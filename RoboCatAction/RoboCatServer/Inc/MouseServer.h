class MouseServer : public Mouse
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn( new MouseServer() ); }
	void HandleDying() override;
	virtual bool		HandleCollisionWithPlayer( RoboCat* inCat ) override;
	uint32_t GetClassId() { return 'MOUS'; }
protected:
	MouseServer();

};