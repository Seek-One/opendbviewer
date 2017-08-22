OpenDBViewer
============

OpenDBViewer is a simple database explorer for Windows, Linux and Max OS X.

- Github: https://github.com/Jet1oeil/opendbviewer
- Developer website: http://www.jet1oeil.com/
- Bugs reporting: https://github.com/Jet1oeil/opendbviewer/issues

Features
--------
- Compatibility with SQLITE and MySQL database.
- SQL syntax highlithings
- Multi database, table and worksheet view

This program is licenced under the GNU GENERAL PUBLIC LICENSE Version 3.

Requirements
------------

- Qt version 5

Compiling from source
---------------------

Install dependencies:

    apt-get install qtbase5-dev

Get the code:

    git clone https://github.com/Jet1oeil/opendbviewer.git && cd opendbviewer

Compile:

    ./build.sh regen
    make

Run:

    ./opendbviewer
