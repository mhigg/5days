#pragma once
#include <vector>
#include "VECTOR2.h"
#include "classObj.h"
#include "MAP_ID.h"

//�Z�k��`
#define lpMapCtrl MapCtrl::GetInstance()
#define lpGetChipSize MapCtrl::GetInstance().GetChipSize()

//using ListObj_itr = uniqueObjList::iterator;

enum DIR
{
	DIR_UP,
	DIR_DOWN,
	DIR_RIGHT,
	DIR_LEFT,
	DIR_MAX
};


class MapCtrl
{
public:
	static MapCtrl & GetInstance(void)
	{
		static MapCtrl s_instance;
		return s_instance;
	}

	VECTOR2 & GetChipSize(void);

	//mapSize,chipSize,drawOffset�̎擾�Ə�����,ϯ���ް��z��̏�����
	bool SetUp(const VECTOR2& size, const VECTOR2& chipSize, const VECTOR2 drawOffset);
	
	//SetMove�֐��Ŏ擾��������mapData�ɕۑ�
	bool SetMapData(const VECTOR2& pos, MAP_ID id);

	MAP_ID GetMapData(VECTOR2 mapPos);
	
	//�擾�������(mapData)�����Ƃɕ`�� modeFlag:true��EditMode false��GameMode
	void Draw(void);

	bool SetUpGameObj(sharedListObj objList);

private:
	MapCtrl();
	~MapCtrl();
	void calcArea(void);		// �w�nϽ���v�Z

	std::vector<MAP_ID*> mapData;		//�z��S�� �񎟌���
	std::vector<MAP_ID> mapData_Base;	//�߲���p�@�ꎟ����

	VECTOR2 mapSize;		//ϯ�߂�Ͻ�ڐ� ��������x �c������y
	VECTOR2 chipSize;		//1Ͻ�̻���
	VECTOR2 drawOffset;		//�`�掞�̵̾��

	int mapCnt;

	int bgImage;
};