DEPENDENCY="cluon"
cp ../../header/$DEPENDENCY* .
docker build . -t ${PWD##*/}:armhf
rm $DEPENDENCY*
