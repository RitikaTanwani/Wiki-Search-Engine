#!/bin/bash
./install.sh $1
./run.sh file1 MYINDEX_FILE
g++ query.cpp porter.c -w
./a.out SEC PRIMARY MYINDEX_FILE 
