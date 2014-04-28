#!/bin/bash
for file in `ls 201101153_split_files/`
do
  ./searcher "201101153_split_files/$file" 
done

./merger "./INDEXES_FOLDER/"  $2
   
