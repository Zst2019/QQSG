#pragma once
#ifndef _GameDirectX9_H
#define _GameDirectX9_H
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include "Module.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")
#pragma warning(disable:4996)

class DirectX92D
{
public:
	//��������;
	DirectX92D();
	~DirectX92D();
	//��ʼ��D3Directx9;
	BOOL InitD3D(HWND hWnd, int Width, int Height, bool FullScreen);
	//��ȡͼƬ��Ϣ;
	D3DXIMAGE_INFO GetImageInfo(const wchar_t *filename);
	//��ȡ�ڴ�ͼƬ��Ϣ;
	D3DXIMAGE_INFO GetImageInfoInMemory(void *SrcData, UINT SrcLen);
	//�����ڴ�����;
	LPDIRECT3DTEXTURE9 LoadMemTexture(void* ImgStream, UINT Imglen, D3DCOLOR transcolor = NULL);
	//���������ļ�;
	LPDIRECT3DTEXTURE9 LoadTexture(const wchar_t* filename, D3DCOLOR transcolor = NULL);
	//���������ļ�;
	LPDIRECT3DTEXTURE9 LoadRectTexture(const wchar_t* filename, UINT Width, UINT Height, D3DCOLOR transcolor = D3DCOLOR_XRGB(0, 0, 0));
	//����һ������;(����,x,y,���,�߶�,���ű���,��ת����,��ɫ);
	BOOL DrawTexture(LPDIRECT3DTEXTURE9 texture, float x, float y, UINT width = 50, UINT height = 50, float scale = 1.0f, float rotation = 0.0f, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));
	// ������֡����;(����,x,y,���,�߶�,����֡,ͼƬ���֡��,���ű���,��ת����,��ɫ);
	BOOL DrawAnimateTexture(LPDIRECT3DTEXTURE9 image, int x, int y, int width, int height, int frame = 0, int columns = 1, float scale = 1.0f, float rotation = 0.0f, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));
	//����ͼƬ���ĵ����;;(ͼƬ�ļ�,x,y,���,�߶�,���ű���,��ת����,��ɫ);
	BOOL DrawImageCenterTexture(const wchar_t *ImageName, int x, int y, int width = 50, int height = 50, float scale = 1.0f, float rotation = 0.0f, D3DCOLOR Imagecolor = D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR Drawcolor = D3DCOLOR_XRGB(255, 255, 255));
	//����ͼƬ�Զ����С����;;(ͼƬ�ļ�,x,y,���,�߶�,���ű���,��ת����,��ɫ);
	BOOL DrawImageRectTexture(const wchar_t *ImageName, int x, int y, int width, int height, float scale = 1.0f, float rotation = 0.0f, D3DCOLOR Imagecolor = D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR Drawcolor = D3DCOLOR_XRGB(255, 255, 255));
	//����ͼƬ����;;(ͼƬ�ļ�,x,y,���,�߶�,���ű���,��ת����,��ɫ);
	BOOL DrawImageTexture(const wchar_t *ImageName, int x, int y, int width = 50, int height = 50, float scale = 1.0f, float rotation = 0.0f, D3DCOLOR Imagecolor = D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR Drawcolor = D3DCOLOR_XRGB(255, 255, 255));
	//����ֱ��;
	BOOL DrawLineto(float x1, float y1, float x2, float y2, float Width = 1.0f, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));
	//��������;
	BOOL DrawFont(const char *OutFont, int FontSize, int Weight, BOOL ltalic, const char *FontName, LPRECT rect, D3DCOLOR Color = D3DCOLOR_XRGB(0, 0, 0));
	//�����ı�����;
	BOOL DrawTextFont(const char *OutFont, int FontSize, int Weight, BOOL ltalic, const char *FontName, LPRECT rect, D3DCOLOR Color = D3DCOLOR_XRGB(0, 0, 0));
	//��ȡD3Dx9����
	LPDIRECT3D9 GetDirectx9();
	//��ȡD3D�豸����;
	LPDIRECT3DDEVICE9 GetD3Devicex9();
	//��ȡ����������;
	LPDIRECT3DSURFACE9 GetD3DSurfacex9();
	HRESULT Clear(DWORD Count, D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
	//���ƾ���;
	void DrawRectagle(float left, float Right, float up, float bottom, float Width = 1.0f, D3DCOLOR Color = D3DCOLOR_XRGB(0, 0, 0));

private:
	//����D3Dȫ�ֱ���;
	LPDIRECT3D9 Directx9;
	LPDIRECT3DDEVICE9 D3DDevice9;
	LPDIRECT3DSURFACE9 D3DSurface9;
	LPD3DXLINE Dxline;
	LPD3DXFONT DxFont;
	LPD3DXSPRITE D3DSprite;
	D3DXIMAGE_INFO Imginfo = { 0 };
};
#endif