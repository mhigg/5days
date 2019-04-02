#include "DxLib.h"
#include "SceneMng.h"
#include "BaseScene.h"
#include "GameScene.h"
#include "GameCtrl.h"

#define SCREEN_SIZE_X 800			//‰æ–Ê‰ð‘œ“x
#define SCREEN_SIZE_Y 600


SceneMng::SceneMng()
{
	Init();
}

void SceneMng::Run(void)
{
	activeScene = std::make_unique<GameScene>();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		gameCtrl->UpDate();
		activeScene = activeScene->UpDate(std::move(activeScene), (*gameCtrl));
		framCnt++;
	}
}

VECTOR2 SceneMng::GetScreenSize(void)
{
	return VECTOR2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
}

VECTOR2 SceneMng::GetGameScreenSize(void)
{
	return VECTOR2(GAME_SCREEN_SIZE_X, GAME_SCREEN_SIZE_Y);
}

VECTOR2 SceneMng::GetDrawOffset(void)
{
	return drawOffset;
}

void SceneMng::SetDrawOffset(VECTOR2 offset)
{
	drawOffset = offset;
}

void SceneMng::SetFrame(int fram)
{
	framCnt = fram;
}

int SceneMng::GetFrame(void)
{
	return framCnt;
}




int SceneMng::Init(void)
{
	SetWindowText("EncampmentGame");
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return - 1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	gameCtrl = std::make_unique<GameCtrl>();

	framCnt = 0;

	return 0;
}