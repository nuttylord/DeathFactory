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


	//peristance related stuffs -DL
	void ReadHighScore();
	void UpdateHighScore();
	bool UpdateExistingHighScore(ScoreBoardManager::Entry inScore);
	void WriteHighScores();
	int getPlayerScore(std::string name);



private:
	Server();
	
	// TL - server based quadtree collisions. 
	unique_ptr<Quadtree>		mQuadtree;
	std::set< GameObjectPtr >	objectSet;
	void						DoObjectCollision();

	bool		InitNetworkManager();
	void		SetupWorld();

};