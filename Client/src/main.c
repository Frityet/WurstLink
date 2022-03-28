#include <stdio.h>
#include <emscripten.h>

#include "logger.h"
#include "wurstlink_types.h"

//Located in game/, networking/, and rendering/
extern void game_setup(void),   *game_main(void *),
            net_setup(void),    *net_main(void *),
            render_setup(void), render_main(void);



int main()
{
    LOG_INFO("Started WurstLink");

    render_setup(), net_setup(), game_setup();

    //pthreads don't work unless you configure your HTTP server for it
//    pthread_create(&networking_thread, NULL, net_main, NULL);
//    pthread_create(&game_thread, NULL, game_main, NULL);

    net_main(NULL), game_main(NULL);

    //Rendering must run on the main thread or else it breaks.
    emscripten_set_main_loop(render_main, 0, false);
}