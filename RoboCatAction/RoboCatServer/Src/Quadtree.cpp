#include <RoboCatServerPCH.h>
#include <Quadtree.h>

using std::set; 

Quadtree::Quadtree(float _x, float _y, float _width, float _height, int _level, int _maxObjects) :
	x(_x),
	y(_y),
	width(_width),
	height(_height),
	level(_level),
	maxObjects(_maxObjects)
{
	mBoundingRect.x = _x;
	mBoundingRect.y = _y;
	mBoundingRect.w = _width;
	mBoundingRect.h = _height;
}

Quadtree::Quadtree(int _x, int _y, int _width, int _height, int _level, int _maxObjects) : 
	x(_x),
	y(_y),
	width(_width),
	height(_height),
	level(_level),
	maxObjects(_maxObjects)
{
	mBoundingRect.x = _x;
	mBoundingRect.y = _y;
	mBoundingRect.w = _width;
	mBoundingRect.h = _height;
}
//void Quadtree::Clear() 
//{ 
//	// calls the "set" objects own clear function. 
//	//mCollisionSet.clear();
//	mNESet.clear();
//	mNWSet.clear();
//	mSESet.clear();
//	mSWSet.clear();
//}

void Quadtree::Clear(set< GameObjectPtr > set)
{

}
Quadtree::~Quadtree()
{
	mNW.reset();
	mNE.reset();
	mSW.reset();
	mSE.reset();
	//delete mNW;
	//Clear();
	/*if (mNW != nullptr)
	{
		mNW->~Quadtree();
		mNE->~Quadtree();
		mSW->~Quadtree();
		mSE->~Quadtree();
	}*/
}


void Quadtree::CheckObjectCollision(set<GameObjectPtr>& collisionSet, vector<vector<GameObjectPtr>>& QuadVector)
{
	// start with clearing the sets to make for new collisions. 
	//Clear();

	// The magic.
	if (level != 1 && collisionSet.size() < (maxObjects) /*|| level == maxObjects*/) {
		// this code block runs if the number of objects at the current level is less then the number of the level
		// this does not guarantee maximum efficiency but is a very close estimate as each bottom level 
		// collision check has an algorith of N^2
		// at this point, it is assumed that the Quadtree has filtered out a large number of the unnessecary collision checks.

		if (collisionSet.size() > 0)
		{
			// temporarily create a vector for populating.
			vector<GameObjectPtr> outputVector;

			size_t size = collisionSet.size();

			for (GameObjectPtr obj : collisionSet)
			{
				
				outputVector.push_back(obj);
			}
				

			// N E S T E D V E C T O R O U T P U T 
			QuadVector.push_back(outputVector);
		}
		// R E C U R S I O N
		return;
	}
	else {

		// there is a need for another layer of quadtrees, create and populate it! 
		
		mNW.reset( new Quadtree(x, y, width / 2.0f, height / 2.0f, level + 1, maxObjects));
		mNE.reset( new Quadtree(x + width / 2.0f, y, width / 2.0f, height / 2.0f, level + 1, maxObjects));
		mSW.reset( new Quadtree(x, y + height / 2.0f, width / 2.0f, height / 2.0f, level + 1, maxObjects));
		mSE.reset( new Quadtree(x + width / 2.0f, y + height / 2.0f, width / 2.0f, height / 2.0f, level + 1, maxObjects));

		// gather data in sets from graph
		// push everything down the chain of quads and check data again
		// use contains(FloatRect rect, set<SceneNode*> node) to check for collision. 
		// once sets reach max level quad or dont exceed maxObjects (going beyond would yield null)
		// compile them into pairs with SceneNode::checkNodeCollision()
		quadRect = mNE->getBoundingRect();
		for (GameObjectPtr object : collisionSet)
		{
			if (contains(quadRect, object))
				mNESet.insert(object);
		}
		quadRect = mNW->getBoundingRect();
		for (GameObjectPtr object : collisionSet)
		{
			if (contains(quadRect, object))
				mNWSet.insert(object);
		}
		quadRect = mSE->getBoundingRect();
		for (GameObjectPtr object : collisionSet)
		{
			if (contains(quadRect, object))
				mSESet.insert(object);
		}
		quadRect = mSW->getBoundingRect();
		for (GameObjectPtr object : collisionSet)
		{
			if (contains(quadRect, object))
				mSWSet.insert(object);
		}
		// clear the set once you are done !
		collisionSet.clear();

		// pass the sets down the quadtree to be checked for collision or further dispersal
		/*mNE->setCollisionSet(mNESet);
		mNW->setCollisionSet(mNWSet);
		mSE->setCollisionSet(mSESet);
		mSW->setCollisionSet(mSWSet);*/

		// run the quad tree collision deeper down the tree. these functions act recursively.
		// if the compiler returns from SE->CheckObjectCollision() ever then it would have already 
		// searched the smallest and most SE quad that has less than (maxObjects) gameobjects in them. 
		// it then returns back up the tree in a DFS search fashion. 
		// once a tree is searched, it is then deallocated of all its memory as its return has already
		// been passed up the QuadVector reference. 
		// i have seen too many data leaks _/( . .)\_
		if(!mSESet.empty())
			mSE->CheckObjectCollision(mSESet, QuadVector);
		//mSESet.clear();
		//mSE->~Quadtree();
		if (!mSWSet.empty())
			mSW->CheckObjectCollision(mSWSet, QuadVector);
		//mSWSet.clear();
		//mSW->~Quadtree();
		if (!mNESet.empty())
			mNE->CheckObjectCollision(mNESet, QuadVector);
		//mNESet.clear();
		//mNE->~Quadtree();
		if (!mNWSet.empty())
			mNW->CheckObjectCollision(mNWSet, QuadVector);
		//mNWSet.clear();
		//mNW->~Quadtree();
		return;
	}
}

//void Quadtree::setCollisionSet( const std::set<GameObjectPtr>& collisionSet )
//{
//	mCollisionSet = collisionSet;
//}

SDL_Rect Quadtree::getBoundingRect() const
{
	if (this != nullptr)
		return mBoundingRect;
	else
	{
		SDL_Rect rect;
		rect.x = this->x;
		rect.y = this->y;
		rect.w = this->width;
		rect.h = this->height;
		return rect;
	}
		
}

bool Quadtree::contains(SDL_Rect quad, GameObjectPtr node) {
	//return object->getBoundingRect().intersects(sf::FloatRect(child->x, child->y, child->width, child->height));
	if (node)
	{
		SDL_Rect nodeRect;

		nodeRect.x = (node->GetLocation().mX - node->GetCollisionRadius() / 2);
		nodeRect.y = (node->GetLocation().mY - node->GetCollisionRadius() / 2);
		nodeRect.w = (node->GetCollisionRadius() * 5);
		nodeRect.h = (node->GetCollisionRadius() * 5);
		// TL - this is SDL's very own boolean type. 
		// It is so obnoxious that it needs to have its own memory allocation and cant just return true like an 
		// obedient little library!!! 
		SDL_bool check = SDL_HasIntersection(&nodeRect, &quad);
		if (check)
			return true;
		else
			return false;

	}
}
