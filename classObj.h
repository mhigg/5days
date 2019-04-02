#pragma once
#include <list>
#include <memory>
#include "Obj.h"

using uniqueObj = std::unique_ptr<Obj>;			//Obj‚ÌÕÆ°¸Îß²ÝÀ
using uniqueObjList = std::list<uniqueObj>;		//ObjÕÆ°¸Îß²ÝÀ‚ÌØ½Ä
using sharedListObj = std::shared_ptr<uniqueObjList>;	//ObjÕÆ°¸Îß²ÝÀ‚ÌØ½Ä‚Ì¼ª±°ÄÞÎß²ÝÀ
using ObjList_itr = uniqueObjList::iterator;	//uniqueObjList‚Ì²ÃÚ°À


struct AddObjList
{

	ObjList_itr operator()(sharedListObj objList, uniqueObj objPtr)
	{
		objList->push_back(std::move(objPtr));
		ObjList_itr itr = objList->end();
		itr--;
		return itr;
	}
};