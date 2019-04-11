#include "Main.h"
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	WNDCLASS WinClass = { 0 };
	WinClass.hInstance = hInstance;
	WinClass.lpszClassName = ClassName;
	WinClass.lpfnWndProc = WndProc;
	WinClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WinClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WinClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	WinClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	int WinX = (GetSystemMetrics(SM_CXSCREEN) / 2) - (CL_WinWidth / 2),
		WinY = (GetSystemMetrics(SM_CYSCREEN) / 2) - (CL_WinHeight / 2);
	if (!RegisterClass(&WinClass))MessageBox(0, L"ע�ᴰ������ʧ��!", NULL, MB_OK);
	G_hWnd = CreateWindow(ClassName, L"��������������-Ver1.21",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		WinX, WinY, CL_WinWidth, CL_WinHeight, NULL, NULL, hInstance, NULL);
	if (!G_hWnd)MsgTipExit("���ڴ���ʧ��!");
	ShowWindow(G_hWnd, SW_SHOWNORMAL);
	UpdateWindow(G_hWnd);
	MSG msg = { 0 };
	while (GetMessage(&msg,NULL,NULL,NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	char path[256] = { 0 };
	switch (uMsg)
	{
	case WM_CREATE: {
		CoInitialize(nullptr);
		Ime.SetMaxLen(14);
		//��ʼ������;
		RECT ClRect;
		GetClientRect(hWnd, &ClRect);
		if (!D2Dx9.InitD3D(hWnd, ClRect.right - ClRect.left, ClRect.bottom - ClRect.top, false)) MsgTipExit("��ʼ��D3Dʧ��!");
		if (!BASS_Init(-1, 44100, BASS_DEVICE_3D, hWnd, NULL)) MsgTipExit("��ʼ��BASS��Ƶ��ʧ��!");
		LPDIRECT3DDEVICE9 D3DDevice9 = D2Dx9.GetD3Devicex9();
		D3DDevice9->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		D3DDevice9->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		D3DDevice9->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		D3DDevice9->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 8);
		//����BGM;
		PlayerBGM_Login("Login1.mp3");
		MousePoint.OldTick = GetTickCount();
		ImeTick = GetTickCount();
		DrawTipDialogBox();
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)D3DGameRun, NULL, NULL, NULL);
		break;
	}
	case WM_CLOSE:
		BASS_Free();
		ExitProcess(0);
		break;
	case WM_KEYDOWN:
		Ime.SetKeyState(TRUE);
		ImeEvent(wParam);
		break;
	case WM_KEYUP:
		Ime.SetKeyState(FALSE);
		break;
	case WM_MOUSEMOVE:
		MousePoint.KeyCode = wParam;
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		break;
	case WM_LBUTTONDOWN:
		MousePoint.KeyCode = wParam;
		MousePoint.KeyState = WM_LBUTTONDOWN;
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		break;
	case WM_RBUTTONDOWN:
		MousePoint.KeyCode = wParam;
		MousePoint.KeyState = WM_RBUTTONDOWN;
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		break;
	case WM_LBUTTONUP:
		MousePoint.KeyCode = wParam;
		MousePoint.KeyState = WM_LBUTTONUP;
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		break;
	case WM_RBUTTONUP:
		MousePoint.KeyCode = wParam;
		MousePoint.KeyState = WM_RBUTTONUP;
		MousePoint.x = GET_X_LPARAM(lParam);
		MousePoint.y = GET_Y_LPARAM(lParam);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//���뷨��������;
void ImeEvent(WPARAM wParam) {
	int Keych = LOWORD(wParam);
	if (Keych == '\b') {
		Ime.DeleteImeStr();
	}
	else {
		char chc[3] = { 0 };
		if (GetKeyState(VK_CAPITAL)) {
			chc[0] = Keych;
		}
		else {
			if (Keych >= 'A' && Keych <= 'Z') {
				chc[0] = Keych + 32;
			}
			else if (Keych >= VK_NUMPAD0 && Keych <= VK_NUMPAD9) {
				sprintf(chc, "%d", Keych - VK_NUMPAD0);
			}
			else if (Keych >= '0' && Keych <= '9') {
				sprintf(chc, "%d", Keych - '0');
			}
		}
		Ime.SetImeChar(chc);
	}
}
//��Դ�ļ�����;
UINT LoadResourceData(const char *pFile, const char *pResName, void **pAlloc){
	ResouceDataFile ResData;
	ResouceDataFile::ResDataIndexInfo *ResFa = nullptr;
	C_Module Mod;
	const char *path = Mod.GetCurrencyPathFileA(pFile);
	ResFa = ResData.GetResDataIndex(path);
	UINT RNumber = ResData.GetResIndexNumber();
	void *Result = nullptr;
	uLongf UnzipLen = 0;
	for (UINT i = 0; i < RNumber; i++) {
		//����ǵ�¼1��������,�򲥷�;
		char *pFileName = (char*)ResFa[i].pFileName;
		if (!pFileName)continue;
		if (strcmp((char*)ResFa[i].pFileName, pResName) == 0) {
			//�����ڴ���Ƶ��������
			Result = LocalAlloc(LMEM_ZEROINIT, ResFa[i].pOriginSize);
			if (!Result) MsgTipExit("�����ڴ�ʧ��,�ڴ治��!");
			//������ʱ��ѹ�ڴ���Ƶ��������;
			void *TmpAlloc = LocalAlloc(LMEM_ZEROINIT, ResFa[i].pDataSize);
			if (!TmpAlloc) MsgTipExit("�����ڴ�ʧ��,�ڴ治��!");
			//��ȡ��Ƶ�����ݴ����ڴ�
			FILE *pFile = fopen(path, "rb");
			if (!pFile)MsgTipExit("��Դ��ʧ��,��ȷ���ļ��Ƿ���!");
			fseek(pFile, ResFa[i].pOffset, SEEK_SET);
			fread(TmpAlloc, ResFa[i].pDataSize, 1, pFile);
			UnzipLen = ResFa[i].pOriginSize;
			//��ѹ��Ƶ������;
			uncompress((Bytef*)Result, &UnzipLen, (Bytef*)TmpAlloc, ResFa[i].pDataSize);
			//�ͷ��������Դ;
			LocalFree(TmpAlloc);
			fclose(pFile);
			break;
		}
	}
	*pAlloc = Result;
	return UnzipLen;
}
//�ͷż��ص���Դ�ļ�;
void ReleaseResourceData(void *pAlloc) {
	LocalFree(pAlloc);
}
//���ŵ�¼��������
void PlayerBGM_Login(const char *MusicName) {
	UINT UnzipLen = LoadResourceData("Music.db", MusicName, &BGM_Login1);
	StreamBGM1 = BASS_StreamCreateFile(TRUE, BGM_Login1, 0, UnzipLen, BASS_SAMPLE_LOOP);
	if (!StreamBGM1)MsgTipExit("��Ƶ��ʧ��,��ȷ����Դ�ļ��Ƿ���!");
	//���ñ�����������;
	BASS_ChannelSetAttribute(StreamBGM1, BASS_ATTRIB_VOL, 100);
	//������Ƶ;
	BASS_ChannelPlay(StreamBGM1, TRUE);
}
//��Ϸ����
void D3DGameRun() {
	while (DrawGame)
	{
		LPDIRECT3DDEVICE9 D3DDevice9 = D2Dx9.GetD3Devicex9();
		if (!D3DDevice9)return;
		D2Dx9.Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		if (SUCCEEDED(D3DDevice9->BeginScene())) {
			//������ʾ��;
			//if (DrawTip)DrawTipBox();
			if (DrawCreatPlyer)DrawCreatePlayer();
			if (IsTipBox)DrawTipDialogBox();
			ShowFPS();
			D3DDevice9->EndScene();
			D3DDevice9->Present(NULL, NULL, NULL, NULL);
		}
		Sleep(1);
	}

}
//��ʾ�Ի���;
void SetTipDialogBoxText(const char *pText) {
	strcpy(szTipBoxText, pText);
	IsTipBox = TRUE;
}
void InitTipDialogBox() {
	ClTipButton.MaxButton = 2;
	UINT MaxAllocLen = sizeof(ButtonImageInfo) * ClTipButton.MaxButton;
	ClTipButton.Button = (PButtonImageInfo)LocalAlloc(LMEM_ZEROINIT, MaxAllocLen);
	PButtonImageInfo pButon2 = &ClTipButton.Button[0];
	pButon2->ButtonNum = 1;
	pButon2->ButtonType = Button_Dan;
	void *pAl;
	UINT pAllen = LoadResourceData("UIRes\\CreatePlayer\\UIButton.db", "UIRes\\CreatePlayer\\19275-1.bmp", &pAl);
	if (pAllen < 0xFF)MsgTipExit("������Դʧ��,��ȷ����Դ���Ƿ���!");
	pButon2->SelectIndex = 0;
	D3DXIMAGE_INFO info = D2Dx9.GetImageInfoInMemory(pAl, pAllen);
	pButon2->Width = info.Width;
	pButon2->Height = info.Height;
	pButon2->x = (float)(CL_WinWidth / 2) - (info.Width / 2) - 10;
	pButon2->y = (float)(CL_WinHeight / 2) - (info.Height / 2);
	pButon2->Texture = D2Dx9.LoadMemTexture(pAl, pAllen);
	pButon2->ResAlloc = pAl;
	//ȷ�ϰ�ť
	char szFile[3][50] = { "UIRes\\CreatePlayer\\63717-2.bmp","UIRes\\CreatePlayer\\63717-3.bmp" ,"UIRes\\CreatePlayer\\63717-1.bmp" };
	PButtonImageInfo pButon = &ClTipButton.Button[1];
	pButon->ButtonType = Button_Duo;
	pButon->ButtonNum = 3;
	pButon->MultiButton = (PButtonImageInfo)LocalAlloc(LMEM_ZEROINIT, sizeof(ButtonImageInfo) * pButon->ButtonNum);
	if (!pButon->MultiButton)MsgTipExit("�����ڴ�ʧ��,�ڴ治��!");
	pButon->SelectIndex = 0;
	for (UINT i = 0; i < pButon->ButtonNum; i++) {
		pButon->MultiButton[i].x = (float)(CL_WinWidth / 2) - 40;
		pButon->MultiButton[i].y = (float)CL_WinHeight / 2 + 30;
		void *pAl;
		UINT pAllen = LoadResourceData("UIRes\\CreatePlayer\\UIButton.db", szFile[i], &pAl);
		if (pAllen < 0xFF)MsgTipExit("������Դʧ��,��ȷ����Դ���Ƿ���!");
		D3DXIMAGE_INFO info = D2Dx9.GetImageInfoInMemory(pAl, pAllen);
		pButon->MultiButton[i].Width = info.Width;
		pButon->MultiButton[i].Height = info.Height;
		pButon->MultiButton[i].Texture = D2Dx9.LoadMemTexture(pAl, pAllen);
		pButon->MultiButton[i].ResAlloc = pAl;
	}
	ClTipButton.Button[0].SelectIndex = 0;
}
void DrawTipDialogBox() {
	if (ClTipButton.MaxButton < 1)InitTipDialogBox();
	for (UINT i = 0; i < ClTipButton.MaxButton; i++) {
		if (ClTipButton.Button[i].ButtonType == Button_Dan) {
			D2Dx9.DrawTexture(ClTipButton.Button[i].Texture, ClTipButton.Button[i].x, ClTipButton.Button[i].y, ClTipButton.Button[i].Width, ClTipButton.Button[i].Height);
		}
		else if (ClTipButton.Button[i].ButtonType == Button_Duo) {
			ButtonImageInfo &pButon = ClTipButton.Button[i].MultiButton[ClTipButton.Button[i].SelectIndex];
			if (IsRectMouse(pButon.x, pButon.y, pButon.Width, pButon.Height, MousePoint)) {
				if (MousePoint.KeyState == WM_LBUTTONDOWN) {
					ClTipButton.Button[i].SelectIndex = 2;
					if (ClickMouseDelay())IsTipBox = FALSE;
				}
				else ClTipButton.Button[i].SelectIndex = 1;
			}
			else ClTipButton.Button[i].SelectIndex = 0;
			D2Dx9.DrawTexture(pButon.Texture, pButon.x, pButon.y, pButon.Width, pButon.Height);
		}
	}
	RECT lRfont = { (LONG)ClTipButton.Button[0].x + 20, (LONG)ClTipButton.Button[0].y + 20,CL_WinWidth,CL_WinHeight };
	D2Dx9.DrawTextFont(szTipBoxText, 15, 1, FALSE, "����", &lRfont, D3DCOLOR_XRGB(255, 235, 147));
}
//��ʾFPS֡��:
void ShowFPS() {
	//����ÿ��FPS֡��;
	if (OldFpsTick == 0 || GetTickCount() - OldFpsTick > 1000) {
		OldFpsTick = GetTickCount();
		NewFPSCount = FPSCount;
		FPSCount = 0;
	}
	else FPSCount++;
	char FPSOut[50];
	sprintf(FPSOut, "FPS:%d", NewFPSCount);
	RECT lrect = { CL_WinWidth - 80,10,CL_WinWidth,50 };
	D2Dx9.DrawFont(FPSOut, 12, NULL, FALSE, "����", &lrect, D3DCOLOR_XRGB(0, 250, 0));
}
//������ʾ��;
void DrawTipBox() {
	UINT TipWidth = 500, TipHeight = 600, TipTop = 30;
	WCHAR szPath[MAX_PATH];
	C_Module Mod;
	WCHAR szImg_Tip[MAX_PATH];
	wcscpy(szImg_Tip, Mod.GetExePathW());
	wcscat(szImg_Tip, L"UIRes\\��¼UI\\��¼��ɫ\\��ʾ��-1.bmp");
	LPDIRECT3DTEXTURE9 TipBox = D2Dx9.LoadRectTexture(szImg_Tip, TipWidth, TipHeight);
	WCHAR SubmitT[3][256] = { L"UIRes\\��¼UI\\��¼��ɫ\\ȷ��-1.bmp",L"UIRes\\��¼UI\\��¼��ɫ\\ȷ��-2.bmp",L"UIRes\\��¼UI\\��¼��ɫ\\ȷ��-3.bmp" };
	LPDIRECT3DTEXTURE9 Submit[3] = { 0 };
	for (int i = 0; i < 3; i++) {
		wcscpy(szImg_Tip, szPath);
		wcscat(szImg_Tip, SubmitT[i]);
		Submit[i] = D2Dx9.LoadTexture(szImg_Tip);
	}
	D2Dx9.DrawTexture(TipBox, (float)TipWidth / 2, (float)TipTop);
	char TipT[] = "������Ϸ:";
	RECT lrect = { (LONG)TipWidth - 70,(LONG)TipTop + 5,CL_WinWidth,CL_WinHeight };
	D2Dx9.DrawFont(TipT, 25, 10, TRUE, "����", &lrect, D3DCOLOR_XRGB(255, 0, 0));
	char TipText[] = "*.����Ϸ����ѹ�����Ϸ,ʹ�õ���Ϸ��Դ�󲿷�ȡ��\r\n<QQ����>��Ϸ.��:����ʹ�ñ���Ϸ����������;\r\n��������������!"
		"\r\n\r\n*.����Ϸ��<QQ����>���淨����,�ڲ����ݺ��淨ʵ��\r\n����ȫ��ͬ�޷�����<QQ����>�������!�����ע��."
		"\r\n\r\n*.����Ϸ���������뷨�޷��л�,���л������л�����\r\n�������л���Ϸ����"
		"\r\n\r\n*.��������Ϸ��Դ����ʧ��,��ʹ�ü���ģʽ���ҹ���\r\nԱ����."
		"\r\n\r\n*.����Ϸֻ����Ϸ������׷�������֮��,������\r\n<QQ����>����.";
	RECT lprect = { (LONG)TipWidth / 2 +10,(LONG)TipTop + 40,CL_WinWidth,CL_WinHeight };
	D2Dx9.DrawTextFont(TipText, 20, 10, FALSE, "����", &lprect, D3DCOLOR_XRGB(0, 150, 230));
	if (MousePoint.x > (int)TipWidth - 35 && MousePoint.x < (int)TipWidth + 17 && MousePoint.y < (int)TipHeight - 20 && MousePoint.y >(int)TipHeight - 50) {
		if (MousePoint.KeyCode == MK_LBUTTON) { D2Dx9.DrawTexture(Submit[2], (float)TipWidth - 30, (float)TipHeight - 20); DrawTip = FALSE; DrawCreatPlyer = TRUE; }
		else D2Dx9.DrawTexture(Submit[0], (float)TipWidth - 30, (float)TipHeight - 20);
	}
	else {
		D2Dx9.DrawTexture(Submit[1], (float)TipWidth - 30, (float)TipHeight - 20);
	}
	TipBox->Release();
	for (int i = 0; i < 3; i++) {
		Submit[i]->Release();
	}
}
//��ͼ��Դ����;
void LoadMapResourceData(ResouceDataFile::ResMapOInfo *ResMpIOinfo, DrawImageInfo &pMapAlloc, UINT pLen, const char*pMapFile) {
	FILE *pFile = fopen(pMapFile, "rb");
	if (!pFile)MsgTipExit("���������Դ����δ֪����!");
	for (UINT i = 0; i < ResMpIOinfo->MaxCount; i++) {
		ReadResMapInfo mpinf = ResMpIOinfo->Mapinfo[i];
		PDrawMapInfo DrawMp = &pMapAlloc.DrawMap[i];
		DrawMp->x = mpinf.x;
		DrawMp->y = mpinf.y;
		DrawMp->width = mpinf.Width;
		DrawMp->height = mpinf.Height;
		DrawMp->Scale = mpinf.Scale;
		DrawMp->ImgLoadType = mpinf.ImgLoadType;
		DrawMp->rotation = 0.f;
		DrawMp->AnimateMaxCount = mpinf.AnimateCount;
		DrawMp->AnimateDelay = mpinf.AnimateDelay;
		DrawMp->AnimateOldTick = GetTickCount();
		DrawMp->AnimateTickIndex = 0;
		if (mpinf.ImgLoadType == _Image) {
			Bytef *pUnzipAlloc = (Bytef*)LocalAlloc(LMEM_ZEROINIT, mpinf.ImageOriginSize);
			if (!pUnzipAlloc) MsgTipExit("���������Դ����δ֪����!");
			Bytef *pDataAlloc = (Bytef*)LocalAlloc(LMEM_ZEROINIT, mpinf.ImageDataSize);
			if (!pDataAlloc) MsgTipExit("���������Դ����δ֪����!");
			fseek(pFile, mpinf.ImageOffset, SEEK_SET);
			fread(pDataAlloc, 1, mpinf.ImageDataSize, pFile);
			UINT UnzipLen = mpinf.ImageOriginSize;
			uncompress(pUnzipAlloc, (uLongf*)&UnzipLen, pDataAlloc, mpinf.ImageDataSize);
			DrawMp->Texture = D2Dx9.LoadMemTexture(pUnzipAlloc, UnzipLen);
			DrawMp->ResAlloc = pUnzipAlloc;
			LocalFree(pDataAlloc);
		}
		else if (mpinf.ImgLoadType == Animate) {
			DrawMp->Animate = (PDrawMapInfo)LocalAlloc(LMEM_ZEROINIT, sizeof(DrawMapInfo)*mpinf.AnimateCount);
			if (!DrawMp->Animate)MsgTipExit("���������Դ����δ֪����!");
			for (UINT n = 0; n < mpinf.AnimateCount; n++) {
				ReadResMapInfo Anminf = mpinf.Animate[n];
				PDrawMapInfo DrawMpAnimate = DrawMp->Animate;
				Bytef *pUnzipAlloc = (Bytef*)LocalAlloc(LMEM_ZEROINIT, Anminf.ImageOriginSize);
				if (!pUnzipAlloc)MsgTipExit("���������Դ����δ֪����!");
				Bytef *pDataAlloc = (Bytef*)LocalAlloc(LMEM_ZEROINIT, Anminf.ImageDataSize);
				if (!pDataAlloc)MsgTipExit("���������Դ����δ֪����!");
				fseek(pFile, Anminf.ImageOffset, SEEK_SET);
				fread(pDataAlloc, 1, Anminf.ImageDataSize, pFile);
				UINT UnzipLen = Anminf.ImageOriginSize;
				uncompress(pUnzipAlloc, (uLongf*)&UnzipLen, pDataAlloc, Anminf.ImageDataSize);
				DrawMpAnimate[n].Texture = D2Dx9.LoadMemTexture(pUnzipAlloc, Anminf.ImageOriginSize);
				DrawMpAnimate[n].ResAlloc = pUnzipAlloc;
				DrawMpAnimate[n].x = Anminf.x;
				DrawMpAnimate[n].y = Anminf.y;
				DrawMpAnimate[n].width = Anminf.Width;
				DrawMpAnimate[n].height = Anminf.Height;
				DrawMpAnimate[n].Scale = Anminf.Scale;
				DrawMpAnimate[n].ImgLoadType = Anminf.ImgLoadType;
				DrawMpAnimate[n].rotation = 0.f;
				DrawMpAnimate[n].AnimateMaxCount = Anminf.AnimateCount;
				DrawMpAnimate[n].AnimateDelay = Anminf.AnimateDelay;
				LocalFree(pDataAlloc);
			}
		}
	}
	fclose(pFile);
}
//�ͷ��Ѽ��صĵ�ͼ��Դ
void ReleaseMapResource(DrawImageInfo &PMapImageInfo) {
	if (PMapImageInfo.MaxInt < 1 || PMapImageInfo.DrawMap == nullptr)return;
	for (UINT i = 0; i < PMapImageInfo.MaxInt; i++) {
		if (PMapImageInfo.DrawMap[i].ImgLoadType == _Image) {
			if (PMapImageInfo.DrawMap[i].Texture)PMapImageInfo.DrawMap[i].Texture->Release();
			LocalFree(PMapImageInfo.DrawMap[i].ResAlloc);
		}
		else if (PMapImageInfo.DrawMap[i].ImgLoadType == Animate) {
			for (UINT n = 0; n < PMapImageInfo.DrawMap[i].AnimateMaxCount; n++) {
				if (PMapImageInfo.DrawMap[i].Animate[n].Texture)PMapImageInfo.DrawMap[i].Animate[n].Texture->Release();
				LocalFree(PMapImageInfo.DrawMap[i].Animate[n].ResAlloc);
			}
		}
	}
	LocalFree(PMapImageInfo.DrawMap);
	PMapImageInfo = { 0 };
}
//�ͷ��Ѽ��صİ�ť��Դ
void ReleaseButtonResource(ButtonInfo &PMapImageInfo){
	if (PMapImageInfo.MaxButton < 1 || PMapImageInfo.Button == nullptr)return;
	for (UINT i = 0; i < PMapImageInfo.MaxButton; i++) {
		if (PMapImageInfo.Button[i].ButtonType == Button_Dan) {
			if (PMapImageInfo.Button[i].Texture)PMapImageInfo.Button[i].Texture->Release();
			LocalFree(PMapImageInfo.Button[i].ResAlloc);
		}
		else if (PMapImageInfo.Button[i].ButtonType == Button_Duo) {
			for (UINT n = 0; n < PMapImageInfo.Button[i].ButtonNum; n++) {
				if (PMapImageInfo.Button[i].MultiButton[n].Texture)PMapImageInfo.Button[i].MultiButton[n].Texture->Release();
				LocalFree(PMapImageInfo.Button[i].MultiButton[n].ResAlloc);
			}
		}
	}
	LocalFree(PMapImageInfo.Button);
	PMapImageInfo = { 0 };
}
//�ж�����Ƿ���ѡ����
BOOL IsRectMouse(float x, float y, UINT Width, UINT Height, MouseInfo const&MousePoint) {
	float mX = 0.f, mY = 0.f;
	if (MousePoint.x - mX > x && MousePoint.x - mX < x + (float)Width && MousePoint.y - mY > y && MousePoint.y - mY < y + (float)Height) {
		return TRUE;
	}
	return FALSE;
}
//��ʼ�����ƴ��������Դ;
void InitDrawCreatePlayer() {
	C_Module Mod;
	const char *szPath = Mod.GetCurrencyPathFileA("������ɫ����.map");
	ResouceDataFile Resdatfile;
	ResouceDataFile::ResMapOInfo *MpImginfo = Resdatfile.GetMapImageInfo(szPath);
	if (!MpImginfo) MsgTipExit("���������Դ����δ֪����!");
	if (CreatePlayerImgInfo.MaxInt > 0)MsgTipExit("���������Դ����δ֪����!");
	UINT MaxAllocLen = sizeof(DrawMapInfo)*MpImginfo->MaxCount;
	CreatePlayerImgInfo.DrawMap = (PDrawMapInfo)LocalAlloc(LMEM_ZEROINIT, MaxAllocLen);
	CreatePlayerImgInfo.MaxInt = MpImginfo->MaxCount;
	if(!CreatePlayerImgInfo.DrawMap) MsgTipExit("�����ڴ�ʧ��,�ڴ治��!");
	LoadMapResourceData(MpImginfo, CreatePlayerImgInfo, MaxAllocLen, szPath);
	//�������ﶯ��;
	Resdatfile.Release();
	PlayerChange("UIRes\\CreatePlayer\\nv-yy.map", CreatePlayer);
	//���ذ�ť;
	ClButton.MaxButton = 11;
	MaxAllocLen = sizeof(ButtonImageInfo) * ClButton.MaxButton;
	ClButton.Button = (PButtonImageInfo)LocalAlloc(LMEM_ZEROINIT, MaxAllocLen);
	if (!ClButton.Button) MsgTipExit("�����ڴ�ʧ��,�ڴ治��!");
#pragma region ��ť��ʼ��;
	//ְҵѡ��ť;
	char szFile[5][2][50] = { "UIRes\\CreatePlayer\\����ʿ-1.bmp","UIRes\\CreatePlayer\\����ʿ-2.bmp",
		"UIRes\\CreatePlayer\\����-1.bmp","UIRes\\CreatePlayer\\����-2.bmp","UIRes\\CreatePlayer\\����-1.bmp","UIRes\\CreatePlayer\\����-2.bmp",
		"UIRes\\CreatePlayer\\����ʿ-1.bmp","UIRes\\CreatePlayer\\����ʿ-2.bmp","UIRes\\CreatePlayer\\����-1.bmp","UIRes\\CreatePlayer\\����-2.bmp"
	};
	for (UINT t = 0; t < 5; t++) {
		PButtonImageInfo pButon = &ClButton.Button[t];
		pButon->ButtonType = Button_Duo;
		pButon->ButtonNum = 2;
		pButon->MultiButton = (PButtonImageInfo)LocalAlloc(LMEM_ZEROINIT, sizeof(ButtonImageInfo) * pButon->ButtonNum);
		if (!pButon->MultiButton)MsgTipExit("�����ڴ�ʧ��,�ڴ治��!");
		pButon->SelectIndex = 0;
		for (UINT i = 0; i < pButon->ButtonNum; i++) {
			pButon->MultiButton[i].x = (float)(CL_WinWidth / 2) - 200 + (t * 80);
			pButon->MultiButton[i].y = (float)CL_WinHeight - 150;
			void *pAl;
			UINT pAllen = LoadResourceData("UIRes\\CreatePlayer\\UIButton.db", szFile[t][i], &pAl);
			if (pAllen < 0xFF)MsgTipExit("������Դʧ��,��ȷ����Դ���Ƿ���!");
			D3DXIMAGE_INFO info = D2Dx9.GetImageInfoInMemory(pAl, pAllen);
			pButon->MultiButton[i].Width = info.Width;
			pButon->MultiButton[i].Height = info.Height;
			pButon->MultiButton[i].Texture = D2Dx9.LoadMemTexture(pAl, pAllen);
			pButon->MultiButton[i].ResAlloc = pAl;
		}
	}
	ClButton.Button[0].SelectIndex = 1;
	//��ʼ��Ϸ��ť;
	char szStartGame[3][50] = { "UIRes\\CreatePlayer\\��ʼ��Ϸ-2.bmp","UIRes\\CreatePlayer\\��ʼ��Ϸ-3.bmp" ,"UIRes\\CreatePlayer\\��ʼ��Ϸ-1.bmp" };
	PButtonImageInfo pButon = &ClButton.Button[5];
	pButon->ButtonType = Button_Duo;
	pButon->ButtonNum = 3;
	pButon->MultiButton = (PButtonImageInfo)LocalAlloc(LMEM_ZEROINIT, sizeof(ButtonImageInfo) * pButon->ButtonNum);
	if (!pButon->MultiButton)MsgTipExit("�����ڴ�ʧ��,�ڴ治��!");
	pButon->SelectIndex = 0;
	for (UINT i = 0; i < pButon->ButtonNum; i++) {
		pButon->MultiButton[i].x = (float)(CL_WinWidth / 2) +100;
		pButon->MultiButton[i].y = (float)CL_WinHeight - 75;
		void *pAl;
		UINT pAllen = LoadResourceData("UIRes\\CreatePlayer\\UIButton.db", szStartGame[i], &pAl);
		if (pAllen < 0xFF)MsgTipExit("������Դʧ��,��ȷ����Դ���Ƿ���!");
		D3DXIMAGE_INFO info = D2Dx9.GetImageInfoInMemory(pAl, pAllen);
		pButon->MultiButton[i].Width = info.Width;
		pButon->MultiButton[i].Height = info.Height;
		pButon->MultiButton[i].Texture = D2Dx9.LoadMemTexture(pAl, pAllen);
		pButon->MultiButton[i].ResAlloc = pAl;
	}
	//����ѡ��ť;
	char szGuoFile[3][2][50] = { "UIRes\\CreatePlayer\\κ-1.bmp","UIRes\\CreatePlayer\\κ-2.bmp",
		"UIRes\\CreatePlayer\\��-1.bmp","UIRes\\CreatePlayer\\��-2.bmp","UIRes\\CreatePlayer\\��-1.bmp","UIRes\\CreatePlayer\\��-2.bmp",
	};
	for (UINT t = 6; t < 9; t++) {
		PButtonImageInfo pButon = &ClButton.Button[t];
		pButon->ButtonType = Button_Duo;
		pButon->ButtonNum = 2;
		pButon->MultiButton = (PButtonImageInfo)LocalAlloc(LMEM_ZEROINIT, sizeof(ButtonImageInfo) * pButon->ButtonNum);
		if (!pButon->MultiButton)MsgTipExit("�����ڴ�ʧ��,�ڴ治��!");
		pButon->SelectIndex = 0;
		for (UINT i = 0; i < pButon->ButtonNum; i++) {
			pButon->MultiButton[i].x = (float)CL_WinWidth - 180 + ((t - 6) * 45);
			pButon->MultiButton[i].y = (float)35;
			void *pAl;
			UINT pAllen = LoadResourceData("UIRes\\CreatePlayer\\UIButton.db", szGuoFile[t - 6][i], &pAl);
			if (pAllen < 0xFF)MsgTipExit("������Դʧ��,��ȷ����Դ���Ƿ���!");
			D3DXIMAGE_INFO info = D2Dx9.GetImageInfoInMemory(pAl, pAllen);
			pButon->MultiButton[i].Width = info.Width;
			pButon->MultiButton[i].Height = info.Height;
			pButon->MultiButton[i].Texture = D2Dx9.LoadMemTexture(pAl, pAllen);
			pButon->MultiButton[i].ResAlloc = pAl;
		}
	}
	ClButton.Button[6].SelectIndex = 1;
	//�Ա�ѡ��ť;
	char szWmFile[2][2][50] = { "UIRes\\CreatePlayer\\��-1.bmp","UIRes\\CreatePlayer\\��-2.bmp",
		"UIRes\\CreatePlayer\\Ů-1.bmp","UIRes\\CreatePlayer\\Ů-2.bmp"};
	for (UINT t = 9; t < 11; t++) {
		PButtonImageInfo pButon = &ClButton.Button[t];
		pButon->ButtonType = Button_Duo;
		pButon->ButtonNum = 2;
		pButon->MultiButton = (PButtonImageInfo)LocalAlloc(LMEM_ZEROINIT, sizeof(ButtonImageInfo) * pButon->ButtonNum);
		if (!pButon->MultiButton)MsgTipExit("�����ڴ�ʧ��,�ڴ治��!");
		pButon->SelectIndex = 0;
		for (UINT i = 0; i < pButon->ButtonNum; i++) {
			pButon->MultiButton[i].x = (float)CL_WinWidth - 180 + ((t - 9) * 80);
			pButon->MultiButton[i].y = (float)90;
			void *pAl;
			UINT pAllen = LoadResourceData("UIRes\\CreatePlayer\\UIButton.db", szWmFile[t - 9][i], &pAl);
			if (pAllen < 0xFF)MsgTipExit("������Դʧ��,��ȷ����Դ���Ƿ���!");
			D3DXIMAGE_INFO info = D2Dx9.GetImageInfoInMemory(pAl, pAllen);
			pButon->MultiButton[i].Width = info.Width;
			pButon->MultiButton[i].Height = info.Height;
			pButon->MultiButton[i].Texture = D2Dx9.LoadMemTexture(pAl, pAllen);
			pButon->MultiButton[i].ResAlloc = pAl;
		}
	}
	ClButton.Button[10].SelectIndex = 1;
#pragma endregion

}
//���Ʋ���ְҵ���;
void PlayerChange(const char *pMapFile, DrawImageInfo &pMapAlloc) {
	ReleaseMapResource(pMapAlloc);
	//�������ﶯ��;
	C_Module Mod;
	ResouceDataFile Resdatfile;
	ResouceDataFile::ResMapOInfo *MpImginfo = nullptr;
	const char *szPath = Mod.GetCurrencyPathFileA(pMapFile);
	MpImginfo = Resdatfile.GetMapImageInfo(szPath);
	if (!MpImginfo) MsgTipExit("���������Դ����δ֪����!");
	if (pMapAlloc.MaxInt > 0) MsgTipExit("���������Դ����δ֪����!");
	UINT MaxAllocLen = sizeof(DrawMapInfo) * MpImginfo->MaxCount;
	pMapAlloc.DrawMap = (PDrawMapInfo)LocalAlloc(LMEM_ZEROINIT, MaxAllocLen);
	pMapAlloc.MaxInt = MpImginfo->MaxCount;
	LoadMapResourceData(MpImginfo, pMapAlloc, MaxAllocLen, szPath);
}
//������ӳټ���;
BOOL ClickMouseDelay() {
	if (GetTickCount() - MousePoint.OldTick > 300) {
		MousePoint.OldTick = GetTickCount();
		return TRUE;
	}
	return FALSE;
}
void PlayerButton() {
	if (ClickMouseDelay()) {
		switch (���ְҵ)
		{
		case 0:
			if (!����Ա��Ƿ�Ϊ��)PlayerChange("UIRes\\CreatePlayer\\nv-yy.map", CreatePlayer);
			else PlayerChange("UIRes\\CreatePlayer\\nan-yy.map", CreatePlayer);
			strcpy(szCareerTip, "        ����ʿ\r\n\r\nԶ��ؼ����ؼ�߹�ؼʩ����");
			break;
		case 1:
			if (!����Ա��Ƿ�Ϊ��)PlayerChange("UIRes\\CreatePlayer\\nv-js.map", CreatePlayer);
			else PlayerChange("UIRes\\CreatePlayer\\nan-js.map", CreatePlayer);
			strcpy(szCareerTip, "        ����\r\n\r\n��սؼ����ؼƽ��ؼ���ٿ�");
			break;
		case 2:
			if (!����Ա��Ƿ�Ϊ��)PlayerChange("UIRes\\CreatePlayer\\nv-hj.map", CreatePlayer);
			else PlayerChange("UIRes\\CreatePlayer\\nan-hj.map", CreatePlayer);
			strcpy(szCareerTip, "        ����\r\n\r\n��սؼ����ؼ�߷�ؼ̹����");
			break;
		case 3:
			if (!����Ա��Ƿ�Ϊ��)PlayerChange("UIRes\\CreatePlayer\\nv-xs.map", CreatePlayer);
			else PlayerChange("UIRes\\CreatePlayer\\nan-xs.map", CreatePlayer);
			strcpy(szCareerTip, "        ����ʿ\r\n\r\nԶ��ؼ����ؼ����ؼʩ����");
			break;
		case 4:
			if (!����Ա��Ƿ�Ϊ��)PlayerChange("UIRes\\CreatePlayer\\nv-yx.map", CreatePlayer);
			else PlayerChange("UIRes\\CreatePlayer\\nan-yx.map", CreatePlayer);
			strcpy(szCareerTip, "        ����\r\n\r\nԶ��ؼ����ؼ�߹�ؼ��Ƥ��");
			break;
		default:
			break;
		}
	}
}
//������ҽ�ɫ����;
void DrawCreatePlayer() {
	//�ж��Ƿ��ʼ����;
	if (CreatePlayer.DrawMap == nullptr && CreatePlayer.MaxInt < 1 && CreatePlayerImgInfo.DrawMap == nullptr && CreatePlayerImgInfo.MaxInt < 1)InitDrawCreatePlayer();
	//������ʾUI;
	for (UINT i = 0; i < CreatePlayerImgInfo.MaxInt; i++) {
		PDrawMapInfo DrwMp = &CreatePlayerImgInfo.DrawMap[i];
		if (DrwMp->ImgLoadType == _Image) {
			D2Dx9.DrawTexture(DrwMp->Texture, DrwMp->x, DrwMp->y, DrwMp->width, DrwMp->height, DrwMp->Scale, DrwMp->rotation);
		}
		else if (DrwMp->ImgLoadType == Animate) {
			if (GetTickCount() - DrwMp->AnimateOldTick > DrwMp->AnimateDelay) {
				DrwMp->AnimateTickIndex++;
				DrwMp->AnimateOldTick = GetTickCount();
				if (DrwMp->AnimateTickIndex >= DrwMp->AnimateMaxCount)
					DrwMp->AnimateTickIndex = 0;
			}
			DrawMapInfo DrwMpAn = DrwMp->Animate[DrwMp->AnimateTickIndex];
			D2Dx9.DrawTexture(DrwMpAn.Texture, DrwMpAn.x, DrwMpAn.y, DrwMpAn.width, DrwMpAn.height, DrwMpAn.Scale, DrwMpAn.rotation);
		}
	}
	for (UINT i = 0; i < CreatePlayer.MaxInt; i++) {
		PDrawMapInfo DrwMp = &CreatePlayer.DrawMap[i];
		if (DrwMp->ImgLoadType == _Image) {
			D2Dx9.DrawTexture(DrwMp->Texture, DrwMp->x, DrwMp->y, DrwMp->width, DrwMp->height, DrwMp->Scale, DrwMp->rotation);
		}
		else if (DrwMp->ImgLoadType == Animate) {
			if (GetTickCount() - DrwMp->AnimateOldTick > DrwMp->AnimateDelay) {
				DrwMp->AnimateTickIndex++;
				DrwMp->AnimateOldTick = GetTickCount();
				if (DrwMp->AnimateTickIndex >= DrwMp->AnimateMaxCount) {
					DrwMp->AnimateTickIndex = 0;
				}
			}
			DrawMapInfo DrwMpAn = DrwMp->Animate[DrwMp->AnimateTickIndex];
			D2Dx9.DrawTexture(DrwMpAn.Texture, DrwMpAn.x, DrwMpAn.y, DrwMpAn.width, DrwMpAn.height, DrwMpAn.Scale, DrwMpAn.rotation);
		}
	}
	for (UINT i = 0; i < ClButton.MaxButton; i++) {
		if (ClButton.Button[i].ButtonType == Button_Dan) {

		}
		else if (ClButton.Button[i].ButtonType == Button_Duo) {
			ButtonImageInfo &pButon = ClButton.Button[i].MultiButton[ClButton.Button[i].SelectIndex];
			if (IsRectMouse(pButon.x, pButon.y, pButon.Width, pButon.Height, MousePoint)) {
				if (MousePoint.KeyState == WM_LBUTTONDOWN) {
					if (i < 5)���ְҵ = i;
					if (i < 9 && i > 5)��ҹ��� = i - 6;
					switch (i)
					{
					//0-4ְҵ��ť
					//5��ʼ��Ϸ��ť
					//6-8����ѡ��ť
					//9-10�Ա�ѡ��ť
					case 0:
						for (UINT i = 0; i < 5; i++) {
							ClButton.Button[i].SelectIndex = 0;
						}
						ClButton.Button[i].SelectIndex = 1;
						PlayerButton();
						break;
					case 1:
						for (UINT i = 0; i < 5; i++) {
							ClButton.Button[i].SelectIndex = 0;
						}
						ClButton.Button[i].SelectIndex = 1;
						PlayerButton();
						break;
					case 2:
						for (UINT i = 0; i < 5; i++) {
							ClButton.Button[i].SelectIndex = 0;
						}
						ClButton.Button[i].SelectIndex = 1;
						PlayerButton();
						break;
					case 3:
						for (UINT i = 0; i < 5; i++) {
							ClButton.Button[i].SelectIndex = 0;
						}
						ClButton.Button[i].SelectIndex = 1;
						PlayerButton();
						break;
					case 4:
						for (UINT i = 0; i < 5; i++) {
							ClButton.Button[i].SelectIndex = 0;
						}
						ClButton.Button[i].SelectIndex = 1;
						PlayerButton();
						break;
					case 5:
						ClButton.Button[i].SelectIndex = 2;
						if (ClickMouseDelay()){
							if (Ime.GetImeLen() < 1)SetTipDialogBoxText("   ������ʹ�ÿս�ɫ��!\r\n      �������ɫ��!");
							else { 
								CopyMemory(�������, Ime.GetImeString().c_str(), Ime.GetImeLen()); 
								DrawCreatPlyer = FALSE; 
								ReleaseMapResource(CreatePlayer);
								ReleaseMapResource(CreatePlayerImgInfo);
								ReleaseButtonResource(ClButton);
								ReleaseButtonResource(ClTipButton);
								return;
							}
						}
						break;
					case 6:
						for (UINT i = 6; i < 9; i++) {
							ClButton.Button[i].SelectIndex = 0;
						}
						ClButton.Button[i].SelectIndex = 1;
						break;
					case 7:
						for (UINT i = 6; i < 9; i++) {
							ClButton.Button[i].SelectIndex = 0;
						}
						ClButton.Button[i].SelectIndex = 1;
						break;
					case 8:
						for (UINT i = 6; i < 9; i++) {
							ClButton.Button[i].SelectIndex = 0;
						}
						ClButton.Button[i].SelectIndex = 1;
						break;
					case 9:
						for (UINT i = 9; i < 11; i++) {
							ClButton.Button[i].SelectIndex = 0;
						}
						ClButton.Button[i].SelectIndex = 1;
						����Ա��Ƿ�Ϊ�� = TRUE;
						PlayerButton();
						break;
					case 10:
						for (UINT i = 9; i < 11; i++) {
							ClButton.Button[i].SelectIndex = 0;
						}
						ClButton.Button[i].SelectIndex = 1;
						����Ա��Ƿ�Ϊ�� = FALSE;
						PlayerButton();
						break;
					default:
						break;
					}
					
				}
				else {
					if (i == 5)ClButton.Button[i].SelectIndex = 1;
				}
			}
			else {
				if (i == 5)ClButton.Button[i].SelectIndex = 0;
			}
			//D2Dx9.DrawRectagle(pButon.x, pButon.y, pButon.x + pButon.Width, pButon.y + pButon.Height, 1.f, D3DCOLOR_XRGB(255, 0, 0));
			D2Dx9.DrawTexture(pButon.Texture, pButon.x, pButon.y, pButon.Width, pButon.Height);
		}
	}
	//���������;
	Ime.GetIMEString(G_hWnd);
	string imestr = Ime.GetImeString();
	float ImeX = (CL_WinWidth / 2) - 60, ImeY = CL_WinHeight - 50;
	if (GetTickCount() - ImeTick > 1000) {
		if (GetTickCount() - ImeTick > 1800)ImeTick = GetTickCount();
	}
	else D2Dx9.DrawLineto(ImeX + (float)(imestr.length() * 8), ImeY - 15.f, ImeX + (imestr.length() * 8), ImeY, 1.f, D3DCOLOR_XRGB(255, 255, 255));
	RECT lFontRect = { (LONG)ImeX, (LONG)(ImeY - 18.f), CL_WinWidth,CL_WinHeight };
	D2Dx9.DrawTextFont(imestr.c_str(), 16, 1, FALSE, "����", &lFontRect);
	RECT lCareerRect = { CL_WinWidth - 180, 250, CL_WinWidth,CL_WinHeight };
	D2Dx9.DrawTextFont(szCareerTip, 12, 1, FALSE, "����", &lCareerRect, D3DCOLOR_XRGB(255, 255, 255));
}
