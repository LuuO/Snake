#pragma once


//Snake程序的宏

#define TIMER_SNAKEGO   1
#define TIMER_SNAKEADV  2
#define TIMER_CREATE  3

#define X  0
#define Y  1
#define D  2   //方向

#define UP     0
#define DOWN   1
#define LEFT   2
#define RIGHT  3

#define MAP_EMPTY 0
#define MAP_SNAKE 1
#define MAP_FOODS 2

#define DM_GO     0
#define DM_EAT    1
#define DM_DAS    2

#define CO_HEAD    RGB(0, 197, 205)
#define CO_BODY    RGB(0, 245, 255)
#define CO_HEADX   RGB(238, 197, 145)
#define CO_BODYX   RGB(255, 231, 186)
#define CO_BG      RGB(188, 238, 104)
#define CO_CON     RGB(162, 205, 90)
#define CO_FOOD    RGB(255, 193, 37)

#define ED_EXIT 0
#define ED_CANC 1
#define ED_TRUE 5

#define WTCW        -1                                 //窗口宽厚度补偿 None为-1，Thin和对话框为+15
#define WTCH        -1                                 //窗口高厚度补偿 None为-1，Thin和对话框为+38

#define DP_MAP 0
#define DP_EXP 1

#define WINDOWSIZE  (m_ucMapSize*m_ucPixelSize+CON*2)
#define CON         (m_ucPixelSize/2)                                 //边框厚度
#define STATIC      static
#define CMMS        100                                                //必须为复数
