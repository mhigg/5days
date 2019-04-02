#pragma once
#include "BaseScene.h"
class ResultScene :
	public BaseScene
{
public:
	ResultScene();
	~ResultScene();

private:
	unique_Base UpDate(unique_Base own, const GameCtrl &controller);
	int Init(void);
	void Draw(void);
};