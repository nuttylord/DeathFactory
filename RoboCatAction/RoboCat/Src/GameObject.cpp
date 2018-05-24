#include<RoboCatPCH.h>
//#include "..\Inc\GameObject.h"

GameObject::GameObject() :
	mIndexInWorld( -1 ),
	mCollisionRadius( 0.5f ),
	mDoesWantToDie( false ),
	mRotation( 0.f ),
	mNetworkId( 0 ),
	mColor( Colors::White ),
	mScale( 1.0f )
{
	setType(GameObject::Type::GameObj);
}

void GameObject::setType(GameObject::Type input)
{
	// preset values for collision on server with these types of objects. 
	mType = input;
	if (input == GameObject::Type::ShortPlatform)
	{
		SetWidth(.25f);
		SetHeight(.05f);
	}
	else if (input == GameObject::Type::LongPlatform)
	{
		SetWidth(1.25f);
		SetHeight(.05f);
	}
	else if (input == GameObject::Type::PlayerCharacter)
	{
		SetWidth(.13f);
		SetHeight(.25f);
	}
}

void GameObject::Update()
{
	//object don't do anything by default...	
}


Vector3 GameObject::GetForwardVector()	const
{
	//should we cache this when you turn?
	return Vector3( sinf( mRotation ), -cosf( mRotation ), 0.f );
}

void GameObject::SetNetworkId( int inNetworkId )
{ 
	//this doesn't put you in the map or remove you from it
	mNetworkId = inNetworkId; 

}

float GameObject::GetWidth() 
{
	if (getType() == GameObject::Type::ShortPlatform)
	{
		return .25f;
	}
	else if (getType() == GameObject::Type::LongPlatform)
	{
		return 1.25f;
	}
	else if (getType() == GameObject::Type::PlayerCharacter)
	{
		return .13f;
	}
}

float GameObject::GetHeight() 
{
	if (getType() == GameObject::Type::ShortPlatform)
	{
		return .05f;
	}
	else if (getType() == GameObject::Type::LongPlatform)
	{
		return .05f;
	}
	else if (getType() == GameObject::Type::PlayerCharacter)
	{
		return .25f;
	}
}
//
//SDL_Rect& GameObject::getRect()
//{
//	GameObject::Type type = getType();
//	Vector3 loc = GetLocation();
//	SDL_Rect mRect;
//	
//	if (type == GameObject::Type::ShortPlatform)
//	{
//		mRect.x = loc.mX - GetWidth() / 2;
//		mRect.y = loc.mY - GetHeight() / 2;
//		mRect.w = GetWidth();
//		mRect.h = GetHeight();
//	}
//	else if (type == GameObject::Type::LongPlatform)
//	{
//		mRect.x = loc.mX - GetWidth() / 2;
//		mRect.y = loc.mY - GetHeight() / 2;
//		mRect.w = GetWidth();
//		mRect.h = GetHeight();
//	}
//	else if (type == GameObject::Type::SpinningSaw) {
//		mRect.x = loc.mX - GetWidth() / 2;
//		mRect.y = loc.mY - GetHeight() / 2;
//		mRect.w = GetWidth();
//		mRect.h = GetHeight();
//	}
//
//	return mRect;
//}

void GameObject::SetRotation( float inRotation )
{ 
	//should we normalize using fmodf?
	mRotation = inRotation;
}
//
//void setType(GameObject::Type input)
//{
//	mType = input;
//	if (input == GameObject::Type::ShortPlatform)
//	{
//		SetWidth(.25f);
//		SetHeight(.1f);
//	}
//	else if (input == GameObject::Type::LongPlatform)
//	{
//		SetWidth(1.25f);
//		SetHeight(.1f);
//	}
//	else if (input == GameObject::Type::PlayerCharacter)
//	{
//		SetWidth(.15f);
//		SetHeight(.25f);
//	}
//}