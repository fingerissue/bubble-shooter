#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "ImageLayer.h"

// Game Start ���õ� ȭ�� (Space X)
void StartScreenStartImage() {
	Sleep(1);
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer);

	Image images[1] = {
		{"./images/StartScreen_GameStart.bmp", 0, 0, 1},
		//{"StartScreen_GameStart.bmp", 0, 0, 1} // Game Start ���õ� �̹��� (Space X)
	};
	
	imageLayer.imageCount = 1;
	imageLayer.images = images;

	imageLayer.renderAll(&imageLayer); // �̹��� ������
}

// Game Start ���õ� ȭ�� (Space O)
void StartScreenStartSpaceImage() {
	Sleep(1);
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer);
	Image images[1] = {
		{"./images/StartScreen_GameStartSpace.bmp", 0, 0, 1} // Game Start ���õ� �̹��� (Space O)
	};

	imageLayer.imageCount = 1;
	imageLayer.images = images;

	imageLayer.renderAll(&imageLayer); // �̹��� ������
}

// Exit ���õ� ȭ�� (Space X)
void StartScreenExitImage() {
	Sleep(1);

	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer);

	Image images[1] = {
		{"./images/StartScreen_Exit.bmp", 0, 0, 1} // Exit ���õ� �̹��� (Space X)
	};

	imageLayer.imageCount = 1;
	imageLayer.images = images;

	imageLayer.renderAll(&imageLayer); // �̹��� ������
}

// Exit ���õ� ȭ�� (Space O)
void StartScreenExitSpaceImage() {
	Sleep(1);

	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer);

	Image images[1] = {
		{"./images/StartScreen_ExitSpace.bmp", 0, 0, 1} // Exit ���õ� �̹��� (Space O)
	};

	imageLayer.imageCount = 1;
	imageLayer.images = images;

	imageLayer.renderAll(&imageLayer); // �̹��� ������
}