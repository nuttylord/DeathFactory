#pragma once
//#include <RoboCatServerPCH.h>
//#include "GameObject.h"

#include <vector>
#include <set>

class Quadtree {
public: 
	Quadtree(float x, float y, float width, float height, int level, int maxObjects);
	

	~Quadtree();
	
	void				CheckObjectCollision(std::set< GameObject::Pair >& collisionPairs);
	bool				contains(SDL_Rect quad, GameObject* node);
	SDL_Rect			getBoundingRect() const;

	void				setCollisionSet(std::set<GameObjectPtr> collisionSet);
private:
	void						Clear();

	float						x;
	float						y;
	float						width;
	float						height;
	int							level;
	int							maxObjects;

	std::set<GameObjectPtr>		mCollisionSet;
	std::set<GameObjectPtr>		mNESet, mNWSet, mSWSet, mSESet;
	SDL_Rect					mBoundingRect;

	Quadtree *					mParent; //actually never used lul soz john 
	
	Quadtree *					mNW;
	Quadtree *					mNE;
	Quadtree *					mSW;
	Quadtree *					mSE;
};