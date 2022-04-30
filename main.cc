#include "app.h"


int main(int argc, char *argv[])
{
    if (App::instance().parse_args(argc, argv)) {
        App::instance().usage();
        return 1;
    }
    App::instance().run();

    return 0;
}
