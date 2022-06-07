#include <stdio.h>

#include "rendering/renderer.h"
#include "logger.h"

int main()
{
	LOG_INFO("Started!");

	struct Renderer rend;
	enum RendererInitialisationError err;
	if (!(err = Renderer_initialise(&rend))) {
		LOG_FATAL("Could not initialise renderer! Error: %d", err);
		return EXIT_FAILURE;
	}
}
