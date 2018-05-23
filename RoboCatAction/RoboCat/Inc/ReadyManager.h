#pragma once

//forward declaration
class ReadyPlayer;

class ReadyManager
{
public:

	static void StaticInit();
	static std::unique_ptr< ReadyManager >	sInstance;

	

	ReadyPlayer*	GetEntry(uint32_t inPlayerId); //for all your getting needs
	bool	RemoveEntry(uint32_t inPlayerId); // to check that their are no dupes before we add (ScoreBoardManager)

	void	AddEntry(uint32_t inPlayerId, const string& inPlayerName);
	void	ChangeReadyState(uint32_t inPlayerId, bool inReadyState); //false when joins, true once they press a key

	void	CheckPlayerCount();

	bool AllReady();

	bool	Write(OutputMemoryBitStream& inOutputStream) const;
	bool	Read(InputMemoryBitStream& inInputStream);

	bool	IsEveryoneReady() const { return mEveryoneReady; }

	bool	IsGamePlaying() const { return mGamePlaying; }
	bool	IsGameFinished() const { return mGameFinished; }


	//void	StartGame();
	void	ResetGame();

	void	SetEveryoneReady(bool inEveryoneReady);
	void	SetPlaying(bool playing);


	void	StartGame();


	const vector< ReadyPlayer >&	GetEntries()	const { return mEntries; }


private:
	ReadyManager();
	vector< ReadyPlayer >	mEntries;

	int mTimeToGameStart;
	int mMatchTimer;
	bool mEveryoneReady;
	bool mGamePlaying;

	//bool mEveryoneReady;
	bool mPlaying;
	bool mGameFinished;
};