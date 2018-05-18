class Enviroment : public GameObject
{
public:
	CLASS_IDENTIFICATION('ENVT', GameObject)

	enum Type
	{
		MovingPlatform,
		SpinningSaw,
		GasPipe,
		Fan,
		ConveyorBelt,
		LongPlatform,
		ShortPlatform,
		TypeCount
	};

	enum EEnviromentReplicationState
	{
		EERS_Pose = 1 << 0,
		//EERS_Type = 1 << 1,

		EERS_AllState = EERS_Pose
	};

	static GameObject* StaticCreate() { return new Enviroment(); }

	//void addObject(GameObject*);
public:

	//virtual Type			getType();

	//void					playLocalSound(CommandQueue& commands, SoundEffect::ID effect);
	//virtual unsigned int	getCategory() const;
	//virtual sf::FloatRect	getBoundingRect() const;

	virtual unsigned int	getCategory() const;


	virtual uint32_t	Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const override;
	virtual void		Read(InputMemoryBitStream& inInputStream) override;


	virtual bool HandleCollisionWithPlayer(RoboCat* incat);

	Type getType() { return mType; }

protected:
	Enviroment();


private:
	Type					mType;
	//sf::Sprite			mSprite;
	//Animation				mGasPipeLeak;
	//Animation				mSpinningSaw;
	//Animation				mConveyorBelt;
	bool					mPlayedGasLeakSound;
	bool					mPlayedFanBoostSound;

};
