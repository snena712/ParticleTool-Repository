#include "WindowFileDealer.h"

// �E�B���h�E����f�[�^���[�h
bool WindowFileDealer::WindowsFileLoad(HWND hWnd, TCHAR* fname, int sz)
{
	char cdir[255];
	GetCurrentDirectory(255, cdir);

	OPENFILENAME o;
	fname[0] = _T('\0');
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);						//      �\���̃T�C�Y
	o.hwndOwner = hWnd;                             //      �e�E�B���h�E�̃n���h��
	o.lpstrInitialDir = cdir;						//      �����t�H���_�[
	o.lpstrFile = fname;							//      �擾�����t�@�C������ۑ�����o�b�t�@
	o.nMaxFile = sz;                                //      �擾�����t�@�C������ۑ�����o�b�t�@�T�C�Y
	o.lpstrFilter = _TEXT("EFE�t�@�C��(*.EFE)\0*.EFE\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");
	o.lpstrDefExt = _TEXT("TXT");
	o.lpstrTitle = _TEXT("�e�L�X�g�t�@�C�����w��");
	o.nFilterIndex = 1;
	return GetOpenFileName(&o);
}

// �E�B���h�E����摜PNG�f�[�^���[�h
bool WindowFileDealer::WindowsPNGFileLoad(HWND hWnd, TCHAR* fname, int sz)
{
	char cdir[255];
	GetCurrentDirectory(255, cdir);

	OPENFILENAME o;
	fname[0] = _T('\0');
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);						//      �\���̃T�C�Y
	o.hwndOwner = hWnd;                             //      �e�E�B���h�E�̃n���h��
	o.lpstrInitialDir = cdir;						//      �����t�H���_�[
	o.lpstrFile = fname;							//      �擾�����t�@�C������ۑ�����o�b�t�@
	o.nMaxFile = sz;                                //      �擾�����t�@�C������ۑ�����o�b�t�@�T�C�Y
	o.lpstrFilter = _TEXT("PNG�t�@�C��(*.PNG)\0*.PNG\0") _TEXT("�S�Ẵt�@�C��(*.*)\0*.*\0");
	o.lpstrDefExt = _TEXT("TXT");
	o.lpstrTitle = _TEXT("�e�L�X�g�t�@�C�����w��");
	o.nFilterIndex = 1;
	return GetOpenFileName(&o);
}

// �E�B���h�E����f�[�^�Z�[�u
bool WindowFileDealer::WindowsFileSave(HWND hWnd, TCHAR* fname, int sz)
{
	OPENFILENAME o;
	static TCHAR strFile[MAX_PATH];

	ZeroMemory(&o, sizeof(o));
	fname[0] = _T('\0');
	o.lStructSize = sizeof(o);
	o.hwndOwner = hWnd;
	o.lpstrFilter = TEXT("EFE�t�@�C�� {*.EFE}\0*.EFE\0")
		TEXT("All files {*.*}\0*.*\0\0");
	o.lpstrFile = fname;
	o.lpstrDefExt = _TEXT("efe");
	o.nMaxFile = sz;
	o.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

	return 	GetSaveFileName(&o);
}


//-----------------------------
// create by Suehara
//-----------------------------