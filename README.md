# SDSGC Information Tool
This program is a tool for searching for information about the mobile game, Seven Deadly Sins Grand Cross. For example, you can ask for a list of the characters that are good, the characters you don't have, the characters you have at a certain ultimate move level, the characters that are currently acquirable. Personally, I made the tool to help myself easily see how good a draw is, without going back 100 times to the heroes list! I find access to information in game pretty much impossible, so I hope that this tool can help you too. :)

## Table of Contents
- [Features](#features)
- [Installation](#installation)
	- [Releases](#releases)
	- [Build from source](#build-from-source)
- [License](#license)
- [Thank You!](#thank-you)

## Features

## Installation
### Releases
First release for Windows is now available. See the [Releases](https://github.com/Lucaz8de/SDSGC-information-tool/releases) section on the right of the page.

### Build from source
I really recommend this. All you need is a compiler for C++, e.g. GCC.

It is most convenient if you also have Git and CMake; then you can download and build the application by running these commands from the command line.
```bash
git clone https://github.com/Lucaz8de/SDSGC-information-tool.git
cd SDSGC-information-tool
cmake -S . -B build
cmake --build build
```

If you're on Windows and want to get all of these tools in one place, you can use [MSYS2](https://www.msys2.org/). (On other platforms, they'll be available from your package manager.)

If you don't have Git, you can click the "Code" button at the top of this page then download and extract the source. If you don't have CMake, you can compile directly using whatever compiler you have, for example by running a command very much like this one.

```bash
g++ src/*.cpp -o build/sdsgc-information-tool
```

Once installed, please make sure you run the application from the command line in the install directory.
```bash
cd build
./sdsgc*
```

## License
You can do whatever you want with this. [LICENSE](LICENSE) 

## Thank You!
With thanks to:
- Auth0 for the repository structure and README file.

