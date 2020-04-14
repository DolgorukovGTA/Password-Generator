#include "Main.h"
#include "MainWindow.h"

#include "AboutWindow.h"
#include "InfoButton.h"

#include "ControlButtons.h"
#include "GenerationButton.h"

// ���������� ����������:
HINSTANCE hInst;

// ���������� ��������� ����:
LRESULT CALLBACK mainWindowProcedure(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(nCmdShow);
	UNREFERENCED_PARAMETER(lpCmdLine);

	const char* mainWindowClassName = "Main Window";
	const char* mainWindowCaption = "��������� ������� by Dolgorukov";

	if (FindWindow(mainWindowClassName, mainWindowCaption)) {
		MessageBox(NULL, "��������� ������� ��� �������, ������� ��������� ��� ���� �������.",
			"���� ���!", MB_ICONERROR);
		return EXIT_FAILURE;
	}

	if (FAILED(registerWindowClass(mainWindowClassName, mainWindowProcedure, hInst))) {
		return EXIT_FAILURE;
	}

	// ������ ���������� ���, ����� ���� ���������� �� �������� ������:
	uint16_t mainWindowWidth = 250; // x
	uint16_t mainWindowHeight = 315; // y

	// ������� �������� ���������� ������ ���� ������������ ��� ��������:
	std::pair<uint16_t, uint16_t> posPair = getWindowCenterCoordinates(mainWindowWidth, mainWindowHeight);

	// ��� ���� �� ��������� ����� ����������, ��� ������� ��������� ...
	// ... ����������� � ��������� WM_SIZE

	HWND mainWindow = CreateWindowEx(WS_EX_LAYERED,
		mainWindowClassName, mainWindowCaption, WS_POPUP, posPair.first, posPair.second,
		mainWindowWidth, mainWindowHeight, NULL, NULL, hInst, NULL);

	if (FAILED(mainWindow)) {
		return EXIT_FAILURE;
	}

	UpdateWindow(mainWindow);
	ShowWindow(mainWindow, SW_SHOWNORMAL);

	MSG msg;
	// ���������� ������� �������������, ����-�������� �������� �������� �������
	HACCEL hAcc = LoadAccelerators(hInst, "Acc Table");

	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(mainWindow, hAcc, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK mainWindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool checkBoxStatuses[5] = { true, true, true, true, true };
	static HWND numberOfPasswordsEditControl, passwordsLengthEditControl;

	// ������ ����� �������������� � WM_PAINT, ������������ ...
	// ... � WM_CREATE
	static HFONT captionFont, textFont;

	switch (message)
	{

	case WM_KEYDOWN: {
		if (LOWORD(wParam) == VK_TAB) {
			SetFocus(passwordsLengthEditControl);
		}

		break;
	}


	// ������ ������� �������� ��������� ����, ����� �� ��� �������������
	// �� ������ �����:
	case WM_SIZE: {
		if (LOWORD(wParam) == SIZE_RESTORED) {
			std::thread thr([window] {
				smoothWindowApprearance(window, false);
			});

			thr.detach();
		}
		
		break;
	}

	case WM_CREATE: {

		// ��������� �������:
		captionFont = CreateFont(22, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0,
			DEFAULT_QUALITY, FF_SWISS, "Segoe UI");

		textFont = CreateFont(18, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0,
			DEFAULT_QUALITY, FF_SWISS, "Segoe UI");

		// ������ "�������":
		HWND closeButton = CreateWindow("Button", NULL,
			WS_CHILD | WS_VISIBLE | BS_BITMAP /*| BS_OWNERDRAW */, 220, 5, 25, 25, 
			window, reinterpret_cast<HMENU>(CLOSE_BUTTON), NULL, NULL);

		// ������������� ������ �������� �������� �����, ����� ���� ��������� ��������:
		HBITMAP closeButtonBitmap = LoadBitmap(GetModuleHandle(NULL),
			MAKEINTRESOURCE(IDB_CLOSE_BUTTON));

		SendMessage(closeButton, BM_SETIMAGE, IMAGE_BITMAP, reinterpret_cast<LPARAM>(closeButtonBitmap));
		DeleteObject(closeButtonBitmap);

		// ������������� ��������� ��������� ��� ������, ����� �������� ������ ��� ...
		// ... ��������� �� ������:
		SetWindowSubclass(closeButton, controlButtonProcedure, 0, 0);

		// ������ "��������":
		HWND minimizeButton = CreateWindow("Button", NULL,
			WS_CHILD | WS_VISIBLE | BS_BITMAP, 190, 5, 25, 25, window, 
			reinterpret_cast<HMENU>(MINIMIZE_BUTTON), NULL, NULL);

		// ���������� �������� �� ��������:
		HBITMAP minimizeButtonBitmap = LoadBitmap(GetModuleHandle(NULL),
			MAKEINTRESOURCE(IDB_MINIMIZE_BUTTON));

		// ������������� ����������� ���������, ��� � �����, ��� ������ �����������:
		SetWindowSubclass(minimizeButton, controlButtonProcedure, 0, 0);

		// ������������� ������ �������� ������ �����:
		SendMessage(minimizeButton, BM_SETIMAGE, IMAGE_BITMAP, 
			reinterpret_cast<LPARAM>(minimizeButtonBitmap));
		DeleteObject(minimizeButtonBitmap); // ��������� ��������

		// ���� ����� "���������� �������":
		numberOfPasswordsEditControl = CreateWindow("Edit", "64", // ����� �� ���������: 64
			WS_CHILD | WS_VISIBLE, 195, 49, 40, 20, window, NULL, NULL, NULL);

		SendMessage(numberOfPasswordsEditControl, WM_SETFONT, reinterpret_cast<WPARAM>(textFont), TRUE);

		// ���� ����� "����� ������� ������":
		passwordsLengthEditControl = CreateWindow("Edit", "32", // ����� �� ���������: 32
			WS_CHILD | WS_VISIBLE, 195, 72, 40, 20, window, NULL, NULL, NULL);

		SendMessage(passwordsLengthEditControl, WM_SETFONT, reinterpret_cast<WPARAM>(textFont), TRUE);

		// ��������, ���������� ����� �� �������, ����� �������� �� ����� �� ������������:
		CreateWindow("Button", NULL,
			WS_CHILD | WS_VISIBLE | BS_CHECKBOX | SS_WHITERECT,
			10, 100, 15, 25, window, reinterpret_cast<HMENU>(3), hInst, NULL);

		CreateWindow("Button", NULL,
			WS_CHILD | WS_VISIBLE | BS_CHECKBOX | SS_WHITERECT,
			10, 122, 15, 25, window, reinterpret_cast<HMENU>(4), hInst, NULL);

		CreateWindow("Button", NULL,
			WS_CHILD | WS_VISIBLE | BS_CHECKBOX | SS_WHITERECT,
			10, 144, 15, 25, window, reinterpret_cast<HMENU>(5), hInst, NULL);

		CreateWindow("Button", NULL,
			WS_CHILD | WS_VISIBLE | BS_CHECKBOX | SS_WHITERECT,
			10, 166, 15, 25, window, reinterpret_cast<HMENU>(6), hInst, NULL);

		CreateWindow("Button", NULL,
			WS_CHILD | WS_VISIBLE | BS_CHECKBOX | SS_WHITERECT,
			10, 188, 15, 25, window, reinterpret_cast<HMENU>(7), hInst, NULL);

		for (uint8_t i = 3; i <= 7; i++) {
			CheckDlgButton(window, i, BST_CHECKED);
		}

		// ������ ���������:
		HWND generationButton = CreateWindow("BUTTON", NULL,
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
			10, 220, 230, 40, window, reinterpret_cast<HMENU>(GENERATE_BUTTON), hInst, NULL);

		SetWindowSubclass(generationButton, generationButtonProcedure, NULL, NULL);


		// ������ ����������:
		HWND infoButton = CreateWindow("BUTTON", NULL,
			WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			10, 265, 230, 40, window, reinterpret_cast<HMENU>(INFO_BUTTON), hInst, NULL);

		SetWindowSubclass(infoButton, infoButtonProcedure, NULL, NULL);
		break;
	}

	case WM_COMMAND: {

		switch (LOWORD(wParam))
		{

		case CLOSE_BUTTON: {

			HWND aboutWindow = FindWindow("About Program Window", "���������� � ���������");

			if (aboutWindow) {
				std::thread thr(smoothWindowHide, aboutWindow, true);
				thr.detach();
			}

			smoothWindowHide(window, true);

			// ������������ �������� �������:
			DeleteObject(captionFont);
			DeleteObject(textFont);

			ExitProcess(EXIT_SUCCESS);
			break;
		}

		case MINIMIZE_BUTTON: {

			HWND aboutWindow = FindWindow("About Program Window", "���������� � ���������");

			if (aboutWindow) {
				std::thread thr(smoothWindowHide, aboutWindow, true);
				thr.detach();
			}

			smoothWindowHide(window, false);

			SendMessage(window, WM_SYSCOMMAND, SC_MINIMIZE, lParam);
			Sleep(300);
			break;
		}

		case GENERATE_BUTTON: {

			// �������� �������� ��������� �� ����������� �� "�������� ����������"
			uint8_t trueValuesAmount = 0;
			for (uint8_t i = 0; i < (sizeof(checkBoxStatuses) - 1); i++) {
				if (checkBoxStatuses[i]) {
					++trueValuesAmount;
				}
			}

			if (!trueValuesAmount) {
				MessageBox(window,
					"���� �� � ������ �� ��������� ������ ������ �������.\n������� ��������� ���������� ���������.",
					"���-�� ���� ����� �� ���...", MB_ICONERROR);
				break;
			}

			// ������ � ������ "���-�� �������" � "����� ������� ������":
			char numberOfPasswordsBuffer[5];
			char passwordsLengthBuffer[5];

			GetWindowText(numberOfPasswordsEditControl, &numberOfPasswordsBuffer[0],
				sizeof(numberOfPasswordsBuffer));
			GetWindowText(passwordsLengthEditControl, &passwordsLengthBuffer[0],
				sizeof(numberOfPasswordsBuffer));

			uint16_t numberOfPasswords = static_cast<uint16_t>(atoi(numberOfPasswordsBuffer));
			uint16_t passwordsLength = static_cast<uint16_t>(atoi(passwordsLengthBuffer));

			if (!numberOfPasswords && !passwordsLength) {
				MessageBox(window, 
					"���� ����������� ������� � ������ ������� ������� - ������ � ����� �����",
					"���, ��� �� ����-������ �����...", MB_ICONERROR);
				break;
			}
			else {
				if (!numberOfPasswords || numberOfPasswords < 5 || numberOfPasswords > 4096) {
					MessageBox(window, 
						"���� ����-�� ������� - ������ �����!\n����������� ��������: 8, ������������: 4096.",
						"�-�-�-�! ����� �������� ������� ������...", MB_ICONERROR);
					break;
				}

				if (!passwordsLength || passwordsLength < 4 || passwordsLength > 256) {
					MessageBox(window, 
						"���� ������ ������� ������� - ������ �����!\n����������� ��������: 4, ������������: 256.",
						"׸��! � � ��� �� ����������� ������?", MB_ICONERROR);
					break;
				}
			}

			// �������� ��������� �� ������ � �������� (��-������, ������� ������� ���������) � ����� ��������� �������:
			std::pair<char**, uint16_t> genPair = generatePasswords(numberOfPasswords, passwordsLength, &checkBoxStatuses[0]);
			
			// �������� ������ �� ���������� ���������� ��� ������ ����������:
			char** &passwordsArray = genPair.first;
			uint16_t &generationTime = genPair.second;

			// �������� ������ ���� � �����:
			char* outputFileFullPath = getOutputFileName(numberOfPasswords, passwordsLength);

			// ���������� ������ � ���� �� ����������� ����:
			uint16_t writingTime = writePasswordsArrayToFile(passwordsArray, passwordsLength, 
				numberOfPasswords, outputFileFullPath);

			// ����������� �� �������� ������ � ���� ��� ������������:
			char geBuf[10];
			char wrBuf[10];

			(generationTime != 0) ? sprintf(geBuf, "%u", generationTime) : sprintf(geBuf, "< 1");
			(writingTime != 0) ? sprintf(wrBuf, "%u", writingTime) : sprintf(wrBuf, "< 1");

			char messageWithFilePath[300];
			sprintf(messageWithFilePath,
				"��������������� ������ �������� � ���� '%s'\n����� ���������: %s ��\n����� ������: %s ��",
				outputFileFullPath, geBuf, wrBuf);

			MessageBox(window, messageWithFilePath, "�������������!", MB_OK);

			// ������������ ������, ���������� ��� ������� � ��������:
			for (uint16_t i = 0; i < numberOfPasswords; i++)
				delete[] passwordsArray[i];

			delete[] passwordsArray;
			break;
		}

		case INFO_BUTTON:

			if (FindWindow("About Program Window", "���������� � ���������")) {
				MessageBox(window, "���� � ����������� � ��������� ��� �������.", 
					"����� ������� ������ ���?", MB_ICONERROR);
				break;
			}
			 
			// ������������ ���� ��� ������ ���������� � ����������:
			char* aboutWindowClassName = "About Program Window";
			char* aboutWindowCaption = "���������� � ���������";

			// �������-���������� ��������������� ���� ��������� � ����� AboutWindow.cpp
			if (FAILED(registerWindowClass(aboutWindowClassName, aboutWindowProcedure, hInst))) {
				MessageBox(window, "�� ������� ���������������� ����� ���� '� ���������'",
					"��-��, ������� � ��� ������������...", MB_ICONERROR);
				break;
			};

			uint16_t aboutWindowWeight = 250;
			uint16_t aboutWindowHeight = 250;

			std::pair<uint16_t, uint16_t> posPair = getWindowCenterCoordinates(aboutWindowWeight, aboutWindowHeight);

			HWND aboutWindow = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST, 
				aboutWindowClassName, aboutWindowCaption, WS_POPUP, posPair.first, posPair.second, 
				aboutWindowWeight, aboutWindowHeight, window, NULL, hInst, NULL); // ���� �������� �������� �� ��������� � ���������

			if (FAILED(aboutWindow)) {
				MessageBox(window, "�� ������� ������� ���� '� ���������'",
					"���i����...", MB_ICONERROR);
				break;
			};

			UpdateWindow(aboutWindow);
			ShowWindow(aboutWindow, SW_SHOWNORMAL);

			std::thread thr([aboutWindow] {
				smoothWindowApprearance(aboutWindow, false);
			});

			thr.detach();
			break;
		}

		// ������������ ��������, ���������� ��� ������ 2 ������
		if (LOWORD(wParam) >= 3 && LOWORD(wParam) <= 7) {
			checkBoxStatuses[wParam - 3] = setCheckBoxTrueOrFalse(window, static_cast<uint8_t>(wParam));
		}
	}

	case WM_PAINT: {

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(window, &ps);

		SelectObject(hdc, captionFont);
		SetBkColor(hdc, RGB(29, 29, 29));
		SetTextColor(hdc, RGB(255, 255, 255));

		// ������ ����-���������� ���������� ���, ������� ���������� ������� strlen
		TextOut(hdc, 10, 6, "��������� �������", strlen("��������� �������"));

		SelectObject(hdc, textFont);

		TextOut(hdc, 10, 52, "���-�� �������:", strlen("���-�� �������:"));
		TextOut(hdc, 10, 74, "����� ������� ������:", strlen("����� ������� ������:"));

		TextOut(hdc, 30, 103, "������������ �����", strlen("������������ �����"));
		TextOut(hdc, 30, 125, "������������ ��������� �����", strlen("������������ ��������� �����"));
		TextOut(hdc, 30, 147, "������������ �������� �����", strlen("������������ �������� �����"));
		TextOut(hdc, 30, 169, "������������ �������", strlen("������������ �������"));
		TextOut(hdc, 30, 191, "�������� ����������", strlen("�������� ����������"));

		EndPaint(window, &ps);
		break;
	}

	case WM_CTLCOLORSTATIC:
		return reinterpret_cast<INT_PTR>(CreateSolidBrush(RGB(29, 29, 29)));

	case WM_LBUTTONDOWN:
		SendMessage(window, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		break;

	default:
		return DefWindowProc(window, message, wParam, lParam);
	}

	return 0;
}