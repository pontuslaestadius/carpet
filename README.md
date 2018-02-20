# carpet
Contributors:
Sebastian Fransson
Kosara Golemshinska
Pontus Laestadius
Elaine Qvarnström

## Introduction
An self driving university car project based on the cloun library. The software is able to utitlize the OpenDaVinci library to follow a lane and perform Vehicle to Vehicle communication as well as platooning.


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

## Installing and Running

1. First and foremost, Clone the repository to a local directory.
```
git clone https://github.com/pontuslaestadius/carpet
```
2. Make sure that Docker is installed on your machine (link above).
3. The next step is to install OpenDaVINCI & Cluon (links above).

> Building the repository can be done using cmake in a seperate directory, before using make in the source code directory.
```
cd carpet/
mkdir build/ && cd build/
cmake ..
cd ../
make .
./a.out
```

**How to run the test cases:**

Download header test file: [Catch2](https://github.com/catchorg/Catch2)

Run the following command when testing the cloned software.
```
cmake test
```




