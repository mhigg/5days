#include "DxLib.h"

//���ޯ��ү���ޗp��`
#ifdef _DEBUG
#define AST() {\
	CHAR ast_mes[256];\
	wsprintf( ast_mes, "%s %d�s��\0", __FILE__, __LINE__ );\
	MessageBox( 0, ast_mes, "���ĕ\��", MB_OK );\
	}
#else
#define AST()
#endif	//_DEBUG

#define TITLE_IMAGE_SIZE_X	700						//����ۺނ̉�����
#define TITLE_IMAGE_SIZE_Y	300						//����ۺނ̏c����
#define CHAR_SEL_IMAGE_SIZE_X	400					//��׾�ۺނ̉�����
#define CHAR_SEL_IMAGE_SIZE_Y	80					//��׾�ۺނ̏c����
#define START_IMAGE_SIZE_X	210
#define START_IMAGE_SIZE_Y	75
#define OK_IMAGE_SIZE_X		150
#define OK_IMAGE_SIZE_Y		150

#define WIN_IMAGE_SIZE_X	450						//�����Җ��摜�̉�����
#define WIN_IMAGE_SIZE_Y	(300 / CHAR_ID_MAX)		//�����Җ��摜�̏c����
#define STAND_IMAGE_SIZE_X	500						//��ׂ̗����G�̉�����
#define STAND_IMAGE_SIZE_Y	500						//��ׂ̗����G�̗����G�̏c����
#define ICON_IMAGE_SIZE_X	64
#define ICON_IMAGE_SIZE_Y	64

#define BLOCK_PTN_MAX		7		//��ۯ��摜������ݐ�
#define BLOCK_CNTUP_INV		3		//��ۯ������ޱ��߂����ڰъԊu
#define BLOCK_MAX_SIZE_FLAM	(BLOCK_PTN_MAX - 1) * BLOCK_CNTUP_INV 		//���ޱ��߂ɕK�v�ȑ��ڰѐ�

#define CHIP_CNT_X			23		//�ްщ�ʂ̉���Ͻ��
#define CHIP_CNT_Y			14		//�ްщ�ʂ̏c��Ͻ��
#define CHIP_SIZE_X			32		//1Ͻ�̉�����
#define CHIP_SIZE_Y			32		//1Ͻ�̏c����

#define SCREEN_SIZE_X		800		//��ʂ̉�����
#define SCREEN_SIZE_Y		600		//��ʂ̏c����
#define GAME_SCREEN_X		CHIP_SIZE_X			//�ްѴر�̵̾��
#define GAME_SCREEN_Y		(CHIP_SIZE_Y * 3)	//�ްѴر�̵̾��
#define GAME_SCREEN_SIZE_X	(CHIP_CNT_X * CHIP_SIZE_X)
#define GAME_SCREEN_SIZE_Y	(CHIP_CNT_Y * CHIP_SIZE_Y)

#define PLAYER_SIZE_X		32
#define PLAYER_DEF_SPEED	2		//���@����̫�Ľ�߰��

#define ANIM_IMAGE_MAX		4
#define ANIM_SPEED			5

#define WAIT_CNT			120
#define QB_FLAM_MAX			240

enum GMODE {
	GMODE_TITLE,		//���ĉ��
	GMODE_EXPLAIN,		//�������
	GMODE_INIT,			//������
	GMODE_CHAR_SEL,		//��׾ډ��
	GMODE_BEF_MAIN,		//��[�������[��
	GMODE_MAIN,			//�ްщ��
	GMODE_RESULT,		//ػ��ĉ��
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
	IMAGE_TYPE_CHAR,	//�ްђ��ɑ��삷��摜
	IMAGE_TYPE_FACE,	//�籲��
	IMAGE_TYPE_STAND,	//�����G
	IMAGE_TYPE_MAX
};

enum DIR {				//����
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
	KEY_TBL_SPEED,		//��߰��
	KEY_TBL_ACT,		//���
	KEY_TBL_MAX
};

#define KEY_TBL_DIR_MAX	(KEY_TBL_LEFT + 1)		//DIR_MAX�̒���

GMODE gameMode;			//���݂̹ް�Ӱ��
GMODE gameModeOld;		//1�ڰёO�̹ް�Ӱ��

typedef struct {
	bool item;
	bool move;
	bool qb;		//qb��ٗp
	bool result;	//ػ��ėp
	bool decide;	//��׾ڸČ���p
}Flg;

//���W�\����
typedef struct {
	int x;			//x���W
	int y;			//y���W
}Pos;

//��ڲ԰�\����
typedef struct {
	CHAR_ID charID;		//�I���ID
	Pos pos;
	int speed;			//�ړ����x
	Flg flg;			//���
	int animCnt;		//��Ұ��̶݂���
	DIR dir;			//����
	int chID;
}Player;

//��ׂ̉摜�p�\����
typedef struct {
	int image[DIR_MAX][ANIM_IMAGE_MAX];		//��׉摜ID
	int faceImage;							//��摜ID
	int standImage;							//�����GID
}CHAR_IMAGE;

#define PLAYER_MAX 2	//��ڲ԰��

int winnerID;		//������ID
int flamCnt;		//�o���ڰѐ�
int qbFlamCnt;		//qb��ٔ������̌o���ڰѐ�
int stopFlamCnt;
int fontLen;		//�����̒���
int fontSize;		//̫�Ļ���

Player player[PLAYER_MAX];
CHAR_IMAGE charImage[CHAR_ID_MAX];
int mapData[CHIP_CNT_Y][CHIP_CNT_X];		//ϯ���ް��i�[
Pos itemPos;

//�ړ����̷�ؽ�
const int keyList[PLAYER_MAX][KEY_TBL_MAX] = {
	{ KEY_INPUT_W,  KEY_INPUT_D,     KEY_INPUT_S,    KEY_INPUT_A,    KEY_INPUT_LSHIFT,   KEY_INPUT_LCONTROL },
	{ KEY_INPUT_UP, KEY_INPUT_RIGHT, KEY_INPUT_DOWN, KEY_INPUT_LEFT, KEY_INPUT_RCONTROL, KEY_INPUT_RSHIFT}
};

int keyFlgSp;			//��߰��������׸�
int keyFlgSpOld;		//1�ڰёO�̽�߰��������׸�
int keyFlg[PLAYER_MAX][KEY_TBL_MAX];
int keyFlgOld[PLAYER_MAX][KEY_TBL_MAX];

//����p�r�̲Ұ��
int titleImage;			//���ى��
int charSelImage;		//��׾ڸ�
int okImage;			//OK
int startImage;			//��[�������[��
int gameBgImage;		//�w�i
int blockImage[BLOCK_PTN_MAX];		//��ۯ��摜
int winTxtImage[CHAR_ID_MAX];		//�����Җ�����
int seedImage;

//�������ߐ錾
//������
bool SysInit(void);
bool GameInit(void);

void KeyFlg(int num);

//�ްђ�
void GameTitle(void);
void GameExplain(void);
void CharSelect(int num);
void SelectID(int num);
void BefGameStart(void);
void GameMain(void);
void GameResult(void);

//��ڲ԰�n
void CharCtl(int);
void CharSkill(int num, Pos pos);
bool CharHitObj(Pos, int);

//��ۯ��n
void InitMapID(void);
void SetMapID(Pos);
void UpDateMap(void);

//�`��n
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
			flamCnt = 0;		//�ް�Ӱ�ނ��؂�ւ�������۸ر
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
		//�c�FCHAR_ID	��IMAGE_TYPE	���,����,�����G
		{"image/madoka.png",	"image/icon_madoka.png",	"image/madoka_st.png"},
		{"image/sayaka.png",	"image/icon_sayaka.png",	"image/sayaka_st.png"},
		{"image/mami.png",		"image/icon_mami.png",		"image/mami_st.png"},
		{"image/kyoko.png",		"image/icon_kyoko.png",		"image/kyoko_st.png"},
		{"image/homura.png",	"image/icon_homura.png",	"image/homura_st.png"},
		{"image/qb.png"	,		"image/icon_qb.png",		"image/qb_st.png"} 
	};

	//-------------- ���я���
	SetWindowText("MADOKA_SNAKE");
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);		//800�~600�ޯ�65536�FӰ�ނɐݒ�
	ChangeWindowMode(true);								//true:����޳Ӱ�� false:�ٽ�ذ�

	if (DxLib_Init() == -1)		//DxLib_Init():DxLibײ���؂̏�����
	{							//���s��-1 �����͂���ȊO
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

	//��׉摜�Ǎ�����
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
			fileNameList[j][IMAGE_TYPE_CHAR],	//�ǂݍ��މ摜
			DIR_MAX * ANIM_IMAGE_MAX,			//�������S��
			ANIM_IMAGE_MAX, DIR_MAX,			//���Əc�̕�����
			CHIP_SIZE_X, CHIP_SIZE_Y,			//�����ނƏc����
			charImage[j].image[0],
			false) == -1)
		{
			AST();
			return false;
		}
		//&charImage[j].image[0][0] �� charImage[j].image[0]	�����Ӗ�
	}

	//��ۯ��摜
	if (LoadDivGraph(
		"image/block.png",				//�ǂݍ��މ摜
		BLOCK_PTN_MAX,					//�������S��
		BLOCK_PTN_MAX, 1,				//���Əc�̕�����
		CHIP_SIZE_X, CHIP_SIZE_Y,		//�����ނƏc����
		blockImage,
		false) == -1)
	{
		AST();
		return false;
	}

	//�����Җ��摜
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
		player[j].flg.result = true;		//���s����
		player[j].flg.decide = false;		//��׾ڸď��
		player[j].flg.qb = false;			//QB���
		player[j].flg.move = true;			//�قނ罷�
		player[j].flg.item = false;			//���ю擾���
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

//ϯ�ߊ֐��Q
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

	//�z��̔ԍ������߂�
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
					//��ۯ��̻��ޱ���
					mapData[y][x] += 2;
				}
			}
		}
	}

}

//���׸ނ̍X�V
void KeyFlg(int num)
{
	keyFlgOld[num][DIR_RIGHT] = keyFlg[num][DIR_RIGHT];
	keyFlg[num][DIR_RIGHT] = CheckHitKey(keyList[num][DIR_RIGHT]);
	keyFlgOld[num][DIR_LEFT] = keyFlg[num][DIR_LEFT];
	keyFlg[num][DIR_LEFT] = CheckHitKey(keyList[num][DIR_LEFT]);
	keyFlgOld[num][KEY_TBL_ACT] = keyFlg[num][KEY_TBL_ACT];
	keyFlg[num][KEY_TBL_ACT] = CheckHitKey(keyList[num][KEY_TBL_ACT]);
}

//��ڲ԰
void CharCtl(int num)		//�n���ꂽ�l��num��int�^�œ����
{
	if (0 > num || num >= PLAYER_MAX)		//�l�͈̔�����
	{
		AST();
		return;
	}										//�l���ω�����Ȃ炻�̂��тɔ͈��������ׂ�

	//player�̈ړ�����
	//player��Ͻ�̒��S�ɂ���Ƃ�
	if (player[num].pos.x % CHIP_SIZE_X == 0 && player[num].pos.y % CHIP_SIZE_Y == 0)
	{
		if (CharHitObj(player[num].pos, num))
		{
			//�ǂɓ������Ă�����A�����Ԃɂ���
			player[num].flg.result = false;
			return;
		}
		SetMapID(player[num].pos);
		for(int dir = 0; dir < KEY_TBL_DIR_MAX; dir++)
		{
			if (CheckHitKey(keyList[num][dir]))
			{
				player[num].dir = (DIR)dir;		//(DIR)dir : �񋓌^�ϐ�DIR��dir�Ԗ�
			}
		}
	}

	if (CheckHitKey(keyList[num][KEY_TBL_SPEED]))		//LSHIFT�܂���RCONTROL���������߰�ޔ���
	{
		player[num].speed = PLAYER_DEF_SPEED / 2;
	}
	else
	{
		if (player[num].pos.x % 2 == 0 && player[num].pos.y % 2 == 0)	//���W�������Ȃ��߰�ނ�߂�
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

//���
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
			//����̏���
			player[num].flg.result = false;
			break;
		case CHAR_ID_SAYAKA:
			//��߰�ޱ��߂���������
			//player[num].flg.sy = true;
			//syFlamCnt = 0;

			break;
		case CHAR_ID_MAMI:
			//èہĘŰ�(���������ۯ���u��)
			for (int y = 0; y < CHIP_CNT_Y; y++)
			{
				for (int x = 0; x < CHIP_CNT_X; x++)
				{
					if (player[num].pos.x % 2 == 0 && player[num].pos.y % 2 == 0)	//���W������
					{
						//�ォ�������Ȃ牡
						if (player[num].dir == DIR_UP || player[num].dir == DIR_DOWN)
						{
							if (y == tmpPos.y && mapData[y][x] != 0)
							{
								mapData[y][x] = 1;
							}
						}
						//�E���������Ȃ�c
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
			//�O�㍶�E����ۯ��j��
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
			//���Ԃ��~�߂�
			player[rival].flg.move = false;
			stopFlamCnt = 0;
			break;
		case CHAR_ID_QB:
			//��������ޱ���
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

//��ڲ԰�ƕǂƂ̓����蔻��
bool CharHitObj(Pos pos, int num)
{
	bool rtnFlg = false;	//false:�����ޱ���	true:���ޱ��ߊ���
	Pos tmpPos = { pos.x / CHIP_SIZE_X, pos.y / CHIP_SIZE_Y };

	//map�͈͓���
	if ((tmpPos.x >= 0) && (tmpPos.x < CHIP_CNT_X)
		&& (tmpPos.y >= 0) && (tmpPos.y < CHIP_CNT_Y))
	{
		//player�̂���Ͻ����ۯ������ޱ��ߍς݂Ȃ�
		if (mapData[tmpPos.y][tmpPos.x] >= BLOCK_MAX_SIZE_FLAM)
		{
			rtnFlg = true;
		}
		if (tmpPos.x == itemPos.x && tmpPos.y == itemPos.y)
		{
			player[num].flg.item = true;
		}
	}
	//map�͈͊O�̂Ƃ�
	else
	{
		rtnFlg = true;
	}

	return rtnFlg;
}

//��݊֘A
//�ް�����
void GameTitle(void)
{
	if (keyFlgSp == 1 && keyFlgSpOld == 0)		//�������u��
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
			"�X�y�[�X�L�[�������ăX�^�[�g�I",
			0x00ffffff,
			false
		);
	}
	ScreenFlip();
}

//�������
void GameExplain(void)
{
	if (keyFlgSp == 1 && keyFlgSpOld == 0)		//�������u��
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
		"�y�����̃v���C���[�z\n�v : �� �` : �E �r : �� �c : ��\n���r�������� : ����\n���b������ : ����X�L��"
	);
	DrawFormatString(
		SCREEN_SIZE_X * 3 / 5,
		GAME_SCREEN_Y + TITLE_IMAGE_SIZE_Y,
		0x00000000,
		"�y�E���̃v���C���[�z\n�� : �� �� : �E �� : �� �� : ��\n�E�r�������� : ����\n�E�b������ : ����X�L��"
	);

	if ((flamCnt / 30) % 2 == 0)
	{
		DrawString(
			270,
			20,
			"�X�y�[�X�L�[�������ăL�����N�^�[�I���ցI",
			0x00000000,
			false
		);
	}
	ScreenFlip();
}

//��׾ڸĉ��
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
	
	player[num].charID = (CHAR_ID)player[num].chID;		//(DIR)dir : �񋓌^�ϐ�DIR��dir�Ԗ�

	ClsDrawScreen();
	DrawGraph(0, 0, gameBgImage, 1);

	//��ʏ�̕����\��
	DrawGraph(
		SCREEN_SIZE_X / 2 - CHAR_SEL_IMAGE_SIZE_X / 2,
		0,
		charSelImage,
		true
	);

	//��ױ��ݕ\��
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

	//�����G�\��
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
			fontLen = GetDrawFormatStringWidth("�o�q�d�`�r�d�@�f�r�o�`�b�d�f�@�j�d�x");
			DrawFormatString(
				SCREEN_SIZE_X / 2 - fontLen / 2,
				SCREEN_SIZE_Y - CHIP_SIZE_Y * 4,
				0x00ff0000,
				"�o�q�d�`�r�d�@�f�r�o�`�b�d�f�@�j�d�x"
			);
			SetFontSize(fontSize);
		}
	}

	ScreenFlip();
}

//��׾ڸď���
void SelectID(int num)
{
	//decide�׸ނ�false(��������)�AcharID��CHAR_ID_MAX�͈͓̔��̂Ƃ�
	if (!(player[num].flg.decide))
	{
		//�E�ɐi��ł�������
		if (player[num].chID < CHAR_ID_MAX - 1)
		{
			if (keyFlg[num][DIR_RIGHT] == 1 && keyFlgOld[num][DIR_RIGHT] == 0)
			{
				player[num].chID++;
				//��וύX�����Ƃ��A������ׂɂȂ�����������̷�ׂɍs��
				if (player[0].chID == player[1].chID)
				{
					//�������ɷ�ׂ�����Ȃ�{�A�͈͊O�ɂȂ�Ȃ緬ׂ�ύX�����Ȃ�
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
		//���ɖ߂��Ă��������A�����͉E�ύX�����Ɠ���
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
	//�����������true
	if (CheckHitKey(keyList[num][DIR_UP]))
	{
		player[num].flg.decide = true;
	}
	//������������false
	if (CheckHitKey(keyList[num][DIR_DOWN]))
	{
		player[num].flg.decide = false;
	}
}

//��[���X�^�[�g�̕���
void BefGameStart(void)
{
	if (flamCnt > WAIT_CNT)
	{
		gameMode = GMODE_MAIN;
	}

	//WAIT_CNT�̓r���Łu�X�^�[�g�v�����ɕς���
	int srcY = (flamCnt < WAIT_CNT * 2 / 3 ? 0 : START_IMAGE_SIZE_Y);

	ClsDrawScreen();

	//�w�i�E���݁E���췬ׂ̕`��
	ObjDraw();

	//�u��[���X�^�[�g�v�̕`��
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

	//���췬ׂ̱�Ұ���
	ObjAnim();
}

//�ް�Ҳ�
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
			CharCtl(j);		//()���̒l���֐��ɓn��
		}
	}
	for (int p = 0; p < PLAYER_MAX; p++)
	{
		if (!(player[p].flg.result))			
		{
			gameMode = GMODE_RESULT;
		}
		else			//�����҂�����ꍇ
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

//Ҳݕ`��
void GameDraw(void)
{
	ClsDrawScreen();
	ObjDraw();			//��ו`��
	//qb�̽�ٕ`��
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

	ObjAnim();			//��Ұ��݂̍X�V
}

void ObjAnim(void)		//��Ұ��ݗp
{
	for (int j = 0; j < PLAYER_MAX; j++)
	{
		player[j].animCnt++;
	}
}

void ObjDraw(void)
{
	DrawGraph(0, 0, gameBgImage, 1);

	//��ױ��ݕ\��
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

	//��ۯ��`��
	for (int y = 0; y < CHIP_CNT_Y; y++)
	{
		for (int x = 0; x < CHIP_CNT_X; x++)
		{
			if (mapData[y][x] > 0)
			{
				int tmpID = mapData[y][x] / BLOCK_CNTUP_INV;
				tmpID = (tmpID >= BLOCK_PTN_MAX ? BLOCK_PTN_MAX - 1 : tmpID);
				DrawGraph(
					GAME_SCREEN_X + (CHIP_SIZE_X * x),		//�̾�ĕ�+1Ͻ�̉�����*Ͻ�ԍ�
					GAME_SCREEN_Y + (CHIP_SIZE_Y * y),		//�̾�ĕ�+1Ͻ�̏c����*Ͻ�ԍ�
					blockImage[tmpID],
					true
				);
			}
		}
	}

	//��ڲ԰�`��
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

//ػ��ĉ��
void GameResult(void)
{
	if (keyFlgSp == 1 && keyFlgSpOld == 0)
	{
		gameMode = GMODE_TITLE;
	}

	ClsDrawScreen();
	ObjDraw();
	if ((winnerID >= 0) && (winnerID < PLAYER_MAX))		//�Е�����
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
	else if (winnerID == -1)		//��۰
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
		DrawString(270, 20, "�o�q�d�`�r�d�@�f�r�o�`�b�d�f�@�j�d�x", 0x00000000, false);
	}
	ScreenFlip();
}

/*
typedef struct �^�O��{
	�����o�[�ϐ�1,
	�����o�[�ϐ�2,
		�E
		�E
}�\���̖�;
�^�O���A�\���̖��̏ȗ��͉\�@���������A�\���̖����ȗ�����Ʊ����̂��悤���Ȃ��Ȃ�
�\���̖�(��ٰ�ߖ�)���^���Ƃ��Ďg����

��؂̓��I�m��
�۰���Z�q(->)

��������۸��т����s���ꂽ�Ƃ��Ɏ����m�ۂ����
˰�߁F���I�����s�����܂܂ɂ��������ذ����N����

�K�v�ɉ�������؂��Ƃ�

�摜�͉摜�œǂݍ���Ŋm�ۂ��Ă���
�u���Ԗڂ��g���v���Ƃ�������ڲ԰�ɂ킽��

const �F �ȒP�Ɍ�����RO

5/22�ۑ�
�E�Ƃ肠�������R�ɓ�����悤�ɂ���
�E�ړ����Ɍ�����ς���
�������͂̕����̏���ۑ����A����𗘗p���ĕ`�悷��
����x�����͂������炸���Ɠ��������Ɉړ�����悤�ɂ���
��Ͻ�ڒP�ʂŌ�����ς���悤�ɂ���

5�ڰт��ƂɊG���؂�ւ��悤�ɂ���
5�ڰт��� = (���� / 5)
�܂足����1�ɂȂ�܂Œʏ��5�{������悤�ɂ���

��ڲ԰�̑O�ɏ������邩��ɏ������邩����ڲ԰�̐������ɂ����܂�

5/25�ۑ�
�E���ޱ��߂Ƃ��̕`��

5/29�`
320*320�̉摜�������Ă���

�c���������@�\
�E��׾ڸ�
�E�ްъJ�n�O�̑҂�

5/31
��������݂������Ƒ��x����������@�\
1P:LSHIFT	2P:RSHIFT
�����x�𗎂Ƃ������Ō�A���肷��ƈꐶ�Ȃ���Ȃ��Ȃ�c��������Ȃ�
���R��
��ڲ԰�̈ړ����x����ɂȂ�ƁA���W�̒l��Ͻ�̔���͈͂ƐH������Ă��܂�����
*/