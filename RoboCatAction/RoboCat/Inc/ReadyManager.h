#pragma once
class ReadyManager
{
public:

	static void StaticInit();
	static std::unique_ptr< ReadyManager >	sInstance;

	class ReadyPlayer
	{
	public:
		ReadyPlayer() {};

		ReadyPlayer(uint32_t inPlayerID, const string& inPlayerName, bool ready);

		uint32_t		GetPlayerId()	const { return mPlayerId; }
		const string&	GetPlayerName()	const { return mPlayerName; }

		bool			GetReadyState()		const { return mReadyState; }
		void			SetReadyState(bool inReadyState);

		bool			Write(OutputMemoryBitStream& inOutputStream) const;
		bool			Read(InputMemoryBitStream& inInputStream);


		static uint32_t	GetSerializedSize();

		void SetDisplayMessage(string input) {mDisplayMessage = input; }
		string GetDisplayMessage() { return mDisplayMessage; }

	private:
		uint32_t mPlayerId;
		string	mPlayerName;
		int		mReadyState;
		string mDisplayMessage;

	};

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
	void	SetEveryoneReady(bool inEveryoneReady);
	void	SetGamePlaying(bool gamePlaying);


	void	StartGame();
	void	ResetGame();

	const vector< ReadyPlayer >&	GetEntries()	const { return mEntries; }


private:
	ReadyManager();
	vector< ReadyPlayer >	mEntries;

	int mTimeToGameStart;
	int mMatchTimer;
	bool mEveryoneReady;
	bool mGamePlaying;
	bool mGameFinished;
};