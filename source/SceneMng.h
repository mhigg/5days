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

	void Run(void);							// ゲームループ
	VECTOR2 GetScreenSize(void);			// 画面のサイズを取得
	VECTOR2 GetGameScreenSize(void);		// ゲームエリアのサイズを取得
	VECTOR2 GetDrawOffset(void);			// 描画オフセットの取得
	void SetDrawOffset(VECTOR2 offset);		// 描画オフセットの設定
	void SetFrame(int fram);				// 経過フレーム数を設定
	int GetFrame(void);						// 経過フレーム数を取得

private:
	SceneMng();

	int Init(void);		// ゲーム全体的な初期化

	VECTOR2 drawOffset;	// 描画オフセット
	int framCnt;		// 経過フレーム数

	unique_Base activeScene;
	std::unique_ptr<GameCtrl> gameCtrl;
};

