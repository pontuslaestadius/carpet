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
FORCE="no"
DOCKER_NAME="carpet_toolchain"
DOCKER_IMAGE="docker_toolchain_image"

# Command-line argument
for i in "$@"
do
case $i in
    -b|--build)
    BUILD="yes"

    docker build -t $DOCKER_IMAGE .
    exit

    shift # past argument=value
    ;;
    -l|--local)
    
    sh docker_runtime.sh
    exit

    shift # past argument=value
    ;;
    -f|--force)
    FORCE="yes"
    shift # past argument=value
    ;;
    -d|--delete)
    
    docker rm carpet_toolchain
    exit
    
    shift # past argument=value
    ;;
    -h|--help)
	cat docker/chain_help.txt
    exit
    shift # past argument=value
    ;;
    *)
    echo "Unknown argument: '$i'"
          # unknown option
    ;;
esac
done

# Runs a docker container.
# If a container already exists and is running.
if [ "$(docker ps -aq -f status=running -f name=$DOCKER_NAME)" ]; then

    # Without force, prompt the user with a choice.
    if [ "${FORCE}" = "no" ];
        then
            # clean up with prompt
            echo "The container is already running. Do you want to continue? [Y/N]:"
            read -n 1 REPLY
            echo    # (optional) move to a new line

        else # In force, the user has no option.
            REPLY="Y" 
        fi 

    if [[ $REPLY =~ ^[Yy]$ ]];
    then
        docker stop $DOCKER_NAME
    else
        exit
    fi

fi

# If a container already exists.
if [ "$(docker ps -aq -f status=exited -f name=$DOCKER_NAME)" ]; then
    # cleanup
    docker rm $DOCKER_NAME
fi

# run your container:
# deletes container on exit.
# deletes container on host boot.
# set name to $DOCKER_NAME.
# mount the volume in current directory.
#docker run -v $PWD -i --rm -d  $DOCKER_IMAGE
#docker exec $DOCKER_NAME bash $PWD/docker/docker_runtime.sh
docker run --rm -d --net=host --name $DOCKER_NAME -v $PWD:/ $DOCKER_IMAGE /bin/sh

