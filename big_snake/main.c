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
	//����ߵ����е㣬����ͷ����β
	BODY list[HIGH * WIDE];
	int size;
	BODY food;
	COORD coord;
	//�ƶ�����
	int dx;
	int dy;
	//��β�����������
	BODY tail;
	//����
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
	//��ʼ����ͷ����λ��
	pSnake->list[0].x = WIDE / 2;
	pSnake->list[0].y = HIGH / 2;
	pSnake->list[1].x = WIDE / 2 - 1;
	pSnake->list[1].y = HIGH / 2;
	//��ʼ������
	pSnake->size = 2;
	//��ʼ��ʳ��λ��
	init_food(&(pSnake->food));
	//��ʼ���ƶ��������ң�
	pSnake->dx = 1;
	pSnake->dy = 0;
	//��ʼ������
	pSnake->score = 0;
	return;
}

void show_wall()//��ʾ��ͼ�߽�
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

void show_ui(SNAKE* pSnake)//��ʾ�ߺ�ʳ��
{
	//��ʾ��ͷ����
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
	//��ʾʳ��
	pSnake->coord.X = pSnake->food.x;
	pSnake->coord.Y = pSnake->food.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pSnake->coord);
	putchar('#');
	//�����β�ۼ�
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
	//��¼�ƶ�ǰ����β�����������
	pSnake->tail = pSnake->list[pSnake->size - 1];
	//�ƶ���������
	int i;
	for (i = pSnake->size - 1; i > 0; i--)
	{
		pSnake->list[i] = pSnake->list[i - 1];
	}
	//�ƶ���ͷ����
	pSnake->list[0].x += pSnake->dx;
	pSnake->list[0].y += pSnake->dy;
	
	return;
}

void control_direction(SNAKE* pSnake)
{
	char  key = 0;
	while (_kbhit()) //�ж��Ƿ��°���,���²�����0 
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
		pSnake->list[0].y == pSnake->food.y)//�Ե�ʳ����
	{
		//�߳���һ��ʳ��λ��ˢ��
		pSnake->size++;//������list�����Ԫ�أ��´��ƶ����Զ����
		init_food(&(pSnake->food));//ʳ�ﲻ����ʧ�������µ�ʳ�Ｔ��
		pSnake->score += 10;//��������
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
		move_snake(pSnake);//�ƶ���������
		eat_body(pSnake);//�Ƿ�Ե�����
		//���ʱ��
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