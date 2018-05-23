#include <RoboCatPCH.h>

bool ReadyPlayer::Write(OutputMemoryBitStream& inOutputStream) const
{
	bool didSucceed = true;

	inOutputStream.Write(mPlayerId); // 1st

	inOutputStream.Write(mPlayerName); //2nd
	inOutputStream.Write(mReady); //3rd
	inOutputStream.Write(mDisplayMessage); //4th

	return didSucceed;
}

bool ReadyPlayer::Read(InputMemoryBitStream& inInputStream)
{
	bool didSucceed = true;

	inInputStream.Read(mPlayerId); //1st
	inInputStream.Read(mPlayerName); //2nd
	int readyState; 
	inInputStream.Read(readyState); //3rd

	if (didSucceed) {
		SetReady(readyState);
	}

	inInputStream.Read(mDisplayMessage); //4th

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
	
	SetReady(ready);
}

//changes ready state and handles removing the display message
void ReadyPlayer::SetReady(bool inReady)
{
	mReady = inReady;

}