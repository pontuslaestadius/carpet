DEPENDENCY="cluon"
DEPENDENCY2="messages"
cp ../../header/$DEPENDENCY* .
cp ../../$DEPENDENCY2* .
docker build . -t ${PWD##*/}:armhf
rm $DEPENDENCY* $DEPENDENCY2*
