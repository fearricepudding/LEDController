# LEDController
**THIS PROJECT IS STILL IN ITS INFANCY AND IS NOT YET READY**

[TOC]

### About
This project exists as a fun solution to driving my own LED strips and learn CPP along the way. I hope for this project to become something useful that might help others out looking for something similar. If you have any ideas or have a better way of doing something, feel free to get involved!

### Getting started
##### Requirements
-  Boost: system, thread and chrono
- Currently only supports raspberry pi

##### building
```bash
git clone https://github.com/fearricepudding/LEDController
cd LEDController
mkdir build && cd build
cmake ..
make
```

##### Running
```bash
sudo ./server
```

### Project Goals
- Support and drive different types of LED strips
- Remote control from mobile devices
- API for expansion and modification
- engine for playing and controling animations
- Animation/sequence builder and editor
- Animation/sequence sharing
- Animation/sequence selector
- cron jobs (turn off at night)
- simple getting started process
- documentation for modifying

### Issues and Contributing
Feel free to create any issues
Do the usual contributing stuff.