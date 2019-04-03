#pragma once
#include "MapCtrl.h"
#include "Player.h"
#include "VECTOR2.h"

#define lpStageMng StageMng::GetInstance()

using MAP_ARY = std::array<int, PL_MAX>;

class StageMng
{
public:
	static StageMng& GetInstance(void)
	{
		static StageMng s_instance;
		return s_instance;
	}

	// 陣地ﾏｽ数計算
	void calcArea(void);

	// 陣地数の取得
	MAP_ARY GetMapCnt(void);

	// 陣地数をゼロクリアする
	void ResetMapCnt(void);

	// プレイヤーごとに陣地数を加算する
	void AddMapCnt(PL_NUMBER player, VECTOR2 pos);

	// 陣地数計算のためのﾏｯﾌﾟｻｲｽﾞを設定
	void SetMapSize(VECTOR2 mapSize);

private:
	StageMng();

	MAP_ARY mapCnt;		// 1Pの陣地数
	VECTOR2 mapSize;	// ﾏｯﾌﾟｻｲｽﾞ
	VECTOR2 chipSize;
};

