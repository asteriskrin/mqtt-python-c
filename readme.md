
# MQTT with Python and C
A C program that sends message to broker and a python program that receives the message.
  

## How to install and run

1. Install Python 3.x.

2. Run `pip install paho-mqtt` to install Paho MQTT module for Python.

3. Install GCC.
```bash
apt update
apt install gcc
```
4. Install Paho MQTT Library for C.
```bash
# Install required dependencies
apt-get install build-essential cmake libssl-dev

# Download the library
git clone https://github.com/eclipse/paho.mqtt.c.git

# Build it
cd paho.mqtt.c
cmake -Bbuild -H. -DPAHO_WITH_SSL=ON
sudo cmake --build build/ --target install

# Set LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/path/to/mqtt-python-c/c-publisher/lib:$LD_LIBRARY_PATH
```

5. Compile C program (`publisher.c`)
```bash
cd c-publisher
gcc publisher.c -o publisher -I./include -L./lib -lpaho-mqtt3c
```

6. Run the python program.
```bash
cd python-subscriber
py subscriber.py
```
7. Run the C program.
```bash
cd c-publisher
chmod +x publisher
./publisher
```

<img width="100%" alt="image" src="https://github.com/asteriskrin/mqtt-python-c/assets/8071604/52a27b77-ebb2-44e4-a574-4383f49dfc2f">
<img width="100%" alt="image" src="https://github.com/asteriskrin/mqtt-python-c/assets/8071604/f5046a2a-c896-4008-9da0-901a672202b3">

