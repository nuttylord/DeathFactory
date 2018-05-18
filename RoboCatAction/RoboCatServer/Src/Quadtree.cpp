#include <RoboCatServerPCH.h>
#include <Quadtree.h>
//#include ""
//#include <sdl>

using std::set; 

Quadtree::Quadtree(float _x, float _y, float _width, float _height, int _level, int _maxLevel) :
	x(_x),
	y(_y),
	width(_width),
	height(_height),
	level(_level),
	maxObjects(_maxLevel)
	//mBoundingRect(_x, _y, _width, _height)
{
}

void Quadtree::Clear() 
{ 
	// calls the "set" objects own clear function. 
	mNESet.clear();
	mNWSet.clear();
	mSESet.clear();
	mSWSet.clear();
}

Quadtree::~Quadtree()
{
	Clear();
	mCollisionSet.clear();
	delete mNW;
	delete mNE;
	delete mSW;
	delete mSE;
}


void Quadtree::CheckObjectCollision(std::set< GameObject::Pair >& collisionPairs)
{
	// start with clearing the sets to make for new collisions. 
	Clear();

	// The magic.
	if (level != 1 && mCollisionSet.size() < (maxObjects) /*|| level == maxObjects*/) {
		// this code block runs if the number of objects at the current level is less then the number of the level
		// this does not guarantee maximum efficiency but is a very close estimate as each bottom level 
		// collision check has an algorith of N^2
		// at this point, it is assumed that the Quadtree has filtered out a large number of the unnessecary collision checks.

		// create a local pair to avoid re-initialisation. 
		GameObject::Pair pair;

		// deallocate the tree below this so as to avoid filling up memory. 
		if (mNW != NULL)
		{
			mNW->~Quadtree();
			mNE->~Quadtree();
			mSW->~Quadtree();
			mSE->~Quadtree();
		}

		for (set<GameObjectPtr>::iterator it1 = mCollisionSet.begin(); it1 != mCollisionSet.end(); it1++) {
			for (set<GameObjectPtr>::iterator it2 = mCollisionSet.begin(); it2 != mCollisionSet.end(); it2++) {
				GameObject* first = it1->get();
				GameObject* second = it2->get();
				pair.first = first;
				pair.second = second;
				collisionPairs.insert(pair);
			}
		}
		// build the pair list inside collisionPairs. 
		//FOREACH(GameObject* node1, mCollisionSet)
		//{
		//	FOREACH(GameObject* node2, mCollisionSet)
		//	{
		//		// (node.getCategory() == Category::PlayerCharacter || this->getCategory() == Category::PlayerCharacter)
		//		if (node1 != node2
		//			&& (node1->getCategory() == Category::PlayerCharacter
		//				|| node2->getCategory() == Category::PlayerCharacter)
		//			&& contains(node1->getBoundingRect(), node2))
		//		{

		//			pair.first = node1;
		//			pair.second = node2;
		//			collisionPairs.insert(pair);
		//		}
		//	}
		//}
		// return up the tree because this Quad has just edited the collisionPairs reference and you want your code 
		// to continue checking collisions in other Quads.
		return;
	}
	else {

		// there is a need for another layer of quadtrees, create and populate it! 
		mNW = new Quadtree(x, y, width / 2.0f, height / 2.0f, level + 1, maxObjects);
		mNE = new Quadtree(x + width / 2.0f, y, width / 2.0f, height / 2.0f, level + 1, maxObjects);
		mSW = new Quadtree(x, y + height / 2.0f, width / 2.0f, height / 2.0f, level + 1, maxObjects);
		mSE = new Quadtree(x + width / 2.0f, y + height / 2.0f, width / 2.0f, height / 2.0f, level + 1, maxObjects);

		// gather data in sets from graph
		// push everything down the chain of quads and check data again
		// use contains(FloatRect rect, set<SceneNode*> node) to check for collision. 
		// once sets reach max level quad or dont exceed maxObjects (going beyond would yield null)
		// compile them into pairs with SceneNode::checkNodeCollision()
		
		for (set<GameObjectPtr>::iterator it1 = mCollisionSet.begin(); it1 != mCollisionSet.end(); it1++) {
			GameObject* node = it1->get();
			if (contains(mNE->getBoundingRect(), node))
				mNESet.insert((GameObjectPtr)node);
		}
		for (set<GameObjectPtr>::iterator it1 = mCollisionSet.begin(); it1 != mCollisionSet.end(); it1++) {
			GameObject* node = it1->get();
			if (contains(mNW->getBoundingRect(), node))
				mNWSet.insert((GameObjectPtr)node);
		}
		for (set<GameObjectPtr>::iterator it1 = mCollisionSet.begin(); it1 != mCollisionSet.end(); it1++) {
			GameObject* node = it1->get();
			if (contains(mSE->getBoundingRect(), node))
				mSESet.insert((GameObjectPtr)node);
		}
		for (set<GameObjectPtr>::iterator it1 = mCollisionSet.begin(); it1 != mCollisionSet.end(); it1++) {
			GameObject* node = it1->get();
			if (contains(mSW->getBoundingRect(), node))
				mSWSet.insert((GameObjectPtr)node);
		}

		// pass the sets down the quadtree to be checked for collision or further dispersal
		mNE->setCollisionSet(mNESet);
		mNW->setCollisionSet(mNWSet);
		mSE->setCollisionSet(mSESet);
		mSW->setCollisionSet(mSWSet);

		// run the quad tree collision deeper down the tree. 
		mSE->CheckObjectCollision(collisionPairs);
		mSW->CheckObjectCollision(collisionPairs);
		mNE->CheckObjectCollision(collisionPairs);
		mNW->CheckObjectCollision(collisionPairs);

	}
}

void Quadtree::setCollisionSet(std::set<GameObjectPtr> collisionSet)
{
	mCollisionSet = collisionSet;
}

SDL_Rect Quadtree::getBoundingRect() const
{
	if (this != nullptr)
		return mBoundingRect;
	else
		return SDL_Rect();
}

bool Quadtree::contains(SDL_Rect quad, GameObject* node) {
	//return object->getBoundingRect().intersects(sf::FloatRect(child->x, child->y, child->width, child->height));
	SDL_Rect nodeRect;
	nodeRect.x = (Sint16)node->GetLocation().mX;
	nodeRect.y = (Sint16)node->GetLocation().mY;
	nodeRect.w = (Uint16)node->GetCollisionRadius();
	nodeRect.h = (Uint16)node->GetCollisionRadius();
	if (this != nullptr)
	{
		// TL - this is SDL's very own boolean type. 
		// It is so obnoxious that it needs to have its own memory allocation and cant just return true like an 
		// obedient little library!!! 
		SDL_bool check = SDL_HasIntersection(&nodeRect, &quad);
		if (check)
		{
			return true;
		} 
		else 
		{
			return false;
		}
	}
	return false;
	
	/*return	 !(nodeRect.left < quad.left ||
	nodeRect.top < quad.top ||
	nodeRect.left > quad.left + quad.width ||
	nodeRect.top > quad.top + quad.height ||
	nodeRect.left + nodeRect.width < quad.left ||
	nodeRect.top + nodeRect.height < quad.top ||
	nodeRect.left + nodeRect.width > quad.left + quad.width ||
	nodeRect.top + nodeRect.height > quad.top + quad.height);*/
}
