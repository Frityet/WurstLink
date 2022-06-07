#include "renderer.h"

#include <SDL/SDL.h>

static constructor void Renderer_sdl_initialise()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
}

enum RendererInitialisationError Renderer_initialise(struct Renderer *rend)
{
	rend->window = SDL_CreateWindow("WurstLink", 
									SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
									1280, 720, 
									SDL_WINDOW_OPENGL);

	return RendererInitialisationError_NONE;
}
