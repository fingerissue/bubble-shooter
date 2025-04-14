#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>

#include "ImageLayer.h"
#include "define.h"

char sc[20] = { 0 }; // score 출력할 때 사용하는 변수

void GameOver(int score, int gameover) {
	Sleep(1);
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer);

	Image images[1] = {
		{"GameOverRetry.bmp", 0, 0, 1} // Retry 선택된 이미지
	};

	if (gameover == 1) { // Retry 선택된 이미지 띄우기
		images[0].fileName = "GameOverRetry.bmp";
	}
	else if (gameover == 2) { // Exit 선택된 이미지 띄우기
		images[0].fileName = "GameOverExit.bmp";
	}

	imageLayer.imageCount = 1;
	imageLayer.images = images;
	imageLayer.renderAll(&imageLayer); // 이미지 렌더링

	Sleep(500);
	sprintf(sc, "%d 점", score); // 점수 출력
	printTextWithAngle(imageLayer._consoleDC, 920, 665, "나눔바른고딕", 130, 0, 0, RGB(166, 58, 99), 1, sc);
	Sleep(500);
	imageLayer.renderAll(&imageLayer); // 이미지 렌더링
}