#!/bin/bash

while true 
do
    ./pointcloud-analysis ../image-analysis/images/pc-test-data/
    mv $(cat file_pcd.txt) ~/Projects/inwatec/image-analysis/images/pc-test-data/bkp/
    mv $(cat file_png.txt) ~/Projects/inwatec/image-analysis/images/pc-test-data/bkp/
    echo "ANOTHER ONE!"
done
