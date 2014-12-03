# Qt Flight instrument widgets

This program is a pure Qt implementation of flight instrument widgets, including attitude display indicator (ADI), compass, and list viewer. 

The source is based on part of source code (https://github.com/cmex81/qAttitudeIndicator). We improve their implementation and make it clear to read.


## Requirements:
* Qt4 (sudo apt-get install libqt4-core libqt4-dev)


## Compile:
 `qmake qFlightInstruments.pro` 

 `make`


## Usage:
```
./qFlightInstruments

Keyboard:
    UP    - Pitch +
    DOWN  - Pitch -
    LEFT  - Roll -
    RIGHT - Roll +
    A     - Yaw +
    D     - Yaw -
    W     - Alt +
    S     - Alt -
    J     - H +
    K     - H -
```


examples:

`./fastslam.e -method FAST1 -mode interactive` (FastSLAM 1, user interactive)

`./fastslam.e -method FAST2 -mode waypoints -m example_webmap.mat` (FastSLAM 2, following waypoints, map is "example_webmap.mat")

`./fastslam.e -method EKF1 -mode waypoints -m example_loop1.mat` (EKF SLAM, following waypoints, map is "example_loop1.mat")



## Plateform:
Only test on Linux Mint 16 64-bit. 



## Screenshot:
-![alt text](https://raw.githubusercontent.com/bushuhui/qFlightInstruments/master/screen_shot.png "Screenshot 1")



## Project homepage:
http://www.adv-ci.com/blog/source/qflightinstruments/
