#include "DxLib.h"
#include "Obj.h"
#include "Player.h"
#include "GameCtrl.h"
#include "MapCtrl.h"
#include "MapCursor.h"
#include "SceneMng.h"
#include "MAP_ID.h"
#include "VECTOR2.h"


Player::Player()
{
	keyTbl = {
//			1P					2P
		KEY_INPUT_W,	KEY_INPUT_NUMPAD8,		// ��
		KEY_INPUT_S,	KEY_INPUT_NUMPAD5,		// ��
		KEY_INPUT_A,	KEY_INPUT_NUMPAD4,		// ��
		KEY_INPUT_D,	KEY_INPUT_NUMPAD6		// �E
	};

	posTbl = {
//		MAIN			SUB
		&pos.y,			&pos.x,			// ��
		&pos.y,			&pos.x,			// ��
		&pos.x,			&pos.y,			// ��
		&pos.x,			&pos.y			// �E
	};

	speedTbl = {
		-DEF_SPEED,		// ��
		 DEF_SPEED		// ��
		-DEF_SPEED,		// ��
		 DEF_SPEED		// �E
	};

	dirTbl = {
//		MAIN			OPP				SUB1			SUB2
		DIR_UP,			DIR_DOWN,		DIR_LEFT,		DIR_RIGHT,	// ��
		DIR_DOWN,		DIR_UP,			DIR_LEFT,		DIR_RIGHT,	// ��
		DIR_LEFT,		DIR_RIGHT,		DIR_UP,			DIR_DOWN,	// ��
		DIR_RIGHT,		DIR_LEFT,		DIR_UP,			DIR_DOWN	// �E
	};

	dir = DIR_DOWN;
}


Player::Player(PL_NUMBER player, VECTOR2 drawOffset) :Obj(drawOffset)
{
	skillTbl = {
//			1P					2P
		KEY_INPUT_Z,	KEY_INPUT_NUMPAD3,	//SKILL_BOMB
		KEY_INPUT_X,	KEY_INPUT_NUMPAD2,	//SKILL_STAMP
		KEY_INPUT_C,	KEY_INPUT_NUMPAD1	//SKILL_NOENTRY
	};

	keyTbl = {
//			1P						2P
		KEY_INPUT_W,		KEY_INPUT_NUMPAD8,		// ��
		KEY_INPUT_S,		KEY_INPUT_NUMPAD5,		// ��
		KEY_INPUT_A,		KEY_INPUT_NUMPAD4,		// ��
		KEY_INPUT_D,		KEY_INPUT_NUMPAD6,		// �E
	};

	posTbl = {
//		MAIN			SUB
		&tmpPos.y,		&tmpPos.x,			// ��
		&tmpPos.y,		&tmpPos.x,			// ��
		&tmpPos.x,		&tmpPos.y,			// ��
		&tmpPos.x,		&tmpPos.y			// �E
	};

	speedTbl = {
		-DEF_SPEED,		// ��
		 DEF_SPEED,		// ��
		-DEF_SPEED,		// ��
		 DEF_SPEED		// �E
	};

	dirTbl = {
//		MAIN			OPP				SUB1			SUB2
		DIR_UP,			DIR_DOWN,		DIR_LEFT,		DIR_RIGHT,	// ��
		DIR_DOWN,		DIR_UP,			DIR_LEFT,		DIR_RIGHT,	// ��
		DIR_LEFT,		DIR_RIGHT,		DIR_UP,			DIR_DOWN,	// ��
		DIR_RIGHT,		DIR_LEFT,		DIR_UP,			DIR_DOWN	// �E
	};

	dir = DIR_DOWN;
	(*this).player = player;
}


Player::~Player()
{
}

void Player::SetMove(const GameCtrl & controller, weakListObj objList)
{
	auto ctrl = controller.GetCtrl(KEY_TYPE_NOW);
	auto ctrlOld = controller.GetCtrl(KEY_TYPE_OLD);

	if (!modeFlag)
	{
		// ���[�v���[�h ��������
		//if (ctrl[keyTbl[PL_KEY_MODE][player]] & ~ctrlOld[keyTbl[PL_KEY_MODE][player]])
		//{
		//	AddObjList()(objList, std::make_unique<MapCursor>(pos, player, lpSceneMng.GetDrawOffset()));
		//	modeFlag = true;
		//	return;
		//}

		tmpPos = pos;

		// �v���C���[�̈ړ�����
		auto Move = [&, dir = Player::dir](int keyState, int keyStateOld, int id) {
			if (keyState & ~keyStateOld)
			{
				Player::dir = static_cast<DIR>(id);

				(*posTbl[Player::dir][TBL_MAIN]) += speedTbl[Player::dir];
			}
		};

		for (int id = 0; id < DIR_MAX; id++)
		{
			Move(ctrl[keyTbl[id][player]], ctrlOld[keyTbl[id][player]], id);
		}

		if (pos != tmpPos)
		{
			if (lpMapCtrl.SetMapData(pos, static_cast<MAP_ID>(player + 1)))
			{
				pos = tmpPos;
			}
		}
	}
	//else
	//{
		//if (ctrl[keyTbl[PL_KEY_MODE][player]] & ~ctrlOld[keyTbl[PL_KEY_MODE][player]])
		//{
		//	modeFlag = false;
		//	return;
		//}
	//}


}
