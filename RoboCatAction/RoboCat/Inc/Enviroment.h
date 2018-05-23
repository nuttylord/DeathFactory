class Enviroment : public GameObject
{
public:
	CLASS_IDENTIFICATION('ENVT', GameObject)


	

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
	virtual void			UpdateTextures() {}

	virtual uint32_t		Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const override;
	virtual void			Read(InputMemoryBitStream& inInputStream) override;


	virtual bool			HandleCollisionWithPlayer(RoboCat* incat);

	/*virtual const Type		getType()					const override	{ return mType; }
	virtual void			setType(Type input)			override {
		mType = input; 
		mIsNew = true; 
		setRectType(input);
	}*/
	virtual bool			getIsNew()					override		{ return mIsNew; }
	virtual const void		setIsNew(bool in)			override		{ mIsNew = in; }

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
	bool					mIsNew = false;

};


typedef shared_ptr< Enviroment >	EnviromentPtr;