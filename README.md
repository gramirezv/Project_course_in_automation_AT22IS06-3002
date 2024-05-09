# Project_course_in_automation_AT22IS06-3002
 Repository fro the project course in Automation Technology at Novia University 20224

 ## Installing open62541

 Installing from instructions found in:
 https://www.open62541.org/doc/master/building.html

 Tested on Lubuntu 24.04 running on Virtualbox

 ### Install dependencies

For me on lubuntu didn't need to install python. Check your linux installation.

 ```sudo apt-get install git build-essential gcc pkg-config cmake cmake-curses-gui```

 ### Clone open62541 repository

 Cloning open62541 repository, first navigate to the directory where you want to clone your repository and run this on terminal

```git clone https://github.com/open62541/open62541.git```

### Building and installing open62541

```
cd open62541
mkdir build
cd build
cmake ..
make
```

I wanted to have the full namespace zero. Later we can deceide if we can make it with the reduced

Open ccmake for the project
```ccmake ..```

Then go down to UA_NAMESPACE_ZERO and change it from REDUCED to FULL (you change it by pressing enter)

Then press (c) to configure and wait to finish doing the configurations.
Then press (g) to generate the configuration. 

When that is finished you can rebuild the project again
```make```

Then the last part is to install the OPC UA open62541 stack
```sudo make install```