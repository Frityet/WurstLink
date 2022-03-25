#include <stdio.h>

extern void game_main(void), net_main(void), render_main(void);

int main()
{
    puts("Hello, World!");

    game_main();
    net_main();
    render_main();
}