#include <Quadtree.h>

class Server : public Engine
{
public:

	static bool StaticInit();

	virtual void DoFrame() override;

	virtual int Run();

	void HandleNewClient( ClientProxyPtr inClientProxy );
	void HandleLostClient( ClientProxyPtr inClientProxy );

	RoboCatPtr	GetCatForPlayer( int inPlayerId );
	void	SpawnCatForPlayer( int inPlayerId );


private:
	Server();
	
	// TL - server based quadtree collisions. 
	Quadtree*	mQuadtree;
	void		DoObjectCollision();

	bool		InitNetworkManager();
	void		SetupWorld();

};