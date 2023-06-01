#ifndef _TETRIS_H
#define _TETRIS_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <fstream>

using namespace std;
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 800;
const string TITLE = "TETRIS";


class Tetris
{
private:
	enum { blockW = 30, blockH = 30 };
	enum { line = 20, col = 10 };
	SDL_Texture* background = NULL, * block = NULL;

	SDL_Texture* backgroundMenu = NULL;
	SDL_Texture* tableImage = NULL;
	SDL_Texture* exit = NULL;
	SDL_Rect exitRect = { 0, 0, 54, 54 };
	SDL_Texture* english = NULL;
	SDL_Texture* russian = NULL;
	SDL_Texture* play = NULL;
	SDL_Texture* play_light = NULL;
	SDL_Texture* record = NULL;
	SDL_Texture* record_light = NULL;
	SDL_Texture* rule = NULL;
	SDL_Texture* rule_light = NULL;
	int language = 1; // 1-eng, 2-rus
	fstream myFile;
	int recordTable[5] = {0};

	SDL_Rect sRect = { 0, 0, blockW, blockH }, dRect = { 0, 0, blockW, blockH };
	SDL_Rect sFutRect = { 0, 0, blockW, blockH }, dFutRect = { 0, 0, blockW, blockH };

	SDL_Rect srcTextRect;
	SDL_Rect desTextRect;
	SDL_Texture* Text;

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	int score = 0;
	int field[line][col] = { 0 };
	static const int figure[7][4];
	struct point
	{
		int x, y;
	}item[4], futitem[4], backup[4];

	Uint32 currentTime = 0, lastTime = 0;
	int delay = 0;

	bool running = false;
	bool rotate = false;
	int move = 0;
	int color = 1;
	int futcolor = 1;

public:
	bool isRunning()
	{
		return running;
	}
	bool init();
	bool menu();
	bool showRecord();
	bool showRule();
	void reset();

	bool selectRect(SDL_Rect& rect, const int x, const int y);

	void handleEvents();
	void makeTetromino(SDL_Rect& rect, int x = 0, int y = 0, int w = blockW, int h = blockH);
	void nextTetromino();
	void futureTetromino();
	void gameplay();
	void clean();
	void setCurrentTime(Uint32 t);
	bool checkCollision();
	void updateRenderer();
	void updateField(SDL_Rect& rect, int x, int y);
	void checkLine();
	void checkGameOver();
	void makeText(string text, int x, int y, SDL_Color color);
	SDL_Texture* loadTexture(string path);
	void quitSDL();
};

#endif // _TETRIS_H