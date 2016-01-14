![globjects Logo](globjects-logo.png "globjects")

*globjects* provides an object oriented C++ interface for OpenGL's programmable pipeline (3.1+).
*globjects* is a cross-platform library licenced under the [MIT license](http://opensource.org/licenses/MIT).

The latest release is [globjects-0.4.2](https://github.com/hpicgs/globjects/releases/tag/v0.4.2).

To find out more about globjects and how to use it, check out our [wiki](https://github.com/hpicgs/globjects/wiki).

## Project Health (master)

| Service | System | Compiler | Options | Status |
| ------- | ------ | -------- | ------- | ------ |
| Jenkins | Ubuntu 14.04 | GCC 4.8 | all | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=globjects-linux-gcc4.8)](http://jenkins.hpi3d.de/job/globjects-linux-gcc4.8)|
| Jenkins | Ubuntu 14.04 | GCC 4.9 | all | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=globjects-linux-gcc4.9)](http://jenkins.hpi3d.de/job/globjects-linux-gcc4.9)|
| Jenkins | Ubuntu 14.04 | GCC 5.3 | all | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=globjects-linux-gcc5.3)](http://jenkins.hpi3d.de/job/globjects-linux-gcc5.3)|
| Jenkins | Ubuntu 14.04 | Clang 3.5 | all | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=globjects-linux-clang3.5)](http://jenkins.hpi3d.de/job/globjects-linux-clang3.5) |
| Jenkins | OS X 10.10 | Clang 3.5 | all | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=globjects-osx-clang3.5)](http://jenkins.hpi3d.de/job/globjects-osx-clang3.5) |
| Jenkins | Windows 8.1 | MSVC 2013 Update 5 | all | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=globjects-windows-msvc2013)](http://jenkins.hpi3d.de/job/globjects-windows-msvc2013) |
| Jenkins | Windows 8.1 | MSVC 2015 Update 1 | all | [![Build Status](http://jenkins.hpi3d.de/buildStatus/icon?job=globjects-windows-msvc2015)](http://jenkins.hpi3d.de/job/globjects-windows-msvc2015) |

## Features

ToDo

##### ToDo

ToDo

## Using globjects

ToDo

##### Dependencies

ToDo

##### Linking binaries

ToDo

### Documentation

The documentation is regularly generated and can be found [here](http://libglow.org/doxygen-master).


### Dependencies

The following dev-libraries and programs need to be provided for correct CMake configuration:
* C++11 compatible compiler (e.g. gcc >=4.7, clang >=3.4, MSVC >= 2013 **Update 3**)
* CMake (>= 2.8.12): http://www.cmake.org/
* OpenGL C++ Bindings (glbinding >= 1.0): https://github.com/hpicgs/glbinding
* OpenGL Mathematics (GLM >= 0.9.5): http://glm.g-truc.net/, https://github.com/g-truc/glm
* Window and Context creation (GLFW >= 3.0.4): http://www.glfw.org/ (optional, needed for examples)


### Development Notes

If you are contributing to this project, please keep the following notes in mind:
* Add your name and email to the AUTHORS file.
* Follow coding conventions according to google's [C++ Style Guide](http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml).
* Commits should provide a meaningful  message, which uses the imperative, present tense: "change", not "changed" nor "changes", AND start with a capital letter.
* Commits should always refer to an issue: use ```#xxx```, ```fix #xxx```, or ```close #xxx```.
* Pull Requests are reviewed by at least one other developer on another platform.
* Use lazy initialization as often as possible for time consuming tasks.
* Member Variables always start with ```m_```, ```g_```, and ```s_``` (e.g.,```m_member```, ```g_global```, and ```s_static```)..
* Interface or abstract class names always start with Abstract (e.g., ```AbstractArray```).
* Enforce strict include sequence: cpp-related header, std, glm, glbinding, globjects.
