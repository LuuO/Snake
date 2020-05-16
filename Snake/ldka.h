#include<stdio.h>
#include<conio.h> 
#include<windows.h>
//皮肤
#define UP 'A'
#define DOWN 'V'
#define LEFT '<'
#define RIGHT '>'
#define WALL '#'
#define FOOD 'X'
#define TAIL 'o'
#define EMPTY ' '
//全局变量
FILE *ts;
unsigned long long /*下帧时间*/nexttick = 0;
int lon/*蛇长度*/ = 5, tscore = 4;
int /*帧数*/frameno,/*运行次数*/rt = 0, /*刷新间隔(毫秒)*/speed = 200;
int i, j, k, quit, y = 20, x = 30;
char map[128][128], /*蛇坐标*/*snake[/*地图格数统计*/(128 - 2)*(128 - 2)], /*食物指针*/*foodp, key, /*方向*/fx, name[51] = { "Unknow" }, save = 'N', ft = 1,savecheck = 'N';
//等待1.5秒
void stfhas(void)
{
	unsigned long long a = GetTickCount() + 1500;
	while (1)
		if (GetTickCount() >= a)
			break;
}
//刷新食物坐标
void food(void)
{
	do
		foodp = &map[2 + rand() % (y - 6)][2 + rand() % (x - 6)];
	while (*foodp != EMPTY);
	*foodp = FOOD;
}
//初始化
void start(void)
{
	/*蛇初始坐标*/
	snake[0] = &map[1][5];
	snake[1] = &map[1][4];
	snake[2] = &map[1][3];
	snake[3] = &map[1][2];
	snake[4] = &map[1][1];
	lon/*蛇长度-1*/ = 5;
	frameno/*起始帧数*/ = 0;
	fx = RIGHT;
	for (i = 0;i < y;i++)/*全部空格*/
		for (j = 0;j < x;j++)
			map[i][j] = ' ';
	/*边框*/
	for (i = 0;i < y;i++)
	{
		map[i][0] = WALL;
		map[i][x - 1] = WALL;
	}
	for (j = 0;j < x;j++)
	{
		map[0][j] = WALL;
		map[y - 1][j] = WALL;
	}
	/*蛇*/
	for (k = 0;k <= lon - 2;k++)
		*snake[k] = fx;
	food();
}
//读文件
void read(void)
{
	int  a;
	if (!fopen_s(&ts, "Snake.ini", "r"))
	{
		fscanf_s(ts, "地图大小：%d*%d\n第%d次游戏\n每帧间隔：%d\n最高纪录：%d\n创造者：%s\n是否存档：%c\n", &x, &y, &rt, &speed, &tscore, name, 51, &savecheck);
		if (savecheck == 'Y')
		{
			fscanf_s(ts, "\n———————以下为保存的游戏———————\n\n蛇长度+1：%d\n帧数：%d\n方向：%c\n食物：%d\n蛇地址：", &lon, &frameno, &fx, &a);
			foodp = &map[0][0] + a;
			for (i = 0;i < lon;i++)
			{
				fscanf_s(ts, "%d.", &a);
				snake[i] = &map[0][0] + a;
			}
			fscanf_s(ts, "\n地图：\n");
			for (i = 0;i < 128;i++)
			{
				for (j = 0; j < 128;j++)
					map[i][j] = fgetc(ts);
				fgetc(ts);
			}
		}
		else
			start();
		fclose(ts);
	}
	else
		start();
}
//写文件
void write(void)
{
	fopen_s(&ts, "Snake.ini", "w");
	fprintf(ts, "地图大小：%d*%d\n第%d次游戏\n每帧间隔：%d\n最高纪录：%d\n创造者：%s\n是否存档：%c\n", x, y, rt, speed, tscore, name, save);
	if (save == 'Y')
	{
		fprintf(ts, "\n———————以下为保存的游戏———————\n\n蛇长度+1：%d\n帧数：%d\n方向：%c\n食物：%d\n蛇地址：", lon, frameno, fx, (foodp - &map[0][0]));
		for (i = 0;i < lon;i++)
			fprintf(ts, "%d.", (snake[i] - &map[0][0]));
		fprintf(ts, "\n地图：\n");
		for (i = 0;i < 128;i++)
		{
			for (j = 0; j < 128;j++)
				fputc(map[i][j], ts);
			fputc('\n', ts);
		}
	}
	fclose(ts);
}
//按键控制
void inputc(void)
{
	switch (key)
	{
	case 'W':if (fx != DOWN) fx = UP;break;
	case 'w':if (fx != DOWN) fx = UP;break;
	case 'S':if (fx != UP) fx = DOWN;break;
	case 's':if (fx != UP) fx = DOWN;break;
	case 'A':if (fx != RIGHT) fx = LEFT;break;
	case 'a':if (fx != RIGHT) fx = LEFT;break;
	case 'D':if (fx != LEFT) fx = RIGHT;break;
	case 'd':if (fx != LEFT) fx = RIGHT;break;
	case 'P':printf("暂停！");system("pause");stfhas();break;
	case 'p':printf("暂停！");system("pause");stfhas();break;
	case 'O':
		printf("输入刷新间隔（毫秒）：");
		scanf_s("%d", &speed);
		stfhas();
		break;
	case 'o':
		printf("输入刷新间隔（毫秒）：");
		scanf_s("%d", &speed);
		stfhas();
		break;
	case 'Q':
		stfhas();
		printf("输入Y确认退出，其他键取消：");
		key = _getch();
		if (key == 'y' || key == 'Y')
		{
			stfhas();
			printf("\n是否存档？N为否，其他为是：");
			key = _getch();
			if (!(key == 'n' || key == 'N'))
				save = 'Y';
			write();
			exit(0);
		}
		stfhas();
		break;
	case 'q':
		stfhas();
		printf("输入Y确认退出，其他键取消：");
		key = _getch();
		if (key == 'y' || key == 'Y')
		{
			stfhas();
			printf("\n是否存档？N为否，其他为是：");
			key = _getch();
			if (!(key == 'n' || key == 'N'))
				save = 'Y';
			write();
			exit(0);
		}
		stfhas();
		break;
	case 'Z':
		printf("输入地图大小（长宽各小于128）（x y）：");
		scanf_s("%d %d", &x,&y);
		start();
		rt++;
		write();
		break;
	case 'z':
		printf("输入地图大小（长宽各小于128）（x y）：");
		scanf_s("%d %d", &x, &y);
		start();
		rt++;
		write();
		break;
	default:
		break;
	}
	key = 0;
}
//游戏结束
void gameover(void)
{
	if (lon - 1 > tscore)
	{
		tscore = lon - 1;
		printf("新纪录，输入你的名字（最长50个字符）：");
		scanf_s("%s", name, 51);
	}
	else
		printf("游戏结束！ 重玩");
	write();
	system("pause");
}
//打印map
void printmap(void)
{
	system("cls");
	for (i = 0;i < y;i++)
	{
		for (j = 0;j < x;j++)
			putchar(map[i][j]);
		putchar('\n');
	}
	printf("地图大小：%d*%d 长度%d格 第%d帧 第%d次游戏 每帧间隔%d毫秒\nWASD控制 O更改速度 P暂停 Q退出 Z更改地图大小\n", x, y, lon - 1, ++frameno, rt, speed);
	if (tscore > 4)printf("最高纪录：%d  创造者：%s\n", tscore, name);
}
//蛇地址移位
void move(void)
{
	for (k = lon - 1;k > 0;k--)
		snake[k] = snake[k - 1];
}
//蛇头向前
void eat(void)
{
	if (*snake[0] == EMPTY)
	{
		*snake[lon - 1] = EMPTY;
		*snake[lon - 2] = TAIL;
		*snake[0] = fx;
		printmap();
	}
	else
		if (*snake[0] == FOOD)
		{			
			lon += 1;
			*snake[0] = fx;
			food();
			printmap();
		}
		else
		{
			*snake[0] = fx;
			printmap();
			gameover();
			rt++;
			start();
		}
}
int main()
{
	srand(GetTickCount());
	read();
	rt++;
	write();
	while (1)
	{
		if (_kbhit())
			key = _getch();
		if (GetTickCount() < nexttick)
			continue;
		nexttick = GetTickCount() + speed;
		inputc();
		move();
		switch (fx)
		{
		case UP:
			snake[0] -= 128;
			eat();
			break;
		case DOWN:
			snake[0] += 128;
			eat();
			break;
		case LEFT:
			snake[0] -= 1;
			eat();
			break;
		case RIGHT:
			snake[0] += 1;
			eat();
			break;
		}
		if (ft)
		{
			ft--;
			printf("要开始");
			system("pause");
			stfhas();
		}
	}
	return 0;
}