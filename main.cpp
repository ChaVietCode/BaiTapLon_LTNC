#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include <cstdlib>
#include <ctime>
using namespace std;

// Screen dimension constants
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// Current displayed texture
SDL_Texture *gTexture = NULL;
SDL_Surface *gScreenSurface = NULL;

int step = 10;
int snakeBodyLength = 3;
bool gameContinue = true;
bool gameOver = false;

enum
{
	RIGHT,
	LEFT,
	UP,
	DOWN
};

void snakeMove(SDL_Rect &snakeHead, SDL_Rect *snakeBody, SDL_Rect &point, int &snakeBodyLength, int direction);

void renderImage();
// void renderImage(string path);

void playGame();

int main(int argc, char *args[])
{
	gWindow = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

	playGame();

	return 0;
}

void snakeMove(SDL_Rect &snakeHead, SDL_Rect *snakeBody, SDL_Rect &point, int &snakeBodyLength, int direction)
{
	// di chuyển phần thân
	for (int i = snakeBodyLength - 1; i > 0; i--)
	{
		snakeBody[i].x = snakeBody[i - 1].x;
		snakeBody[i].y = snakeBody[i - 1].y;
	}
	snakeBody[0].x = snakeHead.x;
	snakeBody[0].y = snakeHead.y;

	if (direction == LEFT)
	{
		snakeHead.x = (snakeHead.x + SCREEN_WIDTH - step) % SCREEN_WIDTH;
	}
	else if (direction == RIGHT)
	{
		snakeHead.x = (snakeHead.x + step) % SCREEN_WIDTH;
	}
	else if (direction == DOWN)
	{
		snakeHead.y = (snakeHead.y + step) % SCREEN_HEIGHT;
	}
	else if (direction == UP)
	{
		snakeHead.y = (snakeHead.y + SCREEN_HEIGHT - step) % SCREEN_HEIGHT;
	}

	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer);

	// ve lai cai dau
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
	SDL_RenderFillRect(gRenderer, &snakeHead);
	SDL_SetRenderDrawColor(gRenderer, 102, 255, 51, 0xFF);
	for (int i = 0; i < snakeBodyLength; i++)
	{
		SDL_RenderFillRect(gRenderer, &snakeBody[i]);
	}
	if (snakeHead.x == point.x && snakeHead.y == point.y)
	{
		// chuyển point ra chỗ khác
		point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
		point.y = (rand() % (SCREEN_WIDTH / 10)) * 10;
		snakeBodyLength++;
		snakeBody[snakeBodyLength - 1].x = snakeBody[snakeBodyLength - 2].x;
		snakeBody[snakeBodyLength - 1].y = snakeBody[snakeBodyLength - 2].y;
	}
	// vẽ lại cái để ăn
	SDL_SetRenderDrawColor(gRenderer, 255, 51, 0, 0xFF);
	SDL_RenderFillRect(gRenderer, &point);

	SDL_RenderPresent(gRenderer);
	SDL_Delay(100);
}

// void renderImage(string path)
void renderImage()
{
	SDL_Surface *loadedSurface = IMG_Load("src/image/GameOver.png");
	// SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	SDL_RenderCopy(gRenderer, texture, NULL, NULL);
	SDL_RenderPresent(gRenderer);
}

void playGame()
{
	// tạo phần đầu
	SDL_Rect snakeHead;
	snakeHead.x = SCREEN_HEIGHT / 2;
	snakeHead.y = SCREEN_WIDTH / 2;
	snakeHead.w = 10;
	snakeHead.h = 10;
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
	SDL_RenderFillRect(gRenderer, &snakeHead);

	// tạo phần thân
	int snakeMaxLength = SCREEN_HEIGHT * SCREEN_WIDTH / 100;
	SDL_Rect snakeBody[snakeMaxLength];
	for (int i = 0; i < snakeMaxLength; i++)
	{
		snakeBody[i].w = 10;
		snakeBody[i].h = 10;
	}

	snakeBody[0].x = snakeHead.x - 10;
	snakeBody[0].y = snakeHead.y;
	snakeBody[1].x = snakeHead.x - 20;
	snakeBody[1].y = snakeHead.y;
	snakeBody[2].x = snakeHead.x - 30;
	snakeBody[2].y = snakeHead.y;
	SDL_SetRenderDrawColor(gRenderer, 102, 255, 51, 0xFF);
	SDL_RenderFillRect(gRenderer, &snakeBody[0]);
	SDL_RenderFillRect(gRenderer, &snakeBody[1]);
	SDL_RenderFillRect(gRenderer, &snakeBody[2]);

	// tạo điểm ngẫu nhiên để ăn
	SDL_Rect point;
	point.w = 10;
	point.h = 10;
	srand(time(NULL));
	point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
	point.y = (rand() % (SCREEN_WIDTH / 10)) * 10;

	SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 0xFF);
	SDL_RenderFillRect(gRenderer, &point);

	SDL_RenderPresent(gRenderer);

	int direction = RIGHT;

	bool quit = false;
	SDL_Event e;
	while (!quit)
	{
		if (e.type == SDL_QUIT)
		{
			quit = true;
			break;
		}
		snakeMove(snakeHead, snakeBody, point, snakeBodyLength, direction);

		// nếu đâm vào đuôi
		for (int i = 0; i < snakeBodyLength; i++)
		{
			if (snakeHead.x == snakeBody[i].x && snakeHead.y == snakeBody[i].y)
			{
				snakeBodyLength = 3;
				SDL_Event event;
				bool quit2 = false;
				while (!quit)
				{
					renderImage();
					if (SDL_WaitEvent(&event))
					{
						if (event.type == SDL_QUIT)
						{
							quit = true;
						}
						if (event.key.keysym.sym == SDLK_y)
						{
							playGame();
						}
						if (event.key.keysym.sym == SDLK_n)
						{
							quit = true;
							break;
						}
					}
				}
			}
		}

		// Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{

			// User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
				break;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				// dua phan truoc ve vi tri sau
				for (int i = snakeBodyLength - 1; i > 0; i--)
				{
					snakeBody[i].x = snakeBody[i - 1].x;
					snakeBody[i].y = snakeBody[i - 1].y;
				}
				snakeBody[0].x = snakeHead.x;
				snakeBody[0].y = snakeHead.y;
				if (direction == LEFT)
				{
					if (e.key.keysym.sym == SDLK_DOWN)
					{
						direction = DOWN;
					}
					if (e.key.keysym.sym == SDLK_UP)
					{
						direction = UP;
					}
				}
				if (direction == RIGHT)
				{

					if (e.key.keysym.sym == SDLK_DOWN)
					{
						direction = DOWN;
					}
					if (e.key.keysym.sym == SDLK_UP)
					{
						direction = UP;
					}
				}
				if (direction == UP)
				{
					if (e.key.keysym.sym == SDLK_LEFT)
					{
						direction = LEFT;
					}
					// Tương tự với dịch phải, xuống và lên
					if (e.key.keysym.sym == SDLK_RIGHT)
					{
						direction = RIGHT;
					}
				}
				if (direction == DOWN)
				{
					if (e.key.keysym.sym == SDLK_LEFT)
					{
						direction = LEFT;
					}
					// Tương tự với dịch phải, xuống và lên
					if (e.key.keysym.sym == SDLK_RIGHT)
					{
						direction = RIGHT;
					}
				}

				if (snakeHead.x == point.x && snakeHead.y == point.y)
				{
					// chuyển point ra chỗ khác
					point.x = (rand() % (SCREEN_HEIGHT / 10)) * 10;
					point.y = (rand() % (SCREEN_WIDTH / 10)) * 10;
					snakeBodyLength++;
					snakeBody[snakeBodyLength - 1].x = snakeBody[snakeBodyLength - 2].x;
					snakeBody[snakeBodyLength - 1].y = snakeBody[snakeBodyLength - 2].y;
				}
				// vẽ lại cái để ăn
				SDL_SetRenderDrawColor(gRenderer, 255, 51, 0, 0xFF);
				SDL_RenderFillRect(gRenderer, &point);
				snakeMove(snakeHead, snakeBody, point, snakeBodyLength, direction);
			}
		}
	}
}