#!/bin/bash

for i in $(ls -p | egrep '.+/' | grep -v _); do 
    cd $i; 
    echo -e "Checkando compilacao do projeto: $i\n";
    make; 
    make clean;
    cd ..; 
done