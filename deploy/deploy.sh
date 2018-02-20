docker tag carpet_toolchain_image $DOCKER_ID_USER/carpet_compile
docker tag carpet_deploy_image $DOCKER_ID_USER/carpet_deploy

docker push $DOCKER_ID_USER/carpet_compile
docker push $DOCKER_ID_USER/carpet_deploy
