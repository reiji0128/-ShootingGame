#include "Game.h"
#include "TitleScene.h"

int main(int argc, char** argv)
{
	if (!GAMEINSTANCE.Initialize(1920,1080,true))
	{
		return -1;
	}
	GAMEINSTANCE.setFirstScene(new TitleScene);
	GAMEINSTANCE.GetRenderer()->SetWindowTitle("Game");
	GAMEINSTANCE.Run();
	GAMEINSTANCE.Shutdown();
	return 0;
}