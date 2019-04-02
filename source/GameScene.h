#pragma once
#include "BaseScene.h"
#include "GameCtrl.h"
#include "classObj.h"

#define GAME_TIME 90

typedef struct {
	int allTime;	// �b���̂�
	int minute;		// ��
	int second;		// �b
} GameTime;

class Player;

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();

	unique_Base UpDate(unique_Base own, const GameCtrl & controller);		// �Q�[�����̓��쏈��

private:
	int Init();		// �Q�[���Ɋւ��鏉����

	void Draw();	// �Q�[�����̕`��

	GameTime gameTime;	// ��������

	sharedListObj objList;	//obj�p�@ؽ�
};