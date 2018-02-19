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

When merging to master a release will be created to provide tracability.

Following these [guidelines](http://nvie.com/posts/a-successful-git-branching-model/)

## Dependencies
* [OpenDaVinci](https://github.com/se-research/OpenDaVINCI)
* [Cluon](https://github.com/chrberger/libcluon)

## Installing and Running

First and foremost, Clone the repository to a local directory.
```
git clone https://github.com/pontuslaestadius/carpet
```
Building the repository can be done using cmake in a seperate directory, before using make in the source code directory.
```
cd carpet/
mkdir build/ && cd build/
cmake ..
cd ../
make .
./a.out
```

**How to run the test cases:**

Download all header test: [Catch2](https://github.com/catchorg/Catch2)

```
cmake test
```




