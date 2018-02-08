#!/bin/bash

#################################
### Author: Pontus Laestadius ###
### Last modified: 2018-02-08 ###
#################################

## Run the carpet toolchain inside a docker container.
## View the dockerfile for details on what is run, this file
## only handles docker management.

# This is a modified version of what can be found here.
# https://stackoverflow.com/questions/38576337/execute-bash-command-if-docker-container-does-not-exist

# Default to enable building.
BUILD="no"
DELETE="no"

# Command-line argument
for i in "$@"
do
case $i in
    -b=*|--build=*)
    BUILD="${i#*=}"
    shift # past argument=value
    ;;
	-d|--delete)
    DELETE="yes"
    shift # past argument=value
    ;;
	-h|--help)
	echo "-h		--help			displays this information"
	# echo "-c		--compliment	prints a nicely formatted compliment"
	echo "-b		--build			set build status, yes | no (default)"
	echo "-d		--delete		deletes the toolchain container"
	exit
    shift # past argument=value
    ;;
    *)
	echo "Unknown argument: '$i'"
          # unknown option
    ;;
esac
done

# If we are deleting.
if [ "${DELETE}" = "yes" ] 
then
	docker rm docker_toolchain
	exit
fi

# If we are building a new docker image or not.
if [ "${BUILD}" = "yes" ] 
then
	docker build -t docker_toolchain_image docker/.
fi

# Runs a docker container.
if [ ! "$(docker ps -q -f name=carpet_toolchain)" ]; then
    if [ "$(docker ps -aq -f status=exited -f name=carpet_toolchain)" ]; then
        # cleanup
        docker rm carpet_toolchain
    fi
    # run your container
    docker run -v $PWD -i -d --name carpet_toolchain docker_toolchain_image
fi

