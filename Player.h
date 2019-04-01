#pragma once
//#include <type_traits>
#include <memory>
#include <array>
#include "Obj.h"
#include "VECTOR2.h"
#include "MapCtl.h"

enum DIR_TBL_ID {
	DIR_TBL_MAIN,			// 押しているｷｰの方向
	DIR_TBL_OPP,			// DIR_TBL_MAINの反対方向
	DIR_TBL_SUB1,			// 軸が違う方向１
	DIR_TBL_SUB2,			// 軸が違う方向２
	DIR_TBL_MAX
};

enum TBL_ID {
	TBL_MAIN,
	TBL_SUB,
	TBL_MAX
};

enum SIDE_CHECK {
	OUT_SIDE,
	IN_SIDE,
	SIDE_MAX
};

enum class MOVE_STATE {
	ALL_OK,
	ALL_NG,
	WALL_PASS,
	MAX
};

#define PL_DEF_SPEED 2		// ﾌﾟﾚｲﾔｰの標準ｽﾋﾟｰﾄﾞ

using DIR_TBL_ARRAY	    = std::array<int, DIR_MAX>;
using DIR_TBL_PTR_ARRAY = std::array<int*[TBL_MAX], DIR_MAX>;
using DIR_TBL_DIR_ARRAY = std::array<DIR[DIR_TBL_MAX], DIR_MAX>;
using MAP_MOVE_TBL		= std::array</*MOVE_STATE*/bool, static_cast<int>(MAP_ID::MAX)>;	// static_cast<ｷｬｽﾄしたい型>

class Player :
	public Obj
{
public:
	Player();
	Player(VECTOR2 setUpPos,
		VECTOR2 drawOffset);
	~Player();
	bool InitAnim(void);	// ｱﾆﾒｰｼｮﾝ登録
private:
	void SetMove(const GameController &controller,
		weakListObj objList);

	bool CheckObjType(OBJ_TYPE type);	// ｵﾌﾞｼﾞｪｸﾄのﾀｲﾌﾟをﾁｪｯｸ
	bool DeathPrc(void);
	void GetItem(void);

	DIR_TBL_ARRAY keyIdTbl;				// ｷｰ入力(NUMPAD : 2・4・6・8)
	DIR_TBL_PTR_ARRAY posTbl;			// 座標
	DIR_TBL_ARRAY speedTbl;				// 移動距離
	DIR_TBL_DIR_ARRAY dirTbl;			// ﾌﾟﾚｲﾔｰの方向
	MAP_MOVE_TBL mapMoveTbl;			// 隣のﾏｽのﾏｯﾌﾟIDで移動可能ならtrue、そうでないならfalse

	bool afterKeyFlag;					// 後ｷｰ入力優先ﾌﾗｸﾞ
	int fireLength;
	unsigned int reStartCnt;			// 復帰の際の無敵時間

	unsigned int fireGuardFlag;
};