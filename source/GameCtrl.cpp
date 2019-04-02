#include "DxLib.h"
#include "GameCtrl.h"



GameCtrl::GameCtrl()
{
}


GameCtrl::~GameCtrl()
{
}

const KEY_ARY & GameCtrl::GetCtrl(KEY_TYPE type) const
{
	if (type == KEY_TYPE_NOW)
	{
		return keyData;
	}
	return keyDataOld;
}

void GameCtrl::UpDate()
{
	keyDataOld = keyData;
	GetHitKeyStateAll(&keyData[0]);
}
