#pragma once
#include "../QQSG/Module.h"
#include "../QQSG/GameDirectX9.h"
#include "../QQSG/ResourceFile.hpp"
#include "resource.h"
#include <string.h>
#include <windows.h>
#include <windowsx.h>
#include <mutex>
using namespace ResouceFile;
#pragma pack(push,1)
struct MouseInfo {
	int x;
	int y;
	int KeyCode;
	int KeyState;
	UINT OldTick;
};
#pragma pack(pop)
#pragma region ȫ�ֱ���
#define ThreadMutexLockDeleay 3500
DirectX92D D2Dx9;
HWND G_hWnd;
BOOL DrawThread = TRUE, ISMove, IsTip;
WNDPROC OldDrawWndProc = nullptr, OldEditWndProc = nullptr, OldEditDelayWndProc = nullptr;
MapImageInfo ImageInfo = { 0 };
MouseInfo MousePoint = { 0 }, Mibfh = { 0 };
HINSTANCE G_hInst;
HANDLE G_lpDrawThread = nullptr;
RECT GlRect, ClRect, lRect;
UINT OldFpsTick = 0, FPSCount = 0, NewFPSCount = 0, SelectIndex = 0x8FFFFFF, SelectAnimateIndex = 0x8FFFFFF, CurrencyIndex = 0, UpIndex = 0x8FFFFFF,
OldKeyTick = 0;
int ImageCreenX = 0, ImageCreenY = 0;
MouseInfo BOMouse;
#pragma endregion
BOOL CALLBACK WndProcc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//D3D�����߳�;
void WINAPI DrawD3D();
//���ƴ��ڴ�����;
HRESULT CALLBACK DrawWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//ͼƬ֡����ʱ���ڴ�����;
HRESULT CALLBACK EditAnimateDelayWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//ͼƬ���ű������ڴ�����;
HRESULT CALLBACK EditScaleWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//ɾ����ǰѡ��ͼƬ;
void WINAPI DeleteImageInfo();
//ɾ������ͼƬ;
void WINAPI DeleteAllImage();
//��ͼƬ����һ��;
void WINAPI LayerUpImage();
//��ͼƬ����һ��;
void WINAPI LayerNextImage();
//��ͼƬ�ö���;
void WINAPI LayerTopImage();
//��ͼƬ�õײ�;
void WINAPI LayerBottomImage();
//���������߳�;
void WINAPI RunDrawD3D();
//�رջ����߳�;
void WINAPI CloseDrawD3D();
//���뱳��ͼƬ;
void WINAPI ImportImage();
//���붯��ͼƬ;
void WINAPI ImportAnimateImage();
//��һ֡����;
void WINAPI UpAnimateFrams();
//��һ֡����;
void WINAPI NextAnimateFrams();
//�ö�֡����;
void WINAPI TopAnimateFrams();
//�õ�֡����;
void WINAPI BottomAnimateFrams();
//����һ֡����;
void WINAPI SetUpAnimateFrams();
//����һ֡����;
void WINAPI SetNextAnimateFrams();
//����ͼƬͼ��(������ͼƬ)
void WINAPI HideImageLayer();
//��ʾ��������ͼƬͼ��(����ͼƬ)
void WINAPI ShowAllHideImageLayer();
//�������е�ͼ�ļ�;
void WINAPI ImportMap();
//��ˮƽ����;
void WINAPI AnimateMirror();
//�������ͼƬ;
void WINAPI CopyImageInfo();
//��ʾFPS;
void WINAPI ShowFPS();
//�����߳�
void WINAPI CallThreadFunction(void *FuncAddress);