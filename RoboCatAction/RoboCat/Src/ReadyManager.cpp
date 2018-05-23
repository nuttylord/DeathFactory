#include <RoboCatPCH.h>

std::unique_ptr< ReadyManager >	ReadyManager::sInstance;


void ReadyManager::StaticInit()
{
	sInstance.reset(new ReadyManager());
}

ReadyManager::ReadyManager() :
	mEveryoneReady(false),
	mPlaying(false),
	mGameFinished(false)
{

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
		entry->SetReady(inReady);
	}
}

//checks if the game is ready to start, if so sets everyone ready
void ReadyManager::CheckPlayerCount()
{
	if (!mPlaying)
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

//returns false if finds a player that is not ready 
bool ReadyManager::AllReady() {

	//loop through every player, if we find one not ready return false
	for (ReadyPlayer player : mEntries) {
		if (!player.GetReady())
			return false;
	}

	//means we didn't find a player not ready
	return true;
}


bool ReadyManager::Write(OutputMemoryBitStream& inOutputStream) const
{
	int entryCount = mEntries.size();

	inOutputStream.Write(entryCount);
	for (const ReadyPlayer& entry : mEntries)
	{
		entry.Write(inOutputStream);
	}

	inOutputStream.Write(mPlaying);

	return true;
}

bool ReadyManager::Read(InputMemoryBitStream& inInputStream)
{
	int entryCount;
	inInputStream.Read(entryCount);
	

	mEntries.resize(entryCount);
	for (ReadyPlayer& entry : mEntries)
	{
		entry.Read(inInputStream);
	}

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

//called when the game starts, allows players to move once set
void ReadyManager::SetPlaying(bool Playing)
{
	mPlaying = Playing;
}


void ReadyManager::StartGame()
{
	//read scores from file
	mPlaying = true;
	CheckPlayerCount();
}


bool ReadyManager::ReadyPlayer::Write(OutputMemoryBitStream& inOutputStream) const
{
	bool didSucceed = true;

	inOutputStream.Write(mPlayerId);
	inOutputStream.Write(mPlayerName);
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
		SetReady(readyState);
	}

	inInputStream.Read(mDisplayMessage);

	return didSucceed;
}


