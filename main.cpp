#include "Windows.h"
#include "DxLib.h"
#include "SceneMng.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SceneMng::GetInstance().Run();

	DxLib_End();	// DX×²ÌŞ×Ø‚ÌI—¹ˆ—
	return 0;
}