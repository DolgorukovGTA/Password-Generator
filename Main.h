#pragma once

#define _CRT_SECURE_NO_WARNINGS

// ���������� ��� ����������� ����������� ��������� ���������:
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "comctl32.lib")

#include <Windows.h>
#include <cstring>
#include <cstdint>
#include <commctrl.h>
#include <thread>

#include "WindowsFeatures.h"
#include "resource.h" // ������� �������

// ID ������:
#define CLOSE_BUTTON 1
#define MINIMIZE_BUTTON 2
#define GENERATE_BUTTON 8
#define INFO_BUTTON 9