#pragma once

#include "types.h"
#include "common.h"

#include <SDL/SDL_video.h>

struct Renderer {
	struct SDL_Window *window;
};

enum RendererInitialisationError {
	RendererInitialisationError_NONE,
	RendererInitialisationError_SDL_WINDOW
};

enum RendererInitialisationError Renderer_initialise(struct Renderer *rend);
