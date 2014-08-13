#!/bin/sh

rm -rf "mgf_save";
tar cf "mgf_save" libs res gl_test.h main.cpp mgf.cbp save.sh

# date +'%m-%d-%Y//%r'