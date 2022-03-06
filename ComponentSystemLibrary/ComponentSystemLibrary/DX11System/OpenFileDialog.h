#pragma once
#include  "CWindow.h"
#include <shobjidl.h> 
#include <string>
class OpenFileDialog {
private:
	OPENFILENAME openFileName;
	TCHAR returnFileName[MAX_PATH];
public:
	void SetFilePath(LPCSTR _filter, LPCSTR _title);
	void SetFbxPath();
	void SetHLSLPath();
	void SetTexturePath();
	void SetFolderPath();
	char* GetReturnFileName() { return returnFileName; };
};