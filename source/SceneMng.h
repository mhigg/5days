#pragma once
#include <memory>
#include <list>
#include "BaseScene.h"
#include "GameCtrl.h"
#include "VECTOR2.h"

class Obj;

using ListObj_itr = std::list<std::unique_ptr<Obj>>::iterator;

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	static SceneMng& GetInstance(void)
	{
		static SceneMng s_instance;
		return s_instance;
	}

	void Run(void);							// �Q�[�����[�v
	VECTOR2 GetScreenSize(void);			// ��ʂ̃T�C�Y���擾
	VECTOR2 GetGameScreenSize(void);		// �Q�[���G���A�̃T�C�Y���擾
	VECTOR2 GetDrawOffset(void);			// �`��I�t�Z�b�g�̎擾
	void SetDrawOffset(VECTOR2 offset);		// �`��I�t�Z�b�g�̐ݒ�
	void SetFrame(int fram);				// �o�߃t���[������ݒ�
	int GetFrame(void);						// �o�߃t���[�������擾

private:
	SceneMng();

	int Init(void);		// �Q�[���S�̓I�ȏ�����

	VECTOR2 drawOffset;	// �`��I�t�Z�b�g
	int framCnt;		// �o�߃t���[����

	unique_Base activeScene;
	std::unique_ptr<GameCtrl> gameCtrl;
};

