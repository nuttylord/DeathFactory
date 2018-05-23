class Client : public Engine
{
public:

	static bool StaticInit( );

protected:
	void updateEnvironmentTextures();
	Client();

	//virtual int Run();

	virtual void	DoFrame() override;
	virtual void	HandleEvent( SDL_Event* inEvent ) override;

private:

	bool mupdated;

};