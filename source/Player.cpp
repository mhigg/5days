#include "DxLib.h"
#include "Obj.h"
#include "Player.h"
#include "GameCtrl.h"
#include "MapCtrl.h"
#include "MAP_ID.h"
#include "VECTOR2.h"


Player::Player()
{
	keyTbl = {
//			1P					2P
		KEY_INPUT_W,	KEY_INPUT_NUMPAD8,		// 上
		KEY_INPUT_S,	KEY_INPUT_NUMPAD5,		// 下
		KEY_INPUT_A,	KEY_INPUT_NUMPAD4,		// 左
		KEY_INPUT_D,	KEY_INPUT_NUMPAD6		// 右
	};

	posTbl = {
//		MAIN			SUB
		&pos.y,			&pos.x,			// 上
		&pos.y,			&pos.x,			// 下
		&pos.x,			&pos.y,			// 左
		&pos.x,			&pos.y			// 右
	};

	speedTbl = {
		-DEF_SPEED,		// 上
		 DEF_SPEED		// 下
		-DEF_SPEED,		// 左
		 DEF_SPEED		// 右
	};

	dirTbl = {
//		MAIN			OPP				SUB1			SUB2
		DIR_UP,			DIR_DOWN,		DIR_LEFT,		DIR_RIGHT,	// 上
		DIR_DOWN,		DIR_UP,			DIR_LEFT,		DIR_RIGHT,	// 下
		DIR_LEFT,		DIR_RIGHT,		DIR_UP,			DIR_DOWN,	// 左
		DIR_RIGHT,		DIR_LEFT,		DIR_UP,			DIR_DOWN	// 右
	};

	dir = DIR_DOWN;
}


Player::Player(PL_NUMBER player, VECTOR2 drawOffset) :Obj(drawOffset)
{
	keyTbl = {
//			1P						2P
		KEY_INPUT_W,		KEY_INPUT_NUMPAD8,		// 上
		KEY_INPUT_S,		KEY_INPUT_NUMPAD5,		// 下
		KEY_INPUT_A,		KEY_INPUT_NUMPAD4,		// 左
		KEY_INPUT_D,		KEY_INPUT_NUMPAD6,		// 右
		KEY_INPUT_LSHIFT,	KEY_INPUT_RETURN		// 移動モード切り替え用		
	};

	posTbl = {
//		MAIN			SUB
		&tmpPos.y,		&tmpPos.x,			// 上
		&tmpPos.y,		&tmpPos.x,			// 下
		&tmpPos.x,		&tmpPos.y,			// 左
		&tmpPos.x,		&tmpPos.y			// 右
	};

	speedTbl = {
		-DEF_SPEED,		// 上
		 DEF_SPEED,		// 下
		-DEF_SPEED,		// 左
		 DEF_SPEED		// 右
	};

	dirTbl = {
//		MAIN			OPP				SUB1			SUB2
		DIR_UP,			DIR_DOWN,		DIR_LEFT,		DIR_RIGHT,	// 上
		DIR_DOWN,		DIR_UP,			DIR_LEFT,		DIR_RIGHT,	// 下
		DIR_LEFT,		DIR_RIGHT,		DIR_UP,			DIR_DOWN,	// 左
		DIR_RIGHT,		DIR_LEFT,		DIR_UP,			DIR_DOWN	// 右
	};

	dir = DIR_DOWN;
	(*this).player = player;
}


Player::~Player()
{
}

void Player::SetMove(const GameCtrl & controller)
{
	auto ctrl = controller.GetCtrl(KEY_TYPE_NOW);
	auto ctrlOld = controller.GetCtrl(KEY_TYPE_OLD);

	// 移動モードの切り替え
	if (ctrl[keyTbl[PL_KEY_MODE][player]] & ~ctrlOld[keyTbl[PL_KEY_MODE][player]])
	{

		modeFlag = true;
	}

	tmpPos = pos;

	// プレイヤーの移動処理
	auto Move = [&,dir = Player::dir](int keyState, int keyStateOld, int id) {
		if (keyState & ~keyStateOld)
		{
			Player::dir = static_cast<DIR>(id);

			(*posTbl[Player::dir][TBL_MAIN]) += speedTbl[Player::dir];
		}
	};

	for (int id = 0; id < PL_KEY_MAX - 1; id++)
	{
		Move(ctrl[keyTbl[id][player]], ctrlOld[keyTbl[id][player]], id);
	}

	if (pos != tmpPos)
	{
		if (tmpPos >= VECTOR2(0, 0) && tmpPos <= VECTOR2(550, 550))
		{
			pos = tmpPos;
			lpMapCtrl.SetMapData(pos, static_cast<MAP_ID>(player + 1));
		}
	}
}
