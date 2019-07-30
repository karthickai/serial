<img width="880" height = "80" alt = "Serial Title"
    src="doc/images/readme.jpg">

# Cross-Platform, Serial Port Library built on Boost.Asio in Modern C++ 17 




## Contents

- [Introduction](#introduction)
- [Appearances](#appearances)
- [Description](#description)
- [Requirements](#requirements)
- [Building](#building)
- [Usage](#usage)
- [License](#license)
- [Author](#author)
- [Contact](#contact)


## Introduction

Serial is a C++ header-only library serving as a foundation for writing
interoperable Serial Port Communication by providing **low-level Serial I/O** 
using the consistent asynchronous model of Boost.Asio.

This library is designed for:

* **Ease of Use:** Boost.Asio users will immediately understand Serial.

* **Performance:** Build applications with high Performance.

* **Basis for Further Abstraction.** Components are well-suited for building upon.


## Description

This software is in its first official release. Interfaces
may change in response to user feedback. For recent changes
see the [CHANGELOG](CHANGELOG.md).

* [Official Site](https://github.com/karthickai/serial)
* [Documentation](htt/) (master branch)


## Requirements

This Serial Library requires Boost Libraries

* **C++17:** Robust support for most language features.
* **Boost:** Boost.Asio and some other parts of Boost.
* **OpenSSL:** Required for using TLS/Secure sockets and examples/tests

When using Microsoft Visual C++, Visual Studio 2017 or later is required.

## Building

Serial is header-only. To use it just add the necessary `#include` line
to your source files, like this:
```C++
#include <Serial.hpp>
```

## Building examples
Building tests and examples requires Boost installed.

### Ubuntu/Debian
If installed into a system directory, boost will be automatically found and used.
```bash
sudo apt install libboost-all-dev
```
### Windows
Replace `path` in the following code snippets with the path you installed vcpkg
to. Examples assume a 32-bit build, if you build a 64-bit version replace
`x32-windows` with `x64-windows` in the path.
- Using [vcpkg](https://github.com/Microsoft/vcpkg) and CMD:
```bat
vcpkg install boost-asio --triplet x32-windows
SET BOOST_ROOT=path\installed\x32-windows
```

- Using [vcpkg](https://github.com/Microsoft/vcpkg) and PowerShell:
```powershell
vcpkg install boost-asio --triplet x32-windows
$env:BOOST_ROOT = "path\x32-windows"
```

- Using [vcpkg](https://github.com/Microsoft/vcpkg) and bash:
```bash
vcpkg.exe install boost-asio --triplet x32-windows
export BOOST_ROOT=path/x32-windows
```

### Mac OS
Using [brew](https://github.com/Homebrew/brew):
```bash
brew install boost
export BOOST_ROOT=$(brew --prefix boost)
```

Additional instructions for configuring, using boost libraries
[Boost Wiki](https://github.com/boostorg/boost/wiki/Getting-Started).

## Visual Studio

CMake may be used to generate a very nice Visual Studio solution and
a set of Visual Studio project files using these commands:

```
mkdir build
cd build
cmake ..                                    # for 32-bit Windows builds, or
cmake -G "Visual Studio 16 Win64" ..    # for 64-bit Windows builds (VS2019)
```

The files in the repository are laid out thusly:

```
    build/          Create this to hold executables and project files
    doc/            Source code and scripts for the documentation
    include/        Where the header files are located
    example/        Self contained example programs
    test/           The unit tests for Serial
```

## Usage

These examples are complete, self-contained programs that you can build
and run yourself (they are in the `example` directory).





Distributed under the Boost Software License, Version 1.0.
(See accompanying file [LICENSE_1_0.txt](LICENSE_1_0.txt) or copy at
https://www.boost.org/LICENSE_1_0.txt)


---
## License
The MIT License

Copyright (c) 2019 Karthick P

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

---

## Author

Karthick P <karthick.ps.ai@gmail.com>

## Contact
Please report issues or questions here:
https://github.com/karthickai/serial/issues

