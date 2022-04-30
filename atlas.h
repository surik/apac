#ifndef ATLAS_H

#define ATLAS_H

#include <list>
#include <string>
#include <tinyxml.h>

#include "image.h"

class Atlas
{
    private:
        int _w, _h;
        std::string _prefix;
        std::list<Image> _images;
        TiXmlDocument doc;
        TiXmlElement *root;
        std::list<Image> process(std::string filename);

    public:
        Atlas(std::string prefix, int w, int h, std::list<Image> images);
        std::list<Image> get_images() { return _images; }
        int run();
};

#endif /* end of include guard: ATLAS_H */
