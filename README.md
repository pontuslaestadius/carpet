# carpet

https://github.com/pontuslaestadius/carpet


| Contributors        |
| ------------------- |
| Sebastian Fransson  |
| Kosara Golemshinska |
| Pontus Laestadius   |
| Elaine Qvarnström   |

## Introduction
A self driving university car project based on the cloun library. The software is able to utitlize the OpenDaVinci library to follow a lane and perform Vehicle to Vehicle communication as well as platooning.


## Github Layout
The development branch serves as a place where tested code is integrated and collected in-between releases, while the master branch will serve as a place for the latest release/stable version.
Development of features is separated into branches with appropriate naming.
Test-cases will put within a separate folder to separate tests from deployment code.

When merging to master a release will be created to provide traceability.

Travis CI is used in development to build and test the code. Whenever a contributor pushes a change to the master or development branch and the build/tests fail the contributors to the project are notified via mail. 

Following these [guidelines](http://nvie.com/posts/a-successful-git-branching-model/)

## Dependencies
* [OpenDaVinci](https://github.com/se-research/OpenDaVINCI)
* [Cluon](https://github.com/chrberger/libcluon)
* [Docker](https://www.docker.com/)
* [Valgrind](http://valgrind.org/)

## Installing and Running

1. First and foremost, clone the repository to a local directory.
```
git clone https://github.com/pontuslaestadius/carpet
```
2. Make sure that Docker is installed on your machine (link above).
3. The next step is to install OpenDaVINCI & Cluon (links above).

> Building the repository can be done using cmake in a seperate directory, before using make in the source code directory.
```
cd carpet/
sh chain.sh -l
```

4. Finally, when building is done, run the executable binary to run it on your local machine.
```
cd build/
./carpet
```


> If the image is not copied correctly, try pulling it direcly from our hub:
```
sh chain.sh --pull
```

## Deploying
Deployment of the software is handled using an alpine docker image which compiles and extracts the binaries which can be uploaded to the car using a docker image.
Uploading to the car is done via a docker image stored on the docker hub.  Which is remotly downlaoded and executed on the desired platform.

```
sh chain.sh -b
sh chain.sh --push <dockerID>
```
The build command is used to push the images to your docker hub. Which in our case would be located at pontusla/carpet_compile
To use a different docker hub account, this needs to be specified as an argument to chain.sh script.

On the desired platform you would have to run the remote docker image. In this example we use our own uploaded hub.
```
docker run --rm -ti pontusla/carpet_deploy
```
> -ti is optional and can be omitted.


**How to run the test cases:**

Having linked Travis CI to the repository, tests are run after every pushed commit / during pull-requests.

Testing the current software can be done through the following:

1. First you need the socat library.
```
sudo apt-get install socat
```
2. Run the command
```
socat UDP4-RECVFROM:1236,ip-add-membership=225.0.0.111:0.0.0.0,fork – 
```
3. Compile the software using Cmake.
```
mkdir build
cd build/
cmake ..
make test
```
4. Alternative command which is just a shorthand. And includes compiling test and the software.
```
sh chain.sh -l
```

