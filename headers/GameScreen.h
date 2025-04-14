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

// 게임 배경 화면 가져오기
Image images[300] = {
	{"GameLayout.bmp", 0, 0, 1}, // 게임 맵 이미지
	{"NULL", NEXT_X, NEXT_Y, NEXT_BUBBLE_SIZE}, // 다음 버블 설정
	{"NULL", CURRENT_X, CURRENT_Y, MAIN_BUBBLE_SIZE} // 현재 버블 설정
};

// 현재 점수
int score = 0;


// 벽에 닿았는지 체크하는 함수
int CheckBound(double curX, double curY) { // 필요한 매개변수: 현재 버블의 x, y 좌표
	if ((curX >= MAP_START_X) && (curX <= MAP_START_X + (MAP_MAX_X_COUNT - 1) * MAP_MARGIN) && (curY >= MAP_START_Y) && (curY <= MAP_START_Y + MAP_MARGIN * (MAP_MAX_Y_COUNT + 1))) // 현재 버블의 x, y좌표가 벽에 닿으면
		return 1; // 1 반환
	else
		return 0; // 아니면 0 반환
}

// 버블이 겹치는지 체크하기 위해 필요한 함수 (버블의 중앙값)
int bubbleMidX = 0;
int bubbleMidY = 0;

// 버블이 겹치는지 체크하는 함수
int CheckBubble(char(*Bubble)[20]) { // 필요한 매개변수: 버블 이름
	for (int i = 0; i < MAP_MAX_Y_COUNT * MAP_MAX_X_COUNT; i++) { // 맵의 크기만큼
		if (
 			(images[3 + i].fileName != Bubble[0]) && // 버블이 없지 않고 (= 있고)
			(
			((images[GAME_MAP_END + 5].x >= images[3 + i].x - MAP_MARGIN) && (images[GAME_MAP_END + 5].x <= images[3 + i].x + MAP_MARGIN)) && // 좌우에 버블이 있고
			((images[GAME_MAP_END + 5].y >= images[3 + i].y - MAP_MARGIN) && (images[GAME_MAP_END + 5].y <= images[3 + i].y + MAP_MARGIN)) // 위아래에 버블이 있을 때
			)
				) {
			if ( // 다음에 있는 버블에도 있는지 체크
				(images[3 + i + 1].fileName != Bubble[0]) &&
				(((images[GAME_MAP_END + 5].x >= images[3 + i + 1].x - MAP_MARGIN) && (images[GAME_MAP_END + 5].x <= images[3 + i + 1].x + MAP_MARGIN)) &&
				((images[GAME_MAP_END + 5].y >= images[3 + i + 1].y - MAP_MARGIN) && (images[GAME_MAP_END + 5].y <= images[3 + i + 1].y + MAP_MARGIN)))
				){
				continue; // 있다면 다음 버블을 기준으로 바꿈
			}
			
			// 버블의 현재 위치(중앙값) 저장
			bubbleMidX = images[GAME_MAP_END + 5].x + MAP_MARGIN / 2;
			bubbleMidY = images[GAME_MAP_END + 5].y + MAP_MARGIN / 2;
			return 1; // 1반환
		}
	}	
	return 0; // 0 반환
}

// 버블 위치 저장하는 배열
int currentBubblePosition[2] = { -1, -1 };

// 맵을 이차원 배열에 저장하는 함수
int SaveMap(char(*Bubble)[20], int* map, char curBubble[20]) { // 필요한 매개변수: 버블 이름, 맵, 현재 버블 이름
	int temp = 0; // 현재 버블 이름을 저장하는 변수

	for (int i = 0; i < 7; i++) {
		if (!strcmp(curBubble, Bubble[i])) {
			temp = i; // 현재 버블이랑 이름이 같은 버블의 인덱스를 저장
		}
	}
	
	for (int i = 0; i < MAP_MAX_Y_COUNT * MAP_MAX_X_COUNT; i++) { // 맵의 크기만큼 반복하면서
		if (
			((images[3 + i].x <= bubbleMidX && images[3 + i].x + MAP_MARGIN >= bubbleMidX) && // images 배열에 저장된 x값과 y값에 현재 버블의 중앙값이 들어 있다면
			(images[3 + i].y <= bubbleMidY && images[3 + i].y + MAP_MARGIN >= bubbleMidY))
			) {
			if (images[3 + i].fileName == Bubble[0]) { // 버블이 없다면
				map[i] = temp; // 버블 저장
				currentBubblePosition[0] = i / MAP_MAX_X_COUNT; // y좌표 저장
				currentBubblePosition[1] = i % MAP_MAX_X_COUNT; // x좌표 저장
			}
			else { // 버블이 있을 경우
				map[i + MAP_MAX_X_COUNT] = temp; // 그 버블 바로 밑에 저장
				currentBubblePosition[0] = i / MAP_MAX_X_COUNT + 1; // y좌표 저장
				currentBubblePosition[1] = i % MAP_MAX_X_COUNT; // x좌표 저장
			}
		}
	}
}

// 버블 터지는 함수를 구현하기 위해 필요한 변수
int count = 0; // 버블 터지는 개수를 세기 위한 변수
int visited[MAP_MAX_Y_COUNT][MAP_MAX_X_COUNT] = { 0 }; // 방문한 곳 체크하는 배열
char s[20] = { 0 }; // score 출력할 때 사용하는 변수

// 버블 터지는 함수
int BurstBubbleCheck(int(*map)[MAP_MAX_X_COUNT], char(*Bubble)[20], char curBubble[20], int curY, int curX, int preY, int preX) { // 필요한 매개변수: 맵, 버블 이미지 이름, 현재 버블 이미지 이름, 현재 y좌표, 현재 x좌표, 이전 Y좌표, 이전 X좌표
	int temp = 0; // 현재 버블 이름을 저장하는 변수

	for (int i = 0; i < 7; i++) {
		if (!strcmp(curBubble, Bubble[i])) {
			temp = i; // 현재 버블이랑 이름이 같은 버블의 인덱스를 저장
		}
	}

	if ((curX < MAP_MAX_X_COUNT) && (curX >= 0) && (curY < MAP_MAX_Y_COUNT) && (curY >= 0) && (map[curY][curX] != 0)) { // 현재 좌표가 맵 안에 있을 때
		if ((map[curY][curX] == map[curY][curX + 1]) && // 현재 좌표와 오른쪽 좌표가 같고
			!((curY == preY) && (curX + 1 == preX)) && // 직전에 방문한 좌표가 아니고
			(curX + 1 < MAP_MAX_X_COUNT) && (curX + 1 >= 0) && (curY < MAP_MAX_Y_COUNT) && (curY >= 0)) { // 체크할 좌표가 맵 안에 있을 때
			if (visited[curY][curX] <= 2 && visited) { // 방문한 곳이 2번 이하일 때
				visited[curY][curX]++; // 방문한 곳 체크
				visited[curY][curX + 1]++;
				BurstBubbleCheck(map, Bubble, curBubble, curY, curX + 1, curY, curX); // 오른쪽 버블과 이름이 같으면 재귀함수 호출
				count++; // 버블 터지는 개수 증가
			}
		}
		// 위의 if문과 같은 방식으로 아래, 왼쪽, 위, 대각선의 버블과 비교
		if ((map[curY][curX] == map[curY][curX - 1]) &&
			!((curY == preY) && (curX - 1 == preX)) && 
			(curX - 1 < MAP_MAX_X_COUNT) && (curX - 1 >= 0) && (curY < MAP_MAX_Y_COUNT) && (curY >= 0)) { // 왼쪽
			if (visited[curY][curX] <= 2) {
				visited[curY][curX]++;
				visited[curY][curX - 1]++;
				BurstBubbleCheck(map, Bubble, curBubble, curY, curX - 1, curY, curX);
				count++;
			}
		}
		if (map[curY][curX] == map[curY + 1][curX] &&
			!((curY + 1 == preY) && (curX == preX)) && 
			(curX < MAP_MAX_X_COUNT) && (curX >= 0) && (curY + 1 < MAP_MAX_Y_COUNT) && (curY + 1 >= 0)) { // 아래
			if (visited[curY][curX] <= 2) {
				visited[curY][curX]++;
				visited[curY + 1][curX]++;
				BurstBubbleCheck(map, Bubble, curBubble, curY + 1, curX, curY, curX);
				count++;
			}
		}
		if (map[curY][curX] == map[curY - 1][curX] &&
			!((curY - 1 == preY) && (curX == preX)) &&
			(curX < MAP_MAX_X_COUNT) && (curX >= 0) && (curY - 1 < MAP_MAX_Y_COUNT) && (curY - 1 >= 0)) { // 위
			if (visited[curY][curX] <= 2) {
				visited[curY][curX]++;
				visited[curY - 1][curX]++;
				BurstBubbleCheck(map, Bubble, curBubble, curY - 1, curX, curY, curX);
				count++;
			}
		}
		if (map[curY][curX] == map[curY + 1][curX + 1] &&
			!((curY + 1 == preY) && (curX + 1 == preX)) && 
			(curX + 1 < MAP_MAX_X_COUNT) && (curX + 1 >= 0) && (curY + 1 < MAP_MAX_Y_COUNT) && (curY + 1 >= 0)) { // 오른쪽 아래
			if (visited[curY][curX] <= 2) {
				visited[curY][curX]++;
				visited[curY + 1][curX + 1]++;
				BurstBubbleCheck(map, Bubble, curBubble, curY + 1, curX + 1, curY, curX);
				count++;
			}
		}
		if (map[curY][curX] == map[curY + 1][curX - 1] &&
			!((curY + 1 == preY) && (curX - 1 == preX)) &&
			(curX - 1 < MAP_MAX_X_COUNT) && (curX - 1 >= 0) && (curY + 1 < MAP_MAX_Y_COUNT) && (curY + 1 >= 0)) { // 왼쪽 아래
			if (visited[curY][curX] <= 2) {
				visited[curY][curX]++;
				visited[curY + 1][curX - 1]++;
				BurstBubbleCheck(map, Bubble, curBubble, curY + 1, curX - 1, curY, curX);
				count++;
			}
		}
		if (map[curY][curX] == map[curY - 1][curX + 1] &&
			!((curY - 1 == preY) && (curX + 1 == preX)) &&
			(curX + 1 < MAP_MAX_X_COUNT) && (curX + 1 >= 0) && (curY - 1 < MAP_MAX_Y_COUNT) && (curY - 1 >= 0)) { // 오른쪽 위
			if (visited[curY][curX] <= 2) {
				visited[curY][curX]++;
				visited[curY - 1][curX + 1]++;
				BurstBubbleCheck(map, Bubble, curBubble, curY - 1, curX + 1, curY, curX);
				count++;
			}
		}
		if (map[curY][curX] == map[curY - 1][curX - 1] &&
			!((curY - 1 == preY) && (curX + 1 == preX)) &&
			(curX - 1 < MAP_MAX_X_COUNT) && (curX - 1 >= 0) && (curY - 1 < MAP_MAX_Y_COUNT) && (curY - 1 >= 0)) { // 왼쪽 위
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

// 실제로 버블 터지는 장면 구현
void BurstBubble(int(*map)[MAP_MAX_X_COUNT], int(*visited)[MAP_MAX_X_COUNT], int burst) { // 필요한 매개변수: 맵, 방문한 곳, 카운트 숫자
	if (burst >= 2) { // 3 이상일 경우
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

// 'q'키를 눌렀을 때 각 모드에 따라 실행
int Pause(char key, int *map, int mode) {
	if (key == 'q') { // Pasue Save 열기
		return 1;
	}
	else if (key == 'w') { // Pause Load 열기
		return 2;
	}
	else if (key == 'a') { // Pause Save 실행
		FILE* fp = fopen("GameMap.txt", "w"); // 맵 저장할 파일 열기 (쓰기 모드로)
		FILE* sc = fopen("score.txt", "w"); // 점수 저장할 파일 열기 (쓰기 모드로)
		
		if (!strcmp(images[6 + GAME_MAP_END].fileName, "PauseSave.bmp")) { // PauseSave일 경우
			for (int i = 0; i < (MAP_MAX_Y_COUNT + 1) * MAP_MAX_X_COUNT; i++) {
				fprintf(fp, "%d ", map[i]); // 맵 저장
			}

			fclose(fp);

			fprintf(sc, "%d", score); // 점수 저장
			fclose(sc);
			
			return 3; // 완료 표시 띄우기
		}
		return 2; // Pause Load 열기 (원래 모드)
	}
	else if (key == 's') { // Pause Load 실행
		FILE* fp = fopen("GameMap.txt", "r"); // 맵 저장할 파일 열기 (읽기 모드로)
		FILE* sc = fopen("score.txt", "r"); // 점수 저장할 파일 열기 (읽기 모드로)
		
		if (!strcmp(images[6 + GAME_MAP_END].fileName, "PauseLoad.bmp")) { // PauseLoad일 경우
			for (int i = 0; i < (MAP_MAX_Y_COUNT + 1) * MAP_MAX_X_COUNT; i++) {
				fscanf(fp, "%d ", &map[i]); // 맵 불러오기
			}

			fclose(fp);
			
			fscanf(sc, "%d", &score); // 점수 불러오기
			fclose(sc);
			
			return 4;
		}
		return 1; // Pause Save 열기 (원래 모드)
	}
	else if (key == 'e') { // 끄기
		return 0;
	}
	else {
		return mode;
	}
}

// 게임 배경 화면 및 GameMap 출력
int GameLayoutImage(int (*map)[MAP_MAX_X_COUNT], char(*Bubble)[20], int angle, int *shoot, int *mode, int pause) { // 필요한 매개변수: 게임맵, 버블 이미지 이름, 각도, 슛 여부, 멈춤모드
	Sleep(1);
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer);
	
	// GameMap의 정보를 images 배열 안에 넣음
	int i = 3; // GameMap정보를 넣을 images 배열의 시작지점
	int j = 0; // GameMap의 y좌표
	int k = 0; // GameMap의 x좌표
	while (i < 3 + MAP_MAX_X_COUNT * MAP_MAX_Y_COUNT) { // GameMap의 모든 정보를 넣음
		if (k % MAP_MAX_X_COUNT == 0 && k != 0) { // 한 줄 끝으로 가면
			k = 0; // x좌표를 0으로 설정하고
			j++; // y좌표를 1 증가
		}

		// images 배열에 GameMap의 각 버블 설정
		images[i].fileName = Bubble[map[j][k]];
		images[i].x = MAP_START_X + k % MAP_MAX_X_COUNT * MAP_MARGIN;
		images[i].y = MAP_START_Y + j % MAP_MAX_Y_COUNT * MAP_MARGIN;
		images[i].scale = MAIN_BUBBLE_SIZE;

		i++; // images 배열의 다음 인덱스로 이동
		k++; // x좌표를 1 증가
	}

	// 다음 버블 설정
	images[1].fileName = Bubble[map[MAP_MAX_Y_COUNT][0]];

	// 현재 버블 설정
	images[2].fileName = Bubble[map[MAP_MAX_Y_COUNT][1]];

	// 발사하는 버블 설정
	images[GAME_MAP_END + 5].fileName = "NULL";
	images[GAME_MAP_END + 5].x = CURRENT_X;
	images[GAME_MAP_END + 5].y = CURRENT_Y;
	images[GAME_MAP_END + 5].scale = MAIN_BUBBLE_SIZE;

	imageLayer.transparentColor = RGB(0, 255, 0); // 초록색을 투명색으로 설정

	imageLayer.imageCount = 300;
	imageLayer.images = images;

	// 좌표값 설정
	double currentX = CURRENT_X;
	double currentY = CURRENT_Y;
	double tempX = currentX;
	double tempY = currentY;
	int first = 1; // 처음 발사하는 버블인지 확인하는 변수를 기본값으로 1로 설정

	// 'q'키 눌렀을 시의 모드 초기화
	images[6 + GAME_MAP_END].fileName = "NULL";
	images[6 + GAME_MAP_END].x = 400;
	images[6 + GAME_MAP_END].y = 400;
	images[6 + GAME_MAP_END].scale = 0.5;

	if (*shoot == 1) { // 발사했을 때
		images[GAME_MAP_END + 5].fileName = Bubble[map[MAP_MAX_Y_COUNT][1]]; // 발사하는 버블 이미지 불러오기
		// 맵에서
		map[MAP_MAX_Y_COUNT][1] = map[MAP_MAX_Y_COUNT][0]; // 다음 버블을 현재 버블로 설정
		map[MAP_MAX_Y_COUNT][0] = map[MAP_MAX_Y_COUNT][2]; // 다다음 버블을 다음 버블로 설정
		map[MAP_MAX_Y_COUNT][2] = rand() % 6 + 1; // 다다음 버블을 랜덤으로 설정

		// 이미지 레이어에서
		images[1].fileName = Bubble[map[MAP_MAX_Y_COUNT][0]]; // 다음 버블로 설정
		images[2].fileName = Bubble[map[MAP_MAX_Y_COUNT][1]]; // 현재 버블로 설정

		switch (angle) // 각도에 따라서
		{
		case 1: // 30도
			while (1) {
				for (int i = 0; ; i += 10) { // 왼쪽으로 가는 모션
					if (first && CheckBound(currentX - i * 0.01, CURRENT_Y - (i * sin(PI * 30 / 180) * 0.01)) && !CheckBubble(Bubble)) { // 첫번째 실행하고, 벽에 부딪히지 않았고, 버블이 겹치지 않았을 때
						// 첫번째 체크를 넣은 이유: 첫번째는 중앙에서 시작하지만, 이후로는 맵 사이드 끝에서 시작하기 때문
						currentX = CURRENT_X - i; // 왼쪽으로 1만큼 이동할 때
						currentY = CURRENT_Y - i * sin(PI * 30 / 180); // 위쪽으로 sin30도 만큼 이동한 좌표값 저장

						images[GAME_MAP_END + 5].x = currentX; // 위에서 저장한 좌표값으로 갱신
						images[GAME_MAP_END + 5].y = currentY;
						imageLayer.renderAll(&imageLayer); // 이미지 렌더링
						sprintf(s, "%d 점", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "나눔바른고딕", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else if (CheckBound(tempX - i * 0.01, tempY - (i * sin(PI * 30 / 180) * 0.01)) && !CheckBubble(Bubble)) { // 벽에 부딪히지 않았고, 버블이 겹치지 않았을 때 (2번째 실행 이후)
						tempX = currentX - i; // 왼쪽으로 1만큼 이동할 때
						tempY = currentY - i * sin(PI * 30 / 180); // 위쪽으로 sin30도 만큼 이동한 좌표값 저장

						images[GAME_MAP_END + 5].x = tempX; // 위에서 저장한 좌표값으로 갱신
						images[GAME_MAP_END + 5].y = tempY;

						imageLayer.renderAll(&imageLayer); // 이미지 렌더링
						sprintf(s, "%d 점", score); // score 변수를 문자열로 바꿔주고
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "나눔바른고딕", 100, 0, 0, RGB(255, 255, 255), 1, s); // 화면에 출력
					}
					else { // 벽에 부딪히거나, 버블이 겹쳤을 때
						if (first) { // 첫번째 실행이라면
							tempX = currentX; // 현재 좌표값을 temp에 저장
							tempY = currentY;
						}
						else { // 첫번째 실행이 아니라면
							currentX = tempX; // temp에 저장된 좌표값을 currentX에 저장
							currentY = tempY;
						}
						break; // 반복문 탈출
					}
				}
				for (int i = 10; ; i += 10) { // 오른쪽으로 가는 모션
					if (CheckBound(tempX + i * 0.01, tempY - i * (sin(PI * 150 / 180) * 0.01)) && !CheckBubble(Bubble)) { // 벽에 부딪히지 않았고, 버블이 겹치지 않았을 때
						tempX = currentX + i; // 오른쪽으로 1만큼 이동할 때
						tempY = currentY - i * sin(PI * 150 / 180); // 위쪽으로 sin150도 만큼 이동한 좌표값 저장

						images[GAME_MAP_END + 5].x = tempX; // 위에서 저장한 좌표값으로 갱신
						images[GAME_MAP_END + 5].y = tempY;

						imageLayer.renderAll(&imageLayer); // 이미지 렌더링
						sprintf(s, "%d 점", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "나눔바른고딕", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else { // 벽에 부딪히거나, 버블이 겹쳤을 때
						currentX = tempX; // temp에 저장된 좌표값을 currentX에 저장
						currentY = tempY;
						first = 0; // sin30도로 이동하게 되면 대각선 왼쪽 위로 이동하게 되는데, 이쪽 부분은 오른쪽으로 이동하는 모션이기 때문에 첫번째 실행이 아님. 따라서, first를 0으로 바꿔줌
						break; // 반복문 탈출
					}
				}
				if (!CheckBound(tempX + i * 0.01, tempY - i * (sin(PI * 150 / 180) * 0.01)) || CheckBubble(Bubble)) { // 오른쪽으로 가서 벽에 부딪히면 왼쪽으로 (while의 1번째 for문) 이동해야 하고 
					if (CheckBubble(Bubble)) { // 버블이 겹치면 겹쳐진 위치에 가장 가까운 배열에 저장해 준다
						SaveMap(Bubble, map, images[GAME_MAP_END + 5].fileName);
						int burst = BurstBubbleCheck(map, Bubble, images[GAME_MAP_END + 5].fileName, currentBubblePosition[0], currentBubblePosition[1], 100, 100);
						BurstBubble(map, visited, burst);
					}
					break;
				}
			}
			break;
		// 60도
		case 2: // 각도를 제외한 로직은 case 1과 동일하기 때문에 자세한 설명은 생략함
			while (1) {
				for (int i = 0; ; i += 10) { // 왼쪽으로 이동하는 모션
					if (first && CheckBound(currentX - i * 0.01, CURRENT_Y - (i * sin(PI * 60 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						currentX = CURRENT_X - i;
						currentY = CURRENT_Y - i * sin(PI * 60 / 180); // 위쪽으로 sin60도 만큼 이동

						images[GAME_MAP_END + 5].x = currentX;
						images[GAME_MAP_END + 5].y = currentY;
						imageLayer.renderAll(&imageLayer); // 이미지 렌더링
						sprintf(s, "%d 점", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "나눔바른고딕", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else if (CheckBound(tempX - i * 0.01, tempY - (i * sin(PI * 60 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						tempX = currentX - i;
						tempY = currentY - i * sin(PI * 60 / 180);

						images[GAME_MAP_END + 5].x = tempX;
						images[GAME_MAP_END + 5].y = tempY;

						imageLayer.renderAll(&imageLayer); // 이미지 렌더링
						sprintf(s, "%d 점", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "나눔바른고딕", 100, 0, 0, RGB(255, 255, 255), 1, s);
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
				for (int i = 10; ; i += 10) { // 오른쪽으로 이동하는 모션
					if (CheckBound(tempX + i * 0.01, tempY - i * (sin(PI * 120 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						tempX = currentX + i;
						tempY = currentY - i * sin(PI * 120 / 180); // 위쪽으로 sin120도 만큼 이동

						images[GAME_MAP_END + 5].x = tempX;
						images[GAME_MAP_END + 5].y = tempY;

						imageLayer.renderAll(&imageLayer); // 이미지 렌더링
						sprintf(s, "%d 점", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "나눔바른고딕", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else {
						currentX = tempX;
						currentY = tempY;
						first = 0;
						break;
					}
				}
				if (!CheckBound(tempX + i * 0.01, tempY - i * (sin(PI * 120 / 180) * 0.01)) || CheckBubble(Bubble)) { // 벽에 부딪히거나 버블에 겹치면 종료
					if (CheckBubble(Bubble)) {
						SaveMap(Bubble, map, images[GAME_MAP_END + 5].fileName);
						int burst = BurstBubbleCheck(map, Bubble, images[GAME_MAP_END + 5].fileName, currentBubblePosition[0], currentBubblePosition[1], 100, 100);
						BurstBubble(map, visited, burst);
						
					}
					break;
				}
			}
			break;
		// 90도
		case 3:
			for (int i = 0; ; i += 10) {
				currentX = CURRENT_X; // x좌표는 그대로
				currentY = CURRENT_Y - i; // y좌표를 위로 이동
				if (CheckBound(currentX, currentY) && !CheckBubble(Bubble)) { // 벽에 부딪히지 않고 버블에 겹치지 않으면
					images[GAME_MAP_END + 5].x = currentX;
					images[GAME_MAP_END + 5].y = currentY;
				}

				if (!CheckBound(currentX, currentY - i * 0.01) || CheckBubble(Bubble)) { // 벽에 부딪히거나 버블에 부딪히면 종료
					if (CheckBubble(Bubble)) {
						SaveMap(Bubble, map, images[GAME_MAP_END + 5].fileName);
						int burst = BurstBubbleCheck(map, Bubble, images[GAME_MAP_END + 5].fileName, currentBubblePosition[0], currentBubblePosition[1], 100, 100);
						BurstBubble(map, visited, burst);
					}
					break;
				}
				imageLayer.renderAll(&imageLayer); // 이미지 렌더링
				sprintf(s, "%d 점", score);
				printTextWithAngle(imageLayer._consoleDC, 1150, 310, "나눔바른고딕", 100, 0, 0, RGB(255, 255, 255), 1, s);
			}
			break;
		// 120도
		case 4: // 각도를 제외한 로직은 case 1과 동일하기 때문에 자세한 설명은 생략함
			while (1) {
				for (int i = 0; ; i += 10) { // 오른쪽으로 이동하는 모션
					if (first && CheckBound(currentX + i * 0.01, CURRENT_Y - (i * sin(PI * 120 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						currentX = CURRENT_X + i; // 오른쪽으로 이동하기 때문에 x좌표 증가
						currentY = CURRENT_Y - i * sin(PI * 120 / 180); // 위쪽으로 sin120도 만큼 이동

						images[GAME_MAP_END + 5].x = currentX;
						images[GAME_MAP_END + 5].y = currentY;
						imageLayer.renderAll(&imageLayer); // 이미지 렌더링
						sprintf(s, "%d 점", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "나눔바른고딕", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else if (CheckBound(tempX + i * 0.01, tempY - (i * sin(PI * 120 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						tempX = currentX + i;
						tempY = currentY - i * sin(PI * 120 / 180);

						images[GAME_MAP_END + 5].x = tempX;
						images[GAME_MAP_END + 5].y = tempY;

						imageLayer.renderAll(&imageLayer); // 이미지 렌더링
						sprintf(s, "%d 점", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "나눔바른고딕", 100, 0, 0, RGB(255, 255, 255), 1, s);
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
				for (int i = 10; ; i += 10) { // 왼쪽으로 이동하는 모션
					if (CheckBound(tempX - i * 0.01, tempY - i * (sin(PI * 60 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						tempX = currentX - i; // 왼쪽으로 이동하기 때문에 x좌표 감소
						tempY = currentY - i * sin(PI * 60 / 180); // 위쪽으로 sin60도 만큼 이동

						images[GAME_MAP_END + 5].x = tempX;
						images[GAME_MAP_END + 5].y = tempY;

						imageLayer.renderAll(&imageLayer); // 이미지 렌더링
						sprintf(s, "%d 점", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "나눔바른고딕", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else {
						currentX = tempX;
						currentY = tempY;
						first = 0;
						break;
					}
				}
				if (!CheckBound(tempX + i * 0.01, tempY - i * (sin(PI * 60 / 180) * 0.01)) || CheckBubble(Bubble)) { // 벽에 부딪히거나 버블에 부딪히면 종료
					if (CheckBubble(Bubble)) {
						SaveMap(Bubble, map, images[GAME_MAP_END + 5].fileName);
						int burst = BurstBubbleCheck(map, Bubble, images[GAME_MAP_END + 5].fileName, currentBubblePosition[0], currentBubblePosition[1], 100, 100);
						BurstBubble(map, visited, burst);
					}
					break;
				}
			}
			break;
		// 150도
		case 5: // 각도를 제외한 로직은 case 1과 동일하기 때문에 자세한 설명은 생략함
			while (1) {
				for (int i = 0; ; i += 10) { // 오른쪽으로 이동하는 모션
					if (first && CheckBound(currentX + i * 0.01, CURRENT_Y - (i * sin(PI * 150 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						currentX = CURRENT_X + i;
						currentY = CURRENT_Y - i * sin(PI * 150 / 180); // 위쪽으로 sin150도 만큼 이동

						images[GAME_MAP_END + 5].x = currentX;
						images[GAME_MAP_END + 5].y = currentY;
						imageLayer.renderAll(&imageLayer); // 이미지 렌더링
						sprintf(s, "%d 점", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "나눔바른고딕", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else if (CheckBound(tempX + i * 0.01, tempY - (i * sin(PI * 150 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						tempX = currentX + i;
						tempY = currentY - i * sin(PI * 150 / 180);

						images[GAME_MAP_END + 5].x = tempX;
						images[GAME_MAP_END + 5].y = tempY;
						imageLayer.renderAll(&imageLayer); // 이미지 렌더링
						sprintf(s, "%d 점", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "나눔바른고딕", 100, 0, 0, RGB(255, 255, 255), 1, s);
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
				for (int i = 10; ; i += 10) { // 왼쪽으로 이동하는 모션
					if (CheckBound(tempX - i * 0.01, tempY - i * (sin(PI * 30 / 180) * 0.01)) && !CheckBubble(Bubble)) {
						tempX = currentX - i;
						tempY = currentY - i * sin(PI * 30 / 180); // 위쪽으로 sin30도 만큼 이동

						images[GAME_MAP_END + 5].x = tempX;
						images[GAME_MAP_END + 5].y = tempY;

						imageLayer.renderAll(&imageLayer); // 이미지 렌더링
						sprintf(s, "%d 점", score);
						printTextWithAngle(imageLayer._consoleDC, 1150, 310, "나눔바른고딕", 100, 0, 0, RGB(255, 255, 255), 1, s);
					}
					else {
						currentX = tempX;
						currentY = tempY;
						first = 0;
						break;
					}
				}
				if (!CheckBound(tempX + i * 0.01, tempY - i * (sin(PI * 30 / 180) * 0.01)) || CheckBubble(Bubble)) { // 벽에 부딪히거나 버블에 부딪히면 종료
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
		*shoot = 0; // 슛의 값을 0으로 바꿔줌
		images[GAME_MAP_END + 5].fileName = "NULL"; // 발사했던 버블 없애줌
	}
	else { // Pause 처리
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
		
		imageLayer.renderAll(&imageLayer); // 이미지 렌더링
		sprintf(s, "%d 점", score);
		printTextWithAngle(imageLayer._consoleDC, 1150, 310, "나눔바른고딕", 100, 0, 0 , RGB(255, 255, 255), 1, s);
	}
	return score;
}

// 새로운 라인 생성하는 함수
void MakeNewLine(int(*map)[MAP_MAX_X_COUNT], char(*Bubble)[20], int start, int last) { // 필요한 매개변수: 게임맵, 버블 이미지 이름, 시작줄, 끝줄
	int temp[MAP_MAX_Y_COUNT][MAP_MAX_X_COUNT] = { 0 }; // 새로운 라인을 생성하기 위해 임시로 저장할 배열
	if (start == -1) { // 시작줄이 -1이면 맨 위에 라인을 생성하고 아래 있는 라인들을 1칸씩 밑으로 내림
		for (int i = 0; i < MAP_MAX_Y_COUNT - 1; i++) {
			for (int j = 0; j < MAP_MAX_X_COUNT; j++) {
				temp[i + 1][j] = map[i][j];
			}
		}
		for (int i = 0; i < MAP_MAX_X_COUNT; i++) { // 맨 위에 라인을 생성
			temp[0][i] = rand() % 6 + 1;
		}
		for (int i = 0; i < MAP_MAX_Y_COUNT; i++) { // 맵에 새로운 라인을 저장
			for (int j = 0; j < MAP_MAX_X_COUNT; j++) {
				map[i][j] = temp[i][j];
			}
		}
	}

	for (int i = start; i < last; i++) { // 시작줄부터 끝줄까지
		for (int j = 0; j < MAP_MAX_X_COUNT; j++) { // 한 줄마다
			map[i][j] = rand() % 6 + 1; // 랜덤으로 뽑아서 저장
		}
	}
}

// 파워 조절 시 필요한 함수
int level = 1;
int mod = 0;

// 레벨 조절하는 함수
int SetLevel(char(*Level)[20], char key) { // 필요한 매개변수: 레벨 이미지 이름, 입력 값
	// 파워 이미지 설정
	images[3 + GAME_MAP_END].fileName = Level[level-1];
	images[3 + GAME_MAP_END].x = LEVEL_X;
	images[3 + GAME_MAP_END].y = LEVEL_Y;
	images[3 + GAME_MAP_END].scale = LEVEL_SIZE;
	
	if (key == 0) { // UP키를 눌렀을 때
		if (mod == 0) { // 홀수번(처음 포함) 눌렀다면
			mod = 1; // 창 키고, 파워 조절 가능한 모드로 전환
		}
		else if (mod == 1) { // 짝수번 눌렀다면
			mod = 0; // 창 끄고, 레벨 반환
			images[3 + GAME_MAP_END].fileName = "NULL";
			return level;
		}
	}

	// 레벨 조절 모드
	if (key == '=') { // + 키를 누르면 레벨 1 증가
		++level;
		if (level > 6) { // 레벨이 6을 넘어가면 1으로 바꿈
			level = 1;
		}
	}
	else if (key == '-') { // -키를 누르면 레벨 1 감소
		--level;
		if (level < 1) {  // 레벨이 0이하면 6으로 바꿈
			level = 6;
		}
	}
		
	images[3 + GAME_MAP_END].fileName = Level[level-1]; // 현재 레벨 출력
}

// 각도 조절하기 위해 필요한 변수
int arrow = 1;

// 각도 조절하는 함수
int SetAngle(char(*Arrow)[20], char key) { // 필요한 매개변수: 화살표 이미지 이름, 입력 값
	// 화샇표(각도) 이미지 설정
	images[4 + GAME_MAP_END].fileName = Arrow[arrow - 1];
	images[4 + GAME_MAP_END].x = ARROW_X;
	images[4 + GAME_MAP_END].y = ARROW_Y;
	images[4 + GAME_MAP_END].scale = ARROW_SIZE;

	if (key == RIGHT) { // 오른쪽 키를 눌렀을 때
		++arrow; // +30도 증가
		if (arrow > 5) { // 150도 넘어가면 30도로 버꿈
			arrow = 1;
		}
	}
	else if (key == LEFT) { // 왼쪽 키를 눌렀을 때
		--arrow; // -30도 증가
		if (arrow < 1) { // 30도 미만이면 150도로 바꿈
			arrow = 5;
		}
	}

	images[4 + GAME_MAP_END].fileName = Arrow[arrow - 1]; // 화살표 출력
	return arrow; // 각도 반환
}

void InitScore() { // 점수 초기화
	score = 0;
}