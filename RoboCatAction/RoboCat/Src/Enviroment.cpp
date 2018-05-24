#include <RoboCatPCH.h>
//#include <half.hpp>
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
		//uint8_t neg = 0;
		//half x1 = half_cast<half, float>(location.mX);
		////float x1 = std::abs(location.mX);
		//half y1 = half_cast<half, float>(location.mY);
		////unsigned long x = (*(long*)&x1 << 16);//(uint32_t)((location.mX > 0 ? *(long*)&location.mX: *(long*)(location.mX)) >> 12);
		////unsigned long y = (*(long*)&y1 << 16);//(std::abs(location.mY) >> 12);
		//
		//uint16_t first = half_cast<uint8_t, half>(x1);
		//uint16_t second = half_cast<uint8_t, half>(y1);
		//uint32_t packet = first | second;
		//LOG("this is the output: %d %d %d", (double)x1, (double)y1, packet);
		inOutputStream.Write(location.mX);
		inOutputStream.Write(location.mY);
		
		
		/*if (location.mX < 0) { neg += 2; }
		if (location.mY < 0) { neg += 1; }
		inOutputStream.Write(neg);*/
		
		writtenState |= EERS_Pose;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}
	
	if (inDirtyState & EERS_Type)
	{
		inOutputStream.Write((bool)true);


		inOutputStream.Write(getType());

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
		//float rotation;
		//uint32_t packet;
		//inInputStream.Read(packet);
		//// split the int apart with a mask
		//uint16_t partX = (uint16_t)((packet & 0xFFFF0000) >> 16);
		//uint16_t partY = (uint16_t)(packet & 0x0000FFFF);

		//// bitshifting and masks to get them back into floats. 
		//float mX = (partX >> 8);
		//mX += 1.f / (float)(partX & 0x00FF);
		//float mY = (partY >> 8);
		//mY += 1.f / (float)(partY & 0x00FF);
		//
		//uint8_t neg;
		//inInputStream.Read(neg);
		//if (neg & 0b00000010) { mX *= -1; }
		//if (neg & 0b00000001) { mY *= -1; }

		//location.mX = mX;
		//location.mY = mY;
		//LOG("this is the client output:  x: %d,  y: %d", mX, mY);
		//
		
		inInputStream.Read(location.mX);
		inInputStream.Read(location.mY);
		SetLocation(location);


	}
	
	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Type type;
		inInputStream.Read(type);
		setType(type);
		//UpdateTextures();
	}
	
}

bool Enviroment::HandleCollisionWithPlayer(RoboCat* incat)
{

	//depending on the enviroment type we will handle collision differently(from their end).

	/*if (getType() == Type::Background || getType() == Type::TitleScreen)
		return true;
	else*/
		return true;
}


Enviroment::Enviroment()
{
	//default 

	SetCollisionRadius(0.25f);
}

