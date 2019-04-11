#ifndef _ResouceFile_H
#define _ResouceFile_H
#include "../QQSG/zlib/zlib.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <d3d9.h>
#include <d3dx9.h>

#pragma warning(disable:4996)
namespace ResouceFile {
	//ȫ�����ݽṹ
	typedef struct _ImportFile
	{
		char pFile[MAX_PATH];
		UINT AnimateMaxCount;
		BOOL IsAnimate = FALSE;
		_ImportFile *AnimateFile;
	}ImportFile, *PImportFile;
	typedef enum _ImageLoadStyle {
		_Image = 0xF1,
		Animate = 0xF2,
		Button = 0xF3,
	}ImageLoadStyle;
	typedef struct _AnimateImage {
		float x;
		float y;
		UINT Width;
		UINT Height;
		float Scale;
		LPDIRECT3DTEXTURE9 Texture;
		WCHAR ImageFile[MAX_PATH];
	}AnimateImage, *PAnimateImage;
	typedef struct _ImageTexturInfo
	{
		float x;
		float y;
		UINT Width;
		UINT Height;
		float Scale;
		ImageLoadStyle ImgLoadType;
		BOOL IsHide;
		LPDIRECT3DTEXTURE9 Texture;
		PAnimateImage Animate;
		UINT AnimateTickIndex;
		UINT OldAnimateTick;
		UINT AnimateMaxCout;
		UINT AnimateDelay;
		WCHAR ImageFile[MAX_PATH];
		D3DCOLOR RectAngleColor;
		BOOL HeightLight;
	}ImageTexturInfo, *PImageTexturInfo;
	typedef struct _MapImageInfo
	{
		PImageTexturInfo Image;
		UINT MaxImage;
	}MapImageInfo, *PMapImageInfo;
	typedef struct _ReadResMapInfo {
		UINT ImageOffset;
		UINT ImageDataSize;
		UINT ImageOriginSize;
		float x;
		float y;
		UINT Width;
		UINT Height;
		float Scale;
		/*BOOL IsMirror;
		BOOL MirrorVertical;*/
		UINT AnimateDelay;
		ImageLoadStyle ImgLoadType;
		UINT AnimateCount;
		_ReadResMapInfo *Animate;
	}ReadResMapInfo, *PReadResMapInfo;
	class ResourceAlloc {
	public:
		ResourceAlloc();
		~ResourceAlloc();
		void Release();
		UINT GetLenth();
		UINT GetFileCount();
	private:
		void *pAlloc = nullptr;

	};
	//�����;
	typedef ResourceAlloc ResourceAllocC, *PResourceAlloc;
	//��Դ���ݲ�����
	class  ResouceDataFile
	{
	public:
		//��ȡ�ļ����ش���ö����
		enum GetAllgpxFilepathFromfolderError
		{
			Succeed,
			Out_of_memory,
			None_Find,
		};
#pragma pack(push,1)
		//��Դ���ļ�ͷ��
		struct ResDataHeader
		{
			char Header[4];
			UINT IndexNumber;
			UINT IndexPoint;
			UINT IndexSize;
		};
		//��Դ�����������
		struct ResDataIndexInfo {
			int pOffset;
			int pNone;
			int pOriginSize;
			int pDataSize;
			void *pFileName;
		};
		//��Դ�����������
		struct ResDataIndexInfo2 {
			int pNone;
			int pOffset;
			int pOriginSize;
			int pDataSize;
		};
		//��Դ�ļ���Ϣ;
		struct ResouceFileInfo
		{
			void *FileName;
			int pOffset;
			int pOriginSize;
			int pDataSize;
		};
		//��ͼλ����Ϣ;
		struct ResMapInfo {
			UINT ImageOffset;
			UINT ImageDataSize;
			UINT ImageOriginSize;
			ResMapInfo *Animate;
			UINT AnimateCount;
			UINT LoadStyle;
		};
		struct ResMapOInfo
		{
			UINT MaxCount;
			PReadResMapInfo Mapinfo;
		};
		struct WriteResMapInfo {
			UINT ImageOffset;
			UINT ImageDataSize;
			UINT ImageOriginSize;
			float x;
			float y;
			UINT Width;
			UINT Height;
			float Scale;
			UINT AnimateDelay;
			ImageLoadStyle ImgLoadType;
			UINT AnimateCount;
		};
		struct ResMapInfoHeader {
			char header[4];
			UINT IndexNumber;
			UINT IndexPoint;
			UINT IndexSize;
		};
#pragma pack(pop)
		ResouceDataFile();
		~ResouceDataFile();
		//��ȡ��Դ������������(��Դ���ļ���,��������ջ�����)�ɹ���������������,ʧ�ܷ���0;
		ResDataIndexInfo *GetResDataIndex(const char *FilePath);
		//�����Դ�ļ���
		BOOL PackageResouce(const char *FilePath);
		//��ȡ��Դ����������;
		UINT GetResIndexNumber();
		//�����ͼ��;
		BOOL PackageMap(const MapImageInfo &Resmpinfo);
		//��ȡ�����ͼ�ļ���
		ResMapOInfo *GetMapImageInfo(const char *FilePath);
		//��ȡ�ڴ�����ͼ�ļ���
		ResMapOInfo *GetMapImageInfoImport(const char *FilePath, PImportFile &pImportFile);
		void Release();
	private:
		ResDataIndexInfo *GetDataIndexAlloc = nullptr;//ȫ��������;
		int DataIndexNumber = 0;//ȫ������������;
		ResouceFileInfo *GvResfile = nullptr;//ȫ����Դ�ļ���;
		UINT FileCount = 0, MaxCount = 300;//ȫ����Դ�ļ�����,����ļ�������;
		ResMapOInfo G_MapImage = { 0 };
		//��ȡ����Ŀ¼(ANSII����)
		void GetExePathA(char*dirbuf, int len);
		//��ȡ�ļ�·��;
		char *GetResDirectory(const char*dirbuf, char*sourbuf);
		//ö����Դ�ļ����ļ���;
		GetAllgpxFilepathFromfolderError GetAllgpxFilepathFromfolder(const char*  Path);
		//���󻺳���
		void* AmplifyIndexArray(UINT MaxCount);
		ResourceAlloc pResAlloc;
	};
}
#endif