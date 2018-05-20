#include <RoboCatPCH.h>
#include <set>
//zoom hardcoded at 100...if we want to lock players on screen, this could be calculated from zoom
const float HALF_WORLD_HEIGHT = 3.6f;
const float HALF_WORLD_WIDTH = 6.4f;

RoboCat::RoboCat() :
	GameObject(),
	mMaxRotationSpeed( 3.f ), // changed from 5
	mMaxLinearSpeed( 50.f ),
	mVelocity( Vector3::Zero ),
	mWallRestitution( 0.1f ),
	mCatRestitution( 0.1f ),
	mThrustDir( 0.f ),
	mPlayerId( 0 ),
	mIsShooting( false ),
	mHealth( 10 ),
	mAcceleration(8), //new
	mFriction(30) //new - higher value means lower friction
{
	SetCollisionRadius( 0.5f );
}

void RoboCat::ProcessInput( float inDeltaTime, const InputState& inInputState )
{
	//process our input....

	//turning...
	float newRotation = GetRotation() + inInputState.GetDesiredHorizontalDelta() * mMaxRotationSpeed * inDeltaTime;
	SetRotation( newRotation );

	//moving...
	float inputForwardDelta = inInputState.GetDesiredVerticalDelta();
	mThrustDir = inputForwardDelta;


	mIsShooting = inInputState.IsShooting(); 

}

void RoboCat::AdjustVelocityByThrust( float inDeltaTime )
{
	//just set the velocity based on the thrust direction -- no thrust will lead to 0 velocity
	//simulating acceleration makes the client prediction a bit more complex

	Vector3 forwardVector = GetForwardVector(); // return Vector3( sinf( mRotation ), -cosf( mRotation ), 0.f );

	//Apply acceleration
	mVelocity = mVelocity + forwardVector * ( mThrustDir * inDeltaTime * mAcceleration );

	//Subtract friction - Friction is a fraction of our acceleration in inverse direction
	//mVelocity = mVelocity + (-1 * (mVelocity / mVelocity.Length()) * mFriction ); //vector divide operator in MathHelper...

	Vector3 friction = mVelocity * -1; // inverse direction of velocity

	if(friction.Length() != 0 )
		friction = friction / mFriction; //normalize

	mVelocity = mVelocity + friction;

	//check not exceeding max speed
	if (mVelocity.Length() > mMaxLinearSpeed) {
		
		mVelocity.mX = (mVelocity.mX / mVelocity.Length()) * mMaxLinearSpeed;
		mVelocity.mY = (mVelocity.mY / mVelocity.Length()) * mMaxLinearSpeed;
	}
}

void RoboCat::SimulateMovement( float inDeltaTime )
{
	//simulate us...
	AdjustVelocityByThrust( inDeltaTime );


	SetLocation( GetLocation() + mVelocity * inDeltaTime );

	ProcessCollisions();
}

void RoboCat::Update()
{
	
}

void RoboCat::ProcessCollisions()
{
	//right now just bounce off the sides..
	ProcessCollisionsWithScreenWalls();

	//float sourceRadius = GetCollisionRadius();
	//Vector3 sourceLocation = GetLocation();
	// TL - to use a quadtree, we use sets. 
	//std::set<GameObject*> collisionSet;

	//now let's iterate through the world and see what we hit...
	//note: since there's a small number of objects in our game, this is fine.
	//but in a real game, brute-force checking collisions against every other object is not efficient.
	//it would be preferable to use a quad tree or some other structure to minimize the
	//number of collisions that need to be tested.
	
	// TL - Realised quickly that this needs to be in world.cpp
	//for (auto goIt = World::sInstance->GetGameObjects().begin(), end = World::sInstance->GetGameObjects().end(); goIt != end; ++goIt)
	//{
	//	GameObject* target = goIt->get();
	//	
	//	// TL - so the first aim is to make a set of all the collidable objects in the scene. 
	//	//target.
	//}
	//s

}

void RoboCat::ProcessCollisionsWithScreenWalls()
{
	Vector3 location = GetLocation();
	float x = location.mX;
	float y = location.mY;

	float vx = mVelocity.mX;
	float vy = mVelocity.mY;

	float radius = GetCollisionRadius();

	//if the cat collides against a wall, the quick solution is to push it off
	if( ( y + radius ) >= HALF_WORLD_HEIGHT && vy > 0 )
	{
		mVelocity.mY = -vy * mWallRestitution;
		location.mY = HALF_WORLD_HEIGHT - radius;
		SetLocation( location );
	}
	else if( y <= ( -HALF_WORLD_HEIGHT - radius ) && vy < 0 )
	{
		mVelocity.mY = -vy * mWallRestitution;
		location.mY = -HALF_WORLD_HEIGHT - radius;
		SetLocation( location );
	}

	if( ( x + radius ) >= HALF_WORLD_WIDTH && vx > 0 )
	{
		mVelocity.mX = -vx * mWallRestitution;
		location.mX = HALF_WORLD_WIDTH - radius;
		SetLocation( location );
	}
	else if(  x <= ( -HALF_WORLD_WIDTH - radius ) && vx < 0 )
	{
		mVelocity.mX = -vx * mWallRestitution;
		location.mX = -HALF_WORLD_WIDTH - radius;
		SetLocation( location );
	}
}

uint32_t RoboCat::Write( OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState ) const
{
	uint32_t writtenState = 0;

	if( inDirtyState & ECRS_PlayerId )
	{
		inOutputStream.Write( (bool)true );
		inOutputStream.Write( GetPlayerId() );

		writtenState |= ECRS_PlayerId;
	}
	else
	{
		inOutputStream.Write( (bool)false );
	}


	if( inDirtyState & ECRS_Pose )
	{
		inOutputStream.Write( (bool)true );

		Vector3 velocity = mVelocity;
		inOutputStream.Write( velocity.mX );
		inOutputStream.Write( velocity.mY );

		Vector3 location = GetLocation();
		inOutputStream.Write( location.mX );
		inOutputStream.Write( location.mY );

		inOutputStream.Write( GetRotation() );

		writtenState |= ECRS_Pose;
	}
	else
	{
		inOutputStream.Write( (bool)false );
	}

	//always write mThrustDir- it's just two bits
	if( mThrustDir != 0.f )
	{
		inOutputStream.Write( true );
		inOutputStream.Write( mThrustDir > 0.f );
	}
	else
	{
		inOutputStream.Write( false );
	}

	if( inDirtyState & ECRS_Color )
	{
		inOutputStream.Write( (bool)true );
		inOutputStream.Write( GetColor() );

		writtenState |= ECRS_Color;
	}
	else
	{
		inOutputStream.Write( (bool)false );
	}

	if( inDirtyState & ECRS_Health )
	{
		inOutputStream.Write( (bool)true );
		inOutputStream.Write( mHealth, 4 );

		writtenState |= ECRS_Health;
	}
	else
	{
		inOutputStream.Write( (bool)false );
	}


	


	

	return writtenState;
	

}


