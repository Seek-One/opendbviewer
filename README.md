OpenDBViewer
============

OpenDBViewer is a simple database explorer for Windows, Linux and Max OS X.

- Github: https://github.com/Jet1oeil/opendbviewer
- Developer website: http://www.jet1oeil.com/
- Bugs reporting: https://github.com/Jet1oeil/opendbviewer/issues

Features
--------
- Compatibility with SQLITE, MySQL, PostgreSQL database.
- SQL syntax highlightings
- Multi database, table and worksheet view

License
-------

This program is licenced under the terms of the GNU GENERAL PUBLIC LICENSE Version 3.

Requirements
------------

- Qt version 4 or Qt version 5

Compiling from source
---------------------

Install dependencies:

    apt-get install libqt4-dev libqt4-sql-sqlite libqt4-sql-mysql libqt4-sql-psql

Or:

    apt-get install qtbase5-dev qttools5-dev-tools libqt5sql5-sqlite libqt5sql5-mysql libqt5sql5-psql

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
