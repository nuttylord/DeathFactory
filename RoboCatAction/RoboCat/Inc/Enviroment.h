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
		EERS_Type = 1 << 1,

		EERS_AllState = EERS_Pose | EERS_Type
	};

	static GameObject* StaticCreate() { return new Enviroment(); }

	virtual uint32_t GetAllStateMask()	const override { return EERS_AllState; }

	

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
	void setType(Type input);

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

typedef shared_ptr< Enviroment >	EnviromentPtr;