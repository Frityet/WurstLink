#include <stdio.h>
#include <emscripten.h>
#include <pthread.h>

#include "logger.h"
#include "worker.h"

//Located in game/, networking/, and rendering/
extern void game_setup(int, const void *),  game_main(int, const void *),
            net_setup(int, const void *),   net_main(int, const void *),
            render_setup(void),             render_main(void);

int main()
{
    LOG_INFO("Started WurstLink");

    struct Worker   networking_worker   = Worker(NULL, NULL),
                    game_worker         = Worker(NULL, NULL);

    Worker$add_task(&networking_worker, Task(&net_setup));
    Worker$add_task(&game_worker, Task(&game_setup));

    Worker$add_task(&networking_worker, Task(&net_main));
    Worker$add_task(&game_worker, Task(&game_main));

    emscripten_set_main_loop(render_main, 0, false);
}