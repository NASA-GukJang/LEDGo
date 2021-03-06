#include <stdio.h> 
#include <windows.h> 
#include <stdlib.h> 
#include <time.h> 
#include <conio.h> 

#define BOARD_HEIGHT 24 // ┃개수 
#define BOARD_WIDTH 32  // ━개수 
#define BOARD_X 4 //보드열x좌표 
#define BOARD_Y 2 //보드행y좌표 

#define CBLOCK_Y 2 
#define DELAY 100//지연 
#define RAND 4//난수 

int static speed = 10;
int curX, curY; //콘솔커서 
int board[BOARD_HEIGHT + 1][BOARD_WIDTH + 2] = { 0, };
int block[4][4] = { { 0, 0, 0, 0 },
					{ 0, 1, 1, 0 },
					{ 0, 1, 1, 0 },
					{ 0, 0, 0, 0 } };

void removeCursor(void)
{
	CONSOLE_CURSOR_INFO curInfo;

	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void setCursor(int x, int y)
{
	COORD pos = { x,y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

COORD getCursor(void)
{
	COORD cur;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);

	cur.X = curInfo.dwCursorPosition.X;
	cur.Y = curInfo.dwCursorPosition.Y;

	return cur;
}

void showBoard(void)
{
	int x, y;

	//중앙보드라인 

	for (x = 1; x <= BOARD_WIDTH + 1; x++)
	{
		board[BOARD_HEIGHT][x] = 1; //board 배열중앙1인식 

		setCursor((BOARD_X)+x * 2, BOARD_Y + BOARD_HEIGHT);  //콘솔좌표 

		printf("━");
	}

	//왼쪽보드라인 

	for (y = 0; y<BOARD_HEIGHT + 1; y++)
	{
		board[y][0] = 1; //board 배열왼쪽1인식 

		setCursor(BOARD_X, BOARD_Y + y);

		if (y == BOARD_HEIGHT)
			printf("┗");

		else
			printf("┃");
	}

	//오른쪽보드라인 

	for (y = 0; y<BOARD_HEIGHT + 1; y++)
	{
		board[y][BOARD_WIDTH + 1] = 1; //board 배열오른쪽1인식 

		setCursor(BOARD_X + (BOARD_WIDTH + 2) * 2, BOARD_Y + y);

		if (y == BOARD_HEIGHT)
			printf("┛");

		else
			printf("┃");
	}

	//모서리값값변경 

	board[20][0] = 1;

	board[20][11] = 1;

	puts(" ");
}

void initial(int x, int y)
{
	setCursor(77, 10);
	printf("떨어뜨릴 라인을 고르시오(1,2,3,4,5,6,7)\n");
	setCursor(77, 12);
	scanf_s("%d", &x);

	switch (x)
	{
	case 1:
		curX = 5;
		break;

	case 2:
		curX = 15;
		break;

	case 3:
		curX = 25;
		break;

	case 4:
		curX = 35;
		break;
	
	case 5:
		curX = 45;
		break;

	case 6:
		curX = 55;
		break;

	case 7:
		curX = 65;
		break;

	default:
		setCursor(77, 20);
		printf("1-7까지의 숫자를 입력하시오\n");
		setCursor(77, 22);
		printf("라인을 입력하시오(1,2,3,4,5,6,7)\n");
		setCursor(77, 2);
		scanf_s("%d", &x);
		break;
	}

	//curX = scanf_s("%d", &x); //_getch();
	curY = y;

	setCursor(curX, y);
}

//removeBlock함수작동전벽인or 아닌지확인 

int detect(int move1, int move2)
{
	int x, y;
	int arrX, arrY; //배열좌표저장 

	COORD pos = getCursor();

	arrX = pos.X + move1;
	arrY = pos.Y + move2;

	arrX = (arrX / 2) - 2;
	arrY = arrY - BOARD_Y;

	for (y = 0; y<4; y++)
	{
		for (x = 0; x<4; x++)
		{
			if ((block[y][x] == 1) && board[arrY + y][arrX + x] == 1)

				return 1;  //보드와벽돌겹침 
		}
	}

	return 0;  //겹치지않음 
}

//벽돌생성 

void showBlock()
{
	int x, y;

	COORD cursor = getCursor();

	int prove;

	//int n=(rotation-1)%4; 

	prove = detect(0, 0);

	if (prove == 0)
	{
		//콘솔창위치설정배열값에서1은■출력은출력없음 

		for (y = 0; y<4; y++)
		{
			for (x = 0; x<4; x++)
			{
				setCursor(cursor.X + (x * 2), cursor.Y + y);

				if (block[y][x] == 1)
					printf("■");
			}
		}
		setCursor(cursor.X, cursor.Y);
	}
}


void removeBlock(int move1, int move2)
{
	int pr;
	int x, y;

	COORD cursor = getCursor();

	pr = detect(move1, move2);

	if (pr == 0)
	{
		for (y = 0; y<4; y++)
		{
			for (x = 0; x<4; x++)
			{
				setCursor(cursor.X + (x * 2), cursor.Y + y);

				if (block[y][x] == 1)
					printf(" ");
			}
		}
		setCursor(cursor.X + move1, cursor.Y + move2);
	}
}

//블록보드판에서1인식 

void boardConginition(int n, int move1, int move2)
{
	int x1, y1;

	COORD pos = getCursor();

	int arrX = pos.X + move1;  //콘솔좌표열 
	int arrY = pos.Y + move2;  //콘솔좌표행 

	int x, y;

	//커서위치정보배열위치정보변경

	arrX = arrX / 2 - 2; //콘솔좌표배열열변환값
	arrY = arrY - 2;     //콘솔좌표배열행변환값

						 //보드판에서블록이동시1인식

	for (y = 0; y<4; y++)
	{
		for (x = 0; x<4; x++)
		{
			if (block[y][x] == 1)
			{
				board[arrY + y][arrX + x] = 1;
			}
		}
	}

	return;
}


void moveBlock(void)
{
	int n;

	int kb;

	int prove;

	srand(time(NULL));

	int c = 2;

	//게임시작끝

	while (1)
	{
		initial(curX, CBLOCK_Y); //블록생성위치좌표

								 // n=(rand()%RAND)*4;//난수생성

								 //n=rand()%RAND;

		n = rand() % 7;//블록모양결정

		n = n * 4;

		// n = 6;

		//블록한개위밑이동

		while (1)
		{
			int ww = 0;

			int var;

			int k = 0;

			int tmp;

			//블록아래로이동

			while (!_kbhit())
			{
				//블록쇼

				showBlock();

				//딜레이타임

				Sleep(DELAY + speed);

				//아래이동시1있느지확인

				if (detect(0, 1) == 1)
				{
					ww = 1;

					boardConginition(n, 0, 0);//보드벽돌배열1추가

											  //control();

					break;
				}
				removeBlock(0, 1);  //board배열+1행
			}

			//detect함수에서배열값1발견시중지

			if (ww == 1)
				break;

			kb = _getch();
		}
	}
}

int main()
{
	removeCursor(); //커서깜박이제거

	setCursor(2, 1); //보드표시시작위치설정

	showBoard(); //보드출력

	moveBlock(); //보드출력움직임

	getchar();
}