#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib,"winmm.lib")

#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "./headers/music.h"
#include "./headers/define.h"
#include "./headers/init.h"
#include "./headers/FirstScreen.h"
#include "./headers/GameScreen.h"
#include "./headers/GameOver.h"

int main() {
	// 1. �ʱ�ȭ �� ����
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer);
	ClearCursor(); // Ŀ�� ���ֱ�
	ReSizeConsole(MAX_X, MAX_Y); // â ũ�� ����
	system("title BubbleShooter"); // �ܼ�â �̸� ����

	// 2. ���� ��ũ��
	StartScreenStartImage(); // ���� ���� ȭ�� ����
	PlaySound(TEXT("./sounds/BubbleShooter_StartScreen.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // ���� ���� ȭ�� ���� ���
	int mod = 1; // Game Start / Exit �� � ���� �����ߴ��� Ȯ���ϴ� ����
	int count = 0; // �����Ÿ��� ���� ����ϴ� ���� (�ð� ī��Ʈ)
	int space = 0; // �����Ÿ��� ���� ����ϴ� ���� (Space ������ ��� ������ �ƴ��� Ȯ���ϴ� ����)
	int game = 0; // ���� ������ �������� Ȯ���ϴ� ����
	while (1) {
		if (_kbhit()) {
			char key = _getch(); // Ű �Է� �ޱ�
			switch (key) {
			case UP: // Game Start
				mod = 1; // ��带 1(Game Start)�� �ٲٱ�
				if (space == 0) { // Space ���� X
					StartScreenStartImage(); // Game Start ���õ� ȭ�� ���� (Space X)
					space = 1; // Space ���� O�� �ٲٱ�
				}
				else { // Space ���� O
					StartScreenStartSpaceImage(); // Game Start ���õ� ȭ�� ���� (Space O)
					space = 0; // Space ���� X�� �ٲٱ�
				}
				break;
			case DOWN: // Exit
				mod = 0; // ��带 0(Exit)�� �ٲٱ�
				if (space == 0) { // Space ���� X
					StartScreenExitImage(); // Exit ���õ� ȭ�� ���� (Space X)
					space = 1; // Space ���� O�� �ٲٱ�
				}
				else { // Space ���� O
					StartScreenExitSpaceImage(); // Exit ���õ� ȭ�� ���� (Space O)
					space = 0; // Space ���� X�� �ٲٱ�
				}
				break;
			case SPACE: // ����
				if (mod == 1) { // Game Start�� �������� ��
					system("cls"); // �ܼ�â �����
					game = 1; // ���� ������ �����ٰ� �˷��ֱ�
					break;
				}
				else if (mod == 0) { // Exit�� �������� ��
					return 0; // ����
				}
			}
		}

		// "Space Ű�� ���� �����ϼ���" �κ� �����Ÿ��� ����
		if (count % 100 == 0) {
			if (mod == 1) { // Game Start����
				if (space == 0) { // Space ���� X
					StartScreenStartImage(); // Game Start ���õ� ȭ�� ���� (Space X)
					space = 1; // Space ���� O�� �ٲٱ�
				}
				else { // Space ���� X
					StartScreenStartSpaceImage(); // Game Start ���õ� ȭ�� ���� (Space O)
					space = 0; // Space ���� X�� �ٲٱ�
				}
			}
			else if (mod == 0) { // Exit����
				if (space == 0) { // Space ���� X
					StartScreenExitImage(); // Exit ���õ� ȭ�� ���� (Space X)
					space = 1; // Space ���� O�� �ٲٱ�
				}
				else { // Space ���� X
					StartScreenExitSpaceImage(); // Exit ���õ� ȭ�� ���� (Space O)
					space = 0; // Space ���� X�� �ٲٱ�
				}
			}
		}
		
		if (game == 1) { // ���� ������ ������ ��
			break; // while�� Ż��
		}
		
		// 10ms ���� count 1 ����
		count++;
		Sleep(10);
	}

	// 3. ���� ����
	PlaySound(NULL, NULL, NULL); // ���� ��ũ������ ����ǰ� �ִ� ���� ����
	PlaySound(TEXT("./sounds/BubbleShooter_Game.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // ���� ȭ�� ���� ���
	
	char Bubble[7][29] = {
		"None", // GameMap�� �ʱⰪ�� 0���� �����߱� ������, ������ 0��°�� �ƹ��͵� ��� X
		"./images/Bubble_Blue.bmp",
		"./images/Bubble_Green.bmp",
		"./images/Bubble_Orange.bmp",
		"./images/Bubble_Purple.bmp",
		"./images/Bubble_Red.bmp",
		"./images/Bubble_Yellow.bmp"
	};
	char Level[6][29] = { // �����Ը� ��Ÿ���� �̹���
		"./images/Level1.bmp",
		"./images/Level2.bmp",
		"./images/Level3.bmp",
		"./images/Level4.bmp",
		"./images/Level5.bmp",
		"./images/Level6.bmp"
	};
	char Arrow[5][29] = { // ����(ȭ��ǥ)�� ��Ÿ���� �̹���
		"./images/Arrow1.bmp",
		"./images/Arrow2.bmp",
		"./images/Arrow3.bmp",
		"./images/Arrow4.bmp",
		"./images/Arrow5.bmp"
	};
	
	count = 0; // count �ʱ�ȭ
	int pause = 0; // 'q'Ű ������ ���� ���� ������ ��� �ʱ�ȭ
	int level = 0; // �������� �ӵ� �ʱ�ȭ
	int GameMap[MAP_MAX_Y_COUNT + 1][MAP_MAX_X_COUNT] = { 0 }; // ���Ӹ�(���ڷ�) ����
	
	srand(time(NULL)); // ���� ���
	GameMap[MAP_MAX_Y_COUNT][0] = rand() % 6 + 1; // ���� ������ �������� ����
	GameMap[MAP_MAX_Y_COUNT][1] = rand() % 6 + 1; // ���� ������ �������� ����
	GameMap[MAP_MAX_Y_COUNT][2] = rand() % 6 + 1; // �ٴ��� ������ �������� ����
	
	MakeNewLine(GameMap, Bubble, 0, 3); // �ʱ� ����(3��) ����
	int angle = SetAngle(Arrow, NULL); // ȭ��ǥ ����
	int shoot = 0; // �߻� ����
	int makeLine = 0; // ���ο� ���� ������ �Ǵ��ϴ� ����

	int score = GameLayoutImage(GameMap, Bubble, NULL, &shoot, &mod, pause); // ȭ�� ���
	int gameover = 1;
	
 	while (1) {
		if (count % ((8 - level) * 50) == 0 && count != 0) { // ���� �ð�����
			for (int i = 0; i < MAP_MAX_X_COUNT; i++) {
				if (GameMap[MAP_MAX_Y_COUNT - 2][i] != 0) { // �� �ؿ� ������ �ִٸ�
					makeLine = 1;
				}
			}
			if (makeLine == 0) {
				MakeNewLine(GameMap, Bubble, -1, NULL); // ���ο� ���� ����	
			}
			else {
				makeLine = 0;
			}
		}
		
		if (_kbhit()) {
			char key = _getch(); // Ű �Է� �ޱ�
			if (!pause) { // Pause�� �ƴ� ����
				switch (key) {
					case LEFT: // ȭ��ǥ ���� (��������)
					case RIGHT:  // ȭ��ǥ ���� (����������)
						angle = SetAngle(Arrow, key);
						break;
					case DOWN: // ������ ���
						break;
					case UP: // �Ŀ� ���� â ����
						level = SetLevel(Level, 0);
						break;
					case '=': // ���̵� ���� (+)
					case '-': // ���̵� ���� (-)
						level = SetLevel(Level, key);
						break;
					case SPACE:  // ���� ����
						playSound("shoot.wav");
						shoot = 1;
						break;
					case 'q': // Pasue Save ����
					case 'w': // Pasue Load ����
						pause = Pause(key, GameMap, pause); // Pause ����
				}
			}
			else { // Pause�̸�
				switch(key){
					case 'q': // Pasue Save ����
					case 'w': // Pasue Load ����
					case 'a': // Pause Save ����
					case 's': // Pause Load ����
					case 'e':
						pause = Pause(key, GameMap, pause); // Pause ����
				}
			}
		}

		for (int l = 0; l < MAP_MAX_X_COUNT; l++) {
			if (GameMap[MAP_MAX_Y_COUNT - 1][l] != 0) { // �� �̻� �� �� �ִ� ������ ���� ���
				Sleep(30);
				PlaySound(NULL, NULL, NULL); // ���� ����
				system("cls"); // ȭ�� �����
				GameOver(score, 1); // ���� ���� ���
				while (1) {
					if (gameover == -1) { // ���Ḧ �����ߴٸ�
						gameover = 1;
						PlaySound(TEXT("./sounds/BubbleShooter_Game.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // ���� ȭ�� ���� ���
						break; // �ٽ� ����
					}
					if (_kbhit()) {
						char key = _getch();
						switch (key) {
						case UP: // Retry ����
							gameover = 1;
							break;
						case DOWN: // Exit ����
							gameover = 2;
							break;
						case SPACE:
							if (gameover == 1) { // Retry�� ���õ�����
								gameover = -1;
								// ���� ���� �� �ʿ��� ������ �ʱ�ȭ
								count = 0;
								pause = 0;
								level = 0;
								for (int i = 0; i < MAP_MAX_Y_COUNT; i++) {
									for (int j = 0; j < MAP_MAX_X_COUNT; j++) {
										GameMap[i][j] = 0;
									}
								}
								GameMap[MAP_MAX_Y_COUNT][0] = rand() % 6 + 1;
								GameMap[MAP_MAX_Y_COUNT][1] = rand() % 6 + 1;
								GameMap[MAP_MAX_Y_COUNT][2] = rand() % 6 + 1;

								MakeNewLine(GameMap, Bubble, 0, 3);
								angle = SetAngle(Arrow, NULL);
								shoot = 0;
								makeLine = 0;
								InitScore();
								break; // �ٽ� üũ�� ��
							}
							else if (gameover == 2) { // Exit�� ���õ�����
								exit(0); // �����ϱ�
							}
						}
					}
					GameOver(score, gameover); // ���� ���� ���
					Sleep(10);
				}
			}
		}
		
		score = GameLayoutImage(GameMap, Bubble, angle, &shoot, &mod, pause); // ���� ȭ�� ����
		
		// 10ms���� count 1 ����
		count++;
		Sleep(10);
	}
	
	return 0;
}