

/*
* the world tracks all the live game objects. Failry inefficient for now, but not that much of a problem
*/
class World
{

public:

	static void StaticInit();

	static std::unique_ptr< World >		sInstance;

	void AddGameObject( GameObjectPtr inGameObject );
	void RemoveGameObject( GameObjectPtr inGameObject );

	void Update();

	const std::vector< GameObjectPtr >&	GetGameObjects()	const	{ return mGameObjects; }

	// TL - check during collision (set) and stored here for checking on server for sync.
	void			setCollisionSetNum(size_t amount)				{ collisionSetNum = amount; }
	const size_t	getCollisionSetNum()					const	{ return collisionSetNum; }

private:

	size_t collisionSetNum;

	World();

	int	GetIndexOfGameObject( GameObjectPtr inGameObject );

	std::vector< GameObjectPtr >	mGameObjects;
	//std::set<GameObjectPtr>

};