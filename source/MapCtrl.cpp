#include <memory>
#include "DxLib.h"
#include "ImageMng.h"
#include "SceneMng.h"
#include "Player.h"
#include "MapCtrl.h"
#include "MAP_ID.h"
#include "classObj.h"


VECTOR2 & MapCtrl::GetChipSize(void)
{
	return MapCtrl::chipSize;
}

bool MapCtrl::SetUp(const VECTOR2 & size, const VECTOR2 & chipSize, const VECTOR2 drawOffset)
{
	//��ʉ𑜓x:size Ͻ�ڂ̻���:chipSize
	MapCtrl::mapSize = size / chipSize;
	MapCtrl::chipSize = chipSize;
	MapCtrl::drawOffset = drawOffset;

	mapData_Base.resize(mapSize.x * mapSize.y);
	mapData.resize(MapCtrl::mapSize.y);
	//mapData��mapData_Base�̘A��
	for (unsigned int x = 0; x < mapData.size(); x++)
	{
		//mapSize.x��������ʂ�����
		mapData[x] = (&mapData_Base[mapSize.x * x]);
	}
	//�Ə�����
	for (unsigned int j = 0; j < mapData_Base.size(); j++)
	{
		//�S�̂�NON������
		mapData_Base[j] = MAP_ID::NON;
	}

	if (bgImage <= 0)
	{
		VECTOR2 scrSize = lpSceneMng.GetScreenSize();
		bgImage = MakeScreen(scrSize.x, scrSize.y, false);
	}

	return true;
}

bool MapCtrl::SetMapData(const VECTOR2& pos, MAP_ID id)
{
	OutputDebugString("SetMapData�J�n");
	VECTOR2 mapPos(pos / chipSize);		//�������g
	if ((mapPos.x < 0)			 || (mapPos.y < 0)
	 || (mapPos.x >= mapSize.x)  || (mapPos.y >= mapSize.y))
	{
		return false;
	}

	mapData[mapPos.y][mapPos.x] = id;
	OutputDebugString("SetMapData�I��");

	return true;
}

MAP_ID MapCtrl::GetMapData(VECTOR2 mapPos)
{
	return mapData[mapPos.y][mapPos.x];
}

void MapCtrl::Draw(void)
{
	SetDrawScreen(bgImage);
	for (int y = 0; y < mapSize.y; y++)
	{
		for (int x = 0; x < mapSize.x; x++)
		{
			MAP_ID id = mapData[y][x];
			ListObj_itr obj;
			switch (id)
			{
			case MAP_ID::NON:
				//��ɕ`�悵�Ȃ�
				break;
			case MAP_ID::P1:
				// ���ꂼ��̃v���C���[�̐F�Ń}�X��`��
				DrawBox(
					drawOffset.x + (x * chipSize.x),
					drawOffset.y + (y * chipSize.y),
					drawOffset.x + (x * chipSize.x) + 50,
					drawOffset.y + (y * chipSize.y) + 50,
					0x00ff0000,
					true
				);
				break;
			case MAP_ID::P2:
				DrawBox(
					drawOffset.x + (x * chipSize.x),
					drawOffset.y + (y * chipSize.y),
					drawOffset.x + (x * chipSize.x) + 50,
					drawOffset.y + (y * chipSize.y) + 50,
					0x000000ff,
					true
				);
				break;
			default:
				//�װ�\��
#ifdef _DEBUG
				DrawBox(
					drawOffset.x + (x * chipSize.x),
					drawOffset.y + (y * chipSize.y),
					drawOffset.x + (x * chipSize.x) + 50,
					drawOffset.y + (y * chipSize.y) + 50,
					0x00ffffff,
					true
				);
#endif
				break;
			}
		}
	}
	SetDrawScreen(DX_SCREEN_BACK);

	DrawGraph(0, 0, bgImage, true);
}

bool MapCtrl::SetUpGameObj(sharedListObj objList)
{
	bool makePlayerFlag = false;
	for (int y = 0; y < mapSize.y; y++)
	{
		for (int x = 0; x < mapSize.x; x++)
		{
			MAP_ID id = mapData[y][x];
			ListObj_itr obj;
			switch (id)
			{
			case MAP_ID::P1:
				obj = AddObjList()(objList, std::make_unique<Player>(PL_1, drawOffset + VECTOR2(0, 0)));
				(*obj)->init("image/player.png", VECTOR2(50, 50), VECTOR2(1, 1), chipSize * VECTOR2(x, y));
				break;
			case MAP_ID::P2:
				if (makePlayerFlag)
				{
					break;
				}
				obj = AddObjList()(objList, std::make_unique<Player>(PL_2, drawOffset + VECTOR2(0, 0)));
				(*obj)->init("image/player.png", VECTOR2(50, 50), VECTOR2(1, 1), chipSize * VECTOR2(x, y));
				break;
			case MAP_ID::NON:
				//�������Ȃ�
				break;
			default:
				break;
			}
		}
	}

	return true;
}


MapCtrl::MapCtrl()
{
}


MapCtrl::~MapCtrl()
{
}