#include <RoboCatClientPCH.h>

EnviromentClient::EnviromentClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("short_pipe"));
	mIsNew = true;

	GameObject::Type type = getType();
	//LOG("output: %d", type);
	if (type == GameObject::Type::ShortPlatform)
		mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("short_pipe"));
	else if (type == GameObject::Type::LongPlatform)
		mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("long_pipe"));
	else if (type == GameObject::Type::SpinningSaw)
		mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("spinning_saw"));
	else if (type == GameObject::Type::TitleScreen)
	{
		mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("title"));
		/*Vector3 loc = GetLocation();
		loc.mZ = -1.f;
		SetLocation(loc);*/
	}
	else if (type == GameObject::Type::Background)
	{
		mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("background"));
		/*Vector3 loc = GetLocation();
		loc.mZ = -1.f;
		SetLocation(loc);*/
	}
	else if (type == GameObject::Type::Fan)
		mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("fan"));
	else
		mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("short_pipe"));
}

void EnviromentClient::UpdateTextures()
{
	//GameObject::Type type = getType();
	////LOG("output: %d", type);
	//if (type == GameObject::Type::ShortPlatform)
	//	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("short_pipe"));
	//else if (type == GameObject::Type::LongPlatform)
	//	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("long_pipe"));
	//else if (type == GameObject::Type::SpinningSaw)
	//	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("spinning_saw"));
	//else if (type == GameObject::Type::TitleScreen)
	//{
	//	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("title"));
	//	/*Vector3 loc = GetLocation();
	//	loc.mZ = -1.f;
	//	SetLocation(loc);*/
	//}
	//else if (type == GameObject::Type::Background)
	//{
	//	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("background"));
	//	/*Vector3 loc = GetLocation();
	//	loc.mZ = -1.f;
	//	SetLocation(loc);*/
	//}
	//else if (type == GameObject::Type::Fan)
	//	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("fan"));
	//else
	//	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("short_pipe"));


	//setIsNew(false);
	
}

void EnviromentClient::Update()
{
	if (firstRun == false)
	{
		GameObject::Type type = getType();
		//LOG("output: %d", type);
		if (type == GameObject::Type::ShortPlatform)
			mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("short_pipe"));
		else if (type == GameObject::Type::LongPlatform)
			mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("long_pipe"));
		else if (type == GameObject::Type::SpinningSaw)
			mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("spinning_saw"));
		else if (type == GameObject::Type::TitleScreen)
		{
			mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("title"));
			/*Vector3 loc = GetLocation();
			loc.mZ = -1.f;
			SetLocation(loc);*/
		}
		else if (type == GameObject::Type::Background)
		{
			mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("background"));
			/*Vector3 loc = GetLocation();
			loc.mZ = -1.f;
			SetLocation(loc);*/
		}
		else if (type == GameObject::Type::Fan)
			mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("fan"));
		else
			mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("short_pipe"));
	}
	
}