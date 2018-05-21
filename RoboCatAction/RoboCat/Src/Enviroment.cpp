#include <RoboCatPCH.h>


unsigned int Enviroment::getCategory() const
{
	return mType;
}

uint32_t Enviroment::Write(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const
{
	uint32_t writtenState = 0;

	//our pose is first so we 
	if (inDirtyState & EERS_Pose)
	{
		inOutputStream.Write((bool)true);

		Vector3 location = GetLocation();
		inOutputStream.Write(location.mX);
		inOutputStream.Write(location.mY);

		inOutputStream.Write(GetRotation());

		writtenState |= EERS_Pose;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}
	
	if (inDirtyState & EERS_Type)
	{
		inOutputStream.Write((bool)true);


		inOutputStream.Write(mType);

		writtenState |= EERS_Type;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}
	


	return writtenState;

}

void Enviroment::Read(InputMemoryBitStream & inInputStream)
{
	bool stateBit;

	inInputStream.Read(stateBit);
	//if state bit is true
	if (stateBit)
	{
		//declare location and rotation
		Vector3 location;
		float rotation;


		
		inInputStream.Read(location.mX);
		inInputStream.Read(location.mY);
		SetLocation(location);

		inInputStream.Read(rotation);
		SetRotation(rotation);

	}
	
	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Type type;
		inInputStream.Read(type);
		setType(type);
	}
	
}

bool Enviroment::HandleCollisionWithPlayer(RoboCat* incat)
{

	//depending on the enviroment type we will handle collision differently.


	return false;
}


Enviroment::Enviroment()
{
	//default 

	SetCollisionRadius(0.25f);
}

