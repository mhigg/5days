#pragma once
#include <string>
#include <map>
#include "VECTOR2.h"

enum ANIM_TBL {
	ANIM_TBL_START_ID,		//ｱﾆﾒｰｼｮﾝの開始
	ANIM_TBL_FRAME,			//ｱﾆﾒｰｼｮﾝのｺﾏ数
	ANIM_TBL_INV,			//ｺﾏごとの間隔
	ANIM_TBL_MAX
};

class GameCtrl;

class Obj
{
public:
	Obj();
	Obj(VECTOR2 drawOffset);	//受け取ったdrawOffsetを自分の持つdrawOffsetに入れる
	bool init(
		std::string fileName,
		VECTOR2 divSize,
		VECTOR2 divCnt
	);	//画像のﾌｧｲﾙ名,分割ｻｲｽﾞ,分割数の初期化
	bool init(
		std::string fileName,
		VECTOR2 divSize,
		VECTOR2 divCnt,
		VECTOR2 pos
	);	//画像のﾌｧｲﾙ名,分割ｻｲｽﾞ,分割数の初期化,座標
	virtual ~Obj();
	void UpDate(const GameCtrl &controller);	//情報更新	ｺﾝﾄﾛｰﾗｰ自体のﾎﾟｲﾝﾀを渡す
	virtual void Draw(void);	//描画
	void Draw(unsigned int id);	//ID指定描画
	const VECTOR2 &GetPos(void);	//座標取得関数

	bool AddAnim(
		std::string animName,
		int id_x,
		int id_y,
		int frame,
		int inv
	);	//ｱﾆﾒｰｼｮﾝの名前,動き,種別,ｺﾏ数,間隔の設定
	bool SetAnim(std::string animName);
	std::string GetAnim(void);

private:
	virtual void SetMove(const GameCtrl &controller) = 0;

	std::string animName;	//表示するｱﾆﾒｰｼｮﾝ名
	std::map<std::string, int[ANIM_TBL_MAX]> animTable;

protected:
	std::string imageName;	//表示する画像のﾌｧｲﾙ名
	VECTOR2 divSize;		//画像の分割ｻｲｽﾞ
	VECTOR2 divCnt;			//画像の分割数

	unsigned int animCnt;	//ｱﾆﾒｰｼｮﾝのｶｳﾝﾄ

	VECTOR2 pos;			//表示する座標
	void SetPos(VECTOR2 pos);

	const VECTOR2 drawOffset;		//描画ｵﾌｾｯﾄ
};