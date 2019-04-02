#pragma once
#include <array>
#include "MapCtrl.h"
#include "Obj.h"

#define DEF_SPEED 50

enum PL_KEY {
	PL_KEY_UP,
	PL_KEY_DOWN,
	PL_KEY_LEFT,
	PL_KEY_RIGHT,
	PL_KEY_MODE,
	PL_KEY_MAX
};

enum DIR_TBL_ID {
	DIR_TBL_MAIN,			// �����Ă��鷰�̕���
	DIR_TBL_OPP,			// DIR_TBL_MAIN�̔��Ε���
	DIR_TBL_SUB1,			// �����Ⴄ�����P
	DIR_TBL_SUB2,			// �����Ⴄ�����Q
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

using DIR_TBL_ARY = std::array<int, DIR_MAX>;
using DIR_TBL_PL_ARY = std::array<int[PL_MAX], PL_KEY_MAX>;
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
	void SetMove(const GameCtrl & controller);

	PL_NUMBER player;
	DIR dir;
	VECTOR2 tmpPos;
	bool modeFlag;		// �ړ����[�h�̕ύX�@true:���[�v�@false:�v���C���[

	DIR_TBL_PL_ARY	keyTbl;
	DIR_TBL_PTR_ARY posTbl;
	DIR_TBL_ARY		speedTbl;
	DIR_TBL_DIR_ARY dirTbl;
};
