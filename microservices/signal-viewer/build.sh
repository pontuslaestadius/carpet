DEPENDENCY="messages.odvd"
cp ../../$DEPENDENCY src/
docker build . -t ${PWD##*/}:armhf
rm src/$DEPENDENCY*
