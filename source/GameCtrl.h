#pragma once
#include <array>

enum KEY_TYPE
{
	KEY_TYPE_NOW,
	KEY_TYPE_OLD,
	KEY_TYPE_MAX
};

using KEY_ARY = std::array<char, 256>;

class GameCtrl
{
public:
	GameCtrl();
	~GameCtrl();

	const KEY_ARY &GetCtrl(KEY_TYPE type) const;
	void UpDate();

private:
	KEY_ARY keyData;
	KEY_ARY keyDataOld;
};

