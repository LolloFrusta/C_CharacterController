#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char** argv) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return -1;
	}
	SDL_Log("SDL is active!");

	int w_width = 1024;
	int w_height = 512;

	SDL_Window* window = SDL_CreateWindow("SDL is active!", 100, 100, w_width, w_height, 0);
	if (!window) {
		SDL_Log("Unable to create window: %s", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		SDL_Log("Unable to create renderer: %s", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	int width;
	int heigth;
	int channel;
	unsigned char* pixels = stbi_load("D:/AIV/act.png", &width, &heigth, &channel, 4);
	if (!pixels) {
		SDL_Log("Unable to open image: %s", stbi__g_failure_reason);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, heigth);
	if (!texture) {
		SDL_Log("Unable to create texture: %s", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	SDL_UpdateTexture(texture, NULL, pixels, width * 4);

	SDL_Vertex vertex[4];
	vertex[0].position.x = 50;
	vertex[0].position.y = 50;
	vertex[0].color.r = 255;
	vertex[0].color.g = 255;
	vertex[0].color.b = 255;
	vertex[0].color.a = 255;
	vertex[0].tex_coord.x = 0;
	vertex[0].tex_coord.y = 0;
	vertex[1].position.x = 200;
	vertex[1].position.y = 50;
	vertex[1].color.r = 255;
	vertex[1].color.g = 255;
	vertex[1].color.b = 255;
	vertex[1].color.a = 255;
	vertex[1].tex_coord.x = 1;
	vertex[1].tex_coord.y = 0;
	vertex[2].position.x = 50;
	vertex[2].position.y = 200;
	vertex[2].color.r = 255;
	vertex[2].color.g = 255;
	vertex[2].color.b = 255;
	vertex[2].color.a = 255;
	vertex[2].tex_coord.x = 0;
	vertex[2].tex_coord.y = 1;
	vertex[3].position.x = 200;
	vertex[3].position.y = 200;
	vertex[3].color.r = 255;
	vertex[3].color.g = 255;
	vertex[3].color.b = 255;
	vertex[3].color.a = 255;
	vertex[3].tex_coord.x = 1;
	vertex[3].tex_coord.y = 1;

	int index[6] = { 0, 1, 2, 1, 3, 2 };

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.h = 64;
	rect.w = 64;

	char title[100];
	Uint64 curr_count = SDL_GetPerformanceCounter();
	Uint64 last_count = curr_count;
	float delta_time = 0;
	float update_time = 0;
	const int speed = 500;

	int running = 1;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = 0;
			}
			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_a)
				{
					if (rect.x > 0)
					{
						rect.x -= speed * delta_time;
					}
				}
				if (event.key.keysym.sym == SDLK_d)
				{
					if (rect.x + rect.w < w_width)
					{
						rect.x += speed * delta_time;
					}
				}
				if (event.key.keysym.sym == SDLK_s)
				{
					if (rect.y + rect.h < w_height)
					{
						rect.y += speed * delta_time;
					}
				}
				if (event.key.keysym.sym == SDLK_w)
				{
					if (rect.y > 0)
					{
						rect.y -= speed * delta_time;
					}
				}
			}
		}

		last_count = curr_count;
		curr_count = SDL_GetPerformanceCounter();
		delta_time = (float)(curr_count - last_count) / (float)SDL_GetPerformanceFrequency();
		int fps = (int)(1.f / delta_time);

		update_time += delta_time;
		if (update_time >= 1)
		{
			update_time -= 1;
			sprintf_s(title, sizeof(title), "Delta Time: %.6f - FPS: %d", delta_time, fps);
			SDL_SetWindowTitle(window, title);
		}

		//SDL_RenderGeometry(renderer, texture, vertex, 4, index, 6);
		SDL_RenderCopy(renderer, texture, NULL, &rect);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
