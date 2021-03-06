#pragma once
#include "BaseScene.h"
#include "GameCtrl.h"
#include "classObj.h"

#define GAME_TIME 90

typedef struct {
	int allTime;	// 秒数のみ
	int minute;		// 分
	int second;		// 秒
} GameTime;

class Player;

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();

	unique_Base UpDate(unique_Base own, const GameCtrl & controller);		// ゲーム中の動作処理

private:
	int Init();		// ゲームに関する初期化

	void Draw();	// ゲーム中の描画

	GameTime gameTime;	// 制限時間

	sharedListObj objList;	//obj用　ﾘｽﾄ
};