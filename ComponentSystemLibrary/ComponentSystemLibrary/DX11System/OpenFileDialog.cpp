#include "OpenFileDialog.h"

void OpenFileDialog::SetFilePath(LPCSTR _filter, LPCSTR _title) {
	auto lowApp =CWindow::Instance();

	static TCHAR currentPath[MAX_PATH];

	if (currentPath[0] == TEXT('\0')) {
		GetCurrentDirectory(MAX_PATH, currentPath);
	}
	if (openFileName.lStructSize == 0) {
		openFileName.lStructSize = sizeof(OPENFILENAME);
		openFileName.hwndOwner = lowApp->GetHandle();
		openFileName.lpstrInitialDir = currentPath;
		openFileName.lpstrFile = returnFileName;
		openFileName.nMaxFile = MAX_PATH;
		openFileName.lpstrFilter = _filter;
		openFileName.Flags = OFN_FILEMUSTEXIST;
	}
	GetOpenFileName(&openFileName);

}
void OpenFileDialog::SetFbxPath() {

	LPCSTR filter = TEXT("FBXファイル(*.fbx)\0*.fbx;\0")
		TEXT("FBXファイル(*.fbx)\0*.fbx\0")
		TEXT("すべてのファイル(*.*)\0*.*\0");

	LPCSTR title = TEXT("FBXファイルを選択します。");

	return SetFilePath(filter, title);
}
void OpenFileDialog::SetHLSLPath() {

	LPCSTR filter = TEXT("HLSLファイル(*.hlsl)\0*.hlsl;\0")
		TEXT("HLSLファイル(*.hlsl)\0*.hlsl\0")
		TEXT("すべてのファイル(*.*)\0*.*\0");

	LPCSTR title = TEXT("HLSLファイルを選択します。");

	return SetFilePath(filter, title);
}

void OpenFileDialog::SetFolderPath() {

	IFileOpenDialog* fileOpen;

	CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&fileOpen));

	DWORD option = 0;
	fileOpen->GetOptions(&option);
	fileOpen->SetOptions(option | FOS_PICKFOLDERS);

	fileOpen->Show(NULL);

	IShellItem* item;
	fileOpen->GetResult(&item);
	PWSTR pszFilePath;
	item->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszFilePath);

	//std::string path = GetStringFromWideString(pszFilePath);
	std::string path = "";
	sprintf_s(returnFileName, path.c_str());
	CoTaskMemFree(pszFilePath);
	
	item->Release();
	fileOpen->Release();

}

void OpenFileDialog::SetTexturePath() {

	LPCSTR filter = TEXT("PNG/TGA/JPGファイル(*.png,*.tga,*.jpg)\0*.png;*.tga;*.jpg;\0")
		TEXT("すべての画像ファイル(*.png,*.tga,*.jpg)\0*.png;*.tga;*.jpg;\0")
		TEXT("PNGファイル(*.png)\0*.png\0")
		TEXT("TGAファイル(*.tga)\0*.tga\0")
		TEXT("JPGファイル(*.jpg)\0*.jpg\0")
		TEXT("すべてのファイル(*.*)\0*.*\0");

	LPCSTR title = TEXT("画像ファイルを選択します。");

	return SetFilePath(filter, title);

}

// りょうせいに聞く