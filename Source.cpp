#include <conio.h>
#include <time.h>
#define MAP_WIDTH 31
#define MAP_HEIGHT 13
#define SCREEN_WIDTH 16
#include <stdio.h>
#include <stdlib.h>

enum {
	CELL_TYPE_NONE,
	CELL_TYPE_HARD_BLOCK,
	CELL_TYPE_SOFT_BLOCK,
	CELL_TYPE_MAX
};

int cells[MAP_HEIGHT][MAP_WIDTH];
char cellAA[][2 + 1] = {
	"　",//CELL_TYPE_NONE,
	"■",//CELL_TYPE_HARD_BLOCK,
	"□"//CELL_TYPE_SOFT_BLOCK,
};
typedef struct {
	int x, y;

}MONSTER;
#define MONSTER_MAX 9
MONSTER monsters[MONSTER_MAX];

int getMonster(int _x,int _y) {
	for (int i = 0; i < MONSTER_MAX; i++)
		if ((monsters[i].x == _x) && (monsters[i].y == _y))
		return i;
	return -1;
}
void setFreePosition(int* pX, int* pY) {
	while (1) {
		int x = rand() % MAP_WIDTH;
		int y = rand() % MAP_HEIGHT;
		switch (cells[y][x]){
		case CELL_TYPE_HARD_BLOCK:
		case CELL_TYPE_SOFT_BLOCK:
			break;
		default:
			{
			int monster = getMonster(x, y);
			if (monster < 0) {
				*pX = x;
				*pY = y;
				return;
			}
		}
		}
	}

}
int main() {
	srand(time(NULL));
	for (int y = 0; y < MAP_HEIGHT; y+=MAP_HEIGHT-1)
		for (int x=0;x<MAP_WIDTH;x++)
			cells[y][x] = CELL_TYPE_HARD_BLOCK;
	for (int y = 0; y < MAP_HEIGHT; y++)
		for (int x = 0; x < MAP_WIDTH; x+=MAP_WIDTH-1)
			cells[y][x] = CELL_TYPE_HARD_BLOCK;
	for (int y = 0; y < MAP_HEIGHT; y++)
		for (int x = 1; x < MAP_WIDTH - 1; x++) {
			if ((x % 2 == 0) && (y % 2 == 0))
				cells[y][x] = CELL_TYPE_HARD_BLOCK;
			else if (rand() % 2)
				cells[y][x] = CELL_TYPE_SOFT_BLOCK;//ソフトブロック
		}
	cells[1][1] = cells[2][1] = cells[1][2] = CELL_TYPE_NONE;
	monsters[0].x = 1;
	monsters[0].y = 1;

	for (int i = 1; i < MONSTER_MAX; i++)
		setFreePosition(&monsters[i].x, &monsters[i].y);
	
	while (1) {
		system("cls");
		//int left = 0;
		int left = monsters[0].x - (SCREEN_WIDTH /2);
		if (left < 0) {
			left = 0;
		}
		if (left > MAP_WIDTH - SCREEN_WIDTH)
			left = MAP_WIDTH - SCREEN_WIDTH;
		for (int y = 0; y < MAP_HEIGHT; y++) {
			for (int x = left; x < left + SCREEN_WIDTH; x++) {
				int monster = getMonster(x, y);
				if (monster < 0)
					printf(cellAA[cells[y][x]]);
				else if (monster > 0)
					printf("敵");
				else
					printf("＠");
			}
			printf("\n");
		}
		int x = monsters[0].x;
		int y = monsters[0].y;
		switch (_getch()) {
		case 'w':y--; break;
		case 's':y++; break;
		case 'a':x--; break;
		case 'd':x++; break;
		}
		switch (cells[y][x]) {
		case CELL_TYPE_HARD_BLOCK:
		case CELL_TYPE_SOFT_BLOCK:
			break;
		default:
			monsters[0].x = x;
			monsters[0].y = y;
			break;
		}
		
	}
}