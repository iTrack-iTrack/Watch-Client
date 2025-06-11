# Watch-Client

The Watch-Client repository is a project that collects sensor information from the user, displays it in the Serial, and uploads it in to the backend.

## Setup

The following are required for this project.

- [PlatformIO](https://platformio.org/)

## Usage

To compile all the source code, the user must execute `platformio run` where it will automatically download all of the packages specified in the `platformio.ini` file, and compile the entirety of the codebase.

To be able to upload the code, the ESP32 must be connected to the computer, in which the command can be run `sudo platformio run -t upload`.

Should the user wish to monitor the Serial of the device, the following can be run `sudo platformio run -t monitor`.
