DEPENDENCY="messages.odvd"
DEPENDENCY2="jquery*"
DEPENDENCY3="libcluon.js"
cp ../../$DEPENDENCY src/
cp ../signal-viewer/src/DEPENDENCY2 src/
cp ../signal-viewer/src/DEPENDENCY3 src/
docker build . -t ${PWD##*/}:armhf
rm src/$DEPENDENCY rm src/$DEPENDENCY2 rm src/$DEPENDENCY3

