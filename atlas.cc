#include <list>
#include <sstream>
#include <png++/png.hpp>
#include <tinyxml.h>

#include "atlas.h"
#include "image.h"


bool height_sort(Image first, Image second)
{
    if (first.get_height() >= second.get_height()) return true;
    else return false;
}


class Pred{
    private:
        int _w, _h;
    public:
        Pred(int w, int h) : _w(w), _h(h) {}

        bool operator()(const Image &img) {
            if (img.get_height() > _h || img.get_width() > _w) return true;
            else return false;
        }
};


Atlas::Atlas(std::string prefix, int w, int h, std::list<Image> images)
: _w(w), _h(h), _prefix(prefix), _images(images)
{
    _images.remove_if(Pred(w, h));
    _images.sort(height_sort);
}


int Atlas::run()
{
    int n = 0;
    TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "", "");
    doc.LinkEndChild(decl);
    root = new TiXmlElement("packet");
    doc.LinkEndChild(root);
    do {
        std::stringstream s;
        s << n;
        _images = process(_prefix + s.str());
        n++;
    } while(_images.size() > 0);
    doc.SaveFile(_prefix  +".xml");
    return 0;
}


std::list<Image> Atlas::process(std::string filename)
{
    int x = 0, y = 0, level_h;
    std::list<Image>::iterator it;
    std::list<Image> temp;
    // FFDH
    level_h = _images.front().get_height();
    for (it = _images.begin(); it != _images.end(); ++it) {
        if (x + (*it).get_width() <= _w) {
            (*it).set_x(x);
            (*it).set_y(y);
            x += (*it).get_width();
            temp.push_back(*it);
        } else {
            std::list<Image>::iterator it1 = it;
            for (++it; it != _images.end(); ++it) {
                if (x + (*it).get_width() <= _w) {
                    (*it).set_x(x);
                    (*it).set_y(y);
                    x += (*it).get_width();
                    temp.push_back(*it);
                } 
            }
            it = it1;
            x = 0;
            y += level_h;
            if ((y + (*it).get_height() <= _h) && (x + (*it).get_width() <= _w)) {
                (*it).set_x(x);
                (*it).set_y(y);
                level_h = (*it).get_height();
                x += (*it).get_width();
                temp.push_back(*it);
            } else break;
        }
    }
    // end FFDH

    png::image<png::rgba_pixel> atlas(_w, _h);
    for (it = temp.begin(); it != temp.end(); ++it) {
        png::image<png::rgba_pixel> *img = (*it).get_png();
        for (int i = 0; i < (*it).get_height(); i++) {
            for (int j = 0; j < (*it).get_width(); j++) {
                int x1 = (*it).get_x();
                int y1 = (*it).get_y();
                png::rgba_pixel px = (*img)[i][j];
                atlas[y1+i][x1+j] = px;
            }
        }
        TiXmlElement *element = new TiXmlElement("layer");
        element->SetAttribute("name", (*it).get_filename());
        element->SetAttribute("width", (*it).get_width());
        element->SetAttribute("height", (*it).get_height());
        element->SetAttribute("atlas", filename + ".png");
        double u0 = static_cast<double> ((*it).get_x()) / _w;
        double v0 = static_cast<double> ((*it).get_y()) / _h;
        double u1 = static_cast<double> ((*it).get_x() + (*it).get_width()) / _w;
        double v1 = static_cast<double> ((*it).get_y() + (*it).get_height()) / _h;
        element->SetDoubleAttribute("u0", u0);
        element->SetDoubleAttribute("v0", v0);
        element->SetDoubleAttribute("u1", u1);
        element->SetDoubleAttribute("v1", v1);
        root->LinkEndChild(element);
        _images.remove(*it);
    }
    atlas.write(filename + ".png");

    return _images;
}
