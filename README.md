# projekt-programistyczny
Symulator ruchu ulicznego

## How to compile and run on Ubuntu
### Requirements
1. gcc verison 5.4 or newer
  * `sudo apt-get install gcc`
2. cmake verison 3.7 or newer
  * `sudo apt-get install cmake`
  * If you have lower version, you can download from [here](https://cmake.org/download/) (Remeber to donwload binary version!) and then:
   * Remove old version: `apt-get purge cmake`
   * Copy binary files:
    * `cp -r bin /usr/`
    * `cp -r doc /usr/share/`
    * `cp -r man /usr/share/`
    * `cp -r share /usr/`
3. SDL2 lib and includes
  * `sudo apt-get install libsdl2-2.0`
  * `sudo apt-get install libsdl2-dev`
### Getting project
1. `git clone https://github.com/Jahoo444/projekt-programistyczny.git`
### Building
1. Go to project root folder
  * `cd projekt-programistyczny/`
2. Run command
  * `cmake .`
3. Run command
  * `make`
### Running program
1. run
  * `./simulation` with precognition
  * `./simulation -nope` without precognition
