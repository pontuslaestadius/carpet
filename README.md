# carpet
Contributors:
Sebastian Fransson
Kosara Golemshinska
Pontus Laestadius
Elaine Qvarnström

# Introduction
An self driving university car project based on the cloun library. The software is able to utitlize the OpenDaVinci library to follow a lane and perform Vehicle to Vehicle communication.


# Github Layout






# Installing and Running

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

How to run the test cases:
```
cmake test
```




