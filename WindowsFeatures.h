#pragma once

// 2-�� ��������: ����������� ���� ��� ��� (����� �������������)
void	smoothWindowApprearance(HWND hWnd, bool isClosing); // ������� ���������
void	smoothWindowHide(HWND hWnd, bool isClosing); // ������� �������

// �������, �������������� ����� ����:
ATOM	registerWindowClass(const char* winClassName, WNDPROC windowProcedure, HINSTANCE hInstance);

// ��������� ��������� ��� ���� ������������ ��� ������ � ������:
std::pair<uint16_t, uint16_t>	getWindowCenterCoordinates(uint16_t xSize, uint16_t ySize);