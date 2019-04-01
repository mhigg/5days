#include "DxLib.h"

//ﾃﾞﾊﾞｯｸﾞﾒｯｾｰｼﾞ用定義
#ifdef _DEBUG
#define AST() {\
	CHAR ast_mes[256];\
	wsprintf( ast_mes, "%s %d行目\0", __FILE__, __LINE__ );\
	MessageBox( 0, ast_mes, "ｱｻｰﾄ表示", MB_OK );\
	}
#else
#define AST()
#endif	//_DEBUG

#define TITLE_IMAGE_SIZE_X	700						//ﾀｲﾄﾙﾛｺﾞの横ｻｲｽﾞ
#define TITLE_IMAGE_SIZE_Y	300						//ﾀｲﾄﾙﾛｺﾞの縦ｻｲｽﾞ
#define CHAR_SEL_IMAGE_SIZE_X	400					//ｷｬﾗｾﾚﾛｺﾞの横ｻｲｽﾞ
#define CHAR_SEL_IMAGE_SIZE_Y	80					//ｷｬﾗｾﾚﾛｺﾞの縦ｻｲｽﾞ
#define START_IMAGE_SIZE_X	210
#define START_IMAGE_SIZE_Y	75
#define OK_IMAGE_SIZE_X		150
#define OK_IMAGE_SIZE_Y		150

#define WIN_IMAGE_SIZE_X	450						//勝利者名画像の横ｻｲｽﾞ
#define WIN_IMAGE_SIZE_Y	(300 / CHAR_ID_MAX)		//勝利者名画像の縦ｻｲｽﾞ
#define STAND_IMAGE_SIZE_X	500						//ｷｬﾗの立ち絵の横ｻｲｽﾞ
#define STAND_IMAGE_SIZE_Y	500						//ｷｬﾗの立ち絵の立ち絵の縦ｻｲｽﾞ
#define ICON_IMAGE_SIZE_X	64
#define ICON_IMAGE_SIZE_Y	64

#define BLOCK_PTN_MAX		7		//ﾌﾞﾛｯｸ画像のﾊﾟﾀｰﾝ数
#define BLOCK_CNTUP_INV		3		//ﾌﾞﾛｯｸがｻｲｽﾞｱｯﾌﾟするﾌﾚｰﾑ間隔
#define BLOCK_MAX_SIZE_FLAM	(BLOCK_PTN_MAX - 1) * BLOCK_CNTUP_INV 		//ｻｲｽﾞｱｯﾌﾟに必要な総ﾌﾚｰﾑ数

#define CHIP_CNT_X			23		//ｹﾞｰﾑ画面の横のﾏｽ数
#define CHIP_CNT_Y			14		//ｹﾞｰﾑ画面の縦のﾏｽ数
#define CHIP_SIZE_X			32		//1ﾏｽの横ｻｲｽﾞ
#define CHIP_SIZE_Y			32		//1ﾏｽの縦ｻｲｽﾞ

#define SCREEN_SIZE_X		800		//画面の横ｻｲｽﾞ
#define SCREEN_SIZE_Y		600		//画面の縦ｻｲｽﾞ
#define GAME_SCREEN_X		CHIP_SIZE_X			//ｹﾞｰﾑｴﾘｱのｵﾌｾｯﾄ
#define GAME_SCREEN_Y		(CHIP_SIZE_Y * 3)	//ｹﾞｰﾑｴﾘｱのｵﾌｾｯﾄ
#define GAME_SCREEN_SIZE_X	(CHIP_CNT_X * CHIP_SIZE_X)
#define GAME_SCREEN_SIZE_Y	(CHIP_CNT_Y * CHIP_SIZE_Y)

#define PLAYER_SIZE_X		32
#define PLAYER_DEF_SPEED	2		//自機のﾃﾞﾌｫﾙﾄｽﾋﾟｰﾄﾞ

#define ANIM_IMAGE_MAX		4
#define ANIM_SPEED			5

#define WAIT_CNT			120
#define QB_FLAM_MAX			240

enum GMODE {
	GMODE_TITLE,		//ｽﾀｰﾄ画面
	GMODE_EXPLAIN,		//説明画面
	GMODE_INIT,			//初期化
	GMODE_CHAR_SEL,		//ｷｬﾗｾﾚ画面
	GMODE_BEF_MAIN,		//よーいすたーと
	GMODE_MAIN,			//ｹﾞｰﾑ画面
	GMODE_RESULT,		//ﾘｻﾞﾙﾄ画面
	GMODE_MAX
};

enum CHAR_ID {
	CHAR_ID_MADOKA,
	CHAR_ID_SAYAKA,
	CHAR_ID_MAMI,
	CHAR_ID_KYOKO,
	CHAR_ID_HOMURA,
	CHAR_ID_QB,
	CHAR_ID_MAX
};

enum IMAGE_TYPE {
	IMAGE_TYPE_CHAR,	//ｹﾞｰﾑ中に操作する画像
	IMAGE_TYPE_FACE,	//顔ｱｲｺﾝ
	IMAGE_TYPE_STAND,	//立ち絵
	IMAGE_TYPE_MAX
};

enum DIR {				//向き
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	DIR_MAX
};

enum KEY_TBL {
	KEY_TBL_UP,
	KEY_TBL_RIGHT,
	KEY_TBL_DOWN,
	KEY_TBL_LEFT,
	KEY_TBL_SPEED,		//ｽﾋﾟｰﾄﾞ
	KEY_TBL_ACT,		//ｽｷﾙ
	KEY_TBL_MAX
};

#define KEY_TBL_DIR_MAX	(KEY_TBL_LEFT + 1)		//DIR_MAXの調整

GMODE gameMode;			//現在のｹﾞｰﾑﾓｰﾄﾞ
GMODE gameModeOld;		//1ﾌﾚｰﾑ前のｹﾞｰﾑﾓｰﾄﾞ

typedef struct {
	bool item;
	bool move;
	bool qb;		//qbｽｷﾙ用
	bool result;	//ﾘｻﾞﾙﾄ用
	bool decide;	//ｷｬﾗｾﾚｸﾄ決定用
}Flg;

//座標構造体
typedef struct {
	int x;			//x座標
	int y;			//y座標
}Pos;

//ﾌﾟﾚｲﾔｰ構造体
typedef struct {
	CHAR_ID charID;		//選択ｷｬﾗID
	Pos pos;
	int speed;			//移動速度
	Flg flg;			//状態
	int animCnt;		//ｱﾆﾒｰｼｮﾝのｶｳﾝﾄ
	DIR dir;			//向き
	int chID;
}Player;

//ｷｬﾗの画像用構造体
typedef struct {
	int image[DIR_MAX][ANIM_IMAGE_MAX];		//ｷｬﾗ画像ID
	int faceImage;							//顔画像ID
	int standImage;							//立ち絵ID
}CHAR_IMAGE;

#define PLAYER_MAX 2	//ﾌﾟﾚｲﾔｰ数

int winnerID;		//勝利者ID
int flamCnt;		//経過ﾌﾚｰﾑ数
int qbFlamCnt;		//qbｽｷﾙ発動時の経過ﾌﾚｰﾑ数
int stopFlamCnt;
int fontLen;		//文字の長さ
int fontSize;		//ﾌｫﾝﾄｻｲｽﾞ

Player player[PLAYER_MAX];
CHAR_IMAGE charImage[CHAR_ID_MAX];
int mapData[CHIP_CNT_Y][CHIP_CNT_X];		//ﾏｯﾌﾟﾃﾞｰﾀ格納
Pos itemPos;

//移動時のｷｰﾘｽﾄ
const int keyList[PLAYER_MAX][KEY_TBL_MAX] = {
	{ KEY_INPUT_W,  KEY_INPUT_D,     KEY_INPUT_S,    KEY_INPUT_A,    KEY_INPUT_LSHIFT,   KEY_INPUT_LCONTROL },
	{ KEY_INPUT_UP, KEY_INPUT_RIGHT, KEY_INPUT_DOWN, KEY_INPUT_LEFT, KEY_INPUT_RCONTROL, KEY_INPUT_RSHIFT}
};

int keyFlgSp;			//ｽﾍﾟｰｽｷｰ押下ﾌﾗｸﾞ
int keyFlgSpOld;		//1ﾌﾚｰﾑ前のｽﾍﾟｰｽｷｰ押下ﾌﾗｸﾞ
int keyFlg[PLAYER_MAX][KEY_TBL_MAX];
int keyFlgOld[PLAYER_MAX][KEY_TBL_MAX];

//特定用途のｲﾒｰｼﾞ
int titleImage;			//ﾀｲﾄﾙ画面
int charSelImage;		//ｷｬﾗｾﾚｸﾄ
int okImage;			//OK
int startImage;			//よーいすたーと
int gameBgImage;		//背景
int blockImage[BLOCK_PTN_MAX];		//ﾌﾞﾛｯｸ画像
int winTxtImage[CHAR_ID_MAX];		//勝利者名文字
int seedImage;

//ﾌﾟﾛﾄﾀｲﾌﾟ宣言
//初期化
bool SysInit(void);
bool GameInit(void);

void KeyFlg(int num);

//ｹﾞｰﾑ中
void GameTitle(void);
void GameExplain(void);
void CharSelect(int num);
void SelectID(int num);
void BefGameStart(void);
void GameMain(void);
void GameResult(void);

//ﾌﾟﾚｲﾔｰ系
void CharCtl(int);
void CharSkill(int num, Pos pos);
bool CharHitObj(Pos, int);

//ﾌﾞﾛｯｸ系
void InitMapID(void);
void SetMapID(Pos);
void UpDateMap(void);

//描画系
void GameDraw(void);
void ObjDraw(void);
void ObjAnim(void);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!(SysInit()))
	{
		return -1;
	}
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		if (gameMode != gameModeOld)
		{
			flamCnt = 0;		//ｹﾞｰﾑﾓｰﾄﾞが切り替わったらｾﾞﾛｸﾘｱ
		}
		else
		{
			flamCnt++;
		}
		gameModeOld = gameMode;
		keyFlgSpOld = keyFlgSp;
		keyFlgSp = CheckHitKey(KEY_INPUT_SPACE);
		for (int p = 0; p < PLAYER_MAX; p++)
		{
			KeyFlg(p);
		}
		switch (gameMode)
		{
		case GMODE_TITLE:
			GameTitle();
			break;
		case GMODE_EXPLAIN:
			GameExplain();
			break;
		case GMODE_INIT:
			GameInit();
			break;
		case GMODE_CHAR_SEL:
			for (int p = 0; p < PLAYER_MAX; p++)
			{
				CharSelect(p);
			}
			break;
		case GMODE_BEF_MAIN:
			BefGameStart();
			break;
		case GMODE_MAIN:
			GameMain();
			break;
		case GMODE_RESULT:
			GameResult();
			break;
		default:
			AST();
			gameMode = GMODE_TITLE;
			break;
		}
	}

	DxLib_End();
	return 0;
}

bool SysInit(void)
{
	const char *fileNameList[CHAR_ID_MAX][IMAGE_TYPE_MAX] = 
	{
		//縦：CHAR_ID	横IMAGE_TYPE	ｷｬﾗ,ｱｲｺﾝ,立ち絵
		{"image/madoka.png",	"image/icon_madoka.png",	"image/madoka_st.png"},
		{"image/sayaka.png",	"image/icon_sayaka.png",	"image/sayaka_st.png"},
		{"image/mami.png",		"image/icon_mami.png",		"image/mami_st.png"},
		{"image/kyoko.png",		"image/icon_kyoko.png",		"image/kyoko_st.png"},
		{"image/homura.png",	"image/icon_homura.png",	"image/homura_st.png"},
		{"image/qb.png"	,		"image/icon_qb.png",		"image/qb_st.png"} 
	};

	//-------------- ｼｽﾃﾑ処理
	SetWindowText("MADOKA_SNAKE");
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);		//800×600ﾄﾞｯﾄ65536色ﾓｰﾄﾞに設定
	ChangeWindowMode(true);								//true:ｳｨﾝﾄﾞｳﾓｰﾄﾞ false:ﾌﾙｽｸﾘｰﾝ

	if (DxLib_Init() == -1)		//DxLib_Init():DxLibﾗｲﾌﾞﾗﾘの初期化
	{							//失敗は-1 成功はそれ以外
		AST();
		return false;
	}

	titleImage = LoadGraph("image/title.png");
	if (titleImage == -1)
	{
		AST();
		return false;
	}
	charSelImage = LoadGraph("image/char_sel.png");
	if (charSelImage == -1)
	{
		AST();
		return false;
	}
	startImage = LoadGraph("image/start_mes.png");
	if (startImage == -1)
	{
		AST();
		return false;
	}
	gameBgImage = LoadGraph("image/bg.png");
	if (gameBgImage == -1)
	{
		AST();
		return false;
	}
	okImage = LoadGraph("image/ok.png");
	if (okImage == -1)
	{
		AST();
		return false;
	}
	seedImage = LoadGraph("image/item.png");
	if (seedImage == -1)
	{
		AST();
		return false;
	}

	//ｷｬﾗ画像読込処理
	for (int f = 0; f < CHAR_ID_MAX; f++)
	{
		charImage[f].faceImage = LoadGraph(fileNameList[f][IMAGE_TYPE_FACE]);
		if (charImage[f].faceImage == -1)
		{
			AST();
			return false;
		}
		charImage[f].standImage = LoadGraph(fileNameList[f][IMAGE_TYPE_STAND]);
		if (charImage[f].standImage == -1)
		{
			AST();
			return false;
		}
	}

	for (int j = 0; j < CHAR_ID_MAX; j++)
	{
		if (LoadDivGraph(
			fileNameList[j][IMAGE_TYPE_CHAR],	//読み込む画像
			DIR_MAX * ANIM_IMAGE_MAX,			//分割数全体
			ANIM_IMAGE_MAX, DIR_MAX,			//横と縦の分割数
			CHIP_SIZE_X, CHIP_SIZE_Y,			//横ｻｲｽﾞと縦ｻｲｽﾞ
			charImage[j].image[0],
			false) == -1)
		{
			AST();
			return false;
		}
		//&charImage[j].image[0][0] と charImage[j].image[0]	同じ意味
	}

	//ﾌﾞﾛｯｸ画像
	if (LoadDivGraph(
		"image/block.png",				//読み込む画像
		BLOCK_PTN_MAX,					//分割数全体
		BLOCK_PTN_MAX, 1,				//横と縦の分割数
		CHIP_SIZE_X, CHIP_SIZE_Y,		//横ｻｲｽﾞと縦ｻｲｽﾞ
		blockImage,
		false) == -1)
	{
		AST();
		return false;
	}

	//勝利者名画像
	if (LoadDivGraph(
		"image/WIN.png",
		CHAR_ID_MAX,
		1, CHAR_ID_MAX,
		WIN_IMAGE_SIZE_X, WIN_IMAGE_SIZE_Y,
		winTxtImage,
		false) == -1)
	{
		AST();
		return false;
	}
	
	fontLen = 0;
	fontSize = GetFontSize();

	keyFlgSp	= 0;
	keyFlgSpOld = 0;
	gameMode	= GMODE_TITLE;

	return true;
}

bool GameInit(void)
{
	InitMapID();

	for (int j = 0; j < PLAYER_MAX; j++)
	{
		player[j].speed = PLAYER_DEF_SPEED;
		player[j].animCnt = 0;
		player[j].flg.result = true;		//勝敗結果
		player[j].flg.decide = false;		//ｷｬﾗｾﾚｸﾄ状態
		player[j].flg.qb = false;			//QBｽｷﾙ
		player[j].flg.move = true;			//ほむらｽｷﾙ
		player[j].flg.item = false;			//ｱｲﾃﾑ取得状態
	}
	player[0].pos.x = CHIP_SIZE_X * 5;
	player[0].pos.y = CHIP_SIZE_Y * 5;
	player[1].pos.x = CHIP_SIZE_X * (CHIP_CNT_X - 5);
	player[1].pos.y = CHIP_SIZE_Y * 5;

	player[0].charID = CHAR_ID_MADOKA;
	player[1].charID = CHAR_ID_HOMURA;
	player[0].chID = CHAR_ID_MADOKA;
	player[1].chID = CHAR_ID_HOMURA;

	player[0].dir = DIR_RIGHT;
	player[1].dir = DIR_LEFT;

	itemPos = { rand() % CHIP_CNT_X,rand() % CHIP_CNT_Y };

	winnerID = -1;

	gameMode = GMODE_CHAR_SEL;
	return true;
}

//ﾏｯﾌﾟ関数群
void InitMapID(void)
{
	for (int y = 0; y < CHIP_CNT_Y; y++)
	{
		for (int x = 0; x < CHIP_CNT_X; x++)
		{
			mapData[y][x] = 0;
		}
	}
}

void SetMapID(Pos pos)
{
	Pos tmpChipPos;
	tmpChipPos.x = pos.x;
	tmpChipPos.y = pos.y;

	//配列の番号を求める
	tmpChipPos.x = tmpChipPos.x / CHIP_SIZE_X;
	tmpChipPos.y = tmpChipPos.y / CHIP_SIZE_Y;

	if (0 <= tmpChipPos.x && tmpChipPos.x < CHIP_CNT_X
	 && 0 <= tmpChipPos.y && tmpChipPos.y < CHIP_CNT_Y)
	{
		if(mapData[tmpChipPos.y][tmpChipPos.x] == 0)
		mapData[tmpChipPos.y][tmpChipPos.x] = 1;
	}
}

void UpDateMap(void)
{
	for (int y = 0; y < CHIP_CNT_Y; y++)
	{
		for (int x = 0; x < CHIP_CNT_X; x++)
		{
			if (mapData[y][x] > 0)
			{
				if (mapData[y][x] < BLOCK_MAX_SIZE_FLAM)
				{
					//ﾌﾞﾛｯｸのｻｲｽﾞｱｯﾌﾟ
					mapData[y][x] += 2;
				}
			}
		}
	}

}

//ｷｰﾌﾗｸﾞの更新
void KeyFlg(int num)
{
	keyFlgOld[num][DIR_RIGHT] = keyFlg[num][DIR_RIGHT];
	keyFlg[num][DIR_RIGHT] = CheckHitKey(keyList[num][DIR_RIGHT]);
	keyFlgOld[num][DIR_LEFT] = keyFlg[num][DIR_LEFT];
	keyFlg[num][DIR_LEFT] = CheckHitKey(keyList[num][DIR_LEFT]);
	keyFlgOld[num][KEY_TBL_ACT] = keyFlg[num][KEY_TBL_ACT];
	keyFlg[num][KEY_TBL_ACT] = CheckHitKey(keyList[num][KEY_TBL_ACT]);
}

//ﾌﾟﾚｲﾔｰ
void CharCtl(int num)		//渡された値をnumにint型で入れる
{
	if (0 > num || num >= PLAYER_MAX)		//値の範囲ﾁｪｯｸ
	{
		AST();
		return;
	}										//値が変化するならそのたびに範囲ﾁｪｯｸすべし

	//playerの移動処理
	//playerがﾏｽの中心にいるとき
	if (player[num].pos.x % CHIP_SIZE_X == 0 && player[num].pos.y % CHIP_SIZE_Y == 0)
	{
		if (CharHitObj(player[num].pos, num))
		{
			//壁に当たっていたら、やられ状態にする
			player[num].flg.result = false;
			return;
		}
		SetMapID(player[num].pos);
		for(int dir = 0; dir < KEY_TBL_DIR_MAX; dir++)
		{
			if (CheckHitKey(keyList[num][dir]))
			{
				player[num].dir = (DIR)dir;		//(DIR)dir : 列挙型変数DIRのdir番目
			}
		}
	}

	if (CheckHitKey(keyList[num][KEY_TBL_SPEED]))		//LSHIFTまたはRCONTROL押したらｽﾋﾟｰﾄﾞ半減
	{
		player[num].speed = PLAYER_DEF_SPEED / 2;
	}
	else
	{
		if (player[num].pos.x % 2 == 0 && player[num].pos.y % 2 == 0)	//座標が偶数ならｽﾋﾟｰﾄﾞを戻す
		{
			player[num].speed = PLAYER_DEF_SPEED;
		}
	}

	if (player[num].flg.item)
	{
		CharSkill(num, player[num].pos);
	}

	switch (player[num].dir)
	{
	case DIR_RIGHT:
		player[num].pos.x += player[num].speed;
		break;
	case DIR_LEFT:
		player[num].pos.x -= player[num].speed;
		break;
	case DIR_DOWN:
		player[num].pos.y += player[num].speed;
		break;
	case DIR_UP:
		player[num].pos.y -= player[num].speed;
		break;
	default:
		AST();
		player[num].pos.y -= player[num].speed;
		break;
	}
}

//ｽｷﾙ
void CharSkill(int num, Pos pos)
{
	Pos tmpPos = { pos.x / CHIP_SIZE_X, pos.y / CHIP_SIZE_Y };
	int rival = (num == 0 ? 1 : 0);

	//skill
	if (keyFlg[num][KEY_TBL_ACT] == 1 && keyFlgOld[num][KEY_TBL_ACT] == 0)
	{
		switch (player[num].charID)
		{
		case CHAR_ID_MADOKA:
			//相手の勝利
			player[num].flg.result = false;
			break;
		case CHAR_ID_SAYAKA:
			//ｽﾋﾟｰﾄﾞｱｯﾌﾟしたかった
			//player[num].flg.sy = true;
			//syFlamCnt = 0;

			break;
		case CHAR_ID_MAMI:
			//ﾃｨﾛ・ﾌｨﾅｰﾚ(直線状にﾌﾞﾛｯｸを置く)
			for (int y = 0; y < CHIP_CNT_Y; y++)
			{
				for (int x = 0; x < CHIP_CNT_X; x++)
				{
					if (player[num].pos.x % 2 == 0 && player[num].pos.y % 2 == 0)	//座標が偶数
					{
						//上か下向きなら横
						if (player[num].dir == DIR_UP || player[num].dir == DIR_DOWN)
						{
							if (y == tmpPos.y && mapData[y][x] != 0)
							{
								mapData[y][x] = 1;
							}
						}
						//右か左向きなら縦
						else
						{
							if (x == tmpPos.x && mapData[y][x] != 0)
							{
								mapData[y][x] = 1;
							}
						}
					}
				}
			}
			break;
		case CHAR_ID_KYOKO:
			//前後左右のﾌﾞﾛｯｸ破壊
			for (int y = 0; y < CHIP_CNT_Y; y++)
			{
				for (int x = 0; x < CHIP_CNT_X; x++)
				{
					if (y == tmpPos.y || x == tmpPos.x)
					{
						if (mapData[y][x] != 0)
						{
							mapData[y][x] = 0;
						}
					}
				}
			}
			break;
		case CHAR_ID_HOMURA:
			//時間を止めｒ
			player[rival].flg.move = false;
			stopFlamCnt = 0;
			break;
		case CHAR_ID_QB:
			//おじゃまﾄﾞｱｯﾌﾟ
			player[num].flg.qb = true;
			qbFlamCnt = 0;
			break;
		default:
			AST();
			break;
		}
		player[num].flg.item = false;
		itemPos = { rand() % CHIP_CNT_X, rand() % CHIP_CNT_Y };
	}
}

//ﾌﾟﾚｲﾔｰと壁との当たり判定
bool CharHitObj(Pos pos, int num)
{
	bool rtnFlg = false;	//false:未ｻｲｽﾞｱｯﾌﾟ	true:ｻｲｽﾞｱｯﾌﾟ完了
	Pos tmpPos = { pos.x / CHIP_SIZE_X, pos.y / CHIP_SIZE_Y };

	//map範囲内で
	if ((tmpPos.x >= 0) && (tmpPos.x < CHIP_CNT_X)
		&& (tmpPos.y >= 0) && (tmpPos.y < CHIP_CNT_Y))
	{
		//playerのいるﾏｽのﾌﾞﾛｯｸがｻｲｽﾞｱｯﾌﾟ済みなら
		if (mapData[tmpPos.y][tmpPos.x] >= BLOCK_MAX_SIZE_FLAM)
		{
			rtnFlg = true;
		}
		if (tmpPos.x == itemPos.x && tmpPos.y == itemPos.y)
		{
			player[num].flg.item = true;
		}
	}
	//map範囲外のとき
	else
	{
		rtnFlg = true;
	}

	return rtnFlg;
}

//ｼｰﾝ関連
//ｹﾞｰﾑﾀｲﾄﾙ
void GameTitle(void)
{
	if (keyFlgSp == 1 && keyFlgSpOld == 0)		//押した瞬間
	{
		gameMode = GMODE_EXPLAIN;
	}

	ClsDrawScreen();
	SetFontSize(fontSize);
	DrawGraph(
		GAME_SCREEN_X,
		GAME_SCREEN_Y,
		titleImage,
		false
	);
	if ((flamCnt / 30) % 2 == 0)
	{
		DrawString(
			SCREEN_SIZE_X / 2 - TITLE_IMAGE_SIZE_X / 4,
			GAME_SCREEN_Y + TITLE_IMAGE_SIZE_Y,
			"スペースキーを押してスタート！",
			0x00ffffff,
			false
		);
	}
	ScreenFlip();
}

//説明画面
void GameExplain(void)
{
	if (keyFlgSp == 1 && keyFlgSpOld == 0)		//押した瞬間
	{
		gameMode = GMODE_INIT;
	}
	ClsDrawScreen();
	DrawBox(
		0,
		0,
		SCREEN_SIZE_X,
		SCREEN_SIZE_Y,
		0x00ffffff,
		true
	);
	DrawGraph(
		SCREEN_SIZE_X/ 2 - TITLE_IMAGE_SIZE_X / 2,
		0,
		titleImage,
		true
	);
	DrawFormatString(
		GAME_SCREEN_X + CHIP_SIZE_X * 2,
		GAME_SCREEN_Y + TITLE_IMAGE_SIZE_Y,
		0x00000000,
		"【左側のプレイヤー】\nＷ : 上 Ａ : 右 Ｓ : 下 Ｄ : 左\n左Ｓｈｉｆｔ : 減速\n左Ｃｔｒｌ : 特殊スキル"
	);
	DrawFormatString(
		SCREEN_SIZE_X * 3 / 5,
		GAME_SCREEN_Y + TITLE_IMAGE_SIZE_Y,
		0x00000000,
		"【右側のプレイヤー】\n↑ : 上 → : 右 ↓ : 下 ← : 左\n右Ｓｈｉｆｔ : 減速\n右Ｃｔｒｌ : 特殊スキル"
	);

	if ((flamCnt / 30) % 2 == 0)
	{
		DrawString(
			270,
			20,
			"スペースキーを押してキャラクター選択へ！",
			0x00000000,
			false
		);
	}
	ScreenFlip();
}

//ｷｬﾗｾﾚｸﾄ画面
void CharSelect(int num)
{
	SelectID(num);
	if(player[0].flg.decide && player[1].flg.decide)
	{
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			gameMode = GMODE_BEF_MAIN;
		}
	}
	
	player[num].charID = (CHAR_ID)player[num].chID;		//(DIR)dir : 列挙型変数DIRのdir番目

	ClsDrawScreen();
	DrawGraph(0, 0, gameBgImage, 1);

	//画面上の文字表示
	DrawGraph(
		SCREEN_SIZE_X / 2 - CHAR_SEL_IMAGE_SIZE_X / 2,
		0,
		charSelImage,
		true
	);

	//ｷｬﾗｱｲｺﾝ表示
	DrawGraph(
		GAME_SCREEN_X - CHIP_SIZE_X + ICON_IMAGE_SIZE_X,
		0,
		charImage[player[0].charID].faceImage,
		true
	);
	DrawGraph(
		SCREEN_SIZE_X - ICON_IMAGE_SIZE_X * 2,
		0,
		charImage[player[1].charID].faceImage,
		true
	);

	//立ち絵表示
	DrawGraph(
		GAME_SCREEN_X - 60,
		CHIP_SIZE_Y * 3,
		charImage[player[0].charID].standImage,
		true
	);
	DrawGraph(
		SCREEN_SIZE_X - STAND_IMAGE_SIZE_X + 60,
		CHIP_SIZE_Y * 3,
		charImage[player[1].charID].standImage,
		true
	);

	if (player[0].flg.decide)
	{
		DrawGraph(
			STAND_IMAGE_SIZE_X * 3 / 4 - OK_IMAGE_SIZE_X + CHIP_SIZE_X,
			CHIP_SIZE_Y * 2,
			okImage,
			true
		);
	}
	if (player[1].flg.decide)
	{
		DrawGraph(SCREEN_SIZE_X - OK_IMAGE_SIZE_X,
			CHIP_SIZE_Y * 2,
			okImage,
			true
		);
	}
	if (player[0].flg.decide && player[1].flg.decide)
	{
		if ((flamCnt / 20) % 2 == 0)
		{
			SetFontSize(40);
			fontLen = GetDrawFormatStringWidth("ＰＲＥＡＳＥ　’ＳＰＡＣＥ’　ＫＥＹ");
			DrawFormatString(
				SCREEN_SIZE_X / 2 - fontLen / 2,
				SCREEN_SIZE_Y - CHIP_SIZE_Y * 4,
				0x00ff0000,
				"ＰＲＥＡＳＥ　’ＳＰＡＣＥ’　ＫＥＹ"
			);
			SetFontSize(fontSize);
		}
	}

	ScreenFlip();
}

//ｷｬﾗｾﾚｸﾄ処理
void SelectID(int num)
{
	//decideﾌﾗｸﾞがfalse(未決定状態)、charIDがCHAR_ID_MAXの範囲内のとき
	if (!(player[num].flg.decide))
	{
		//右に進んでいく処理
		if (player[num].chID < CHAR_ID_MAX - 1)
		{
			if (keyFlg[num][DIR_RIGHT] == 1 && keyFlgOld[num][DIR_RIGHT] == 0)
			{
				player[num].chID++;
				//ｷｬﾗ変更したとき、同じｷｬﾗになったらもう一つ先のｷｬﾗに行く
				if (player[0].chID == player[1].chID)
				{
					//もう一つ先にｷｬﾗがいるなら＋、範囲外になるならｷｬﾗを変更させない
					if (player[num].chID + 1 < CHAR_ID_MAX)
					{
						player[num].chID++;
					}
					else
					{
						player[num].chID--;
					}
				}
			}
		}
		//左に戻っていく処理、理屈は右変更処理と同じ
		if (player[num].chID > CHAR_ID_MADOKA)
		{
			if (keyFlg[num][DIR_LEFT] == 1 && keyFlgOld[num][DIR_LEFT] == 0)
			{
				player[num].chID--;
				if (player[0].chID == player[1].chID)
				{
					if (player[num].chID - 1 >= CHAR_ID_MADOKA)
					{
						player[num].chID--;
					}
					else
					{
						player[num].chID++;
					}
				}
			}
		}
	}
	//上を押したらtrue
	if (CheckHitKey(keyList[num][DIR_UP]))
	{
		player[num].flg.decide = true;
	}
	//下を押したらfalse
	if (CheckHitKey(keyList[num][DIR_DOWN]))
	{
		player[num].flg.decide = false;
	}
}

//よーいスタートの部分
void BefGameStart(void)
{
	if (flamCnt > WAIT_CNT)
	{
		gameMode = GMODE_MAIN;
	}

	//WAIT_CNTの途中で「スタート」文字に変える
	int srcY = (flamCnt < WAIT_CNT * 2 / 3 ? 0 : START_IMAGE_SIZE_Y);

	ClsDrawScreen();

	//背景・ｱｲｺﾝ・操作ｷｬﾗの描画
	ObjDraw();

	//「よーいスタート」の描画
	DrawRectGraph(
		SCREEN_SIZE_X / 2 - START_IMAGE_SIZE_X / 2,
		SCREEN_SIZE_Y / 2 - START_IMAGE_SIZE_Y / 2,
		0,
		srcY,
		START_IMAGE_SIZE_X,
		START_IMAGE_SIZE_Y,
		startImage,
		true,
		false
	);
	ScreenFlip();

	//操作ｷｬﾗのｱﾆﾒｰｼｮﾝ
	ObjAnim();
}

//ｹﾞｰﾑﾒｲﾝ
void GameMain(void)
{
	int tmpWinnerID = -1;
	UpDateMap();
	for (int j = 0; j < PLAYER_MAX; j++)
	{
		if (!(player[j].flg.move))
		{
			stopFlamCnt++;
			if (!(player[j].flg.move) && stopFlamCnt > QB_FLAM_MAX)
			{
				player[j].flg.move = true;
			}
		}
		else
		{
			CharCtl(j);		//()内の値を関数に渡す
		}
	}
	for (int p = 0; p < PLAYER_MAX; p++)
	{
		if (!(player[p].flg.result))			
		{
			gameMode = GMODE_RESULT;
		}
		else			//勝利者がいる場合
		{
			tmpWinnerID = p;		
		}
	}
	if (gameMode == GMODE_RESULT)
	{
		winnerID = tmpWinnerID;
	}
	GameDraw();
}

//ﾒｲﾝ描画
void GameDraw(void)
{
	ClsDrawScreen();
	ObjDraw();			//ｷｬﾗ描画
	//qbのｽｷﾙ描画
	for (int p = 0; p < PLAYER_MAX; p++)
	{
		if (player[p].flg.qb)
		{
			int q = (p == 0 ? 1 : 0);
			DrawRotaGraph(
				SCREEN_SIZE_X / 2,
				SCREEN_SIZE_Y,
				1.5f,
				0.0f,
				charImage[player[p].charID].standImage,
				true
			);
			qbFlamCnt++;
			if (qbFlamCnt > QB_FLAM_MAX)
			{
				player[p].flg.qb = false;
			}
		}
	}
	ScreenFlip();

	ObjAnim();			//ｱﾆﾒｰｼｮﾝの更新
}

void ObjAnim(void)		//ｱﾆﾒｰｼｮﾝ用
{
	for (int j = 0; j < PLAYER_MAX; j++)
	{
		player[j].animCnt++;
	}
}

void ObjDraw(void)
{
	DrawGraph(0, 0, gameBgImage, 1);

	//ｷｬﾗｱｲｺﾝ表示
	DrawGraph(
		GAME_SCREEN_X - CHIP_SIZE_X + ICON_IMAGE_SIZE_X,
		0,
		charImage[player[0].charID].faceImage,
		true
	);
	DrawGraph(
		SCREEN_SIZE_X - ICON_IMAGE_SIZE_X * 2,
		0,
		charImage[player[1].charID].faceImage,
		true
	);

	if (!(player[0].flg.item) && !(player[1].flg.item))
	{
		DrawGraph(
			GAME_SCREEN_X + itemPos.x * CHIP_SIZE_X,
			GAME_SCREEN_Y + itemPos.y * CHIP_SIZE_Y,
			seedImage,
			true
		);
	}

	//ﾌﾞﾛｯｸ描画
	for (int y = 0; y < CHIP_CNT_Y; y++)
	{
		for (int x = 0; x < CHIP_CNT_X; x++)
		{
			if (mapData[y][x] > 0)
			{
				int tmpID = mapData[y][x] / BLOCK_CNTUP_INV;
				tmpID = (tmpID >= BLOCK_PTN_MAX ? BLOCK_PTN_MAX - 1 : tmpID);
				DrawGraph(
					GAME_SCREEN_X + (CHIP_SIZE_X * x),		//ｵﾌｾｯﾄ分+1ﾏｽの横ｻｲｽﾞ*ﾏｽ番号
					GAME_SCREEN_Y + (CHIP_SIZE_Y * y),		//ｵﾌｾｯﾄ分+1ﾏｽの縦ｻｲｽﾞ*ﾏｽ番号
					blockImage[tmpID],
					true
				);
			}
		}
	}

	//ﾌﾟﾚｲﾔｰ描画
	for (int p = 0; p < PLAYER_MAX; p++)
	{
		int tmpAnimID = (player[p].animCnt / ANIM_SPEED) % ANIM_IMAGE_MAX;

		DrawGraph(
			GAME_SCREEN_X + player[p].pos.x,
			GAME_SCREEN_Y + player[p].pos.y - (CHIP_SIZE_Y / 3),
			charImage[player[p].charID].image[player[p].dir][tmpAnimID],
			true
		);
	}
}

//ﾘｻﾞﾙﾄ画面
void GameResult(void)
{
	if (keyFlgSp == 1 && keyFlgSpOld == 0)
	{
		gameMode = GMODE_TITLE;
	}

	ClsDrawScreen();
	ObjDraw();
	if ((winnerID >= 0) && (winnerID < PLAYER_MAX))		//片方勝利
	{
		DrawGraph(
			SCREEN_SIZE_X / 2 - STAND_IMAGE_SIZE_X / 2,
			SCREEN_SIZE_Y / 2 - STAND_IMAGE_SIZE_Y / 2,
			charImage[player[winnerID].charID].standImage,
			true
		);
		DrawGraph(
			SCREEN_SIZE_X / 2 - WIN_IMAGE_SIZE_X / 2,
			SCREEN_SIZE_Y / 2 - WIN_IMAGE_SIZE_Y / 2,
			winTxtImage[player[winnerID].charID],
			true
		);
	}
	else if (winnerID == -1)		//ﾄﾞﾛｰ
	{
		DrawGraph(
			SCREEN_SIZE_X / 2 - WIN_IMAGE_SIZE_X / 2,
			SCREEN_SIZE_Y / 2 - WIN_IMAGE_SIZE_Y / 2,
			winTxtImage[CHAR_ID_QB],
			true
		);
	}
	else
	{
		AST();
	}
	if ((flamCnt / 30) % 2 == 0)
	{
		DrawString(270, 20, "ＰＲＥＡＳＥ　’ＳＰＡＣＥ’　ＫＥＹ", 0x00000000, false);
	}
	ScreenFlip();
}

/*
typedef struct タグ名{
	メンバー変数1,
	メンバー変数2,
		・
		・
}構造体名;
タグ名、構造体名の省略は可能　※ただし、構造体名を省略するとｱｸｾｽのしようがなくなる
構造体名(ｸﾞﾙｰﾌﾟ名)を型名として使える

ﾒﾓﾘの動的確保
ｱﾛｰ演算子(->)

ｽﾀｯｸはﾌﾟﾛｸﾞﾗﾑが実行されたときに自動確保される
ﾋｰﾌﾟ：動的←実行したままにしたらﾒﾓﾘﾘｰｸが起こる

必要に応じてﾒﾓﾘをとる

画像は画像で読み込んで確保しておく
「何番目を使う」ことだけをﾌﾟﾚｲﾔｰにわたす

const ： 簡単に言うとRO

5/22課題
・とりあえず自由に動けるようにする
・移動時に向きを変える
→ｷｰ入力の方向の情報を保存し、それを利用して描画する
◎一度ｷｰ入力をしたらずっと同じ方向に移動するようにする
◎ﾏｽ目単位で向きを変えるようにする

5ﾌﾚｰﾑごとに絵が切り替わるようにする
5ﾌﾚｰﾑごと = (ｶｳﾝﾀ / 5)
つまりｶｳﾝﾀが1になるまで通常の5倍かかるようにする

ﾌﾟﾚｲﾔｰの前に処理するか後に処理するかでﾌﾟﾚｲﾔｰの生き死にが決まる

5/25課題
・ｻｲｽﾞｱｯﾌﾟとその描画

5/29～
320*320の画像を持ってくる

残り実装する機能
・ｷｬﾗｾﾚｸﾄ
・ｹﾞｰﾑ開始前の待ち

5/31
特定のﾎﾞﾀﾝを押すと速度が半減する機能
1P:LSHIFT	2P:RSHIFT
※速度を落としたが最後、下手すると一生曲がれなくなる…かもしれない
理由↓
ﾌﾟﾚｲﾔｰの移動速度が奇数になると、座標の値がﾏｽの判定範囲と食い違ってしまうため
*/