#include <RoboCatClientPCH.h>

void EnviromentClient::Read(InputMemoryBitStream & inInputStream)
{
	bool stateBit;


	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 location;
		inInputStream.Read(location.mX);
		inInputStream.Read(location.mY);

		//dead reckon ahead by rtt, since this was spawned a while ago!
		//SetLocation(location + velocity * NetworkManagerClient::sInstance->GetRoundTripTime());


		float rotation;
		inInputStream.Read(rotation);
		SetRotation(rotation);
	}


}

EnviromentClient::EnviromentClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("block"));
}

EnviromentClient::EnviromentClient(Type inType)
{
	mSpriteComponent.reset(new SpriteComponent(this));
	mType = inType;


	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("block"));



}
