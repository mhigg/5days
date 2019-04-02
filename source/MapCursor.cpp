#include "DxLib.h"
#include "MapCursor.h"
#include "SceneMng.h"
#include "GameCtrl.h"
#include "MapCtrl.h"

#define EDIT_KEY_GET_DEF_RNG (15)

MapCursor::MapCursor()
{
	inputFlam = EDIT_KEY_GET_DEF_RNG;
	keyGetRng = EDIT_KEY_GET_DEF_RNG;
	warpFlag = false;

	keyTable = {
//			1P						2P
		KEY_INPUT_W,		KEY_INPUT_NUMPAD8,		// ��
		KEY_INPUT_S,		KEY_INPUT_NUMPAD5,		// ��
		KEY_INPUT_A,		KEY_INPUT_NUMPAD4,		// ��
		KEY_INPUT_D,		KEY_INPUT_NUMPAD6,		// �E
		KEY_INPUT_LCONTROL,	KEY_INPUT_RCONTROL		// �ړ����[�h�؂�ւ��p		
	};
	speedTable = { MOVE_SPEED, -MOVE_SPEED, MOVE_SPEED, -MOVE_SPEED };
}

MapCursor::MapCursor(VECTOR2 setUpPos, PL_NUMBER player, VECTOR2 drawOffset):Obj(drawOffset)
{
	pos = setUpPos;
	(*this).player = player;

	inputFlam = EDIT_KEY_GET_DEF_RNG;
	keyGetRng = EDIT_KEY_GET_DEF_RNG;
	warpFlag = false;

	keyTable = {
//			1P						2P
		KEY_INPUT_W,		KEY_INPUT_NUMPAD8,		// ��
		KEY_INPUT_S,		KEY_INPUT_NUMPAD5,		// ��
		KEY_INPUT_A,		KEY_INPUT_NUMPAD4,		// ��
		KEY_INPUT_D,		KEY_INPUT_NUMPAD6,		// �E
		KEY_INPUT_LCONTROL,	KEY_INPUT_RCONTROL		// �ړ����[�h�؂�ւ��p		
	};
	speedTable = { -MOVE_SPEED, MOVE_SPEED, -MOVE_SPEED, MOVE_SPEED };
}


MapCursor::~MapCursor()
{
}

void MapCursor::Draw(void)
{
	DrawBox(pos.x, pos.y, pos.x + 50, pos.y + 50, 0x0000ff00, true);
}

void MapCursor::SetMove(const GameCtrl & controller, weakListObj objList)
{
	auto ctrl = controller.GetCtrl(KEY_TYPE_NOW);
	auto ctrlOld = controller.GetCtrl(KEY_TYPE_OLD);

	VECTOR2 tmpPos(pos);
	DIR_TBL_PTR_ARY tmpTable = { &tmpPos.y,0, &tmpPos.y,0, &tmpPos.x,0, &tmpPos.x,0 };

	auto Move = [&](DIR dir)
	{
		if (ctrl[keyTable[dir][player]])
		{
			(*tmpTable[dir][TBL_MAIN]) += speedTable[dir];
		}
	};

	// �ړ�����
	for (int j = 0; j < DIR_MAX; j++)
	{
		Move(static_cast<DIR>(j));
	}

	VECTOR2 gameScrSize = lpSceneMng.GetGameScreenSize();

	if ((tmpPos.x < gameScrSize.x) & (tmpPos.x >= 0)
	 &  (tmpPos.y < gameScrSize.y) & (tmpPos.y >= 0))
	{
		if (tmpPos != pos)
		{
			inputFlam++;
			if (inputFlam >= keyGetRng)
			{
				// 1�񉟂���
				// �����ɗ��邽�тɾ�۸ر�Ɗ�ꐔ�̔���
				pos = tmpPos;
				inputFlam = 0;
			}
		}
		else
		{
			// ���𗣂��Ă���Ƃ�
			inputFlam = EDIT_KEY_GET_DEF_RNG;
		}
	}

	if (ctrl[keyTable[PL_KEY_MODE][player]] & ~ctrlOld[keyTable[PL_KEY_MODE][player]])
	{
		if (lpMapCtrl.SetMapData(pos, static_cast<MAP_ID>(player + 1))
		 && lpMapCtrl.GetMapData(pos) == static_cast<MAP_ID>(player + 1))
		{
			warpFlag = true;
		}
	}
}

bool MapCursor::CheckDeath(void)
{
	return warpFlag;
}
