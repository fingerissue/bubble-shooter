#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

//mci를 사용하여 소리를 재생하기 위한 함수
void playSound(TCHAR* fileName) {
	TCHAR command[100];
	wsprintf(command, TEXT("play %s"), fileName);
	mciSendString(command, NULL, 0, NULL);
}

//재생중이던 소리를 멈추기 위한 함수
void stopSound(wchar_t* fileName) {
	wchar_t command[100];
	wsprintf(command, TEXT("stop %s"), fileName);
	mciSendString(command, NULL, 0, NULL);
}