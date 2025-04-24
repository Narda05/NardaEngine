#include <NardaEngine/Inc/SpEngine.h>

class MainState : public NardaEngine::AppState
{
public:
	void Initialize()
	{
		LOG("MAIN STATE INITIALIZED");
		mLifeTime = 2.0f;
	}
	void Terminate()
	{
		LOG("MAIN STATE TERMINATED");
	}
	void Update(float deltaTime) override
	{
		mLifeTime -= deltaTime;
		if (mLifeTime <= 0.0f)
		{
			NardaEngine::MainApp().ChangeState("GameState");
		}
	}
private:
	float mLifeTime = 0.0f;
};

class GameState : public NardaEngine::AppState
{
public:
	void Initialize() override
	{
		LOG("GAME STATE INITIALIZED");
		mLifeTime = 2.0f;
	}
	void Terminate() override
	{
		LOG("GAME STATE TERMINATED");
	}
	void Update(float deltaTime) override
	{
		mLifeTime -= std::max(deltaTime, 0.01f);
		if (mLifeTime <= 0.0f)
		{
			LOG("GAMESTATE QUIT");
			NardaEngine::MainApp().ChangeState("MainState");
		}
	}
private:
	float mLifeTime = 0.0f;
};


int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	NardaEngine::AppConfig config;
	config.appName = L"Hello Window";

	NardaEngine::App& myApp = NardaEngine::MainApp();
	myApp.AddAppState<MainState>("MainState");
	myApp.AddAppState<GameState>("GameState");
	myApp.Run(config);

	return 0;
}