#include "DxLib.h"
#include "ResultScene.h"
#include "GameScene.h"
#include "StageMng.h"
#include "GameCtrl.h"
#include "MapCtrl.h"



ResultScene::ResultScene()
{
	Init();
}


ResultScene::~ResultScene()
{
}

unique_Base ResultScene::UpDate(unique_Base own, const GameCtrl & controller)
{
	auto ctrl = controller.GetCtrl(KEY_TYPE_NOW);
	auto ctrlOld = controller.GetCtrl(KEY_TYPE_OLD);

	if (ctrl[KEY_INPUT_SPACE] & ~(ctrlOld[KEY_INPUT_SPACE]))
	{
		return std::make_unique<GameScene>();
	}

	

	Draw();

	return std::move(own);
}

int ResultScene::Init(void)
{
	lpStageMng.calcArea();

	int mapCnt1P = lpStageMng.GetMapCnt()[0];
	int mapCnt2P = lpStageMng.GetMapCnt()[1];
	return 0;
}

void ResultScene::Draw(void)
{
	ClsDrawScreen();

	ScreenFlip();
}
