#pragma once
class ReadyPlayer
{
public:
	ReadyPlayer() {};

	ReadyPlayer(uint32_t inPlayerID, const string& inPlayerName, bool ready);

	uint32_t		GetPlayerId()	const { return mPlayerId; }
	const string&	GetPlayerName()	const { return mPlayerName; }

	//bool			GetReadyState()		const { return mReadyState; }
	//void			SetReadyState(bool inReadyState);

	bool			GetReady()		const { return mReady; }
	void			SetReady(bool inReady);

	bool			Write(OutputMemoryBitStream& inOutputStream) const;
	bool			Read(InputMemoryBitStream& inInputStream);



	void SetDisplayMessage(string input) { mDisplayMessage = input; }
	string GetDisplayMessage() { return mDisplayMessage; }

private:
	uint32_t mPlayerId;
	string	mPlayerName;

	int		mReady;
	string mDisplayMessage;

};