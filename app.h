#ifndef APP_H

#define APP_H

#include <string>
#include <list>

#include "image.h"

class App {
    private:
        App() {};
        App(const App& root);
        App& operator=(const App&);
        std::string _dir, _prefix;
        int _atlas_w, _atlas_h;
        std::list<Image> get_images();

    public:
        int run();
        int mkpath(const char *s, mode_t mode);
        int parse_args(int argc, char *argv[]);
        void usage(void);
        static App &instance() {
            static App _instance;
            return _instance;
        }
};

#endif
