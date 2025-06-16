OpenDBViewer
============

OpenDBViewer is a simple database explorer for Windows, Linux and Mac OS X.

- Github: https://github.com/Jet1oeil/opendbviewer
- Developer website: http://www.jet1oeil.com/
- Bugs reporting: https://github.com/Jet1oeil/opendbviewer/issues

Features
--------
- Compatibility with SQLITE, MySQL, PostgreSQL database.
- SQL syntax highlightings
- Multi database, table and worksheet view
- Big database loading
- Open SQLite table directly with drag'n drop
- History of last connection
- Edit database directly from the table view

License
-------

This program is licensed under the terms of the GNU GENERAL PUBLIC LICENSE Version 3.

Requirements
------------

- CMake
- Qt version 4, 5 or 6
- ImageMagick

Compiling from source
---------------------

> for Archlinux users, a package [`opendbviewer-git`](https://aur.archlinux.org/packages/opendbviewer-git) is available in AUR

Install dependencies:
  * in debian OS (with Qt4) :
```
    apt-get install git cmake g++ imagemagick libqt4-dev libqt4-sql-sqlite libqt4-sql-mysql libqt4-sql-psql
```
  * in debian OS (with Qt5) :
```
    apt-get install git cmake g++ imagemagick qtbase5-dev qttools5-dev qttools5-dev-tools libqt5sql5-sqlite libqt5sql5-mysql libqt5sql5-psql
```
  * in debian OS (with Qt6) :
```
    apt-get install git cmake g++ imagemagick qt6-base-dev qt6-tools-dev qt6-base-dev-tools
 
```
  * in fedora OS : 
```
    dnf install cmake ImageMagick gcc-c++ qt5-devel qt5-qtbase-mysql qt5-qtbase-postgresql
```
Get the code:

    git clone https://github.com/Jet1oeil/opendbviewer.git && cd opendbviewer

Compile:

    ./build.sh regen
    make

Run:

    ./opendbviewer

Screenshot
----------

![alt text](https://raw.githubusercontent.com/Jet1oeil/opendbviewer/master/doc/opendbviewer-screenshot-1.png)
![alt text](https://raw.githubusercontent.com/Jet1oeil/opendbviewer/master/doc/opendbviewer-screenshot-2.png)
![alt text](https://raw.githubusercontent.com/Jet1oeil/opendbviewer/master/doc/opendbviewer-screenshot-3.png)
