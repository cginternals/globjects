![globjects Logo](globjects-logo.png "globjects")

*globjects* is a [MIT licensed](http://opensource.org/licenses/MIT), strict C++ [OpenGL API](http://www.opengl.org) objects wrapper based on [glbinding](https://github.com/cginternals/glbinding).

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
