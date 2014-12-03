# Qt Flight instrument widgets

This program is a pure Qt implementation of flight instrument widgets, including attitude display indicator (ADI), compass, and list viewer. These widgets are wrote in single .h/.cpp, therefore is easy to add to your project. In addition it is easy to modify.

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



## Plateform:
Only test on Linux Mint 16 64-bit. 



## Screenshot:
-![alt text](https://raw.githubusercontent.com/bushuhui/qFlightInstruments/master/screen_shot.png "Screenshot 1")



## Project homepage:
http://www.adv-ci.com/blog/source/qflightinstruments/
