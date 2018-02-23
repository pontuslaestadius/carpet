DOCKER_ID_USER=$1

# Bind the local with the remote.
docker tag carpet_compile $DOCKER_ID_USER/carpet_compile

# Push them to the hub.
docker push $DOCKER_ID_USER/carpet_compile
