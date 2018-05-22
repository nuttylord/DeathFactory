
#define CLASS_IDENTIFICATION( inCode, inClass ) \
enum { kClassId = inCode }; \
virtual uint32_t GetClassId() const { return kClassId; } \
static GameObject* CreateInstance() { return static_cast< GameObject* >( new inClass() ); } \



// an object in a game, idk, look up unity ok! 
class GameObject
{
public:
	//typedef std::pair<GameObject*, GameObject*> Pair; // TL - used for collision pairs in the QuadTree structure. 
	//typedef std::pair<GameObjGameObject*> Ptr;
public:

	CLASS_IDENTIFICATION( 'GOBJ', GameObject )

	GameObject();
	virtual ~GameObject() {} 

	// TL - Danny, here is where magic can happen! use this enum to set up as many different types of object as possible
	enum Type
	{ 
		MovingPlatform,
		SpinningSaw, // pixel dimensions width:65, height:65
		GasPipe, // pixel dimensions width:55, height:16
		Fan,
		ConveyorBelt,
		LongPlatform,
		ShortPlatform, // pixel dimensions width:55, height:16
		GameObj,
		Background,
		PlayerCharacter, // pixel dimensions width:20, height:31
		TitleScreen,
		TypeCount
	};

	//virtual SDL_Rect&					getRect();
	//virtual void setType(inputType);

	//virtual Enviroment* GetAsEnviroment() {return nullptr; }
	virtual const Type		getType()				const	{ return mType; }
	virtual void			setType(Type input);

	virtual void setRectType(Type input)
	{
		mType = input;
		if (input == GameObject::Type::ShortPlatform)
		{
			SetWidth(.25f);
			SetHeight(.1f);
		}
		else if (input == GameObject::Type::LongPlatform)
		{
			SetWidth(1.25f);
			SetHeight(.1f);
		}
		else if (input == GameObject::Type::PlayerCharacter)
		{
			SetWidth(.15f);
			SetHeight(.25f);
		}
	}
	virtual void			UpdateTextures() {} // this is inherited and called on environment pointers

	virtual bool			getIsNew()						{ return mIsNew; }
	virtual const void		setIsNew(bool in)				{ mIsNew = in; }

	virtual	RoboCat*	GetAsCat()							{ return nullptr; }
	// New Functions for server
	virtual const Vector3&	GetVelocity()			const	{ return Vector3(0,0,0); } 
	virtual const bool		GetIsPunching()			const	{ return false; }
	virtual const float		GetWallRestitution()	const	{ return 0; }
	virtual const float		GetCatRestitution()		const	{ return 0; }

	const bool				GetIsDirty()			const	{ return mIsDirty; }
	void					SetIsDirty(bool isDirty)		{ mIsDirty = isDirty; }

	virtual uint32_t GetAllStateMask()	const { return 0; }

	//return whether to keep processing collision
	virtual bool	HandleCollisionWithPlayer( RoboCat* inCat ) { ( void ) inCat; return true; }

	virtual void	Update();

	virtual void	HandleDying() {}

			void	SetIndexInWorld( int inIndex )						{ mIndexInWorld = inIndex; }
			int		GetIndexInWorld()				const				{ return mIndexInWorld; }

			void	SetRotation( float inRotation );
			float	GetRotation()					const				{ return mRotation; }

			void	SetScale( float inScale )							{ mScale = inScale; }
			float	GetScale()						const				{ return mScale; }


	const Vector3&		GetLocation()				const				{ return mLocation; }
			void		SetLocation( const Vector3& inLocation )		{ mLocation = inLocation; }

			float		GetCollisionRadius()		const				{ return mCollisionRadius; }
			void		SetCollisionRadius( float inRadius )			{ mCollisionRadius = inRadius; }

			Vector3		GetForwardVector()			const;


			void		SetColor( const Vector3& inColor )				{ mColor = inColor; }
	const Vector3&		GetColor()					const				{ return mColor; }

			bool		DoesWantToDie()				const				{ return mDoesWantToDie; }
			void		SetDoesWantToDie( bool inWants )				{ mDoesWantToDie = inWants; }

			int			GetNetworkId()				const				{ return mNetworkId; }
			void		SetNetworkId( int inNetworkId );

	virtual uint32_t	Write( OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState ) const	{ (void)inOutputStream; (void)inDirtyState; return 0; }
	virtual void		Read( InputMemoryBitStream& inInputStream )									{ (void)inInputStream; }


	

	virtual const float			GetWidth()								const	{ return mWidth; }
	virtual const float			GetHeight()								const	{ return mHeight; }
	virtual void				SetWidth(float width)							{ mWidth = width; }
	virtual void				SetHeight(float height)							{ mHeight = height; }
	virtual const uint8_t		GetHealth()								const	{ return 0; }
	virtual void				SetHealth(uint32_t input)						{}
	

private:
	//Type											mType
	//SDL_Rect										mRect{ 0.f,0.f,0.f,0.f};

	float											mWidth;
	float											mHeight;
	Type											mType;
	bool											mIsDirty = false;
	bool											mIsNew = false;

	Vector3											mLocation;
	Vector3											mColor;
	
	float											mCollisionRadius;


	float											mRotation;
	float											mScale;
	int												mIndexInWorld;

	bool											mDoesWantToDie;

	int												mNetworkId;

};


typedef shared_ptr< GameObject >	GameObjectPtr;