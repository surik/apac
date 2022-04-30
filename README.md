# Atlas packer

Some code from 2013.

This application takes PNGs from a given directory and packs them into a package.

A Package contains an XML and a set of PNG atlases.  The XML contains references to original images in the atlases.

## Usage

Put some png files to `./img` and then run

    $ make docker
    $ docker run -v`pwd`:/root apac -o package -s 1024 -d ./img

It will generate `package.xml` and a set of `package*.png` files in current directory.
