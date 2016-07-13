![globjects Logo](globjects-logo.png "globjects")

*globjects* is an [MIT licensed](http://opensource.org/licenses/MIT), strict C++ [OpenGL API](http://www.opengl.org) objects wrapper based on [glbinding](https://github.com/cginternals/glbinding).

*globjects* provides object-oriented interfaces to the OpenGL API (3.0 and higher).
The main goals are much reduced code to use OpenGL in your rendering software and fewer errors
due to the underlying [glbinding](https://github.com/cginternals/glbinding) and abstraction
levels on top. Typical processes are automated and missing features in the used OpenGL driver
are partially simulated or even emulated.

An example OpenGL snippet may look like this:
```cpp
// Plain OpenGL API

GLuint program = glCreateProgram();
GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);

glCompileShader(vertexShader);
glCompileShader(fragmentShader);

glAttachShader(program, vertexShader);
glAttachShader(program, fragmentShader);

glLinkProgram(program);

glUseProgram(program);
glUniform2f(glGetUniformLocation(program, "extent"), 1.0, 0.5);
```

However, the same functionality can be achieved with globjects using the following code:
```cpp
// globjects API

Program* program = new Program();

program->attach(
  Shader::fromString(GL_VERTEX_SHADER, vertexShaderSource), 
  Shader::fromString(GL_FRAGMENT_SHADER, fragmentShaderSource)
);

program->setUniform<glm::vec2>("extent", glm::vec2(1.0, 0.5)));
```
Additionally, this code performs can perform ```glGetError``` checks after each call and check the shaders for compiler errors and the program for linker errors.

###### globjects for Enterprise

Want to integrate globjects in your software? Our team of globjects experts will work closely with your team to help you integrate, customize, and support your globjects setup.

Visit [Enterprise Support and Services](https://www.cginternals.com) for more details.

## Resources
* [List of supported concepts](#supported-concepts)
* [Examples](https://github.com/cginternals/globjects/wiki/examples)
* [Project Health](#project-health)

###### Installation and Development
* [Install Instructions](#install-instructions)
* [Build form Source](#build-instructions)
* [Tips for Linking](#tips-for-linking)
* [Basic Example](#basic-example)

###### Feature Documentation and Code Snippets
* [Reference pointer as memory model](#reference-pointers)
* [Shader templates](#shader-templates)
* [Manual strategy override](#strategy-override)

## Project Health

| Service | System | Compiler | Status |
| ------- | ------ | -------- | -----: |
|  [Travis-CI](https://travis-ci.org/cginternals/globjects) | Ubuntu 14.04 | GCC 4.8, Clang 3.5 | [![Build Status](https://travis-ci.org/cginternals/globjects.svg?branch=master)](https://travis-ci.org/cginternals/globjects) |
| [Coverity](https://scan.coverity.com/projects/6829?tab=overview) | Ubuntu 14.04 | GCC 5.3 | [![Coverity Status](https://scan.coverity.com/projects/6829/badge.svg)](https://scan.coverity.com/projects/6829) |
| Jenkins <br><br><br><br> | Ubuntu 14.04 <br><br><br><br> | GCC 4.8 <br> GCC 4.9 <br> GCC 5.3 <br> Clang 3.5 <br> | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=globjects-linux-gcc4.8)](http://jenkins.hpi3d.de/job/globjects-linux-gcc4.8) <br> [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=globjects-linux-gcc4.9)](http://jenkins.hpi3d.de/job/globjects-linux-gcc4.9) <br> [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=globjects-linux-gcc5.3)](http://jenkins.hpi3d.de/job/globjects-linux-gcc5.3) <br> [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=globjects-linux-clang3.5)](http://jenkins.hpi3d.de/job/globjects-linux-clang3.5) <br> |
| Jenkins <br><br> | Windows 10 <br><br> | MSVC 2013 Update 5 <br>  MSVC 2015 Update 1 <br> | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=globjects-windows-msvc2013)](http://jenkins.hpi3d.de/job/globjects-windows-msvc2013) <br> [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=globjects-windows-msvc2015)](http://jenkins.hpi3d.de/job/globjects-windows-msvc2015) <br> |
| Jenkins | OS X 10.10 | AppleClang 6.0 | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=globjects-osx-clang3.5)](http://jenkins.hpi3d.de/job/globjects-osx-clang3.5) |

Please note that our OS X build node is currently broken (physically). However, *globjects* is maintained for OS X as well and there are many people using it on OS X on a regular basis.


## Install Instructions

*globjects* is available for different platforms using different distribution channels. 
You can either download the source and manually [compile](#build-instructions) it or use one of the [pre-compiled releases](https://github.com/cginternals/globjects/releases) of this repository.
For systems providing package managers, we generally strive for packages in these package managers.

###### Windows

The various globjects packages can be installed either by downloading an installer, e.g., the [latest x64 installer](https://github.com/cginternals/globjects/releases/download/v1.0.0/globjects-1.0.0-msvc2015-x64-installer.exe) for Microsoft Visual Studio 2015, or downloading and extracting one of the precompiled archives, e.g. [runtime](https://github.com/cginternals/globjects/releases/download/v1.0.0/globjects-1.0.0-msvc2015-x64-runtime.zip),
[examples](https://github.com/cginternals/globjects/releases/download/v1.0.0/globjects-1.0.0-msvc2015-x64-examples.zip), and
[dev](https://github.com/cginternals/globjects/releases/download/v1.0.0/globjects-1.0.0-msvc2015-x64-dev.zip).
Alternatively, download the source code and commence [building from source](#build-instructions).

###### Ubuntu

*globjects* is provided on Ubuntu using PPAs.
For Ubuntu 16.04 (xenial), 15.10 (wily), and 15.04 (vivid) use the [current PPA](https://launchpad.net/~cginternals/+archive/ubuntu/ppa), for Ubuntu 14.04 (trusty) use the [backports PPA](https://launchpad.net/~cginternals/+archive/ubuntu/backports-ppa).
Using the current PPA as example, the following lines install *globjects* including the GLFW examples:
```shell
> sudo apt-add repository ppa:cginternals/ppa
> sudo apt-get update
> sudo apt-get install libglobjects-examples-glfw
> # start example
> /usr/share/globjects/computeshader
```
To use globjects as dependency, install the development package:
```shell
> sudo apt-get install libglobjects-dev libglobjects-dbg
```
Alternatively, download the source code and commence [building from source](#build-instructions).

###### OS X

The package manager on OS X we depend on is homebrew. The package there is called [globjects](http://brewformulas.org/Globjects).
To install *globjects* using homebrew, execute the following line:
```shell
> brew install globjects
```

Alternatively, download the source code and commence [building from source](#build-instructions).

###### Debian-based Systems

There is currently no precompiled package maintained. Please download the source code and commence [building from source](#build-instructions).


## Build Instructions

##### Prerequisites and Dependencies

The only mandatory run-time dependencies of globjects are the STL of the used compiler, glbinding, and an OpenGL driver library, dynamically linked with your application. However, compiling *globjects* requires the following required and optional dependencies:
* [CMake](https://cmake.org/) 3.0 or higher for building globjects from source (mandatory for any build from source)
* [git](https://git-scm.com/) for version control and script supporting tasks
* [GLFW](http://www.glfw.org/) 3.0 or higher for examples
* [cpplocate](https://github.com/cginternals/cpplocate) for the examples
* [Qt5](http://www.qt.io/developers/) 5.0 or higher for the qt-based example
* [Doxygen](http://www.stack.nl/~dimitri/doxygen/) 1.8 or higher for generating the documentation on your system
  * [graphviz](http://www.graphviz.org/) for generating diagrams (optional)

##### Compile Instructions

For compilation, a C++11 compliant compiler, e.g., GCC 4.8, Clang 3.3, MSVC 2013 **Update 3**, is required.

First, download the source code [as archive](https://github.com/cginternals/globjects/releases) or via git:
```shell
> git clone https://github.com/cginternals/globjects.git
> cd globjects
```
Then, depending on the version of globjects you want to build, choose the appropriate tag or branch, e.g., for the 1.0.0 release:
```shell
> git fetch --tags
> git checkout v1.0.0
```
The actual compilation can be done using CMake and your favorite compiler and IDE.

For building *globjects* CMake via command line can be used (should work on all systems):

First create a build directory (we do not recommend in-source builds):
```shell
> mkdir build
> cd build
```

Configure *glbinding* with your prefered or default generator, e.g., for Visual Studio 2015 in x64 use
(note: some IDEs have integrated support for CMake projects, e.g., Qt Creator, and allow you to skip the manual project configuration):
```shell
> cmake .. -G "Visual Studio 14 2015 Win64"
```

In order to compile the project, either use you favorite Editor/IDE with the created project or use CMake as follows:
```shell
> cmake --build .
```


## Tips for Linking

We suggest using the build system of globjects for a smooth integration: [CMake](https://cmake.org/)
For it, *globjects* provides a find configuration script that should be installed into your system or at least accessible by CMake. 
In the projects CMakeLists.txt, add one of the following lines:
```
find_package(globjects QUIET) # if you want to check for existance
find_package(globjects REQUIRED) # if it is really required in your project
```
Finally, just link glbinding to your own library or executable:
```
target_link_libraries(${target} ... PUBLIC globjects::globjects)
```
