#pragma once
class VECTOR2
{
public:
	//default
	VECTOR2();
	//with data
	VECTOR2(int x, int y);
	~VECTOR2();
	int x, y;

	//µÍßÚ°À°‚Ìµ°ÊÞ°Û°ÄÞ
	//‘ã“ü‰‰ŽZŽq
	VECTOR2& operator = (const VECTOR2& vec);

	//“YŽš‰‰ŽZŽq
	int& operator[](int i);

	//”äŠr‰‰ŽZŽq
	//ÅŒã‚Ìconst‚ÍRO‚Ìˆó
	bool operator==(const VECTOR2& vec) const;
	bool operator!=(const VECTOR2& vec) const;
	bool operator>=(const VECTOR2& vec) const;
	bool operator<=(const VECTOR2& vec) const;
	bool operator>(const VECTOR2& vec) const;
	bool operator>(int k) const;
	bool operator<(const VECTOR2& vec) const;


	//’P€‰‰ŽZŽq
	//Žl‘¥‰‰ŽZ
	VECTOR2& operator += (const VECTOR2& vec);
	VECTOR2& operator -= (const VECTOR2& vec);
	VECTOR2& operator *= (int k);
	VECTOR2& operator /= (int k);
	//•„†‚ð‰Á‚¦‚é
	VECTOR2  operator+()const;
	VECTOR2  operator-()const;
};

//ÍÞ¸ÄÙ‚Ì‰‰ŽZ
//VECTOR2 + int
VECTOR2 operator+(const VECTOR2 &u, int v);

//VECTOR2 - int
VECTOR2 operator-(const VECTOR2 &u, int v);

//VECTOR2 + VECTOR2
VECTOR2 operator+(const VECTOR2 &u, const VECTOR2 &v);

//VECTOR2 - VECTOR2
VECTOR2 operator-(const VECTOR2 &u, const VECTOR2 &v);

//VECTOR2 * int
VECTOR2 operator*(const VECTOR2 &u, int v);

//int * VECTOR2
VECTOR2 operator*(int u, const VECTOR2 &v);

//VECTOR2 * VECTOR2
VECTOR2 operator*(const VECTOR2 &u, const VECTOR2 &v);

//VECTOR2 / int
VECTOR2 operator/(const VECTOR2 &u, int v);

//VECTOR2 / VECTOR2
VECTOR2 operator/(const VECTOR2 &u, const VECTOR2 &v);

//VECTOR2 % int
VECTOR2 operator%(const VECTOR2 &u, int v);

//VECTOR2 % VECTOR2
VECTOR2 operator%(const VECTOR2 &u, const VECTOR2 &v);
