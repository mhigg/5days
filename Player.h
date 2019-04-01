#pragma once
//#include <type_traits>
#include <memory>
#include <array>
#include "Obj.h"
#include "VECTOR2.h"
#include "MapCtl.h"

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

#define PL_DEF_SPEED 2		// ��ڲ԰�̕W����߰��

using DIR_TBL_ARRAY	    = std::array<int, DIR_MAX>;
using DIR_TBL_PTR_ARRAY = std::array<int*[TBL_MAX], DIR_MAX>;
using DIR_TBL_DIR_ARRAY = std::array<DIR[DIR_TBL_MAX], DIR_MAX>;
using MAP_MOVE_TBL		= std::array</*MOVE_STATE*/bool, static_cast<int>(MAP_ID::MAX)>;	// static_cast<���Ă������^>

class Player :
	public Obj
{
public:
	Player();
	Player(VECTOR2 setUpPos,
		VECTOR2 drawOffset);
	~Player();
	bool InitAnim(void);	// ��Ұ��ݓo�^
private:
	void SetMove(const GameController &controller,
		weakListObj objList);

	bool CheckObjType(OBJ_TYPE type);	// ��޼ު�Ă����߂�����
	bool DeathPrc(void);
	void GetItem(void);

	DIR_TBL_ARRAY keyIdTbl;				// ������(NUMPAD : 2�E4�E6�E8)
	DIR_TBL_PTR_ARRAY posTbl;			// ���W
	DIR_TBL_ARRAY speedTbl;				// �ړ�����
	DIR_TBL_DIR_ARRAY dirTbl;			// ��ڲ԰�̕���
	MAP_MOVE_TBL mapMoveTbl;			// �ׂ�Ͻ��ϯ��ID�ňړ��\�Ȃ�true�A�����łȂ��Ȃ�false

	bool afterKeyFlag;					// �㷰���͗D���׸�
	int fireLength;
	unsigned int reStartCnt;			// ���A�̍ۂ̖��G����

	unsigned int fireGuardFlag;
};