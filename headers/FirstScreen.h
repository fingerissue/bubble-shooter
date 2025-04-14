#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "ImageLayer.h"

// Game Start 선택된 화면 (Space X)
void StartScreenStartImage() {
	Sleep(1);
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer);

	Image images[1] = {
		{"./images/StartScreen_GameStart.bmp", 0, 0, 1},
		//{"StartScreen_GameStart.bmp", 0, 0, 1} // Game Start 선택된 이미지 (Space X)
	};
	
	imageLayer.imageCount = 1;
	imageLayer.images = images;

	imageLayer.renderAll(&imageLayer); // 이미지 렌더링
}

// Game Start 선택된 화면 (Space O)
void StartScreenStartSpaceImage() {
	Sleep(1);
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer);
	Image images[1] = {
		{"./images/StartScreen_GameStartSpace.bmp", 0, 0, 1} // Game Start 선택된 이미지 (Space O)
	};

	imageLayer.imageCount = 1;
	imageLayer.images = images;

	imageLayer.renderAll(&imageLayer); // 이미지 렌더링
}

// Exit 선택된 화면 (Space X)
void StartScreenExitImage() {
	Sleep(1);

	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer);

	Image images[1] = {
		{"./images/StartScreen_Exit.bmp", 0, 0, 1} // Exit 선택된 이미지 (Space X)
	};

	imageLayer.imageCount = 1;
	imageLayer.images = images;

	imageLayer.renderAll(&imageLayer); // 이미지 렌더링
}

// Exit 선택된 화면 (Space O)
void StartScreenExitSpaceImage() {
	Sleep(1);

	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer);

	Image images[1] = {
		{"./images/StartScreen_ExitSpace.bmp", 0, 0, 1} // Exit 선택된 이미지 (Space O)
	};

	imageLayer.imageCount = 1;
	imageLayer.images = images;

	imageLayer.renderAll(&imageLayer); // 이미지 렌더링
}