#include <RoboCatClientPCH.h>
#include <SDL_image.h>

std::unique_ptr< TextureManager >		TextureManager::sInstance;

void TextureManager::StaticInit()
{
	sInstance.reset( new TextureManager() );
}

TextureManager::TextureManager()
{
	//CacheTexture( "cat", "../Assets/cat.png" );
	CacheTexture( "title", "../Assets/TitleScreen.png" );
	CacheTexture( "background", "../Assets/Factory2.png" );
	CacheTexture( "mouse", "../Assets/coin.png" );
	CacheTexture( "yarn", "../Assets/yarn.png" );
	//CacheTexture( "factory", "../Assets/Factory.png" );
	CacheTexture( "fan", "../Assets/fan.png" );
	CacheTexture( "spinning_saw", "../Assets/spinning_saw.png" );
	CacheTexture( "short_pipe", "../Assets/pipe_small.png" );
	CacheTexture( "long_pipe", "../Assets/pipe_long.png" );
	CacheTexture( "broken_pipe", "../Assets/broken_pipe.png" );
	CacheTexture( "walking_left", "../Assets/singleWalkingLeft.png" );
	CacheTexture( "walking_right", "../Assets/singleWalkingRight.png" );
	//CacheTexture( "entities", "../Assets/Entities.png");
	
}
	

TexturePtr	TextureManager::GetTexture( const string& inTextureName )
{
	// unordered_map< string, TexturePtr > = mNameToTextureMap
	return mNameToTextureMap[ inTextureName ];
}

bool TextureManager::CacheTexture( string inTextureName, const char* inFileName )
{
	SDL_Texture* texture = IMG_LoadTexture( GraphicsDriver::sInstance->GetRenderer(), inFileName );

	if( texture == nullptr )
	{
		LOG("Failed to load Texture (TextureManager): %s", inFileName);
		SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", inFileName );
		return false;
	}

	int w, h;
	SDL_QueryTexture( texture, nullptr, nullptr, &w, &h );

	// Set the blend mode up so we can apply our colors
	SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );
	
	TexturePtr newTexture( new Texture( w, h, texture ) );

	mNameToTextureMap[ inTextureName ] = newTexture;

	return true;

}