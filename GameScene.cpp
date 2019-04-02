#include "DxLib.h"
#include "SceneMng.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "Obj.h"
#include "Player.h"
#include "MapCtrl.h"
#include "GameCtrl.h"
#include "MAP_ID.h"
#include "classObj.h"



GameScene::GameScene()
{
	Init();
}


GameScene::~GameScene()
{
}

int GameScene::Init()
{
	if (!objList)
	{
		objList = std::make_shared<uniqueObjList>();
	}
	objList->clear();
	lpSceneMng.SetDrawOffset(VECTOR2(GAME_SCREEN_X, GAME_SCREEN_Y));
	lpMapCtrl.SetUp(VECTOR2(GAME_SCREEN_SIZE_X, GAME_SCREEN_SIZE_Y), VECTOR2(CHIP_SIZE, CHIP_SIZE), lpSceneMng.GetDrawOffset());
	lpMapCtrl.SetMapData(VECTOR2(0, 0), MAP_ID::P1);
	lpMapCtrl.SetMapData(VECTOR2(550, 550), MAP_ID::P2);
	lpMapCtrl.SetUpGameObj(objList);

	lpSceneMng.SetFrame(0);
	gameTime.allTime = GAME_TIME;

	return 0;
}

void GameScene::Draw()
{
	ClsDrawScreen();

	lpMapCtrl.Draw();

	for (auto itr = objList->begin(); itr != objList->end(); itr++)
	{
		(*itr)->Draw();
	}
	
	for (int y = 50; y < 600; y += 50)
	{
		DrawLine(0, y, 600, y, 0x00ffffff);
	}
	for (int x = 50; x <= 600; x += 50)
	{
		DrawLine(x, 0, x, 600, 0x00ffffff);
	}

	DrawFormatString(600, 0, 0x00ffffff, "残り %d 分 %d 秒", gameTime.minute, gameTime.second);

	ScreenFlip();
}

unique_Base GameScene::UpDate(unique_Base own, const GameCtrl & controller)
{
	auto ctrl = controller.GetCtrl(KEY_TYPE_NOW);
	auto ctrlOld = controller.GetCtrl(KEY_TYPE_OLD);

	if (ctrl[KEY_INPUT_SPACE] & ~(ctrlOld[KEY_INPUT_SPACE]))
	{
		// 終了の合図から何秒か後に移行する
		return std::make_unique<ResultScene>();
	}

	if (gameTime.allTime > 0)
	{
		// プレイヤーの動作処理
		for (auto itr = objList->begin(); itr != objList->end(); itr++)
		{
			(*itr)->UpDate(controller);
		}

		Draw();

		gameTime.allTime = GAME_TIME - lpSceneMng.GetFrame() / 60;
		gameTime.minute = gameTime.allTime / 60;
		gameTime.second = gameTime.allTime % 60;
	}
	else
	{
		// 終了の合図になるものを表示する
		Draw();
	}

	return move(own);
}
