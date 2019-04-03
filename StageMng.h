#pragma once
#include "MapCtrl.h"
#include "Player.h"
#include "VECTOR2.h"

#define lpStageMng StageMng::GetInstance()

using MAP_ARY = std::array<int, PL_MAX>;

class StageMng
{
public:
	static StageMng& GetInstance(void)
	{
		static StageMng s_instance;
		return s_instance;
	}

	// �w�nϽ���v�Z
	void calcArea(void);

	// �w�n���̎擾
	MAP_ARY GetMapCnt(void);

	// �w�n�����[���N���A����
	void ResetMapCnt(void);

	// �v���C���[���Ƃɐw�n�������Z����
	void AddMapCnt(PL_NUMBER player, VECTOR2 pos);

	// �w�n���v�Z�̂��߂�ϯ�߻��ނ�ݒ�
	void SetMapSize(VECTOR2 mapSize);

private:
	StageMng();

	MAP_ARY mapCnt;		// 1P�̐w�n��
	VECTOR2 mapSize;	// ϯ�߻���
	VECTOR2 chipSize;
};

