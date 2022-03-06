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

	LPCSTR filter = TEXT("FBX�t�@�C��(*.fbx)\0*.fbx;\0")
		TEXT("FBX�t�@�C��(*.fbx)\0*.fbx\0")
		TEXT("���ׂẴt�@�C��(*.*)\0*.*\0");

	LPCSTR title = TEXT("FBX�t�@�C����I�����܂��B");

	return SetFilePath(filter, title);
}
void OpenFileDialog::SetHLSLPath() {

	LPCSTR filter = TEXT("HLSL�t�@�C��(*.hlsl)\0*.hlsl;\0")
		TEXT("HLSL�t�@�C��(*.hlsl)\0*.hlsl\0")
		TEXT("���ׂẴt�@�C��(*.*)\0*.*\0");

	LPCSTR title = TEXT("HLSL�t�@�C����I�����܂��B");

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

	LPCSTR filter = TEXT("PNG/TGA/JPG�t�@�C��(*.png,*.tga,*.jpg)\0*.png;*.tga;*.jpg;\0")
		TEXT("���ׂẲ摜�t�@�C��(*.png,*.tga,*.jpg)\0*.png;*.tga;*.jpg;\0")
		TEXT("PNG�t�@�C��(*.png)\0*.png\0")
		TEXT("TGA�t�@�C��(*.tga)\0*.tga\0")
		TEXT("JPG�t�@�C��(*.jpg)\0*.jpg\0")
		TEXT("���ׂẴt�@�C��(*.*)\0*.*\0");

	LPCSTR title = TEXT("�摜�t�@�C����I�����܂��B");

	return SetFilePath(filter, title);

}

// ��傤�����ɕ���