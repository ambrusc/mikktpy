#
# Copyright (c) 2013 Ambrus Csaszar
#

from distutils.core import Extension, setup

mikktspace = Extension(
  '_mikktspace', 
  sources=['mikktpy.i', 'mikktpy.cpp', 'mikktspace.c'],
  swig_opts=['-modern', '-c++'],
  extra_compile_args=['/EHsc'])

setup (name = 'MikkTSpace',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [mikktspace])
