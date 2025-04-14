#pragma once
#pragma warning(disable:4996)

#include <stdlib.h>
#include <windows.h>

// 커서 없애주는 함수
void ClearCursor() {
	CONSOLE_CURSOR_INFO c = { 0 };
	c.dwSize = 1;
	c.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
}

// 콘솔창 사이즈 변경 함수
void ReSizeConsole(int w, int h) {
	char chTemp[30];
	sprintf(chTemp, "mode con cols=%d lines=%d", w, h);
	system(chTemp);
}