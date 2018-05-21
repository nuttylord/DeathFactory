class YarnServer : public Yarn
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn( new YarnServer() ); }
	void HandleDying() override;

	virtual bool		HandleCollisionWithPlayer( RoboCat* inCat ) override;

	virtual void Update() override;
	uint32_t GetClassId() { return 'YARN'; }
protected:
	YarnServer();

private:
	float mTimeToDie;

};