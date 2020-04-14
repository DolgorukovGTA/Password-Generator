#include "Main.h"
#include "ControlButtons.h" // ������ "�������", "��������"
#include "Version.h"

LRESULT CALLBACK aboutWindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP aboutProgramBitmap;
	static HFONT captionFont, textFont;

	switch (message)
	{

	case WM_CREATE: {

		captionFont = CreateFont(22, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0,
			DEFAULT_QUALITY, FF_SWISS, "Segoe UI");

		textFont = CreateFont(18, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0,
			DEFAULT_QUALITY, FF_SWISS, "Segoe UI");

		aboutProgramBitmap = LoadBitmap(GetModuleHandle(NULL),
			MAKEINTRESOURCE(IDB_ABOUT_ICON));

		// ������ "�������":
		HWND closeButton = CreateWindow("Button", NULL,
			WS_CHILD | WS_VISIBLE | BS_BITMAP, 220, 5, 25, 25,
			window, reinterpret_cast<HMENU>(CLOSE_BUTTON), NULL, NULL);

		// ��������� �������� �� ��������:
		HBITMAP closeButtonBitmap = LoadBitmap(GetModuleHandle(NULL), 
			MAKEINTRESOURCE(IDB_CLOSE_BUTTON));

		// ������������� �������� ������:
		SendMessage(closeButton, BM_SETIMAGE, IMAGE_BITMAP, reinterpret_cast<LPARAM>(closeButtonBitmap));
		DeleteObject(closeButtonBitmap); // �������� ������ ��� �� �����������, ��������� �

		// ������������� ��������� ��������� ��� ������, ����� �������� ������ ��� ...
		// ... ��������� �� ������:
		SetWindowSubclass(closeButton, controlButtonProcedure, 0, 0);

		break;
	}

	case WM_PAINT: {

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(window, &ps);

		SetBkColor(hdc, RGB(29, 29, 29));
		SetTextColor(hdc, RGB(255, 255, 255));
		SelectObject(hdc, captionFont);

		TextOut(hdc, 10, 6, "���������� � ���������", strlen("���������� � ���������"));

		HDC bitmapHdc = CreateCompatibleDC(NULL);
		SelectObject(bitmapHdc, aboutProgramBitmap);

		RECT rect;
		GetWindowRect(window, &rect);

		// �������� ������������ ������������ �������� ����, � ������� ����� ���������� �����������
		// ������ - 250 ����., ������ - 250 ����.
		uint8_t picX = static_cast<uint8_t>((rect.right - rect.left - 128) / 2);
		uint8_t picY = static_cast<uint8_t>(((rect.top - rect.bottom - 128) - 40) / 2);

		BitBlt(hdc, picX, picY, 128, 128, bitmapHdc, 0, 0, SRCCOPY);
		SelectObject(hdc, textFont);

		// �������� ���������� ������ �������� � ������� ������� ����� ���������� ����� ����
		GetClientRect(window, &rect);

		// �������� ��� ��������, ����� ��������� ���� �������������, ���������� ���� ����� ����� ���������� ����
		rect.top = 195;

		// ���������� ������ � ���� ������ ����������:
		char infoText[64];
		sprintf(infoText, "������ %s, ����� �� %s\r\n�����: Dolgorukov", version, releaseDate);

		// ������ ���������� ����-���������� ���, ������� ���������� strlen:
		DrawText(hdc, infoText, static_cast<int>(strlen(infoText)), &rect, DT_CENTER);

		DeleteDC(bitmapHdc);

		EndPaint(window, &ps);
		break;
	}

	case WM_COMMAND: {

		if (LOWORD(wParam) == CLOSE_BUTTON) {

			// �������� ������� ������ ����������:
			DeleteObject(aboutProgramBitmap);

			// �������� �������:
			DeleteObject(captionFont);
			DeleteObject(textFont);

			smoothWindowHide(window, true);
			break;
		}
	}

	case WM_LBUTTONDOWN:
		SendMessage(window, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		break;

	default:
		return DefWindowProc(window, message, wParam, lParam);

	}

	return 0;
}