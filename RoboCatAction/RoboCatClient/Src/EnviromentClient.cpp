#include <RoboCatClientPCH.h>

EnviromentClient::EnviromentClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));
	


		mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("block"));
	
	if(mType == Enviroment::Type::GasPipe)
		mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("pipe"));
}
