#!/bin/sh

rm -rf "mgf_save";
tar cf "mgf_save" src res libs gl_test.h main.cpp mgf.cbp save.sh

# date +'%m-%d-%Y//%r'