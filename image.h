#ifndef IMAGE_H

#define IMAGE_H

#include <string>
#include <png++/png.hpp>

class Image {
    private:
        int _h, _w, _x, _y, _fit;
        std::string _dir, _filename;
        png::image<png::rgba_pixel> _img;

    public:
        Image(std::string dir, std::string filename) : 
            _dir(dir), _filename(filename), _fit(false) {}
        bool load();

        png::image<png::rgba_pixel> *get_png() { return &_img; }
        int get_width() const { return _w; }
        int get_height() const { return _h; }
        int get_x() const { return _x; }
        int get_y() const { return _y; }
        void set_x(double x) { _x = x; }
        void set_y(double y) { _y = y; }
        std::string get_dir() const { return _dir; }
        std::string get_filename()  const { return _filename; }
        std::string get_path() const { return (_dir + "/" + _filename); }
        bool get_fit() { return _fit; }
        void set_fit() { _fit = true; }

        bool operator==(const Image& r) const {
            if (this->get_path().compare(r.get_path()) == 0) return true;
            else return false;
        }
};

#endif /* end of include guard: IMAGE_H */
