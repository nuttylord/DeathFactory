#include <RoboCatPCH.h>

std::unique_ptr< ReadyManager >	ReadyManager::sInstance;


void ReadyManager::StaticInit()
{
	sInstance.reset(new ReadyManager());
}

ReadyManager::ReadyManager() :
	mEveryoneReady(false),
	mGamePlaying(false),
	mGameFinished(false)
{

}

ReadyManager::ReadyPlayer::ReadyPlayer(uint32_t inPlayerId, const string& inPlayerName, bool ready) :
	mPlayerId(inPlayerId),
	mPlayerName(inPlayerName)
{
	//set to false when we first make the player
	SetReadyState(ready);
}

//changes ready state and will update a displayed message to be added later on
void ReadyManager::ReadyPlayer::SetReadyState(bool inReadyState)
{
	mReadyState = inReadyState;

}



ReadyManager::ReadyPlayer::ReadyPlayer(uint32_t PlayerId, const string& PlayerName, bool ready) :
	mPlayerId(PlayerId),
	mPlayerName(PlayerName)
{
	//set to false when we first make the player
	SetReady(ready);
}

//changes ready state and handles removing the display message
void ReadyManager::ReadyPlayer::SetReady(bool inReady)
{
	mReady = inReady;

}

//pass in player ID, returns a pointer to the player
ReadyManager::ReadyPlayer* ReadyManager::GetEntry(uint32_t inPlayerId)
{
	for (ReadyPlayer &entry : mEntries)
	{
		if (entry.GetPlayerId() == inPlayerId)
		{
			return &entry;
		}
	}

	return nullptr;
}

//remove entry, usually if theres a duplicate name in the list
bool ReadyManager::RemoveEntry(uint32_t inPlayerId)
{
	if (mEntries.empty())
		return false;


	for (auto entryIterator = mEntries.begin(), endIt = mEntries.end(); entryIterator != endIt; ++entryIterator)
	{
		if ((*entryIterator).GetPlayerId() == inPlayerId)
		{
			mEntries.erase(entryIterator);
			return true;
		}
	}

	CheckPlayerCount();
	return false;
}

void ReadyManager::AddEntry(uint32_t inPlayerId, const string& inPlayerName)
{
	//if this player id exists already, remove it first- it would be crazy to have two of the same id
//Add player to the manager
void ReadyManager::AddEntry(uint32_t inPlayerId, const string& inPlayerName)
{

	RemoveEntry(inPlayerId);

	mEntries.emplace_back(inPlayerId, inPlayerName, true);

	CheckPlayerCount();
}


//change a players ready state
void ReadyManager::ChangeReadyState(uint32_t inPlayerId, bool inReady)
{
	ReadyPlayer* entry = GetEntry(inPlayerId);
	if (entry)
	{
		entry->SetReadyState(inReady);
		entry->SetReady(inReady);
	}
}

//checks if the game is ready to start, if so sets everyone ready
void ReadyManager::CheckPlayerCount()
{
	if (mGamePlaying)
	{
		for (ReadyPlayer& entry : mEntries)
		{
			//what to do if the game is running ?
			

		}
	}
	else //game is not running (not enough ready players yet)
	{
		int readyPlayers = 0;

		if (AllReady() && mEntries.size() > 1)
			SetEveryoneReady(true);
		else
			SetEveryoneReady(false);

		// if everyone is ready, change the display message
		if (IsEveryoneReady()) {

			for (ReadyPlayer& player : mEntries)
			{
				//remove display message
				std::string display = " ";

				player.SetDisplayMessage(display);
			}
		}


		//if we're not ready yet
		if (!IsEveryoneReady()) {

			for (ReadyPlayer& entry : mEntries)
			{
				std::string display;

				// set display message to user
				if (mEntries.size() < 2)
					display = "Waiting on other players";

				entry.SetDisplayMessage(display);
			}
		} //when everyone ready

	}
}

bool ReadyManager::AllReady() {

	//loop through every player, if we find one not ready return false
	for (ReadyPlayer player : mEntries) {
		if (!player.GetReadyState())
			return false;
	}

	//means we didn't find a player not ready
	return true;
}


bool ReadyManager::Write(OutputMemoryBitStream& inOutputStream) const
{
	int entryCount = mEntries.size();

	//we don't know our player names, so it's hard to check for remaining space in the packet...
	//not really a concern now though
	inOutputStream.Write(entryCount);
	for (const ReadyPlayer& entry : mEntries)
	{
		entry.Write(inOutputStream);
	}

	inOutputStream.Write(mGamePlaying);
	inOutputStream.Write(mPlaying);

	return true;
}

bool ReadyManager::Read(InputMemoryBitStream& inInputStream)
{
	int entryCount;
	inInputStream.Read(entryCount);
	//just replace everything that's here, it don't matter...
	mEntries.resize(entryCount);
	for (ReadyPlayer& entry : mEntries)
	{
		entry.Read(inInputStream);
	}

	bool gamePlaying;
	inInputStream.Read(gamePlaying);
	SetGamePlaying(gamePlaying);
	bool Playing;
	inInputStream.Read(Playing);

	SetPlaying(Playing);

	return true;
}

//called if AllReady returns true
void ReadyManager::SetEveryoneReady(bool inEveryoneReady)
{
	mEveryoneReady = inEveryoneReady;
}

void ReadyManager::SetGamePlaying(bool gamePlaying)
{
	mGamePlaying = gamePlaying;
//called when the game starts, allows players to move once set
void ReadyManager::SetPlaying(bool Playing)
{
	mPlaying = Playing;
}


void ReadyManager::StartGame()
{
	//read scores from file
	mGamePlaying = true;
	mPlaying = true;
	CheckPlayerCount();
}


bool ReadyManager::ReadyPlayer::Write(OutputMemoryBitStream& inOutputStream) const
{
	bool didSucceed = true;

	inOutputStream.Write(mPlayerId);
	inOutputStream.Write(mPlayerName);
	inOutputStream.Write(mReadyState);
	inOutputStream.Write(mReady);
	inOutputStream.Write(mDisplayMessage);

	return didSucceed;
}

bool ReadyManager::ReadyPlayer::Read(InputMemoryBitStream& inInputStream)
{
	bool didSucceed = true;

	inInputStream.Read(mPlayerId);
	inInputStream.Read(mPlayerName);
	int readyState;
	inInputStream.Read(readyState);

	if (didSucceed) {
		SetReadyState(readyState);
		SetReady(readyState);
	}

	inInputStream.Read(mDisplayMessage);

	return didSucceed;
}


