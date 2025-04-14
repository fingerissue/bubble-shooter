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
	// 1. 초기화 및 설정
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer);
	ClearCursor(); // 커서 없애기
	ReSizeConsole(MAX_X, MAX_Y); // 창 크기 변경
	system("title BubbleShooter"); // 콘솔창 이름 변경

	// 2. 메인 스크린
	StartScreenStartImage(); // 게임 시작 화면 띄우기
	PlaySound(TEXT("./sounds/BubbleShooter_StartScreen.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // 게임 시작 화면 음악 재생
	int mod = 1; // Game Start / Exit 중 어떤 것을 선택했는지 확인하는 변수
	int count = 0; // 깜빡거리기 위해 사용하는 변수 (시간 카운트)
	int space = 0; // 깜빡거리기 위해 사용하는 변수 (Space 설명을 띄울 것인지 아닌지 확인하는 변수)
	int game = 0; // 게임 시작을 눌렀는지 확인하는 변수
	while (1) {
		if (_kbhit()) {
			char key = _getch(); // 키 입력 받기
			switch (key) {
			case UP: // Game Start
				mod = 1; // 모드를 1(Game Start)로 바꾸기
				if (space == 0) { // Space 설명 X
					StartScreenStartImage(); // Game Start 선택된 화면 띄우기 (Space X)
					space = 1; // Space 설명 O로 바꾸기
				}
				else { // Space 설명 O
					StartScreenStartSpaceImage(); // Game Start 선택된 화면 띄우기 (Space O)
					space = 0; // Space 설명 X로 바꾸기
				}
				break;
			case DOWN: // Exit
				mod = 0; // 모드를 0(Exit)로 바꾸기
				if (space == 0) { // Space 설명 X
					StartScreenExitImage(); // Exit 선택된 화면 띄우기 (Space X)
					space = 1; // Space 설명 O로 바꾸기
				}
				else { // Space 설명 O
					StartScreenExitSpaceImage(); // Exit 선택된 화면 띄우기 (Space O)
					space = 0; // Space 설명 X로 바꾸기
				}
				break;
			case SPACE: // 선택
				if (mod == 1) { // Game Start를 선택했을 때
					system("cls"); // 콘솔창 지우기
					game = 1; // 게임 시작을 눌렀다고 알려주기
					break;
				}
				else if (mod == 0) { // Exit를 선택했을 때
					return 0; // 종료
				}
			}
		}

		// "Space 키를 눌러 선택하세요" 부분 깜빡거리기 구현
		if (count % 100 == 0) {
			if (mod == 1) { // Game Start에서
				if (space == 0) { // Space 설명 X
					StartScreenStartImage(); // Game Start 선택된 화면 띄우기 (Space X)
					space = 1; // Space 설명 O로 바꾸기
				}
				else { // Space 설명 X
					StartScreenStartSpaceImage(); // Game Start 선택된 화면 띄우기 (Space O)
					space = 0; // Space 설명 X로 바꾸기
				}
			}
			else if (mod == 0) { // Exit에서
				if (space == 0) { // Space 설명 X
					StartScreenExitImage(); // Exit 선택된 화면 띄우기 (Space X)
					space = 1; // Space 설명 O로 바꾸기
				}
				else { // Space 설명 X
					StartScreenExitSpaceImage(); // Exit 선택된 화면 띄우기 (Space O)
					space = 0; // Space 설명 X로 바꾸기
				}
			}
		}
		
		if (game == 1) { // 게임 시작을 눌렀을 때
			break; // while문 탈출
		}
		
		// 10ms 마다 count 1 증가
		count++;
		Sleep(10);
	}

	// 3. 게임 시작
	PlaySound(NULL, NULL, NULL); // 메인 스크린에서 재생되고 있던 음악 멈춤
	PlaySound(TEXT("./sounds/BubbleShooter_Game.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // 게임 화면 음악 재생
	
	char Bubble[7][29] = {
		"None", // GameMap의 초기값을 0으로 설정했기 때문에, 버블의 0번째는 아무것도 출력 X
		"./images/Bubble_Blue.bmp",
		"./images/Bubble_Green.bmp",
		"./images/Bubble_Orange.bmp",
		"./images/Bubble_Purple.bmp",
		"./images/Bubble_Red.bmp",
		"./images/Bubble_Yellow.bmp"
	};
	char Level[6][29] = { // 빠르게를 나타내는 이미지
		"./images/Level1.bmp",
		"./images/Level2.bmp",
		"./images/Level3.bmp",
		"./images/Level4.bmp",
		"./images/Level5.bmp",
		"./images/Level6.bmp"
	};
	char Arrow[5][29] = { // 각도(화살표)를 나타내는 이미지
		"./images/Arrow1.bmp",
		"./images/Arrow2.bmp",
		"./images/Arrow3.bmp",
		"./images/Arrow4.bmp",
		"./images/Arrow5.bmp"
	};
	
	count = 0; // count 초기화
	int pause = 0; // 'q'키 눌렀을 시의 선택 가능한 모드 초기화
	int level = 0; // 내려오는 속도 초기화
	int GameMap[MAP_MAX_Y_COUNT + 1][MAP_MAX_X_COUNT] = { 0 }; // 게임맵(숫자로) 설정
	
	srand(time(NULL)); // 랜덤 사용
	GameMap[MAP_MAX_Y_COUNT][0] = rand() % 6 + 1; // 다음 버블을 랜덤으로 설정
	GameMap[MAP_MAX_Y_COUNT][1] = rand() % 6 + 1; // 현재 버블을 랜덤으로 설정
	GameMap[MAP_MAX_Y_COUNT][2] = rand() % 6 + 1; // 다다음 버블을 랜덤으로 설정
	
	MakeNewLine(GameMap, Bubble, 0, 3); // 초기 라인(3줄) 생성
	int angle = SetAngle(Arrow, NULL); // 화살표 생성
	int shoot = 0; // 발사 여부
	int makeLine = 0; // 새로운 라인 만들지 판단하는 변수

	int score = GameLayoutImage(GameMap, Bubble, NULL, &shoot, &mod, pause); // 화면 출력
	int gameover = 1;
	
 	while (1) {
		if (count % ((8 - level) * 50) == 0 && count != 0) { // 일정 시간마다
			for (int i = 0; i < MAP_MAX_X_COUNT; i++) {
				if (GameMap[MAP_MAX_Y_COUNT - 2][i] != 0) { // 맨 밑에 라인이 있다면
					makeLine = 1;
				}
			}
			if (makeLine == 0) {
				MakeNewLine(GameMap, Bubble, -1, NULL); // 새로운 라인 생성	
			}
			else {
				makeLine = 0;
			}
		}
		
		if (_kbhit()) {
			char key = _getch(); // 키 입력 받기
			if (!pause) { // Pause가 아닐 동안
				switch (key) {
					case LEFT: // 화살표 조절 (왼쪽으로)
					case RIGHT:  // 화살표 조절 (오른쪽으로)
						angle = SetAngle(Arrow, key);
						break;
					case DOWN: // 아이템 사용
						break;
					case UP: // 파워 조절 창 띄우기
						level = SetLevel(Level, 0);
						break;
					case '=': // 난이도 조절 (+)
					case '-': // 난이도 조절 (-)
						level = SetLevel(Level, key);
						break;
					case SPACE:  // 버블 날림
						playSound("shoot.wav");
						shoot = 1;
						break;
					case 'q': // Pasue Save 띄우기
					case 'w': // Pasue Load 띄우기
						pause = Pause(key, GameMap, pause); // Pause 끄기
				}
			}
			else { // Pause이면
				switch(key){
					case 'q': // Pasue Save 띄우기
					case 'w': // Pasue Load 띄우기
					case 'a': // Pause Save 실행
					case 's': // Pause Load 실행
					case 'e':
						pause = Pause(key, GameMap, pause); // Pause 끄기
				}
			}
		}

		for (int l = 0; l < MAP_MAX_X_COUNT; l++) {
			if (GameMap[MAP_MAX_Y_COUNT - 1][l] != 0) { // 더 이상 쏠 수 있는 라인이 없을 경우
				Sleep(30);
				PlaySound(NULL, NULL, NULL); // 음악 멈춤
				system("cls"); // 화면 지우고
				GameOver(score, 1); // 게임 오버 출력
				while (1) {
					if (gameover == -1) { // 종료를 선택했다면
						gameover = 1;
						PlaySound(TEXT("./sounds/BubbleShooter_Game.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // 게임 화면 음악 재생
						break; // 다시 시작
					}
					if (_kbhit()) {
						char key = _getch();
						switch (key) {
						case UP: // Retry 선택
							gameover = 1;
							break;
						case DOWN: // Exit 선택
							gameover = 2;
							break;
						case SPACE:
							if (gameover == 1) { // Retry가 선택됐으면
								gameover = -1;
								// 게임 진행 시 필요한 변수들 초기화
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
								break; // 다시 체크한 후
							}
							else if (gameover == 2) { // Exit가 선택됐으면
								exit(0); // 종료하기
							}
						}
					}
					GameOver(score, gameover); // 게임 오버 출력
					Sleep(10);
				}
			}
		}
		
		score = GameLayoutImage(GameMap, Bubble, angle, &shoot, &mod, pause); // 게임 화면 띄우기
		
		// 10ms마다 count 1 증가
		count++;
		Sleep(10);
	}
	
	return 0;
}