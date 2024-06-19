# INES - 2D

Creation of a translation model from RoboSim to C++ using soccer simulation 2D as a case study.

### Libs

Install all required dependencies

```sh
$ sudo apt-get update
$ sudo apt install build-essentials cmake libboost-all-dev
```

# Build and execute

To build and execute the translation, run the following commands in the project root directory:

```sh
$ mkdir build
$ cd build
$ cmake ..
$ make -j$(nproc)
$ ./sim_2d_sm
```
