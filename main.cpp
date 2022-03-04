#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include <cstdlib>
#include <ctime>

// Screen dimension constants
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// Current displayed texture
SDL_Texture *gTexture = NULL;

int main(int argc, char *args[])
{
	gWindow = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

	// tạo phần đầu
	SDL_Rect snakeHead;
	snakeHead.x = SCREEN_HEIGHT / 2;
	snakeHead.y = SCREEN_WIDTH / 2;
	snakeHead.w = 10;
	snakeHead.h = 10;
	SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
	SDL_RenderFillRect(gRenderer, &snakeHead);

	//tạo phần thân
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
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
	SDL_RenderFillRect(gRenderer, &snakeBody[0]);
	SDL_RenderFillRect(gRenderer, &snakeBody[1]);
	SDL_RenderFillRect(gRenderer, &snakeBody[2]);

	int step = 10;
	int snakeBodyLength = 3;

	// tạo điểm ngẫu nhiên để ăn
	SDL_Rect point;
	point.w = 10;
	point.h = 10;
	srand(time(NULL));
	point.x = (rand() % (SCREEN_HEIGHT / 10 + 1)) * 10;
	point.y = (rand() % (SCREEN_WIDTH / 10 + 1)) * 10;

	SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 0xFF);
	SDL_RenderFillRect(gRenderer, &point);

	SDL_RenderPresent(gRenderer);

	bool quit = false;
	SDL_Event e;
	while (!quit)
	{
		// Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			// User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_KEYDOWN)
			{
				// dua phan truoc ve vi tri sau
				for (int i = snakeBodyLength - 1; i > 0; i--)
				{
					snakeBody[i].x = snakeBody[i - 1].x;
					snakeBody[i].y = snakeBody[i - 1].y;
				}
				snakeBody[0].x = snakeHead.x;
				snakeBody[0].y = snakeHead.y;



				snakeHead.x = (snakeHead.x + SCREEN_WIDTH - step) % SCREEN_WIDTH;
				

				if (e.key.keysym.sym == SDLK_LEFT)
				{
					snakeHead.x = (snakeHead.x + SCREEN_WIDTH - step) % SCREEN_WIDTH;
				}
				// Tương tự với dịch phải, xuống và lên
				if (e.key.keysym.sym == SDLK_RIGHT)
				{
					snakeHead.x = (snakeHead.x + step) % SCREEN_WIDTH;
				}
				if (e.key.keysym.sym == SDLK_DOWN)
				{
					snakeHead.y = (snakeHead.y + step) % SCREEN_HEIGHT;
				}
				if (e.key.keysym.sym == SDLK_UP)
				{
					snakeHead.y = (snakeHead.y + SCREEN_HEIGHT - step) % SCREEN_HEIGHT;
				}

				// xoa het man hinh
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
				SDL_RenderClear(gRenderer);

				// ve lai cai dau
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
				SDL_RenderFillRect(gRenderer, &snakeHead);
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);

				// nếu con rắn đến chỗ cái để ăn
				if (snakeHead.x == point.x && snakeHead.y == point.y)
				{
					// chuyển point ra chỗ khác
					point.x = (rand() % (SCREEN_HEIGHT / 10 + 1)) * 10;
					point.y = (rand() % (SCREEN_WIDTH / 10 + 1)) * 10;
					snakeBodyLength++;
					snakeBody[snakeBodyLength-1].x=snakeBody[snakeBodyLength-2].x;
					snakeBody[snakeBodyLength-1].y=snakeBody[snakeBodyLength-2].y;
				}

				// ve lai phan than
				for (int i = 0; i < snakeBodyLength; i++)
				{
					SDL_RenderFillRect(gRenderer, &snakeBody[i]);
				}

				// vẽ lại cái để ăn
				SDL_SetRenderDrawColor(gRenderer, 0, 255, 255, 0xFF);
				SDL_RenderFillRect(gRenderer, &point);

				SDL_RenderPresent(gRenderer);
			}
		}
	}
	return 0;
}