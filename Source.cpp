#include <conio.h>
#include <time.h>
#define MAP_WIDTH 31
#define MAP_HEIGHT 13
#define SCREEN_WIDTH 16
#include <stdio.h>
#include <stdlib.h>

enum {
	DIRECTION_NORTH,
	DIRECTION_WEST,
	DIRECTION_SOUTH,
	DIRECTION_EAST,
	DIRECTION_MAX
};
typedef struct {
	int x, y;
	int count;

}BOMB;
#define BOMB_MAX 10
#define BOMB_COUNT_MAX 10
BOMB bombs[BOMB_MAX];

int directions[][2] = {
	{0,-1},//DIRECTION_NORTH,
	{-1,0},//DIRECTION_WEST,
	{0,1},//DIRECTION_SOUTH,
	{1,0}//DIRECTION_EAST,
};
enum {
	CELL_TYPE_NONE,
	CELL_TYPE_HARD_BLOCK,
	CELL_TYPE_SOFT_BLOCK,
	CELL_TYPE_EXPLOSION,
	CELL_TYPE_MAX
};

int cells[MAP_HEIGHT][MAP_WIDTH];
char cellAA[][2 + 1] = {
	"　",//CELL_TYPE_NONE,
	"■",//CELL_TYPE_HARD_BLOCK,
	"□",//CELL_TYPE_SOFT_BLOCK,
	"※"//CELL_TYPE_EXPLOSION,
};
typedef struct {
	int x, y;
	int direction;
	bool isDead;
}MONSTER;
#define MONSTER_MAX 9
MONSTER monsters[MONSTER_MAX];

int getMonster(int _x,int _y) {
	for (int i = 0; i < MONSTER_MAX; i++)
		if ((!monsters[i].isDead)&&(monsters[i].x == _x) && (monsters[i].y == _y))
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
int getBomb(int _x, int _y) {
	for (int i = 0; i < BOMB_MAX; i++)
		if ((bombs[i].count > 0) && (bombs[i].x == _x) && (bombs[i].y == _y))
			return i;
	return -1;

}
bool checkCanMove(int _x,int _y) {
	switch (cells[_y][_x]) {
	case CELL_TYPE_HARD_BLOCK:
	case CELL_TYPE_SOFT_BLOCK:
		return false;
	default:
		if ((getMonster(_x, _y) < 0)
			&& getBomb(_x,_y)<0)
			return true;
		return false;
		break;
	}

}
int getFreeBomb() {
	for (int i = 0; i < BOMB_MAX; i++) {
		if (bombs[i].count <= 0)
			return i;
		return -1;
	}
}
void explosion(int _x,int _y) {
	cells[_y][_x] = CELL_TYPE_EXPLOSION; //爆発
	for (int j = 0; j < DIRECTION_MAX; j++) {//全方向
		int x = _x;//爆発中心
		int y = _y;
		for (int i = 0; i < 2; i++) {
			x += directions[j][0];//2マス
			y += directions[j][1];
			if (cells[y][x] == CELL_TYPE_HARD_BLOCK)
				break;
			else if (cells[y][x] == CELL_TYPE_SOFT_BLOCK) {
				cells[y][x] = CELL_TYPE_EXPLOSION;
				break;
			}
			else {
				int monster = getMonster(x, y);
				if (monster > 1) {
					monsters[monster].isDead = true;
				}
				cells[y][x] = CELL_TYPE_EXPLOSION;//爆発
			}
		}
	}
}

int main() {
	srand(time(NULL));
	for (int y = 0; y < MAP_HEIGHT; y += MAP_HEIGHT - 1)
		for (int x = 0; x < MAP_WIDTH; x++)
			cells[y][x] = CELL_TYPE_HARD_BLOCK;
	for (int y = 0; y < MAP_HEIGHT; y++)
		for (int x = 0; x < MAP_WIDTH; x += MAP_WIDTH - 1)
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
		int left = monsters[0].x - (SCREEN_WIDTH / 2);
		if (left < 0) {
			left = 0;
		}
		if (left > MAP_WIDTH - SCREEN_WIDTH)
			left = MAP_WIDTH - SCREEN_WIDTH;

		for (int y = 0; y < MAP_HEIGHT; y++) {
			for (int x = left; x < left + SCREEN_WIDTH; x++) {
				int monster = getMonster(x, y);
				int bomb = getBomb(x, y);
				if (monster > 0)
					printf("敵");
				else if (monster == 0)
					printf("＠");
				else if (bomb >= 0) {
					char aa[] = "０";

					aa[1] += bombs[bomb].count;
					printf(aa);
				}
				else
					printf(cellAA[cells[y][x]]);
			}
			printf("\n");
		}
		for (int y = 1; y < MAP_HEIGHT - 1; y++)
			for (int x = 1; x < MAP_WIDTH - 1; x++) {
				if (cells[y][x] == CELL_TYPE_EXPLOSION)
					cells[y][x] = CELL_TYPE_NONE;
			}
		int x = monsters[0].x;
		int y = monsters[0].y;
		switch (_getch()) {
		case 'w':y--; break;
		case 's':y++; break;
		case 'a':x--; break;
		case 'd':x++; break;
		case ' ':
			int bomb = getFreeBomb();
			if (bomb >= 0) {
				bombs[bomb].x = monsters[0].x;
				bombs[bomb].y = monsters[0].y;
				bombs[bomb].count = BOMB_COUNT_MAX;
			}break;
		}
		if (checkCanMove(x, y)) {
			monsters[0].x = x;
			monsters[0].y = y;
		}
		for (int i = 0; i < BOMB_MAX; i++) {
			if (bombs[i].count <= 0)
				continue;
			bombs[i].count--;
			if (bombs[i].count <= 0)
				//cells[bombs[i].y][bombs[i].x] = CELL_TYPE_EXPLOSION; 
				explosion(bombs[i].x, bombs[i].y);//爆破
		}
		
			for (int i = 1; i < MONSTER_MAX; i++) {
				if (monsters[i].isDead)
					continue;
				int x = monsters[i].x + directions[monsters[i].direction][0];
				int y = monsters[i].y + directions[monsters[i].direction][1];
				if (checkCanMove(x, y)) {
					monsters[i].x = x;
					monsters[i].y = y;
				}
				else
					monsters[i].direction = rand() % DIRECTION_MAX;
			
			}
	}
}