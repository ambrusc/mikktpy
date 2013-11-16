mikktpy
=======

A simple Python 2.7 wrapper for mikktspace. Only supports unindexed triangles!

(http://wiki.blender.org/index.php/Dev:Shading/Tangent_Space_Normal_Maps)

Prebuilt Binary
-----
(https://www.dropbox.com/s/qgxtcfrwfw6rb3g/mikktpy.zip)


Build
-----
Only tested on Windows. You'll need SWIG and MSVC++ 2008 (express version is okay)

    python setup.py build
    copy build\lib.win32-2.7\_mikktspace.pyd .
    python test.py
