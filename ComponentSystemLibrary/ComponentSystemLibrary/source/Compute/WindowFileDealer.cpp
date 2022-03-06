#include "WindowFileDealer.h"

// ウィンドウからデータロード
bool WindowFileDealer::WindowsFileLoad(HWND hWnd, TCHAR* fname, int sz)
{
	char cdir[255];
	GetCurrentDirectory(255, cdir);

	OPENFILENAME o;
	fname[0] = _T('\0');
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);						//      構造体サイズ
	o.hwndOwner = hWnd;                             //      親ウィンドウのハンドル
	o.lpstrInitialDir = cdir;						//      初期フォルダー
	o.lpstrFile = fname;							//      取得したファイル名を保存するバッファ
	o.nMaxFile = sz;                                //      取得したファイル名を保存するバッファサイズ
	o.lpstrFilter = _TEXT("EFEファイル(*.EFE)\0*.EFE\0") _TEXT("全てのファイル(*.*)\0*.*\0");
	o.lpstrDefExt = _TEXT("TXT");
	o.lpstrTitle = _TEXT("テキストファイルを指定");
	o.nFilterIndex = 1;
	return GetOpenFileName(&o);
}

// ウィンドウから画像PNGデータロード
bool WindowFileDealer::WindowsPNGFileLoad(HWND hWnd, TCHAR* fname, int sz)
{
	char cdir[255];
	GetCurrentDirectory(255, cdir);

	OPENFILENAME o;
	fname[0] = _T('\0');
	ZeroMemory(&o, sizeof(o));
	o.lStructSize = sizeof(o);						//      構造体サイズ
	o.hwndOwner = hWnd;                             //      親ウィンドウのハンドル
	o.lpstrInitialDir = cdir;						//      初期フォルダー
	o.lpstrFile = fname;							//      取得したファイル名を保存するバッファ
	o.nMaxFile = sz;                                //      取得したファイル名を保存するバッファサイズ
	o.lpstrFilter = _TEXT("PNGファイル(*.PNG)\0*.PNG\0") _TEXT("全てのファイル(*.*)\0*.*\0");
	o.lpstrDefExt = _TEXT("TXT");
	o.lpstrTitle = _TEXT("テキストファイルを指定");
	o.nFilterIndex = 1;
	return GetOpenFileName(&o);
}

// ウィンドウからデータセーブ
bool WindowFileDealer::WindowsFileSave(HWND hWnd, TCHAR* fname, int sz)
{
	OPENFILENAME o;
	static TCHAR strFile[MAX_PATH];

	ZeroMemory(&o, sizeof(o));
	fname[0] = _T('\0');
	o.lStructSize = sizeof(o);
	o.hwndOwner = hWnd;
	o.lpstrFilter = TEXT("EFEファイル {*.EFE}\0*.EFE\0")
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