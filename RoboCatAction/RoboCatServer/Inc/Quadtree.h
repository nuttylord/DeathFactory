#pragma once
//#include <RoboCatServerPCH.h>
//#include "GameObject.h"

#include <vector>
#include <set>

using std::set;

class Quadtree {
public: 
	Quadtree(float x, float y, float width, float height, int level, int maxObjects);
	Quadtree(int x, int y, int width, int height, int level, int maxObjects);
	

	~Quadtree();
	
	void				CheckObjectCollision(set<GameObjectPtr>& collisionSet, vector<vector<GameObjectPtr>>& QuadVector);
	bool				contains(SDL_Rect quad, GameObjectPtr node);
	SDL_Rect			getBoundingRect() const;

	//void				setCollisionSet( const std::set<GameObjectPtr>& collisionSet );
private:
	//void						Clear();
	void						Clear(set< GameObjectPtr > set);

	float						x;
	float						y;
	float						width;
	float						height;
	int							level;
	int							maxObjects;
	GameObjectPtr				node;
	SDL_Rect					quadRect;

	//set<GameObjectPtr>			mCollisionSet;
	set<GameObjectPtr>			mNESet, mNWSet, mSWSet, mSESet;
	SDL_Rect					mBoundingRect;

	//Quadtree *					mParent; //actually never used lul soz john. although it would of made for some really nice recursive destruction!.
	
	unique_ptr<Quadtree>		mNW;
	unique_ptr<Quadtree>		mNE;
	unique_ptr<Quadtree>		mSW;
	unique_ptr<Quadtree>		mSE;
};