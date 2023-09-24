#pragma once
#include <windows.h>
#include <fstream>

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SettingsWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateMainWindow();
void CreateSettingsWindow();
void SaveSettings();
std::string GetTextFromEdit(HWND editBox);