#pragma once
#include "ErrorLogger.h"

class WindowContainer;

class RenderWindow
{
public:
	bool Initialize(WindowContainer * pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessages();
	HWND GetHWND() const;
	~RenderWindow();
private:
	void RegisterWindowClass();
	HWND handle = NULL; 
	HINSTANCE hInstance = NULL; 
	std::string title = "";
	std::wstring title_wide = L"";
	std::string classWindow = "";
	std::wstring classwindow_wide = L""; 
	int width = 0;
	int height = 0;
};