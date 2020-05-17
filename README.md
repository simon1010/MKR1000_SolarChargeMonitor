## Arduino MKR 1000 Http server and tornado monitoring client

A tiny project which contains code for an MKR 1000 board:
- which will connect to a specified wifi network
- create an http server which will collect data from the 0th analog pin
- package that data into json.

The python client will connect to the server and collect the data at a specified rate and display it.

### Arduino Prerequisites
- Wifi101 library
- ArduinoJson library

### Python
Tested with python 2.7 with Tornado installed
It should work with newer versions of python aswell, this part is in progress.
