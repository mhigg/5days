#include <algorithm>
#include "DxLib.h"
#include "ClassObj.h"
#include "Player.h"
#include "SceneMng.h"
#include "GameController.h"
#include "Bomb.h"

#define RESTART_TIME 120U

Player::Player()
{
}

Player::Player(VECTOR2 setUpPos,VECTOR2 drawOffset) :Obj(drawOffset)
{
	keyIdTbl = { KEY_INPUT_NUMPAD2,			// 下方向
				 KEY_INPUT_NUMPAD4,			// 左方向
				 KEY_INPUT_NUMPAD6,			// 右方向
				 KEY_INPUT_NUMPAD8, };		// 上方向

	speedTbl = {	PL_DEF_SPEED,			// 下方向
				   -PL_DEF_SPEED,			// 左方向
					PL_DEF_SPEED,	 		// 右方向
				   -PL_DEF_SPEED,	 };		// 上方向

				//		MAIN		  SUB
	posTbl	 = {	   &pos.y,		&pos.x,			// 下方向
					   &pos.x,		&pos.y,			// 左方向
					   &pos.x,		&pos.y,			// 右方向
					   &pos.y,		&pos.x,		};	// 上方向

				//		MAIN			OPP			  SUB1			SUB2
	dirTbl   = {	  DIR_DOWN,		  DIR_UP,	    DIR_LEFT,	  DIR_RIGHT,		// 下方向
					  DIR_LEFT,		  DIR_RIGHT,	DIR_DOWN,	  DIR_UP,			// 左方向
					  DIR_RIGHT,	  DIR_LEFT,		DIR_DOWN,	  DIR_UP,			// 右方向
					  DIR_UP,		  DIR_DOWN,		DIR_LEFT,	  DIR_RIGHT	   };	// 上方向

	mapMoveTbl = {	true,		// CUR
					true,		// FLOOR1
					true,		// FLOOR2
					true,		// BOMB
					true,		// NON	
					false,		// WALL1
					false,		// WALL2
					false,		// BLOCK
					true,		// ITEM_BOMB
					true,		// ITEM_FIRE
					true,		// ITEM_SPEED
					true,		// ITEM_CTL
					true,		// ITEM_BOMB_THROW
					true,		// ITEM_FIRE_GUARD
					true,	};	// MAP_ID_ITEM_WALL_RANDOM
	
	Init("image/bomberman.png", VECTOR2(100 / 5, 128 / 4), VECTOR2(5, 4),setUpPos);
	InitAnim();
	afterKeyFlag = false;
	fireLength = 2;
	reStartCnt = 0U;
	fireGuardFlag = 0U;
}

Player::~Player()
{
}

bool Player::InitAnim(void)
{
	AddAnim("停止", 0, 0, 2, 6, true);
	AddAnim("移動", 0, 2, 2, 6, true);
	AddAnim("死亡", 4, 0, 4, 8, false);
	return true;
}

void Player::SetMove(const GameController & controller, weakListObj objList)
{
	// auto = const KEY_ARRAY(参照の場合は元がconstであればconstをつける)
	auto &keyTbl	= controller.GetCtl(KEY_TYPE_NOW);
	auto &keyTblOld = controller.GetCtl(KEY_TYPE_OLD);
	
	if (GetAnim() == "死亡")
	{
		if (animEndFlag)
		{
			SetAnim("停止");
			reStartCnt = RESTART_TIME;
		}
	}
	if (!fireGuardFlag)
	{
		if (lpMapCtl.GetFireMapData(pos))
		{
			if (DeathPrc())
			{
				return;
			}
		}
	}

	reStartCnt -= (reStartCnt > 0);		// 真の場合の1をｶｳﾝﾄから減らす
	visible = true;
	if ((reStartCnt / 4) % 2)
	{
		visible = false;
	}

	if (keyTbl[KEY_INPUT_SPACE] & (~keyTblOld[KEY_INPUT_SPACE]))
	{
		AddObjList()( objList, std::make_unique<Bomb>( ((pos+10) / lpMapCtl.GetChipSize() )*lpMapCtl.GetChipSize(), fireLength, lpSceneMng.GetDrawOffset()) );
	}

	auto &chipSize = lpMapCtl.GetChipSize().x;

	auto sidePos = [&](DIR dir,VECTOR2 pos,int speed,SIDE_CHECK sideFlag)
	{
		VECTOR2 side;
		switch (dir)
		{
		case DIR_DOWN:
			side = { 0,(chipSize - sideFlag) + speed };
			break;
		case DIR_LEFT:
			side = { speed - (sideFlag ^ 1),0 };
			break;
		case DIR_RIGHT:
			side = { (chipSize - sideFlag) + speed,0 };
			break;
		case DIR_UP:
			side = { 0, speed - (sideFlag ^ 1) };
			break;
		default:
			break;
		}
		return pos + side;
	};
	
	auto CheckMove_Bomb = [&](){
		bool rtnFlag = false;

		//auto tmpList = *(objList.lock());
		//tmpList.remove_if([](sharedObj &obj) {return !(obj->CheckObjType(OBJ_BOMB)); });	// ﾘｽﾄの１要素を探る(型はuniqueObj)
		//for (auto &bomb : tmpList)
		//{
		//	rtnFlag |= (sidePos(dir, pos, speedTbl[dir]) == sidePos(dirTbl[dir][DIR_TBL_OPP], (*bomb).GetPos(), 0));
		//}

		sharedList tmpList(objList.lock()->size());		// ()内でｻｲｽﾞを指定
		// stdのｺﾋﾟｰ系はｺﾋﾟｰ元と同ｻｲｽﾞ以上のものを用意する必要がある(今回はｻｲｽﾞ指定)

		auto last_itr = std::remove_copy_if(objList.lock()->begin(), objList.lock()->end(),
			tmpList.begin(), [](sharedObj &obj) {return !(obj->CheckObjType(OBJ_BOMB)); });		// 返り値は+1の場所

		/*tmpList.erase(last_itr, tmpList.end());
		for (auto &bomb : tmpList)
		{
			rtnFlag |= (sidePos(dir, pos, speedTbl[dir]) == sidePos(dirTbl[dir][DIR_TBL_OPP], (*bomb).GetPos(), 0));
		}*/

		std::for_each(tmpList.begin(), last_itr, [&](auto bomb) {
			rtnFlag |= ( sidePos(dir, pos, 0, IN_SIDE) ==
						 sidePos(dirTbl[dir][DIR_TBL_OPP], (*bomb).GetPos(), 0, OUT_SIDE) );
		});
		return !rtnFlag;
	};

	auto move = [&,dir = Player::dir](DIR_TBL_ID id)	// ｺﾋﾟｰｷｬﾌﾟﾁｬｰ
	{
		if ( keyTbl[keyIdTbl[dirTbl[dir][id]]] )
		{
			// 補正処理
			if ((*posTbl[Player::dir][TBL_SUB]) % chipSize)
			{
				(*posTbl[Player::dir][TBL_SUB])
					= (((*posTbl[Player::dir][TBL_SUB]) + chipSize / 2) / chipSize) * chipSize;
				return true;
			}

			Player::dir = dirTbl[dir][id];
		
			// 移動不可のｵﾌﾞｼﾞｪｸﾄが隣にあった場合
			if (!mapMoveTbl[static_cast<int>( lpMapCtl.GetMapData(sidePos(Player::dir, pos, speedTbl[Player::dir], IN_SIDE)) )])
			{
				return false;
			}
			
			// 爆弾との判定
			if (!CheckMove_Bomb())
			{
				// 爆弾が隣にあった場合
				return false;
			}
			// 移動処理
			(*posTbl[Player::dir][TBL_MAIN]) += speedTbl[Player::dir];
			return true;
		}
		return false;
	};

	if ( !(move( DIR_TBL_ID(DIR_TBL_SUB1 - (afterKeyFlag << 1)) )
	    || move( DIR_TBL_ID(DIR_TBL_SUB2 - (afterKeyFlag << 1)) )) )
	{
		afterKeyFlag = false;
		if ( !(move( DIR_TBL_ID(DIR_TBL_MAIN + (afterKeyFlag << 1)) )
		    || move( DIR_TBL_ID(DIR_TBL_OPP  + (afterKeyFlag << 1)) )) )
		{
			SetAnim("停止");
			return;
		}
	}
	else
	{
		afterKeyFlag  = keyTbl[ keyIdTbl[dirTbl[dir][DIR_TBL_SUB1]] ];
		afterKeyFlag |= keyTbl[ keyIdTbl[dirTbl[dir][DIR_TBL_SUB2]] ];
		afterKeyFlag ^= (int)(GetAnim() == "停止");
	}
	SetAnim("移動");

	/*_RPTN(_CRT_WARN, "player.pos:%d,%d\n", pos.x, pos.y);*/		// ﾌﾟﾚｲﾔｰの座標を示す
	
	// 関数ｵﾌﾞｼﾞｪｸﾄ
	/*struct Move
	{
		void operator()(const KEY_ARRAY &key,int state, int subState[2],int &pos, int addNum)
		{
			if (key[state])
			{
				pos += addNum;
			}
		}
	};*/
	/*Move()(controller.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_NUMPAD2], pos.y,  2);*/

	/*int subState[2] = { KEY_INPUT_NUMPAD8,KEY_INPUT_NUMPAD2 };
	Move()(controller.GetCtl(KEY_TYPE_NOW),KEY_INPUT_NUMPAD4, subState,pos.x, -2);*/

	/*Move()(controller.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_NUMPAD6], pos.x,  2);
	Move()(controller.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_NUMPAD8], pos.y, -2);*/

	// ﾌｧﾝｸｼｮﾝを使用する場合(ﾗﾑﾀﾞを呼び出すことも可能)
	/*std::function<void(int, int&, int)> MoveFunc = Move();
	MoveFunc(controller.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_NUMPAD2], pos.y, 2);
	MoveFunc(controller.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_NUMPAD4], pos.x, -2);
	MoveFunc(controller.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_NUMPAD6], pos.x, 2);
	MoveFunc(controller.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_NUMPAD8], pos.y, -2);*/

	// ﾗﾑﾀﾞ
	/*auto Mover = [](int keyState, int &pos, int addNum)
	{
		if (keyState)
		{
			pos += addNum;
		}
	};
	Mover(controller.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_NUMPAD2], pos.y, 2);
	Mover(controller.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_NUMPAD4], pos.x, -2);
	Mover(controller.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_NUMPAD6], pos.x, 2);
	Mover(controller.GetCtl(KEY_TYPE_NOW)[KEY_INPUT_NUMPAD8], pos.y, -2);*/
}

bool Player::CheckObjType(OBJ_TYPE type)
{
	return (type == OBJ_PLAYER);
}

bool Player::DeathPrc(void)
{
	if (reStartCnt)
	{
		return false;
	}
	dir = DIR_DOWN;		// ｸﾞﾗﾌｨｯｸの都合上
	SetAnim("死亡");
	fireGuardFlag = 0U;
	return true;
}

void Player::GetItem(void)
{
	if ((pos % lpMapCtl.GetChipSize()) != VECTOR2(0, 0))
	{
		return;
	}

	auto itemID = [=]()
	{
		auto id = lpMapCtl.GetMapData(pos);
		return (id == MAP_ID::ITEM_RANDOM ? static_cast<MAP_ID>(ITEM_ID_START + GetRand(ITEM_MAX - 1)) : id);
	};

	auto paramUP = [=](auto& paramFlag, auto limNum)
	{
		paramFlag += (paramFlag < limNum);
		lpMapCtl.SetMapData(pos, MAP_ID::NON);
	};

	switch (itemID())
	{
	case MAP_ID::ITEM_FIRE_GUARD:
		paramUP(fireGuardFlag, 1);
		break;
	}
}
