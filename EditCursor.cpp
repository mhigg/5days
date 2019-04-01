#include "DxLib.h"
#include "SceneMng.h"
#include "GameController.h"
#include "EditCursor.h"


EditCursor::EditCursor(VECTOR2 drawOffset):Obj(drawOffset)	// Obj∏◊ΩÇÃà¯êîïtÇ´∫›Ωƒ◊∏¿ÇåƒÇ‘(èâä˙âªéq)
{
	id = MAP_ID::WALL1;
	keyGetRng = EDIT_KEY_GET_DEF_RNG;
	inputFlam = EDIT_KEY_GET_DEF_RNG;
	mouseFlag = false;
}

EditCursor::EditCursor()
{
	id = MAP_ID::WALL1;
	keyGetRng = EDIT_KEY_GET_DEF_RNG;
	inputFlam = EDIT_KEY_GET_DEF_RNG;
	mouseFlag = false;
}

EditCursor::~EditCursor()
{
}

void EditCursor::Draw(void)
{
	if (mouseFlag)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		Obj::DrawMouseMode(static_cast<int>(id));
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, abs((int)(animCnt % 512) - 256) );
		Obj::DrawMouseMode(static_cast<int>(MAP_ID::CUR));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		animCnt += 10;
	}
	if (!mouseFlag)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		Obj::Draw(static_cast<int>(id));
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, abs((int)(animCnt % 512) - 256) );
		Obj::Draw(static_cast<int>(MAP_ID::CUR));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		animCnt += 10;
	}
}

void EditCursor::SetMove(const GameController &controller, weakListObj objList)		// constÇÇ¬ÇØÇÈÇ∆ÇªÇÍÇçûÇ›Ç≈å^èCè¸éqÇ…Ç»ÇÈ
{
	auto cnt	  = controller.GetCtl(KEY_TYPE_NOW);		// auto = KEY_ARRAY
	auto cntOld	  = controller.GetCtl(KEY_TYPE_OLD);
	auto cntMouse = controller.GetMouseCtl();
	VECTOR2 tmpPos(pos);

	if (cnt[KEY_INPUT_LCONTROL] & (~cntOld[KEY_INPUT_LCONTROL]))
	{
		id = (MAP_ID)((static_cast<int>(id)) + 1
		  >= (static_cast<int>(MAP_ID::MAX))
		   ? (static_cast<int>(MAP_ID::BOMB)) : (static_cast<int>(id)) + 1);
		/*id = MAP_ID(id + 1);
		if (id >= MAP_ID_MAX)
		{
			id = MAP_ID_NON;
		}*/
	}

	if (mouseFlag)
	{
		GetMousePoint(&pos.x, &pos.y);
		int Wheel = GetMouseWheelRotVol();
		if (Wheel > 0)
		{
			id = (MAP_ID)((static_cast<int>(id)) - 1 < (static_cast<int>(MAP_ID::BOMB)) ? (static_cast<int>(MAP_ID::ITEM_SPEED)) : (static_cast<int>(id)) - 1);
		}
		if (Wheel < 0)
		{
			id = (MAP_ID)((static_cast<int>(id)) + 1 >= (static_cast<int>(MAP_ID::MAX)) ? (static_cast<int>(MAP_ID::BOMB)) : (static_cast<int>(id)) + 1);
		}
		if (cntMouse)
		{
			lpMapCtl.SetMapData(VECTOR2(pos.x - lpSceneMng.GetDrawOffset().x,
										pos.y - lpSceneMng.GetDrawOffset().y), id);
		}
	}
	else 
	{
		if (cnt[KEY_INPUT_NUMPAD4])
		{
			tmpPos.x -= divSize.x;
		}
		if (cnt[KEY_INPUT_NUMPAD6])
		{
			tmpPos.x += divSize.x;
		}
		if (cnt[KEY_INPUT_NUMPAD8])
		{
			tmpPos.y -= divSize.y;
		}
		if (cnt[KEY_INPUT_NUMPAD2])
		{
			tmpPos.y += divSize.y;
		}

		if (tmpPos != pos)
		{
			inputFlam++;
			if (inputFlam >= keyGetRng)
			{
				pos = tmpPos;
				inputFlam  = 0;
				keyGetRng /= 2;
				if (keyGetRng < MIN_KEY_RNG)
				{
					keyGetRng = MIN_KEY_RNG;
				}
			}
		}
		else
		{
			keyGetRng = EDIT_KEY_GET_DEF_RNG;
			inputFlam = EDIT_KEY_GET_DEF_RNG;
		}
		if (cnt[KEY_INPUT_SPACE]/* & (~cntOld[KEY_INPUT_SPACE])*/)
		{
			lpMapCtl.SetMapData(pos, id);
		}
	}
	ChangeMode(controller);
}

void EditCursor::ChangeMode(const GameController & controller)
{
	auto cnt	= controller.GetCtl(KEY_TYPE_NOW);		// auto = KEY_ARRAY
	auto cntOld = controller.GetCtl(KEY_TYPE_OLD);

	if (cnt[KEY_INPUT_K] & (~cntOld[KEY_INPUT_K]))
	{
		mouseFlag = false;
		pos = VECTOR2(0, 0);
	}
	if (cnt[KEY_INPUT_M] & (~cntOld[KEY_INPUT_M]))
	{
		mouseFlag = true;
	}
}

bool EditCursor::CheckObjType(OBJ_TYPE type)
{
	return (type == OBJ_EDIT_CUR);
}
