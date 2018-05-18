#pragma once
#include <RoboCatClientPCH.h>
#include <vector>
#include <memory>


class BackgroundObject {

public:
	static void Init();
	static std::unique_ptr<BackgroundObject> sInstance;

	bool Load();
	std::vector<sf::Sprite>& getTexturedWorld();


private:

};