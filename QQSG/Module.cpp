#include "Module.h"

C_Module::C_Module() {
	ZeroMemory(szExePathA, sizeof(szExePathA));
	ZeroMemory(szExePathW, sizeof(szExePathW));
}
C_Module::~C_Module(){

}
//ȡ����Ŀ¼(UNICODE����)
const wchar_t *C_Module::GetExePathW() {
	GetModuleFileNameW(NULL, szExePathW, sizeof(szExePathW));
	(wcsrchr(szExePathW, '\\'))[1] = 0;
	return szExePathW;
}
//ȡ����Ŀ¼(ANSII����);
const char *C_Module::GetExePathA() {
	GetModuleFileNameA(NULL, szExePathA, sizeof(szExePathA));
	(strrchr(szExePathA, '\\'))[1] = 0;
	return szExePathA;
}
//�����༶Ŀ¼;
int C_Module::CreateDirectoryEcA(const char *Dir) const{
	char NewDir[256] = { 0 };
	CopyMemory(NewDir, Dir, strlen(Dir) * sizeof(char));
	(strrchr(NewDir, '\\'))[1] = 0;
	return SHCreateDirectoryExA(nullptr, (LPCSTR)NewDir, nullptr);
}
//�����༶Ŀ¼;
int C_Module::CreateDirectoryEcW(const WCHAR *Dir) const{
	WCHAR NewDir[256] = { 0 };
	CopyMemory(NewDir, Dir, wcslen(Dir) * sizeof(WCHAR));
	(wcsrchr(NewDir, '\\'))[1] = 0;
	return SHCreateDirectoryExW(nullptr, NewDir, nullptr);
}
const wchar_t *C_Module::GetCurrencyPathFileW(const WCHAR* Dir) {
	GetModuleFileNameW(NULL, szCurrencyPathW, sizeof(szCurrencyPathW));
	(wcsrchr(szCurrencyPathW, '\\'))[1] = 0;
	wcscat(szCurrencyPathW, Dir);
	return szCurrencyPathW;
}
const char *C_Module::GetCurrencyPathFileA(const char* Dir) {
	GetModuleFileNameA(NULL, szCurrencyPathA, sizeof(szCurrencyPathA));
	(strrchr(szCurrencyPathA, '\\'))[1] = 0;
	strcat(szCurrencyPathA, Dir);
	return szCurrencyPathA;
}
void MsgTipExit(const char *pTip){
	MessageBoxA(NULL, pTip, "Error:", MB_OK | MB_ICONERROR);
	ExitProcess(NULL);
}
//λͼ����
BOOL C_Module::MirrorBitmap(void *pImageAlloc, UINT pLen, BOOL Vertical) {
	BITMAPFILEHEADER BitmapHeader;
	BITMAPINFOHEADER BitmapInfo;
	PColor_32 pTmp = nullptr;
	//��ȡλͼ���ļ�;
	if (pLen < sizeof(BitmapHeader))return FALSE;
	CopyMemory(&BitmapHeader, pImageAlloc, sizeof(BitmapHeader));
	//��ȡλͼ��Ϣ;
	if (pLen < BitmapHeader.bfSize)return FALSE;
	pTmp = (PColor_32)((char*)pImageAlloc + sizeof(BitmapHeader));
	CopyMemory(&BitmapInfo, pTmp, sizeof(BitmapInfo));
	//�ƶ�ָ��ƫ����λͼ��ɫ������;
	pTmp = (PColor_32)((char*)pImageAlloc + BitmapHeader.bfOffBits);
	//�Ƿ�32λalphaͨ��λͼ;
	if (BitmapInfo.biBitCount == 32) {
		//�Ƿ�ֱ����;
		if (!Vertical) {
			//ˮƽ����;
			UINT MapHeight = 0, MapWidth = 0;
			//ȡλͼ����ֵ(��Ϊ�е�λͼ�Ǹ�������ͼ��)
			//����ֱ�Ӿ���,��Ϊ���㷴��ͼ��,����Ҳ�Ƿ��������Բ�Ӱ��ʵ�ʽ��;
			MapHeight = abs(BitmapInfo.biHeight);
			MapWidth = BitmapInfo.biWidth;
			//����λͼ���ݴ�С,Ҳ����ȡbiSizeImage;
			UINT RGBLen = MapHeight * MapWidth * sizeof(color_32);
			//������λͼ�洢���ڴ�ռ�;
			PColor_32 BitmapRGB = (PColor_32)LocalAlloc(LMEM_ZEROINIT, RGBLen);
			if (!BitmapRGB)return FALSE;
			//ÿ��λͼ����;
			for (UINT i = 0; i < MapHeight; i++) {
				//ÿ��λͼ����;(��Ϊ32λͼ������4�ֽڴ洢,������迼�Ƕ�������);
				for (UINT n = 0; n < MapWidth; n++) {
					//λͼ���ݾ����û�;
					UINT index = i * MapWidth + n, NextIndex = i * MapWidth + MapWidth - 1 - n;
					BitmapRGB[index] = pTmp[NextIndex];
				}
			}
			CopyMemory(pTmp, BitmapRGB, RGBLen);
			LocalFree(BitmapRGB);
		}
		else {
			//��ֱ����;
			UINT MapHeight = 0, MapWidth = 0;
			//ȡλͼ����ֵ(��Ϊ�е�λͼ�Ǹ�������ͼ��)
			//����ֱ�Ӿ���,��Ϊ���㷴��ͼ��,����Ҳ�Ƿ��������Բ�Ӱ��ʵ�ʽ��;
			MapHeight = abs(BitmapInfo.biHeight);
			MapWidth = BitmapInfo.biWidth;
			//����λͼ���ݴ�С,Ҳ����ȡbiSizeImage;
			UINT RGBLen = MapHeight * MapWidth * sizeof(color_32);
			//������λͼ�洢���ڴ�ռ�;
			PColor_32 BitmapRGB = (PColor_32)LocalAlloc(LMEM_ZEROINIT, RGBLen);
			if (!BitmapRGB)return FALSE;
			//ÿ��λͼ����;
			for (UINT i = 0; i < MapHeight; i++) {
				//ÿ��λͼ����;(��Ϊ32λͼ������4�ֽڴ洢,������迼�Ƕ�������);
				for (UINT n = 0; n < MapWidth; n++) {
					//λͼ���ݾ����û�;
					UINT index = i * MapWidth + n, NextIndex = (MapHeight - 1 - i)*MapWidth + n;
					BitmapRGB[index] = pTmp[NextIndex];
				}
			}
			CopyMemory(pTmp, BitmapRGB, RGBLen);
			LocalFree(BitmapRGB);
		}
	}
	return TRUE;
}
