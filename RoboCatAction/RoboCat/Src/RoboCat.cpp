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
	//mFriction(30), //new - higher value means lower friction
	//mGravity(3.f),
	//mJumpStrength(10.f),
	mAcceleration(8.f), //player applies acceleration to velocity - DL
	mFriction(30.f), //higher value means lower friction
	mGravity(7.f), // constant force applied downlwards
	mJumpStrength(200.f), // the force applied when jumping
	mJumpTimer(450.f) // time between jumps
{
	
	SetCollisionRadius( .25f );
	setType(GameObject::Type::PlayerCharacter);
}

// Process input...  - DL
void RoboCat::ProcessInput( float inDeltaTime, const InputState& inInputState )
{
	//process our input....
	if (ReadyManager::sInstance->IsPlaying()) {
		//turning...
		float newRotation = GetRotation() + inInputState.GetDesiredHorizontalDelta() * mMaxRotationSpeed * inDeltaTime;
		//SetRotation( newRotation );
	}

	//Wait for ReadyManager 
	if (ReadyManager::sInstance->IsPlaying()) {

		//turning...
		float newRotation = GetRotation() + inInputState.GetDesiredHorizontalDelta() * mMaxRotationSpeed * inDeltaTime;
		//SetRotation( newRotation );

		//if HorizontalDelta is > 0, we want to move right
		if (inInputState.GetDesiredHorizontalDelta() > 0)
		{
			//set rotation
			SetRotation(1.5708f);
			mVelocity += Vector3(mAcceleration * inDeltaTime, 0, 0);


		}
		//if horizontal delta is < 0, we want to move left
		else if (inInputState.GetDesiredHorizontalDelta() < 0) {

			//set rotation
			SetRotation(4.71239f);
			mVelocity += Vector3(-mAcceleration * inDeltaTime, 0, 0);

		}

		if (inInputState.GetDesiredVerticalDelta() > 0)
		{
			//if Vertical delta is > 1 then we want to jump.
			if (inInputState.GetDesiredVerticalDelta() > 0)
			{
				//if we're not jumping && off cooldown, we are now!!
				if (!mIsJumping && mJumpCounter < 0) {

					mIsJumping = true;
					Jump(inDeltaTime); //Jump!!
					mJumpCounter = mJumpTimer; // set on cooldown

				}

			}

			//moving...
			//float inputForwardDelta = inInputState.GetDesiredHorizontalDelta();
			//mThrustDir = inputForwardDelta;


			mIsShooting = inInputState.IsShooting();
		}
	}

}

//applies acceleration, gravity and friction - DL
void RoboCat::AdjustVelocityByThrust( float inDeltaTime )
{
	
	Vector3 forwardVector = GetForwardVector(); // return Vector3( sinf( mRotation ), -cosf( mRotation ), 0.f );

	//Apply acceleration to forward vector
	mVelocity = mVelocity + forwardVector * ( mThrustDir * inDeltaTime * mAcceleration );

	//Friction vector is inverse of forward vector
	Vector3 friction = mVelocity * -1; 

	//as long as we are not stopped, normalize friction by our friction constant
	if(friction.Length() != 0 )
		friction = friction / mFriction; //normalize 

	//Apply gravity force downwards
	mVelocity += Vector3(0, mGravity * inDeltaTime, 0);

	//apply friction
	mVelocity = mVelocity + friction;



	/*check not exceeding max speed
	if (mVelocity.Length() > mMaxLinearSpeed) {
		
		mVelocity.mX = (mVelocity.mX / mVelocity.Length()) * mMaxLinearSpeed;
		mVelocity.mY = (mVelocity.mY / mVelocity.Length()) * mMaxLinearSpeed;
	}
	*/
}

void RoboCat::SimulateMovement( float inDeltaTime )
{
	if (ReadyManager::sInstance->IsPlaying()) {
		//simulate us...
		
		AdjustVelocityByThrust(inDeltaTime);


		SetLocation(GetLocation() + mVelocity * inDeltaTime);

		ProcessCollisions();
	}
}

void RoboCat::Update()
{
	//if (!ReadyManager::sInstance->IsGamePlaying())
		//SetLocation(Vector3(0, 0, 0));

	DecrementJumpTimer();
}

void RoboCat::ProcessCollisions()
{
	//right now just bounce off the sides..
	ProcessCollisionsWithScreenWalls();

	// the rest happens on the server

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

//Accelerates the player verticaly - DL
void RoboCat::Jump(float inDeltaTime)
{
	mVelocity += Vector3(0, -mJumpStrength * inDeltaTime, 0);
}


//if we're jumping and timer has not been reset yet, tick it down. - DL
void RoboCat::DecrementJumpTimer()
{
	
	if (mIsJumping)
	{
		mJumpCounter--;
	}

	if (mJumpCounter < 0)
	{
		//jump off cooldown
		mIsJumping = false;
	}
}


