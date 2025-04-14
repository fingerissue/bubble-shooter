#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>

#include "ImageLayer.h"
#include "define.h"

char sc[20] = { 0 }; // score ����� �� ����ϴ� ����

void GameOver(int score, int gameover) {
	Sleep(1);
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer);

	Image images[1] = {
		{"GameOverRetry.bmp", 0, 0, 1} // Retry ���õ� �̹���
	};

	if (gameover == 1) { // Retry ���õ� �̹��� ����
		images[0].fileName = "GameOverRetry.bmp";
	}
	else if (gameover == 2) { // Exit ���õ� �̹��� ����
		images[0].fileName = "GameOverExit.bmp";
	}

	imageLayer.imageCount = 1;
	imageLayer.images = images;
	imageLayer.renderAll(&imageLayer); // �̹��� ������

	Sleep(500);
	sprintf(sc, "%d ��", score); // ���� ���
	printTextWithAngle(imageLayer._consoleDC, 920, 665, "�����ٸ����", 130, 0, 0, RGB(166, 58, 99), 1, sc);
	Sleep(500);
	imageLayer.renderAll(&imageLayer); // �̹��� ������
}