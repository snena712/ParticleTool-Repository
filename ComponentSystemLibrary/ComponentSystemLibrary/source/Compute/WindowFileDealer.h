#pragma once
#include <iostream>
#include <fstream> 
#include <sstream> 
#include <windows.h>
#include <tchar.h>

namespace WindowFileDealer
{
	// �E�B���h�E�g�p���Ẵ��[�h�Z�[�u
	bool WindowsFileLoad(HWND hWnd, TCHAR* fname, int sz);
	bool WindowsPNGFileLoad(HWND hWnd, TCHAR* fname, int sz);
	bool WindowsFileSave(HWND hWnd, TCHAR* fname, int sz);
};


//-----------------------------
// create by Suehara
//-----------------------------