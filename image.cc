#include <string>
#include <png++/png.hpp>
#include <sys/types.h>
#include <sys/stat.h>

#include "image.h"

bool Image::load()
{
    struct stat filestat;
    std::string path = _dir + "/" + _filename;
    if (stat(path.c_str(), &filestat)) return false;
    if (S_ISDIR(filestat.st_mode)) return false;

    try {
        _img = png::image<png::rgba_pixel>(path.c_str());
        _w = _img.get_width();
        _h = _img.get_height();
    } catch(std::exception &e) {
        std::cout << e.what() << std::endl;
        return false; 
    }
    return true;
}
