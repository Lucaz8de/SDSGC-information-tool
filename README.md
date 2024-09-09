

# SDSGC Information Tool
This program is a tool for information about characters in the gacha game, Seven Deadly Sins Grand Cross. For example, you can ask for a list of the characters that are good, the characters you don't have, the characters you have at a certain ultimate move level, the characters that are currently acquirable. Personally, I made the tool to help myself easily see how good a draw is, without going back 100 times to the heroes list! I decided to share it in the hope it can help other people too. :)

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
Windows installers are available. See the [Releases](https://github.com/Lucaz8de/SDSGC-information-tool/releases) section on the right of the page.

### Build from source
If you prefer, you can build the program yourself from the source code. (By doing this, you can make changes if you want to, and share them if you want to!) If you're on 64-bit Windows and don't already have development tools, you can keep reading!

#### Downloading the source code
If you have Git on your computer, clone the repository, either using your favourite graphical tool or by running the clone command from the command line.
```bash
git clone https://github.com/Lucaz8de/SDSGC-information-tool.git
```
Alternatively, use the green "Code" button in the top right of this page, download and extract the source code.

#### Building the program
Navigate into the directory (folder) and compile the source code.

```bash
cd SDSGC-information-tool
mkdir build
g++ --static -o build/sdsgc-information-tool src/*.cpp
```
If you want to build using CMake, you can use the included CMakeLists.
```bash
cd SDSGC-information-tool
cmake -S . -B build -DSTATIC=ON
cmake --build build
```
(Note: The static option includes some needed resources in the program file. You can try without if you want to for a particular reason.)

#### Running the program
The program must be run in the correct location. It will look for the data files in the directory named data.

On Windows, you can make a shortcut and run it wherever you want as usual.

On Linux, you can use a desktop launcher that runs the following command:
```bash
bash -c "cd .../SDSGC-information-tool/build; ./sdsgc-information-tool"
```

If you'd like to run it directly in the terminal, remember to navigate into the build directory first.
```bash
cd .../SDSGC-information-tool/build
./sdsgc-information-tool
```

#### On 64-bit Windows and don't already have development tools?
You can download and install [MSYS2](https://www.msys2.org/) and use the development environment MSYS2 UCRT (the U stands for Universal). This development environment is a terminal window you can run commands in. It exists in your MSYS2 install location e.g. C:\msys64 -- i.e. this folder is the location of the files you can access in this terminal.

It comes with a package manager called Pacman. Run the following command a few times to ensure it is up-to-date.
```bash
pacman -Syu
```

Package managers include and manage packages, which you can use to install software. Use Pacman to install the C++ compiler for 64-bit Windows, if you didn't do this already.
```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
```
Optionally, you can install Git too if you want to use it.
```bash
pacman -S git
```

## License
You can do whatever you want with this. [LICENSE](LICENSE) 

## Thank You!
With thanks to:
- Auth0 for the repository structure.

