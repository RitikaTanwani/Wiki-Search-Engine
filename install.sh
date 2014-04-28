#!/bin/bash
g++ -w -O3 BUILD_INDEX.cpp porter.c -o searcher 
g++ -w -O3 MERGE.cpp -o merger

rm -rf 201101153_split_files
mkdir -p 201101153_split_files
cd 201101153_split_files
split -b 50M --suffix-length=4 ../$1
cd ..

python quantizing.py "201101153_split_files/"

rm -rf INDEXES_FOLDER
mkdir -p INDEXES_FOLDER


