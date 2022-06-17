#!/bin/bash

for i in $(ls -p | egrep '.+/' | grep -v _); do 
    cd $i; 
    echo -e "Limpando projeto: $i\n";
    make clean; 
    cd ..; 
done