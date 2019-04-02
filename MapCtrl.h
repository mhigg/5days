#pragma once
#include <vector>
#include "VECTOR2.h"
#include "classObj.h"
#include "MAP_ID.h"

//短縮定義
#define lpMapCtrl MapCtrl::GetInstance()
#define lpGetChipSize MapCtrl::GetInstance().GetChipSize()

//using ListObj_itr = uniqueObjList::iterator;

enum DIR
{
	DIR_UP,
	DIR_DOWN,
	DIR_RIGHT,
	DIR_LEFT,
	DIR_MAX
};


class MapCtrl
{
public:
	static MapCtrl & GetInstance(void)
	{
		static MapCtrl s_instance;
		return s_instance;
	}

	VECTOR2 & GetChipSize(void);

	//mapSize,chipSize,drawOffsetの取得と初期化,ﾏｯﾌﾟﾃﾞｰﾀ配列の初期化
	bool SetUp(const VECTOR2& size, const VECTOR2& chipSize, const VECTOR2 drawOffset);
	
	//SetMove関数で取得した情報をmapDataに保存
	bool SetMapData(const VECTOR2& pos, MAP_ID id);

	MAP_ID GetMapData(VECTOR2 mapPos);
	
	//取得した情報(mapData)をもとに描画 modeFlag:true→EditMode false→GameMode
	void Draw(void);

	bool SetUpGameObj(sharedListObj objList);

private:
	MapCtrl();
	~MapCtrl();

	std::vector<MAP_ID*> mapData;		//配列全域 二次元目
	std::vector<MAP_ID> mapData_Base;	//ﾎﾟｲﾝﾀ用　一次元目

	VECTOR2 mapSize;		//ﾏｯﾌﾟのﾏｽ目数 横方向はx 縦方向はy
	VECTOR2 chipSize;		//1ﾏｽのｻｲｽﾞ
	VECTOR2 drawOffset;		//描画時のｵﾌｾｯﾄ

	int bgImage;
};