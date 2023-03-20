#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>

#define WIDE 60
#define HIGH 25

typedef struct body
{
	int x;
	int y;
}BODY;

typedef struct snake
{
	//组成蛇的所有点，从蛇头到蛇尾
	BODY list[HIGH * WIDE];
	int size;
	BODY food;
	COORD coord;
	//移动方向
	int dx;
	int dy;
	//蛇尾（用于清除）
	BODY tail;
	//分数
	int score;
}SNAKE;

void init_food(BODY* pFood)
{
	srand(time(NULL));
	pFood->x = rand() % (WIDE - 1) + 1;
	pFood->y = rand() % (HIGH - 1) + 1;
	return;
}
 

void init_snake(SNAKE *pSnake)
{
	//初始化蛇头蛇身位置
	pSnake->list[0].x = WIDE / 2;
	pSnake->list[0].y = HIGH / 2;
	pSnake->list[1].x = WIDE / 2 - 1;
	pSnake->list[1].y = HIGH / 2;
	//初始化长度
	pSnake->size = 2;
	//初始化食物位置
	init_food(&(pSnake->food));
	//初始化移动方向（向右）
	pSnake->dx = 1;
	pSnake->dy = 0;
	//初始化分数
	pSnake->score = 0;
	return;
}

void show_wall()//显示地图边界
{
	int i, j;
	for (i = 0; i <= HIGH; i++)
	{
		for (j = 0; j <= WIDE; j++)
		{
			if (i == 0 || i == HIGH || j == 0 || j == WIDE)
			{
				putchar('+');
			}
			else
			{
				putchar(' ');
			}
		}
		putchar('\n');
	}
	return;
}

void show_ui(SNAKE* pSnake)//显示蛇和食物
{
	//显示蛇头蛇身
	int i;
	for (i = 0; i < pSnake->size; i++)
	{
		pSnake->coord.X = pSnake->list[i].x;
		pSnake->coord.Y = pSnake->list[i].y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pSnake->coord);
		if (i == 0)
		{
			printf("@");
		}
		else {
			printf("*");
		}
	}
	//显示食物
	pSnake->coord.X = pSnake->food.x;
	pSnake->coord.Y = pSnake->food.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pSnake->coord);
	putchar('#');
	//清除蛇尾痕迹
	pSnake->coord.X = pSnake->tail.x;
	pSnake->coord.Y = pSnake->tail.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pSnake->coord);
	putchar(' ');
	return;
}

void hide_cursor()
{
	CONSOLE_CURSOR_INFO  cci;
	cci.dwSize = sizeof(cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	return;
}

void move_snake(SNAKE* pSnake)
{
	//记录移动前的蛇尾（用于清除）
	pSnake->tail = pSnake->list[pSnake->size - 1];
	//移动蛇身坐标
	int i;
	for (i = pSnake->size - 1; i > 0; i--)
	{
		pSnake->list[i] = pSnake->list[i - 1];
	}
	//移动蛇头坐标
	pSnake->list[0].x += pSnake->dx;
	pSnake->list[0].y += pSnake->dy;
	
	return;
}

void control_direction(SNAKE* pSnake)
{
	char  key = 0;
	while (_kbhit()) //判断是否按下按键,按下不等于0 
	{
		key = _getch();
	}

	switch (key)
	{
	case 'w':
		pSnake->dy = -1;
		pSnake->dx = 0;
		break;
	case 's':
		pSnake->dy = 1;
		pSnake->dx = 0;
		break;
	case 'a':
		pSnake->dy = 0;
		pSnake->dx = -1;
		break;
	case 'd':
		pSnake->dy = 0;
		pSnake->dx = 1;
		break;
	default:
		break;
	}

	return;
}

void eat_food(SNAKE* pSnake)
{
	if (pSnake->list[0].x == pSnake->food.x &&
		pSnake->list[0].y == pSnake->food.y)//吃到食物了
	{
		//蛇长加一，食物位置刷新
		pSnake->size++;//不用在list中添加元素，下次移动会自动添加
		init_food(&(pSnake->food));//食物不用消失，创建新的食物即可
		pSnake->score += 10;//分数增加
	}

	return;
}

void game_over(SNAKE* pSnake)
{
	pSnake->coord.X = WIDE / 2 - 10;
	pSnake->coord.Y = HIGH + 2;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pSnake->coord);
	printf("game over,score = %d\n", pSnake->score);
	return;
}

void eat_body(SNAKE* pSnake)
{
	int i;
	for (i = 1; i < pSnake->size; i++)
	{
		if (pSnake->list[0].x == pSnake->list[i].x && pSnake->list[0].y == pSnake->list[i].y)
		{
			game_over(pSnake);
			exit(0);
		}
	}
	return;
}

void start_game(SNAKE* pSnake)
{
	while (pSnake->list[0].x > 0 && pSnake->list[0].x < WIDE
		&& pSnake->list[0].y > 0 && pSnake->list[0].y < HIGH)
	{
		show_ui(pSnake);
		control_direction(pSnake);

		eat_food(pSnake);
		move_snake(pSnake);//移动身体坐标
		eat_body(pSnake);//是否吃到身体
		//间隔时间
		Sleep(200);
	}
	game_over(pSnake);
	return;
}

int main(int argc, char* argv[])
{
	hide_cursor();
	SNAKE* pSnake = (SNAKE*)malloc(sizeof(SNAKE));
	init_snake(pSnake);

	show_wall();
	start_game(pSnake);

	//while (1);
	free(pSnake);
	return 0;
}