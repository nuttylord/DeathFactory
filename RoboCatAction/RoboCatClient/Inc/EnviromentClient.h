class EnviromentClient : public Enviroment
{
public:
	static GameObjectPtr StaticCreate() { return GameObjectPtr(new EnviromentClient()); }

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
private:
	SpriteComponentPtr mSpriteComponent;
};
