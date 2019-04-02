#pragma once
#include <vector>
#include "VECTOR2.h"
#include "classObj.h"
#include "MAP_ID.h"

//’Zk’è‹`
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

	//mapSize,chipSize,drawOffset‚Ìæ“¾‚Æ‰Šú‰»,Ï¯ÌßÃŞ°À”z—ñ‚Ì‰Šú‰»
	bool SetUp(const VECTOR2& size, const VECTOR2& chipSize, const VECTOR2 drawOffset);
	
	//SetMoveŠÖ”‚Åæ“¾‚µ‚½î•ñ‚ğmapData‚É•Û‘¶
	bool SetMapData(const VECTOR2& pos, MAP_ID id);

	MAP_ID GetMapData(VECTOR2 mapPos);
	
	//æ“¾‚µ‚½î•ñ(mapData)‚ğ‚à‚Æ‚É•`‰æ modeFlag:true¨EditMode false¨GameMode
	void Draw(void);

	bool SetUpGameObj(sharedListObj objList);

private:
	MapCtrl();
	~MapCtrl();
	void calcArea(void);		// w’nÏ½”ŒvZ

	std::vector<MAP_ID*> mapData;		//”z—ñ‘Sˆæ “ñŸŒ³–Ú
	std::vector<MAP_ID> mapData_Base;	//Îß²İÀ—p@ˆêŸŒ³–Ú

	VECTOR2 mapSize;		//Ï¯Ìß‚ÌÏ½–Ú” ‰¡•ûŒü‚Íx c•ûŒü‚Íy
	VECTOR2 chipSize;		//1Ï½‚Ì»²½Ş
	VECTOR2 drawOffset;		//•`‰æ‚ÌµÌ¾¯Ä

	int mapCnt;

	int bgImage;
};