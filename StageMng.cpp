#include "StageMng.h"
#include "MapCtrl.h"



void StageMng::calcArea(void)
{
	for (int y = 0; y < mapSize.y; y++)
	{
		for (int x = 0; x < mapSize.x; x++)
		{
			MAP_ID id = lpMapCtrl.GetMapData(VECTOR2(x, y));
			if (id != MAP_ID::NON)
			{
				mapCnt[static_cast<PL_NUMBER>(id) - 1]++;
			}
		}
	}
}

MAP_ARY StageMng::GetMapCnt(void)
{
	return mapCnt;
}

void StageMng::ResetMapCnt(void)
{
	mapCnt[0] = 0;
	mapCnt[1] = 0;
}

void StageMng::AddMapCnt(PL_NUMBER player, VECTOR2 pos)
{
	mapCnt[player]++;
	MAP_ID id = lpMapCtrl.GetMapData(pos / lpMapCtrl.GetChipSize());
	if (id == static_cast<MAP_ID>((1 ^ player) + 1))
	{
		mapCnt[1 - player]--;
	}
}

void StageMng::SetMapSize(VECTOR2 mapSize)
{
	this->mapSize = mapSize;
}


StageMng::StageMng()
{
	ResetMapCnt();
}