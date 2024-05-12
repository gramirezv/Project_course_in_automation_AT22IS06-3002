# Prerequisites
OPC UA open62541 stack must be built and installed in the host machine. 
See the main project's [README](../README.md)

# Building example

```
mkdir build && cd build
cmake ..
make
```

# Running examples
Simple OPC UA server run this on terminal:
```./tutorial_server```


Nodeset loading on OPC UA server run this on terminal:
```./server_nodeset_loader ../model/nicla_sense.xml```

This can be accessed now from the windows host
