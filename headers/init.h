#pragma once
#pragma warning(disable:4996)

#include <stdlib.h>
#include <windows.h>

// Ŀ�� �����ִ� �Լ�
void ClearCursor() {
	CONSOLE_CURSOR_INFO c = { 0 };
	c.dwSize = 1;
	c.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);
}

// �ܼ�â ������ ���� �Լ�
void ReSizeConsole(int w, int h) {
	char chTemp[30];
	sprintf(chTemp, "mode con cols=%d lines=%d", w, h);
	system(chTemp);
}