#!/bin/bash""g

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
HELP=".CHAIN_HELP"
FORCE="no"
DOCKER_NAME="carpet_compile"
DOCKER_IMAGE="pontusla/carpet_compile"

# Command-line argument
for i in "$@"
do
case $i in

	-t|--transfer)
	
			docker save carpet_compile.tar carpet_compile:armhf
			scp carpet_compile.tar debian@192.168.8.1:~/
			rm carpet_compile.tar
			ssh debian@192.168.8.1 -t 'sudo sh run.sh'
	
    ;;
	--run)

			# If a container already exists and is running.
			if [ "$(docker ps -aq -f status=running -f name=$DOCKER_NAME)" ]; then

				# Without force, prompt the user with a choice.
				if [ "${FORCE}" = "no" ];
					then
						# clean up with prompt, only works with bash and often not user shell.
						echo "The container is already running. Do you want to continue? [Y/N]:"
						read -n 1 REPLY
						echo    # (optional) move to a new line

					else # In force, the user has no option.
						REPLY="Y" 
					fi 


				if [[ $REPLY =~ ^[Yy]$ ]];	# Use confirmation. Needs BASH not user shell.
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

			docker run -d --net=host --name $DOCKER_NAME -v $PWD:/opt/sources $DOCKER_IMAGE /bin/sh

    ;;
	--pull)
	
			docker pull $DOCKER_IMAGE

    ;;
	--push)
	
			DOCKER_ID_USER=$2
			# Bind the local with the remote.
			docker tag $DOCKER_NAME $DOCKER_ID_USER/$DOCKER_NAME
			# Push them to the hub.
			docker push $DOCKER_ID_USER/$DOCKER_NAME

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
    -f|--force)
    
    			FORCE="yes"
    
    ;;
    -d|--delete)
    
    			docker rm $DOCKER_NAME
    
    ;;
    -h|--help)
    
			cat $HELP
			
    ;;
    *)
    #	Invalid argument
    
			echo "Unknown argument: '$i'"
			cat $HELP
			
	exit 1
    ;;
esac
done


