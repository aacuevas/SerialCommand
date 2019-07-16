# Serial Command
This repository contains a plugin for the [Open Ephys GUI](https://github.com/open-ephys/plugin-GUI) which allows sending commands to a serial device in response to TTL events.

## Usage
The serial command plugin sends commands to a device connected through a serial port each time a rising edge on a selected TTL event is detected by the processor.
Configurable parameters are:
* **Port:** The serial port to which the device is connected
* **Rate:** The baud rate for the serial communication
* **Trigger:** The TTL event that will trigger the command
* **CMD:** The text command that will be send through the serial port for each trigger

If the serial command must end with a newline, the processor allows so by adding \n at the end of the line. Note that at the moment the processor does not actually prcoess escape sequences. This will only work if the \n sequence is located at the end of the line, will not work if it is any other place and will not work with any other escape sequence. Thus multi-line commands or any other command using non-typable characters are not supported

## Installation
The plugin uses a [CMake-based build system](https://open-ephys.atlassian.net/wiki/spaces/OEW/pages/1259110401/Plugin+CMake+Builds) for building and installing.

## TODO
* Enable triggering on falling edge as well as rising
* Allow multiple triggers with individual commands for a single device in a single processor
* Perform actual escaping of the command string before sending it