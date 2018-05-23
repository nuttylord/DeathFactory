class EnviromentClient : public Enviroment
{
public:
	static GameObjectPtr StaticCreate() { return GameObjectPtr(new EnviromentClient()); }



	//virtual void Read(InputMemoryBitStream& inInputStream) override;


	/*enum Type
	{
		MovingPlatform,
		SpinningSaw,
		GasPipe,
		Fan,
		ConveyorBelt,
		LongPlatform,
		ShortPlatform,
		TypeCount
	};*/

	virtual void	UpdateTextures() override;
	virtual void	Update() override;

	void					setTextureSet(bool set)	{ textureSet = set; }
	bool					getTextureSet() { return textureSet; }

	/*virtual const Type		getType()					const override { return mType; }
	virtual void			setType(Type input)			override { mType = input; mIsNew = true; }*/

	virtual bool			getIsNew()					override { return mIsNew; }
	virtual const void		setIsNew(bool in)			override { mIsNew = in; }
protected:
	EnviromentClient();
private:
	bool textureSet = false;
	bool firstRun = false;
	Type mType;
	SpriteComponentPtr mSpriteComponent;
	bool					mIsNew = false;
};
