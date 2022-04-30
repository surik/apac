#include <iostream>
#include <list>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>

#include "app.h"
#include "image.h"
#include "atlas.h"


void App::usage(void)
{
    std::cout <<
        "Usage: apac [options]" << std::endl <<
        "  -?       Display this help screen." << std::endl <<
        "  -h       Set atlas height." << std::endl <<
        "  -w       Set atlas width" << std::endl <<
        "  -s       Set atlas size (height and width)" << std::endl <<
        "  -d       Set directory for read images(png only)." << std::endl <<
        "  -o       Set output prefix for .xml and .png." << std::endl <<
        std::endl <<
        "Example:" << std::endl <<
        "./apac -o pac -s 1024 -d ./img" << std::endl;
}


int App::parse_args(int argc, char *argv[])
{
    int c;

    // default values
    _atlas_w = _atlas_h = 1024;
    _dir = "./";
    _prefix = "apac";

    while ((c = getopt(argc, (char **)argv, "h:w:d:s:f:o:?")) != -1) {
        switch (c) {
            case 'h':
                _atlas_h = atoi(optarg);
                break;
            case 'w':
                _atlas_w = atoi(optarg);
                break;
            case 's':
                _atlas_w = _atlas_h = atoi(optarg);
                break;
            case 'd':
                _dir = std::string(optarg);
                break;
            case 'o':
                _prefix = std::string(optarg);
                break;
            case '?':
                usage();
                exit(1);
            default:
                break;
        }
    }
    return 0;
}


std::list<Image> App::get_images()
{
    DIR *dfd;
    struct dirent *dp;
    std::list<Image> files;

    dfd = opendir(_dir.c_str());
    if (dfd == NULL) {
        std::cerr << "Error oppening " << _dir << std::endl;
        return files;
    }
    while ((dp = readdir(dfd))) {
        Image img(_dir, dp->d_name);
        if (img.load())
            files.push_back(img);
    }

    closedir(dfd);

    return files;
}


int App::run()
{
    std::list<Image> images;
    images = get_images();

    std::string path = _prefix.substr(0, _prefix.rfind("/"));
    Atlas atlas(path, _atlas_w, _atlas_h, images);
    if (atlas.get_images().size() > 0) {
        mkpath(path.c_str(), 0755);
        atlas.run();
    } else
        std::cerr << "No valid PNG files in " << _dir << std::endl;

    return 0;
}


int App::mkpath(const char *s, mode_t mode)
{
    char *q, *r = NULL, *path = NULL, *up = NULL;
    int rv;

    rv = -1;
    if (strcmp(s, ".") == 0 || strcmp(s, "/") == 0)
        return (0);

    if ((path = strdup(s)) == NULL)
        exit(1);

    if ((q = strdup(s)) == NULL)
        exit(1);

    if ((r = dirname(q)) == NULL)
        goto out;

    if ((up = strdup(r)) == NULL)
        exit(1);

    if ((mkpath(up, mode) == -1) && (errno != EEXIST))
        goto out;

    if ((mkdir(path, mode) == -1) && (errno != EEXIST))
        rv = -1;
    else
        rv = 0;

out:
    if (up != NULL)
        free(up);
    free(q);
    free(path);
    return (rv);
}
