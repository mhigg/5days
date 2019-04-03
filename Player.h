#pragma once
#include <array>
#include "MapCtrl.h"
#include "Obj.h"
#include "classObj.h"

#define DEF_SPEED 50


enum SKILL {
	SKILL_WARP,
	SKILL_BOMB,
	SKILL_STAMP,
	SKILL_NOENTRY,
	SKILL_MAX
};

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

enum PL_NUMBER {
	PL_1,
	PL_2,
	PL_MAX
};

using SKILL_ARY = std::array<int[PL_MAX], SKILL_MAX>;
using DIR_TBL_ARY = std::array<int, DIR_MAX>;
using DIR_TBL_PL_ARY = std::array<int[PL_MAX], DIR_MAX>;
using DIR_TBL_PTR_ARY = std::array<int*[TBL_MAX], DIR_MAX>;
using DIR_TBL_DIR_ARY = std::array<DIR[DIR_TBL_MAX], DIR_MAX>;

class Player :
	public Obj
{
public:
	Player();
	Player(PL_NUMBER player, VECTOR2 drawOffset);
	~Player();

private:
	void SetMove(const GameCtrl & controller, weakListObj objList);

	PL_NUMBER player;
	DIR dir;
	VECTOR2 tmpPos;
	bool modeFlag;		// 移動モードの変更　true:ワープ　false:プレイヤー

	SKILL_ARY skillTbl;
	DIR_TBL_PL_ARY	keyTbl;
	DIR_TBL_PTR_ARY posTbl;
	DIR_TBL_ARY		speedTbl;
	DIR_TBL_DIR_ARY dirTbl;

	sharedListObj objList;
};
