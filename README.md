## Arduino MKR 1000 Http server and tornado monitoring client

A tiny project which contains code for an MKR 1000 board:
which will connect to a specified wifi network and create a server which will collect data from the 0-th analog pin and package that data into json.
The python client will connect to the server and collect the data at a specified rate and display it.