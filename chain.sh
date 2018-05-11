#!/bin/bash

#################################
### Author: Pontus Laestadius ###
### Last modified: 2018-05-04 ###
#################################

## Provides a list of helpful shortcode commands which 
## assist with development of the carpet project.

# Default to enable building.
HELP=".CHAIN_HELP"
FORCE="no"
DOCKER_NAME="carpet_compile"
DOCKER_IMAGE="pontusla/carpet_compile"

# Command-line argument
for i in "$@"
do
case $i in

	-t|--transfer)
	
			docker save -o $DOCKER_NAME.tar $DOCKER_NAME:armhf
			scp $DOCKER_NAME.tar debian@192.168.8.1:~/
			rm $DOCKER_NAME.tar
			ssh debian@192.168.8.1 -t 'sh run.sh'
	
    ;;
    -new-microservice)
			
			MS="microservices"
			mkdir "$MS/$2"
			mkdir "$MS/$2/src"
			echo "Copying existing files to $MS/$2"
			cp CMakeLists.txt "$MS/$2"
			cp Dockerfile* "$MS/$2"
			exit 0

	;;
	
	-b|--build)
    	
    		docker build -t carpet_compile .

	;;
	-ba|--build-arm)
    	
    		docker build -t carpet_compile:armhf -f Dockerfile.armhf .

    ;;
	--debug)

			valgrind --tool=memcheck --log-file=".valgrind_output" --leak-check=yes ./build/carpet

	;;
    -l|--local)
    
			DIR="build"
			if [ ! -d "$DIR" ]; then
				mkdir $DIR
			else
				rm $DIR -rf
			fi
			cd $DIR
			cmake -D CMAKE_BUILD_TYPE=Release .. && \
			ctest -T memcheck && \
			make

    ;;

    -d|--delete)
    
    		docker rm $DOCKER_NAME
    
    ;;
    -h|--help)
    
			cat $HELP
			
    ;;
    -ci|--travis)
    
			for dir in microservices/*/
				do
				    dir=${dir%*/}
				    cd microservices/${dir##*/}
				    sh *.sh
				    cd ../../					
				done
			
    ;;
    *)
    #	Invalid argument
    
			echo "Unknown argument: '$i'"
			cat $HELP
			exit 1

    ;;
esac
done
