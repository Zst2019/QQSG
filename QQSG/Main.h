#pragma once
#include "bass.h"
#include "Module.h"
#include "GameDirectX9.h"
#include "zlib\zlib.h"
#include "ResourceFile.hpp"
#include "ImeInput.h"
#include <windows.h>
#include <windowsx.h>

#pragma comment(lib,"bass.lib")
#pragma warning(disable:4996)
using namespace ResouceFile;
#define CL_WinWidth 1024
#define CL_WinHeight 768
#pragma pack(push,1)
#pragma region ȫ�����ݽṹ
struct MouseInfo {
	int x;
	int y;
	int KeyCode;
	int KeyState;
	UINT OldTick;
};
enum _ButtonType {
	Button_Dan = 0x61,
	Button_Duo = 0x62,
};
typedef struct _DrawMapInfo
{
	float x;
	float y;
	UINT width;
	UINT height;
	float Scale;
	float rotation;
	ImageLoadStyle ImgLoadType;
	LPDIRECT3DTEXTURE9 Texture;
	void *ResAlloc;
	_DrawMapInfo *Animate;
	UINT AnimateMaxCount;
	UINT AnimateTickIndex;
	UINT AnimateOldTick;
	UINT AnimateDelay;
}DrawMapInfo, *PDrawMapInfo;
typedef struct _DrawImageInfo
{
	PDrawMapInfo DrawMap;
	UINT MaxInt;
	UINT AnimateState;
}DrawImageInfo, *PDrawImageInfo;
typedef struct _ButtonImageInfo{
	float x;
	float y;
	UINT Width;
	UINT Height;
	UINT SelectIndex;
	UINT ButtonNum;
	_ButtonType ButtonType;
	LPDIRECT3DTEXTURE9 Texture;
	_ButtonImageInfo *MultiButton;
	void *ResAlloc;
}ButtonImageInfo, *PButtonImageInfo;
struct ButtonInfo
{
	PButtonImageInfo Button;
	UINT MaxButton;
};
#pragma pack(pop)
#pragma endregion

#pragma region //ȫ�ֱ���
const WCHAR GVar_ClassName[] = L"QQSGDiy";
char GVar_szCareerTip[256] = { "        ����ʿ\r\n\r\nԶ��ؼ����ؼ�߹�ؼʩ����" };
char GVar_szTipBoxText[1024] = { 0 };
HWND GVar_hWnd;
BOOL GVar_DrawGame = TRUE, GVar_DrawTip = FALSE, GVar_DrawCreatPlyer = TRUE, GVar_IsTipBox = FALSE, GVar_DrawCityMappl = FALSE;
void *GVar_BGM_Login1 = nullptr;
HSTREAM GVar_StreamBGM1;
DirectX92D GVar_D2Dx9;
ImeInput GVar_Ime;
MouseInfo GVar_MousePoint = { 0 };
UINT GVar_OldFpsTick = 0, GVar_FPSCount = 0, GVar_NewFPSCount = 0, GVar_OldTickCount = 0, GVar_PlayerIndex = 0, GVar_Country = 0, GVar_ImeTick = 0;
DrawImageInfo GVar_MapImageInfo = { 0 }, GVar_PlayerImageInfo = { 0 };
UINT GVar_���ְҵ = 0, GVar_��ҹ��� = 0, GVar_CreenCentreX = 500, GVar_CreenCentreY = 500;
BOOL GVar_����Ա��Ƿ�Ϊ�� = FALSE;
char GVar_�������[20] = { 0 };
ButtonInfo GVar_ClButton = { 0 }, GVar_ClTipButton = { 0 };
#pragma endregion
//���ڴ�����;
LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//��Ϸ��Ⱦ����;
void D3DGameRun();
//���ŵ�¼��������;
void PlayerBGM_Login(const char *MusicName);
//��ʾFPS֡��:
void ShowFPS();
//��Դ�ļ�����;
UINT LoadResourceData(const char *pFile, const char *pResName, void **pAlloc);
//�ͷż��ص���Դ�ļ�;
void ReleaseResourceData(void *pAlloc);
//��ͼ��Դ����;
void LoadMapResourceData(ResouceDataFile::ResMapOInfo *ResMpIOinfo, DrawImageInfo &pMapAlloc, UINT pLen, const char*pMapFile);
//�ͷ��Ѽ��صİ�ť��Դ
void ReleaseButtonResource(ButtonInfo &PMapImageInfo);
//�ͷ��Ѽ��صĵ�ͼ��Դ
void ReleaseMapResource(DrawImageInfo &PMapImageInfo);
//���Ʋ���ְҵ���;
void PlayerChange(const char *pMapFile, DrawImageInfo &pMapAlloc);
//������ҽ�ɫ����;
void DrawCreatePlayer();
//���뷨������;
void ImeEvent(WPARAM wParam);
//��ʾ�Ի���;
void SetTipDialogBoxText(const char *pText);
void DrawTipDialogBox();
//������ʾ��;
void DrawTipBox();
//�ж�����Ƿ���ѡ����
BOOL IsRectMouse(float x, float y, UINT Width, UINT Height, MouseInfo const&MousePoint);
//������ӳټ���;
BOOL ClickMouseDelay();
//���Ƴ��е�ͼ;
void DrawCityMap();