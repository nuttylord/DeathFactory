class EnviromentClient : public Enviroment
{
public:
	static GameObjectPtr StaticCreate() { return GameObjectPtr(new EnviromentClient()); }

	//virtual void Read(InputMemoryBitStream& inInputStream) override;


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

	Type mType;

protected:
	EnviromentClient();
	EnviromentClient(Type inType);
private:
	SpriteComponentPtr mSpriteComponent;
};
