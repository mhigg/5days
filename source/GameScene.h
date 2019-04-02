#pragma once
#include "BaseScene.h"
#include "GameCtrl.h"
#include "classObj.h"

#define GAME_TIME 90

typedef struct {
	int allTime;	// •b”‚Ì‚İ
	int minute;		// •ª
	int second;		// •b
} GameTime;

class Player;

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();

	unique_Base UpDate(unique_Base own, const GameCtrl & controller);		// ƒQ[ƒ€’†‚Ì“®ìˆ—

private:
	int Init();		// ƒQ[ƒ€‚ÉŠÖ‚·‚é‰Šú‰»

	void Draw();	// ƒQ[ƒ€’†‚Ì•`‰æ

	GameTime gameTime;	// §ŒÀŠÔ

	sharedListObj objList;	//obj—p@Ø½Ä
};