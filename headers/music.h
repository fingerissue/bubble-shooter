#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

//mci�� ����Ͽ� �Ҹ��� ����ϱ� ���� �Լ�
void playSound(TCHAR* fileName) {
	TCHAR command[100];
	wsprintf(command, TEXT("play %s"), fileName);
	mciSendString(command, NULL, 0, NULL);
}

//������̴� �Ҹ��� ���߱� ���� �Լ�
void stopSound(wchar_t* fileName) {
	wchar_t command[100];
	wsprintf(command, TEXT("stop %s"), fileName);
	mciSendString(command, NULL, 0, NULL);
}