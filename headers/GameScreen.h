#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <stdbool.h>

#include "ImageLayer.h"
#include "define.h"
#include "init.h"

// ���� ��� ȭ�� ��������
Image images[300] = {
	{"GameLayout.bmp", 0, 0, 1}, // ���� �� �̹���
	{"NULL", NEXT_X, NEXT_Y, NEXT_BUBBLE_SIZE}, // ���� ���� ����
	{"NULL", CURRENT_X, CURRENT_Y, MAIN_BUBBLE_SIZE} // ���� ���� ����
};

// ���� ����
int score = 0;


// ���� ��Ҵ��� üũ�ϴ� �Լ�
int CheckBound(double curX, double curY) { // �ʿ��� �Ű�����: ���� ������ x, y ��ǥ
	if ((curX >= MAP_START_X) && (curX <= MAP_START_X + (MAP_MAX_X_COUNT - 1) * MAP_MARGIN) && (curY >= MAP_START_Y) && (curY <= MAP_START_Y + MAP_MARGIN * (MAP_MAX_Y_COUNT + 1))) // ���� ������ x, y��ǥ�� ���� ������
		return 1; // 1 ��ȯ
	else
		return 0; // �ƴϸ� 0 ��ȯ
}

// ������ ��ġ���� üũ�ϱ� ���� �ʿ��� �Լ� (������ �߾Ӱ�)
int bubbleMidX = 0;
int bubbleMidY = 0;

// ������ ��ġ���� üũ�ϴ� �Լ�
int CheckBubble(char(*Bubble)[20]) { // �ʿ��� �Ű�����: ���� �̸�
	for (int i = 0; i < MAP_MAX_Y_COUNT * MAP_MAX_X_COUNT; i++) { // ���� ũ�⸸ŭ
		if (
 			(images[3 + i].fileName != Bubble[0]) && // ������ ���� �ʰ� (= �ְ�)
			(
			((images[GAME_MAP_END + 5].x >= images[3 + i].x - MAP_MARGIN) && (images[GAME_MAP_END + 5].x <= images[3 + i].x + MAP_MARGIN)) && // �¿쿡 ������ �ְ�
			((images[GAME_MAP_END + 5].y >= images[3 + i].y - MAP_MARGIN) && (images[GAME_MAP_END + 5].y <= images[3 + i].y + MAP_MARGIN)) // ���Ʒ��� ������ ���� ��
			)
				) {
			if ( // ������ �ִ� ������ �ִ��� üũ
				(images[3 + i + 1].fileName != Bubble[0]) &&
				(((images[GAME_MAP_END + 5].x >= images[3 + i + 1].x - MAP_MARGIN) && (images[GAME_MAP_END + 5].x <= images[3 + i + 1].x + MAP_MARGIN)) &&
				((images[GAME_MAP_END + 5].y >= images[3 + i + 1].y - MAP_MARGIN) && (images[GAME_MAP_END + 5].y <= images[3 + i + 1].y + MAP_MARGIN)))
				){
				continue; // �ִٸ� ���� ������ �������� �ٲ�
			}
			
			// ������ ���� ��ġ(�߾Ӱ�) ����
			bubbleMidX = images[GAME_MAP_END + 5].x + MAP_MARGIN / 2;
			bubbleMidY = images[GAME_MAP_END + 5].y + MAP_MARGIN / 2;
			return 1; // 1��ȯ
		}
	}	
	return 0; // 0 ��ȯ
}

// ���� ��ġ �����ϴ� �迭
int currentBubblePosition[2] = { -1, -1 };

// ���� ������ �迭�� �����ϴ� �Լ�
int SaveMap(char(*Bubble)[20], int* map, char curBubble[20]) { // �ʿ��� �Ű�����: ���� �̸�, ��, ���� ���� �̸�
	int temp = 0; // ���� ���� �̸��� �����ϴ� ����

	for (int i = 0; i < 7; i++) {
		if (!strcmp(curBubble, Bubble[i])) {
			temp = i; // ���� �����̶� �̸��� ���� ������ �ε����� ����
		}
	}
	
	for (int i = 0; i < MAP_MAX_Y_COUNT * MAP_MAX_X_COUNT; i++) { // ���� ũ�⸸ŭ �ݺ��ϸ鼭
		if (
			((images[3 + i].x <= bubbleMidX && images[3 + i].x + MAP_MARGIN >= bubbleMidX) && // images �迭�� ����� x���� y���� ���� ������ �߾Ӱ��� ��� �ִٸ�
			(images[3 + i].y <= bubbleMidY && images[3 + i].y + MAP_MARGIN >= bubbleMidY))
			) {
			if (images[3 + i].fileName == Bubble[0]) { // ������ ���ٸ�
				map[i] = temp; // ���� ����
				currentBubblePosition[0] = i / MAP_MAX_X_COUNT; // y��ǥ ����
				currentBubblePosition[1] = i % MAP_MAX_X_COUNT; // x��ǥ ����
			}
			else { // ������ ���� ���
				map[i + MAP_MAX_X_COUNT] = temp; // �� ���� �ٷ� �ؿ� ����
				currentBubblePosition[0] = i / MAP_MAX_X_COUNT + 1; // y��ǥ ����
				currentBubblePosition[1] = i % MAP_MAX_X_COUNT; // x��ǥ ����
			}
		}
	}
}

// ���� ������ �Լ��� �����ϱ� ���� �ʿ��� ����
int count = 0; // ���� ������ ������ ���� ���� ����
int visited[MAP_MAX_Y_COUNT][MAP_MAX_X_COUNT] = { 0 }; // �湮�� �� üũ�ϴ� �迭
char s[20] = { 0 }; // score ����� �� ����ϴ� ����

// ���� ������ �Լ�
int BurstBubbleCheck(int(*map)[MAP_MAX_X_COUNT], char(*Bubble)[20], char curBubble[20], int curY, int curX, int preY, int preX) { // �ʿ��� �Ű�����: ��, ���� �̹��� �̸�, ���� ���� �̹��� �̸�, ���� y��ǥ, ���� x��ǥ, ���� Y��ǥ, ���� X��ǥ
	int temp = 0; // ���� ���� �̸��� �����ϴ� ����

	for (int i = 0; i < 7; i++) {
		if (!strcmp(curBubble, Bubble[i])) {
			temp = i; // ���� �����̶� �̸��� ���� ������ �ε����� ����
		}
	}

	if ((curX < MAP_MAX_X_COUNT) && (curX >= 0) && (curY < MAP_MAX_Y_COUNT) && (curY >= 0) && (map[curY][curX] != 0)) { // ���� ��ǥ�� �� �ȿ� ���� ��
		if ((map[curY][curX] == map[curY][curX + 1]) && // ���� ��ǥ�� ������ ��ǥ�� ����
			!((curY == preY) && (curX + 1 == preX)) && // ������ �湮�� ��ǥ�� �ƴϰ�
			(curX + 1 < MAP_MAX_X_COUNT) && (curX + 1 >= 0) && (curY < MAP_MAX_Y_COUNT) && (curY >= 0)) { // üũ�� ��ǥ�� �� �ȿ� ���� ��
			if (visited[curY][curX] <= 2 && visited) { // �湮�� ���� 2�� ������ ��
				visited[curY][curX]++; // �湮�� �� üũ
				visited[curY][curX + 1]++;
				BurstBubbleCheck(map, Bubble, curBubble, curY, curX + 1, curY, curX); // ������ ����� �̸��� ������ ����Լ� ȣ��
				count++; // ���� ������ ���� ����
			}
		}
		// ���� if���� ���� ������� �Ʒ�, ����, ��, �밢���� ����� ��
		if ((map[curY][curX] == map[curY][curX - 1]) &&
			!((curY == preY) && (curX - 1 == preX)) && 
			(curX - 1 < MAP_MAX_X_COUNT) && (curX - 1 >= 0) && (curY < MAP_MAX_Y_COUNT) && (curY >= 0)) { // ����
			if (visited[curY][curX] <= 2) {
				visited[curY][curX]++;
				visited[curY][curX - 1]++;
				BurstBubbleCheck(map, Bubble, curBubble, curY, curX - 1, curY, curX);
				count++;
			}
		}
		if (map[curY][curX] == map[curY + 1][curX] &&
			!((curY + 1 == preY) && (curX == preX)) && 
			(curX < MAP_MAX_X_COUNT) && (curX >= 0) && (curY + 1 < MAP_MAX_Y_COUNT) && (curY + 1 >= 0)) { // �Ʒ�
			if (visited[curY][curX] <= 2) {
				visited[curY][curX]++;
				visited[curY + 1][curX]++;
				BurstBubbleCheck(map, Bubble, curBubble, curY + 1, curX, curY, curX);
				count++;
			}
		}
		if (map[curY][curX] == map[curY - 1][curX] &&
			!((curY - 1 == preY) && (curX == preX)) &&
			(curX < MAP_MAX_X_COUNT) && (curX >= 0) && (curY - 1 < MAP_MAX_Y_COUNT) && (curY - 1 >= 0)) { // ��
			if (visited[curY][curX] <= 2) {
				visited[curY][curX]++;
				visited[curY - 1][curX]++;
				BurstBubbleCheck(map, Bubble, curBubble, curY - 1, curX, curY, curX);
				count++;
			}
		}
		if (map[curY][curX] == map[curY + 1][curX + 1] &&
			!((curY + 1 == preY) && (curX + 1 == preX)) && 
			(curX + 1 < MAP_MAX_X_COUNT) && (curX + 1 >= 0) && (curY + 1 < MAP_MAX_Y_COUNT) && (curY + 1 >= 0)) { // ������ �Ʒ�
			if (visited[curY][curX] <= 2) {
				visited[curY][curX]++;
				visited[curY + 1][curX + 1]++;
				BurstBubbleCheck(map, Bubble, curBubble, curY + 1, curX + 1, curY, curX);
				count++;
			}
		}
		if (map[curY][curX] == map[curY + 1][curX - 1] &&
			!((curY + 1 == preY) && (curX - 1 == preX)) &&
			(curX - 1 < MAP_MAX_X_COUNT) && (curX - 1 >= 0) && (curY + 1 < MAP_MAX_Y_COUNT) && (curY + 1 >= 0)) { // ���� �Ʒ�
			if (visited[curY][curX] <= 2) {
				visited[curY][curX]++;
				visited[curY + 1][curX - 1]++;
				BurstBubbleCheck(map, Bubble, curBubble, curY + 1, curX - 1, curY, curX);
				count++;
			}
		}
		if (map[curY][curX] == map[curY - 1][curX + 1] &&
			!((curY - 1 == preY) && (curX + 1 == preX)) &&
			(curX + 1 < MAP_MAX_X_COUNT) && (curX + 1 >= 0) && (curY - 1 < MAP_MAX_Y_COUNT) && (curY - 1 >= 0)) { // ������ ��
			if (visited[curY][curX] <= 2) {
				visited[curY][curX]++;
				visited[curY - 1][curX + 1]++;
				BurstBubbleCheck(map, Bubble, curBubble, curY - 1, curX + 1, curY, curX);
				count++;
			}
		}
		if (map[curY][curX] == map[curY - 1][curX - 1] &&
			!((curY - 1 == preY) && (curX + 1 == preX)) &&
			(curX - 1 < MAP_MAX_X_COUNT) && (curX - 1 >= 0) && (curY - 1 < MAP_MAX_Y_COUNT) && (curY - 1 >= 0)) { // ���� ��
			if (visited[curY][curX] <= 2) {
				visited[curY][curX]++;
				visited[curY - 1][curX - 1]++;
				BurstBubbleCheck(map, Bubble, curBubble, curY - 1, curX - 1, curY, curX);
				count++;
			}
		}
	}

	return count;
}

// ������ ���� ������ ��� ����
void BurstBubble(int(*map)[MAP_MAX_X_COUNT], int(*visited)[MAP_MAX_X_COUNT], int burst) { // �ʿ��� �Ű�����: ��, �湮�� ��, ī��Ʈ ����
	if (burst >= 2) { // 3 �̻��� ���
		for (int i = 0; i < MAP_MAX_Y_COUNT; i++) {
			for (int j = 0; j < MAP_MAX_X_COUNT; j++) {
				if (visited[i][j] > 0) {
					map[i][j] = 0;
				}
				visited[i][j] = 0;
			}
		}
		for (int i = 0; i < burst; i++) {
			score += 10;
		}
	}
	else {
		for (int i = 0; i < MAP_MAX_Y_COUNT; i++) {
			for (int j = 0; j < MAP_MAX_X_COUNT; j++) {
				visited[i][j] = 0;
			}
		}
	}
	count = 0;
}

// 'q'Ű�� ������ �� �� ��忡 ���� ����
int Pause(char key, int *map, int mode) {
	if (key == 'q') { // Pasue Save ����
		return 1;
	}
	else if (key == 'w') { // Pause Load ����
		return 2;
	}
	else if (key == 'a') { // Pause Save ����
		FILE* fp = fopen("GameMap.txt", "w"); // �� ������ ���� ���� (���� ����)
		FILE* sc = fopen("score.txt", "w"); // ���� ������ ���� ���� (���� ����)
		
		if (!strcmp(images[6 + GAME_MAP_END].fileName, "PauseSave.bmp")) { // PauseSave�� ���
			for (int i = 0; i < (MAP_MAX_Y_COUNT + 1) * MAP_MAX_X_COUNT; i++) {
				fprintf(fp, "%d ", map[i]); // �� ����
			}

			fclose(fp);

			fprintf(sc, "%d", score); // ���� ����
			fclose(sc);
			
			return 3; // �Ϸ� ǥ�� ����
		}
		return 2; // Pause Load ���� (���� ���)
	}
	else if (key == 's') { // Pause Load ����
		FILE* fp = fopen("GameMap.txt", "r"); // �� ������ ���� ���� (�б� ����)
		FILE* sc = fopen("score.txt", "r"); // ���� ������ ���� ���� (�б� ����)
		
		if (!strcmp(images[6 + GAME_MAP_END].fileName, "PauseLoad.bmp")) { // PauseLoad�� ���
			for (int i = 0; i < (MAP_MAX_Y_COUNT + 1) * MAP_MAX_X_COUNT; i++) {
				fscanf(fp, "%d ", &map[i]); // �� �ҷ�����
			}

			fclose(fp);
			
			fscanf(sc, "%d", &score); // ���� �ҷ�����
			fclose(sc);
			
			return 4;
		}
		return 1; // Pause Save ���� (���� ���)
	}
	else if (key == 'e') { // ����
		return 0;
	}
	else {
		return mode;
	}
}

// ���� ��� ȭ�� �� GameMap ���
int GameLayoutImage(int (*map)[MAP_MAX_X_COUNT], char(*Bubble)[20], int angle, int *shoot, int *mode, int pause) { // �ʿ��� �Ű�����: ���Ӹ�, ���� �̹��� �̸�, ����, �� ����, ������
	Sleep(1);
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer);
	
	// GameMap�� ������ images �迭 �ȿ� ����
	int i = 3; // GameMap������ ���� images �迭�� ��������
	int j = 0; // GameMap�� y��ǥ
	int k = 0; // GameMap�� x��ǥ
	while (i < 3 + MAP_MAX_X_COUNT * MAP_MAX_Y_COUNT) { // GameMap�� ��� ������ ����
		if (k % MAP_MAX_X_COUNT == 0 && k != 0) { // �� �� ������ ����
			k = 0; // x��ǥ�� 0���� �����ϰ�
			j++; // y��ǥ�� 1 ����
		}

		// images �迭�� GameMap�� �� ���� ����
		images[i].fileName = Bubble[map[j][k]];
		images[i].x = MAP_START_X + k % MAP_MAX_X_COUNT * MAP_MARGIN;
		images[i].y = MAP_START_Y + j % MAP_MAX_Y_COUNT * MAP_MARGIN;
		images[i].scale = MAIN_BUBBLE_SIZE;

		i++; // images �迭�� ���� �ε����� �̵�
		k++; // x��ǥ�� 1 ����
	}

	// ���� ���� ����
	images[1].fileName = Bubble[map[MAP_MAX_Y_COUNT][0]];

	// ���� ���� ����
	images[2].fileName = Bubble[map[MAP_MAX_Y_COUNT][1]];

	// �߻��ϴ� ���� ����
	images[GAME_MAP_END + 5].fileName = "NULL";
	images[GAME_MAP_END + 5].x = CURRENT_X;
	images[GAME_MAP_END + 5].y = CURRENT_Y;
	images[GAME_MAP_END + 5].scale = MAIN_BUBBLE_SIZE;

	imageLayer.transparentColor = RGB(0, 255, 0); // �ʷϻ��� ��������� ����

	imageLayer.imageCount = 300;
	imageLayer.images = images;

	// ��ǥ�� ����
	double currentX = CURRENT_X;
	double currentY = CURRENT_Y;
	double tempX = currentX;
	double tempY = currentY;
	int first = 1; // ó�� �߻��ϴ� �������� Ȯ���ϴ� ������ �⺻������ 1�� ����

	// 'q'Ű ������ ���� ��� �ʱ�ȭ
	images[6 + GAME_MAP_END].fileName = "NULL";
	images[6 + GAME_MAP_END].x = 400;
	images[6 + GAME_MAP_END].y = 400;
	images[6 + GAME_MAP_END].scale = 0.5;

	if (*shoot == 1) { // �߻����� ��
		images[GAME_MAP_END + 5].fileName = Bubble[map[MAP_MAX_Y_COUNT][1]]; // �߻��ϴ� ���� �̹��� �ҷ�����
		// �ʿ���
		map[MAP_MAX_Y_COUNT][1] = map[MAP_MAX_Y_COUNT][0]; // ���� ������ ���� ����� ����
		map[MAP_MAX_Y_COUNT][0] = map[MAP_MAX_Y_COUNT][2]; // �ٴ��� ������ ���� ����� ����
		map[MAP_MAX_Y_COUNT][2] = rand() % 6 + 1; // �ٴ��� ������ �������� ����

		// �̹��� ���̾��
		images[1].fileName = Bubble[map[MAP_MAX_Y_COUNT][0]]; // ���� ����� ����
		images[2].fileName = Bubble[map[MAP_MAX_Y_COUNT][1]]; // ���� ����� ����

		switch (angle) // ������ ����
		{
		case 1: // 30��
			while (1) {
				for (int i = 0; ; i += 10) { // �������� ���� ���
					if (first && CheckBound(currentX - i * 0.01, CURRENT_Y - (i * sin(PI * 30 / 180) * 0.01)) && !CheckBubble(Bubble)) { // ù��° �����ϰ�, ���� �ε����� �ʾҰ�, ������ ��ġ�� �ʾ��� ��
						// ù��° üũ�� ���� ����: ù��°�� �߾ӿ��� ����������, ���ķδ� �� ���̵� ������ �����ϱ� ����
						currentX = CURRENT_X - i; // �������� 1��ŭ �̵��� ��
						currentY = CURRENT_Y - i * sin(PI * 30 / 180); // �������� sin30�� ��ŭ �̵��� ��ǥ�� ����

						images[GAME_MAP_END + 5].x = currentX; // ������ ������ ��ǥ������ ����
						images[GAME_MAP_END + 5].y = currentY;
						imageLayer.renderAll(&imageLayer); // �̹��� ������
						sprintf(s, "%d ��", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "�����ٸ����", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else if (CheckBound(tempX - i * 0.01, tempY - (i * sin(PI * 30 / 180) * 0.01)) && !CheckBubble(Bubble)) { // ���� �ε����� �ʾҰ�, ������ ��ġ�� �ʾ��� �� (2��° ���� ����)
						tempX = currentX - i; // �������� 1��ŭ �̵��� ��
						tempY = currentY - i * sin(PI * 30 / 180); // �������� sin30�� ��ŭ �̵��� ��ǥ�� ����

						images[GAME_MAP_END + 5].x = tempX; // ������ ������ ��ǥ������ ����
						images[GAME_MAP_END + 5].y = tempY;

						imageLayer.renderAll(&imageLayer); // �̹��� ������
						sprintf(s, "%d ��", score); // score ������ ���ڿ��� �ٲ��ְ�
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "�����ٸ����", 100, 0, 0, RGB(255, 255, 255), 1, s); // ȭ�鿡 ���
					}
					else { // ���� �ε����ų�, ������ ������ ��
						if (first) { // ù��° �����̶��
							tempX = currentX; // ���� ��ǥ���� temp�� ����
							tempY = currentY;
						}
						else { // ù��° ������ �ƴ϶��
							currentX = tempX; // temp�� ����� ��ǥ���� currentX�� ����
							currentY = tempY;
						}
						break; // �ݺ��� Ż��
					}
				}
				for (int i = 10; ; i += 10) { // ���������� ���� ���
					if (CheckBound(tempX + i * 0.01, tempY - i * (sin(PI * 150 / 180) * 0.01)) && !CheckBubble(Bubble)) { // ���� �ε����� �ʾҰ�, ������ ��ġ�� �ʾ��� ��
						tempX = currentX + i; // ���������� 1��ŭ �̵��� ��
						tempY = currentY - i * sin(PI * 150 / 180); // �������� sin150�� ��ŭ �̵��� ��ǥ�� ����

						images[GAME_MAP_END + 5].x = tempX; // ������ ������ ��ǥ������ ����
						images[GAME_MAP_END + 5].y = tempY;

						imageLayer.renderAll(&imageLayer); // �̹��� ������
						sprintf(s, "%d ��", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "�����ٸ����", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else { // ���� �ε����ų�, ������ ������ ��
						currentX = tempX; // temp�� ����� ��ǥ���� currentX�� ����
						currentY = tempY;
						first = 0; // sin30���� �̵��ϰ� �Ǹ� �밢�� ���� ���� �̵��ϰ� �Ǵµ�, ���� �κ��� ���������� �̵��ϴ� ����̱� ������ ù��° ������ �ƴ�. ����, first�� 0���� �ٲ���
						break; // �ݺ��� Ż��
					}
				}
				if (!CheckBound(tempX + i * 0.01, tempY - i * (sin(PI * 150 / 180) * 0.01)) || CheckBubble(Bubble)) { // ���������� ���� ���� �ε����� �������� (while�� 1��° for��) �̵��ؾ� �ϰ� 
					if (CheckBubble(Bubble)) { // ������ ��ġ�� ������ ��ġ�� ���� ����� �迭�� ������ �ش�
						SaveMap(Bubble, map, images[GAME_MAP_END + 5].fileName);
						int burst = BurstBubbleCheck(map, Bubble, images[GAME_MAP_END + 5].fileName, currentBubblePosition[0], currentBubblePosition[1], 100, 100);
						BurstBubble(map, visited, burst);
					}
					break;
				}
			}
			break;
		// 60��
		case 2: // ������ ������ ������ case 1�� �����ϱ� ������ �ڼ��� ������ ������
			while (1) {
				for (int i = 0; ; i += 10) { // �������� �̵��ϴ� ���
					if (first && CheckBound(currentX - i * 0.01, CURRENT_Y - (i * sin(PI * 60 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						currentX = CURRENT_X - i;
						currentY = CURRENT_Y - i * sin(PI * 60 / 180); // �������� sin60�� ��ŭ �̵�

						images[GAME_MAP_END + 5].x = currentX;
						images[GAME_MAP_END + 5].y = currentY;
						imageLayer.renderAll(&imageLayer); // �̹��� ������
						sprintf(s, "%d ��", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "�����ٸ����", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else if (CheckBound(tempX - i * 0.01, tempY - (i * sin(PI * 60 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						tempX = currentX - i;
						tempY = currentY - i * sin(PI * 60 / 180);

						images[GAME_MAP_END + 5].x = tempX;
						images[GAME_MAP_END + 5].y = tempY;

						imageLayer.renderAll(&imageLayer); // �̹��� ������
						sprintf(s, "%d ��", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "�����ٸ����", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else {
						if (first) {
							tempX = currentX;
							tempY = currentY;
						}
						else {
							currentX = tempX;
							currentY = tempY;
						}
						break;
					}
				}
				for (int i = 10; ; i += 10) { // ���������� �̵��ϴ� ���
					if (CheckBound(tempX + i * 0.01, tempY - i * (sin(PI * 120 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						tempX = currentX + i;
						tempY = currentY - i * sin(PI * 120 / 180); // �������� sin120�� ��ŭ �̵�

						images[GAME_MAP_END + 5].x = tempX;
						images[GAME_MAP_END + 5].y = tempY;

						imageLayer.renderAll(&imageLayer); // �̹��� ������
						sprintf(s, "%d ��", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "�����ٸ����", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else {
						currentX = tempX;
						currentY = tempY;
						first = 0;
						break;
					}
				}
				if (!CheckBound(tempX + i * 0.01, tempY - i * (sin(PI * 120 / 180) * 0.01)) || CheckBubble(Bubble)) { // ���� �ε����ų� ���� ��ġ�� ����
					if (CheckBubble(Bubble)) {
						SaveMap(Bubble, map, images[GAME_MAP_END + 5].fileName);
						int burst = BurstBubbleCheck(map, Bubble, images[GAME_MAP_END + 5].fileName, currentBubblePosition[0], currentBubblePosition[1], 100, 100);
						BurstBubble(map, visited, burst);
						
					}
					break;
				}
			}
			break;
		// 90��
		case 3:
			for (int i = 0; ; i += 10) {
				currentX = CURRENT_X; // x��ǥ�� �״��
				currentY = CURRENT_Y - i; // y��ǥ�� ���� �̵�
				if (CheckBound(currentX, currentY) && !CheckBubble(Bubble)) { // ���� �ε����� �ʰ� ���� ��ġ�� ������
					images[GAME_MAP_END + 5].x = currentX;
					images[GAME_MAP_END + 5].y = currentY;
				}

				if (!CheckBound(currentX, currentY - i * 0.01) || CheckBubble(Bubble)) { // ���� �ε����ų� ���� �ε����� ����
					if (CheckBubble(Bubble)) {
						SaveMap(Bubble, map, images[GAME_MAP_END + 5].fileName);
						int burst = BurstBubbleCheck(map, Bubble, images[GAME_MAP_END + 5].fileName, currentBubblePosition[0], currentBubblePosition[1], 100, 100);
						BurstBubble(map, visited, burst);
					}
					break;
				}
				imageLayer.renderAll(&imageLayer); // �̹��� ������
				sprintf(s, "%d ��", score);
				printTextWithAngle(imageLayer._consoleDC, 1150, 310, "�����ٸ����", 100, 0, 0, RGB(255, 255, 255), 1, s);
			}
			break;
		// 120��
		case 4: // ������ ������ ������ case 1�� �����ϱ� ������ �ڼ��� ������ ������
			while (1) {
				for (int i = 0; ; i += 10) { // ���������� �̵��ϴ� ���
					if (first && CheckBound(currentX + i * 0.01, CURRENT_Y - (i * sin(PI * 120 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						currentX = CURRENT_X + i; // ���������� �̵��ϱ� ������ x��ǥ ����
						currentY = CURRENT_Y - i * sin(PI * 120 / 180); // �������� sin120�� ��ŭ �̵�

						images[GAME_MAP_END + 5].x = currentX;
						images[GAME_MAP_END + 5].y = currentY;
						imageLayer.renderAll(&imageLayer); // �̹��� ������
						sprintf(s, "%d ��", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "�����ٸ����", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else if (CheckBound(tempX + i * 0.01, tempY - (i * sin(PI * 120 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						tempX = currentX + i;
						tempY = currentY - i * sin(PI * 120 / 180);

						images[GAME_MAP_END + 5].x = tempX;
						images[GAME_MAP_END + 5].y = tempY;

						imageLayer.renderAll(&imageLayer); // �̹��� ������
						sprintf(s, "%d ��", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "�����ٸ����", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else {
						if (first) {
							tempX = currentX;
							tempY = currentY;
						}
						else {
							currentX = tempX;
							currentY = tempY;
						}
						break;
					}
				}
				for (int i = 10; ; i += 10) { // �������� �̵��ϴ� ���
					if (CheckBound(tempX - i * 0.01, tempY - i * (sin(PI * 60 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						tempX = currentX - i; // �������� �̵��ϱ� ������ x��ǥ ����
						tempY = currentY - i * sin(PI * 60 / 180); // �������� sin60�� ��ŭ �̵�

						images[GAME_MAP_END + 5].x = tempX;
						images[GAME_MAP_END + 5].y = tempY;

						imageLayer.renderAll(&imageLayer); // �̹��� ������
						sprintf(s, "%d ��", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "�����ٸ����", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else {
						currentX = tempX;
						currentY = tempY;
						first = 0;
						break;
					}
				}
				if (!CheckBound(tempX + i * 0.01, tempY - i * (sin(PI * 60 / 180) * 0.01)) || CheckBubble(Bubble)) { // ���� �ε����ų� ���� �ε����� ����
					if (CheckBubble(Bubble)) {
						SaveMap(Bubble, map, images[GAME_MAP_END + 5].fileName);
						int burst = BurstBubbleCheck(map, Bubble, images[GAME_MAP_END + 5].fileName, currentBubblePosition[0], currentBubblePosition[1], 100, 100);
						BurstBubble(map, visited, burst);
					}
					break;
				}
			}
			break;
		// 150��
		case 5: // ������ ������ ������ case 1�� �����ϱ� ������ �ڼ��� ������ ������
			while (1) {
				for (int i = 0; ; i += 10) { // ���������� �̵��ϴ� ���
					if (first && CheckBound(currentX + i * 0.01, CURRENT_Y - (i * sin(PI * 150 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						currentX = CURRENT_X + i;
						currentY = CURRENT_Y - i * sin(PI * 150 / 180); // �������� sin150�� ��ŭ �̵�

						images[GAME_MAP_END + 5].x = currentX;
						images[GAME_MAP_END + 5].y = currentY;
						imageLayer.renderAll(&imageLayer); // �̹��� ������
						sprintf(s, "%d ��", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "�����ٸ����", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else if (CheckBound(tempX + i * 0.01, tempY - (i * sin(PI * 150 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						tempX = currentX + i;
						tempY = currentY - i * sin(PI * 150 / 180);

						images[GAME_MAP_END + 5].x = tempX;
						images[GAME_MAP_END + 5].y = tempY;
						imageLayer.renderAll(&imageLayer); // �̹��� ������
						sprintf(s, "%d ��", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "�����ٸ����", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else {
						if (first) {
							tempX = currentX;
							tempY = currentY;
						}
						else {
							currentX = tempX;
							currentY = tempY;
						}
						break;
					}
				}
				for (int i = 10; ; i += 10) { // �������� �̵��ϴ� ���
					if (CheckBound(tempX - i * 0.01, tempY - i * (sin(PI * 30 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						tempX = currentX - i;
						tempY = currentY - i * sin(PI * 30 / 180); // �������� sin30�� ��ŭ �̵�

						images[GAME_MAP_END + 5].x = tempX;
						images[GAME_MAP_END + 5].y = tempY;

						imageLayer.renderAll(&imageLayer); // �̹��� ������
						sprintf(s, "%d ��", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "�����ٸ����", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else {
						currentX = tempX;
						currentY = tempY;
						first = 0;
						break;
					}
				}
				if (!CheckBound(tempX + i * 0.01, tempY - i * (sin(PI * 30 / 180) * 0.01)) || CheckBubble(Bubble)) { // ���� �ε����ų� ���� �ε����� ����
					if (CheckBubble(Bubble)) {
						SaveMap(Bubble, map, images[GAME_MAP_END + 5].fileName);
						int burst = BurstBubbleCheck(map, Bubble, images[GAME_MAP_END + 5].fileName, currentBubblePosition[0], currentBubblePosition[1], 100, 100);
						BurstBubble(map, visited, burst);
					}
					break;
				}
			}
			break;
		}
		*shoot = 0; // ���� ���� 0���� �ٲ���
		images[GAME_MAP_END + 5].fileName = "NULL"; // �߻��ߴ� ���� ������
	}
	else { // Pause ó��
		if (pause == 0) {
			images[6 + GAME_MAP_END].fileName = "NULL";
		}
		else if (pause == 1) {
			images[6 + GAME_MAP_END].fileName = "PauseSave.bmp";
		}
		else if (pause == 2) {
			images[6 + GAME_MAP_END].fileName = "PauseLoad.bmp";
		}
		else if (pause == 3) {
			images[6 + GAME_MAP_END].fileName = "PauseSaveComplete.bmp";
		}
		else if (pause == 4) {
			images[6 + GAME_MAP_END].fileName = "PauseLoadComplete.bmp";
		}
		
		imageLayer.renderAll(&imageLayer); // �̹��� ������
		sprintf(s, "%d ��", score);
		printTextWithAngle(imageLayer._consoleDC, 1150, 310, "�����ٸ����", 100, 0, 0 , RGB(255, 255, 255), 1, s);
	}
	return score;
}

// ���ο� ���� �����ϴ� �Լ�
void MakeNewLine(int(*map)[MAP_MAX_X_COUNT], char(*Bubble)[20], int start, int last) { // �ʿ��� �Ű�����: ���Ӹ�, ���� �̹��� �̸�, ������, ����
	int temp[MAP_MAX_Y_COUNT][MAP_MAX_X_COUNT] = { 0 }; // ���ο� ������ �����ϱ� ���� �ӽ÷� ������ �迭
	if (start == -1) { // �������� -1�̸� �� ���� ������ �����ϰ� �Ʒ� �ִ� ���ε��� 1ĭ�� ������ ����
		for (int i = 0; i < MAP_MAX_Y_COUNT - 1; i++) {
			for (int j = 0; j < MAP_MAX_X_COUNT; j++) {
				temp[i + 1][j] = map[i][j];
			}
		}
		for (int i = 0; i < MAP_MAX_X_COUNT; i++) { // �� ���� ������ ����
			temp[0][i] = rand() % 6 + 1;
		}
		for (int i = 0; i < MAP_MAX_Y_COUNT; i++) { // �ʿ� ���ο� ������ ����
			for (int j = 0; j < MAP_MAX_X_COUNT; j++) {
				map[i][j] = temp[i][j];
			}
		}
	}

	for (int i = start; i < last; i++) { // �����ٺ��� ���ٱ���
		for (int j = 0; j < MAP_MAX_X_COUNT; j++) { // �� �ٸ���
			map[i][j] = rand() % 6 + 1; // �������� �̾Ƽ� ����
		}
	}
}

// �Ŀ� ���� �� �ʿ��� �Լ�
int level = 1;
int mod = 0;

// ���� �����ϴ� �Լ�
int SetLevel(char(*Level)[20], char key) { // �ʿ��� �Ű�����: ���� �̹��� �̸�, �Է� ��
	// �Ŀ� �̹��� ����
	images[3 + GAME_MAP_END].fileName = Level[level-1];
	images[3 + GAME_MAP_END].x = LEVEL_X;
	images[3 + GAME_MAP_END].y = LEVEL_Y;
	images[3 + GAME_MAP_END].scale = LEVEL_SIZE;
	
	if (key == 0) { // UPŰ�� ������ ��
		if (mod == 0) { // Ȧ����(ó�� ����) �����ٸ�
			mod = 1; // â Ű��, �Ŀ� ���� ������ ���� ��ȯ
		}
		else if (mod == 1) { // ¦���� �����ٸ�
			mod = 0; // â ����, ���� ��ȯ
			images[3 + GAME_MAP_END].fileName = "NULL";
			return level;
		}
	}

	// ���� ���� ���
	if (key == '=') { // + Ű�� ������ ���� 1 ����
		++level;
		if (level > 6) { // ������ 6�� �Ѿ�� 1���� �ٲ�
			level = 1;
		}
	}
	else if (key == '-') { // -Ű�� ������ ���� 1 ����
		--level;
		if (level < 1) {  // ������ 0���ϸ� 6���� �ٲ�
			level = 6;
		}
	}
		
	images[3 + GAME_MAP_END].fileName = Level[level-1]; // ���� ���� ���
}

// ���� �����ϱ� ���� �ʿ��� ����
int arrow = 1;

// ���� �����ϴ� �Լ�
int SetAngle(char(*Arrow)[20], char key) { // �ʿ��� �Ű�����: ȭ��ǥ �̹��� �̸�, �Է� ��
	// ȭ��ǥ(����) �̹��� ����
	images[4 + GAME_MAP_END].fileName = Arrow[arrow - 1];
	images[4 + GAME_MAP_END].x = ARROW_X;
	images[4 + GAME_MAP_END].y = ARROW_Y;
	images[4 + GAME_MAP_END].scale = ARROW_SIZE;

	if (key == RIGHT) { // ������ Ű�� ������ ��
		++arrow; // +30�� ����
		if (arrow > 5) { // 150�� �Ѿ�� 30���� ����
			arrow = 1;
		}
	}
	else if (key == LEFT) { // ���� Ű�� ������ ��
		--arrow; // -30�� ����
		if (arrow < 1) { // 30�� �̸��̸� 150���� �ٲ�
			arrow = 5;
		}
	}

	images[4 + GAME_MAP_END].fileName = Arrow[arrow - 1]; // ȭ��ǥ ���
	return arrow; // ���� ��ȯ
}

void InitScore() { // ���� �ʱ�ȭ
	score = 0;
}