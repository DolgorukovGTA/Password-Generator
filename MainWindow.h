﻿#pragma once

bool							setCheckBoxTrueOrFalse(HWND window, uint8_t nCheckBox);
std::pair<char**, uint16_t>		generatePasswords(uint16_t numberOfPasswords, uint16_t passwordsLength, bool* cbValues);
uint16_t						writePasswordsArrayToFile(char** arr, uint16_t passwordLength, uint16_t passwordsAmount, wchar_t* fileFullPath);
wchar_t*						getOutputFileName(uint16_t passwordsAmount, uint16_t passwordLength);