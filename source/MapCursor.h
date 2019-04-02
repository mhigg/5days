#pragma once
#include <array>
#include "Obj.h"
#include "MapCtrl.h"
#include "Player.h"
#include "classObj.h"
#include "VECTOR2.h"

#define MOVE_SPEED (50)

class MapCursor :
	public Obj
{
public:
	MapCursor();
	MapCursor(VECTOR2 setUpPos, PL_NUMBER player, VECTOR2 drawOffset);		// MAP_ID,inputFlam,keyGetRng,drawOffsetを初期化する
	~MapCursor();
	void Draw(void);					// ｶｰｿﾙの点滅処理 SetDrawBlendMode関数使用

private:
	void SetMove(const GameCtrl &controller, weakListObj objList);
	bool CheckDeath(void);		// ﾜｰﾌﾟﾓｰﾄﾞ中であればfalse

	int keyGetRng;	// 基底ﾌﾚｰﾑ数　ｷｰを離したら初期化
	int inputFlam;	// ｷｰを押しているﾌﾚｰﾑ数　押してないときｾﾞﾛｸﾘｱ
	bool warpFlag;

	DIR_TBL_PL_ARY keyTable;	// 入力したｷｰの方向　speedTableと方向を合わせる
	DIR_TBL_ARY speedTable;		// ﾌﾞﾛｯｸの移動ｽﾋﾟｰﾄﾞ dirTableと方向を合わせる

	PL_NUMBER player;
};

