class YarnClient : public Yarn
{
public:
	static	GameObjectPtr	StaticCreate()		{ return GameObjectPtr( new YarnClient() ); }

	virtual void		Read( InputMemoryBitStream& inInputStream ) override;
	virtual bool		HandleCollisionWithPlayer( RoboCat* inCat ) override;

protected:
	YarnClient();

private:

	SpriteComponentPtr	mSpriteComponent;
};