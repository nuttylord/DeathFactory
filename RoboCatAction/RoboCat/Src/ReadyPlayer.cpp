#include <RoboCatPCH.h>

bool ReadyPlayer::Write(OutputMemoryBitStream& inOutputStream) const
{
	bool didSucceed = true;

	inOutputStream.Write(mPlayerId);
	// how do we make sure the string is read out properly with varying size?
	inOutputStream.Write(mPlayerName); 
	inOutputStream.Write(mReadyState);
	inOutputStream.Write(mReady);
	inOutputStream.Write(mDisplayMessage);

	return didSucceed;
}

bool ReadyPlayer::Read(InputMemoryBitStream& inInputStream)
{
	bool didSucceed = true;

	inInputStream.Read(mPlayerId);
	inInputStream.Read(mPlayerName);
	int readyState; // could this be a bool (8 bit) instead of int ? (32 bit) 
	inInputStream.Read(readyState);

	if (didSucceed) {
		//SetReadyState(readyState);
		SetReady(readyState);
	}

	inInputStream.Read(mDisplayMessage);

	return didSucceed;
}


//ReadyPlayer::ReadyPlayer(uint32_t inPlayerId, const string& inPlayerName, bool ready) :
//	mPlayerId(inPlayerId),
//	mPlayerName(inPlayerName)
//{
//	//set to false when we first make the player
//	SetReadyState(ready);
//}

//changes ready state and will update a displayed message to be added later on
//void ReadyPlayer::SetReadyState(bool inReadyState)
//{
//	mReadyState = inReadyState;
//
//}



ReadyPlayer::ReadyPlayer(uint32_t PlayerId, const string& PlayerName, bool ready) :
	mPlayerId(PlayerId),
	mPlayerName(PlayerName)
{
	//set to false when we first make the player
	SetReady(ready);
}

//changes ready state and handles removing the display message
void ReadyPlayer::SetReady(bool inReady)
{
	mReady = inReady;

}