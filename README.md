# JLVEA-cpp

![version](https://img.shields.io/badge/version-0.0-red) ![version](https://img.shields.io/badge/status-dev-red)![version](https://img.shields.io/badge/build-passing-green) ![version](https://img.shields.io/badge/license-MIT-blue)

## Motivation

## Tech/framework used

* OpenCV 4

## Requirements

* OpenCV 4
* CMake 3.15 or greater

## Features

JLVEA-cpp encrypts a video using permutation-based video encryption mechanism.

## Installation

### Linux(Ubuntu)

```bash
#Install requirements
sudo apt install make snap
sudo snap install cmake

#Download JLVEA source
git clone https://github.com/junhyeok-dev/JLEVA-cpp.git

#Compile
cd JLVEA-cpp
cmake CMakeLists.txt
make
```

## How to use?

```bash
cd JLVEA-cpp
./JLVEA-cpp [encrypt|decrypt] FILENAME SEED
```

A FILENAME argument is name of video file to encrypt/decrypt.

A SEED argument is a key for video encryption.