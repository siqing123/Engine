
#include "GameState.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	using namespace Storm;

	AppConfig appConfig;
	appConfig.appName = L"Test Game";
	appConfig.debugDrawLimit = 1000000;

	MainApp().AddState<GameState>("Game");
	MainApp().Run(appConfig);

	return 0;
}