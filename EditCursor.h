#pragma once
#include <list>
#include <memory>
#include "Obj.h"
#include "ClassObj.h"
#include "BaseScene.h"
#include "VECTOR2.h"
#include "MapCtl.h"

#define EDIT_KEY_GET_DEF_RNG 30		// �ő�ړ��ڰѐ�
#define MIN_KEY_RNG 5				// �ŏ��ړ��ڰѐ�

class EditCursor :
	public Obj
{
public:
	EditCursor(VECTOR2 drawOffset);
	EditCursor();
	~EditCursor();
	void Draw(void);					// �`��p
private:
	void SetMove(const GameController &controller, weakListObj objList);

	MAP_ID id;			// ��ި�Ă�ID
	int keyGetRng;		// �ړ��ڰѐ��̊�l
	int inputFlam;		// �ړ��ڰѐ�
	bool mouseFlag;		// ����Ӱ�ސ؂�ւ��p(ϳ�Ӱ�ނ̍ۂ�true)
	void ChangeMode(const GameController &controller);	// ��Ӱ�ށAϳ�Ӱ�ސ؂�ւ�
	bool CheckObjType(OBJ_TYPE type);
};

