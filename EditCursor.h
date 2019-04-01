#pragma once
#include <list>
#include <memory>
#include "Obj.h"
#include "ClassObj.h"
#include "BaseScene.h"
#include "VECTOR2.h"
#include "MapCtl.h"

#define EDIT_KEY_GET_DEF_RNG 30		// 最大移動ﾌﾚｰﾑ数
#define MIN_KEY_RNG 5				// 最小移動ﾌﾚｰﾑ数

class EditCursor :
	public Obj
{
public:
	EditCursor(VECTOR2 drawOffset);
	EditCursor();
	~EditCursor();
	void Draw(void);					// 描画用
private:
	void SetMove(const GameController &controller, weakListObj objList);

	MAP_ID id;			// ｴﾃﾞｨｯﾄのID
	int keyGetRng;		// 移動ﾌﾚｰﾑ数の基準値
	int inputFlam;		// 移動ﾌﾚｰﾑ数
	bool mouseFlag;		// 操作ﾓｰﾄﾞ切り替え用(ﾏｳｽﾓｰﾄﾞの際にtrue)
	void ChangeMode(const GameController &controller);	// ｷｰﾓｰﾄﾞ、ﾏｳｽﾓｰﾄﾞ切り替え
	bool CheckObjType(OBJ_TYPE type);
};

