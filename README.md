![globjects Logo](globjects-logo.png "globjects")

*globjects* is an [MIT licensed](http://opensource.org/licenses/MIT), cross-platform C++ wrapper for [OpenGL API](http://www.opengl.org) objects.

*globjects* provides object-oriented interfaces to the OpenGL API (3.0 and higher).
It reduces the amount of OpenGL code required for rendering and facilitates coherent OpenGL use by means of an additional abstraction layer to [glbinding](https://github.com/cginternals/glbinding). Common rendering tasks and processes are automated and missing features of specific OpenGL drivers are partially simulated or even emulated at run-time.

![what-is-globjects](https://raw.githubusercontent.com/cginternals/globjects/master/docs/what-is-globjects.png)

The following code snippet shows an exemplary use of the OpenGL API:
```cpp
// OpenGL API

auto program = glCreateProgram();
auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);

glCompileShader(vertexShader);
glCompileShader(fragmentShader);

glAttachShader(program, vertexShader);
glAttachShader(program, fragmentShader);

glLinkProgram(program);

glUseProgram(program);
glUniform2f(glGetUniformLocation(program, "extent"), 1.0f, 0.5f);
```

Using *globjects*, this can be reduced to the following code:
```cpp
// globjects API

auto program = new Program();

program->attach(
  Shader::fromString(GL_VERTEX_SHADER, vertexShaderSource), 
  Shader::fromString(GL_FRAGMENT_SHADER, fragmentShaderSource)
);

program->setUniform("extent", glm::vec2(1.0f, 0.5f)));
```
If enabled, this code checks (1) for GL errors (```glGetError```) after each call, (2) shaders for compilation errors, and (3) the program for linker errors.

## Professional Support

CG Internals offers computer graphics R&D as well as reliable technology and innovative concepts to support your computer graphics visions. 
We provide trainings and can help you integrate and customize globjects in your next project.

Visit [Professional Support and Services](https://www.cginternals.com) for more details.


## Resources

* [Examples](https://github.com/cginternals/globjects/wiki/examples)
* [Project Health](#project-health)

###### Installation and Development

* [Install Instructions](#install-instructions)
* [Build form Source](#build-instructions)
* [Tips for Linking](#tips-for-linking)

###### Context Management

* [Initializing Contexts](#initialize-contexts)

###### Wrapped OpenGL Objects and Code Snippets

* [Global Functions](#global-functions)
* [Buffer](#buffer)
* [Texture](#texture)
* [State](#state)
* [Error](#error)
* [Debug Message](#debug-message)
* [Framebuffer](#framebuffer)
* [Named String](#named-string)
* [Program](#program)
* [Program Pipeline](#program-pipeline)
* [Query](#query)
* [Renderbuffer](#renderbuffer)
* [Sampler](#sampler)
* [Shader](#shader)
* [Sync](#sync)
* [Transform Feedback](#transform-feedback)
* [Uniform](#uniform)
* [Uniform Block](#uniform-block)
* [Vertex Array](#vertex-array)
* [Vertex Attribute Binding](#vertex-attribute-binding)

###### Additional Feature Documentation and Code Snippets

* [Reference pointer as memory model](#reference-pointers)
* [Shader templates](#shader-templates)
* [Manual strategy override](#strategy-override)
* [iostream Integration](#logging)


## Project Health

| Service | System | Compiler | Status |
| ------- | ------ | -------- | -----: |
|  [Travis-CI](https://travis-ci.org/cginternals/globjects) | Ubuntu 14.04 | GCC 4.8, Clang 3.5 | [![Build Status](https://travis-ci.org/cginternals/globjects.svg?branch=master)](https://travis-ci.org/cginternals/globjects) |
|  [Travis-CI](https://travis-ci.org/cginternals/globjects) | OS X | Clang ? | upcoming |
| [Coverity](https://scan.coverity.com/projects/6829?tab=overview) | Ubuntu 14.04 | GCC 5.4 | [![Coverity Status](https://scan.coverity.com/projects/6829/badge.svg)](https://scan.coverity.com/projects/6829) |
| Jenkins <br><br><br><br> | Ubuntu 14.04 <br><br><br><br> | GCC 4.8 <br> GCC 4.9 <br> GCC 5.4 <br> Clang 3.9 <br> | [![Build Status](https://jenkins.hpi3d.de/buildStatus/icon?job=globjects-linux-gcc4.8)](https://jenkins.hpi3d.de/job/globjects-linux-gcc4.8) <br> [![Build Status](https://jenkins.hpi3d.de/buildStatus/icon?job=globjects-linux-gcc4.9)](https://jenkins.hpi3d.de/job/globjects-linux-gcc4.9) <br> [![Build Status](https://jenkins.hpi3d.de/buildStatus/icon?job=globjects-linux-gcc5)](https://jenkins.hpi3d.de/job/globjects-linux-gcc5) <br> [![Build Status](https://jenkins.hpi3d.de/buildStatus/icon?job=globjects-linux-clang3.9)](https://jenkins.hpi3d.de/job/globjects-linux-clang3.9) <br> |
| Jenkins <br><br> | Windows 10 <br><br> | MSVC 2013 Update 5 <br>  MSVC 2015 Update 1 <br> | [![Build Status](https://jenkins.hpi3d.de/buildStatus/icon?job=globjects-windows-msvc2013)](https://jenkins.hpi3d.de/job/globjects-windows-msvc2013) <br> [![Build Status](https://jenkins.hpi3d.de/buildStatus/icon?job=globjects-windows-msvc2015)](https://jenkins.hpi3d.de/job/globjects-windows-msvc2015) <br> |

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
* [glbinding](https://github.com/cginternals/glbinding) as OpenGL API binding
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

Configure *globjects* with your prefered or default generator, e.g., for Visual Studio 2015 in x64 use
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
Finally, just link globjects to your own library or executable:
```
target_link_libraries(${target} ... PUBLIC globjects::globjects)
```

## Initialize Contexts

globjects can handle multiple OpenGL contexts. For each context, you have to initialize the globjects state.
Further, you have to tell globjects which context is active on a per-thread basis.
```cpp
#include <globjects/globjects.h>

// manage contexts
init();

// set explicit context active
setContext(contextID);

// set current context active
setCurrentContext();
```

You can also use glbinding to automatically sync OpenGL active contexts and their glbinding and globjects counterparts:
```cpp
glbinding::Binding::addContextSwitchCallback([](glbinding::ContextHandle handle) {
    setContext(handle);
}
```
The only additional thing to do is telling glbinding when a context is switched (per thread).
```cpp
glbinding::Binding::useContext(handle);
```

## Wrapped OpenGL Objects

##### Global Functions

Some often used functions are wrapped to ease the interface as proposed by the OpenGL API.
```cpp
// somehow similar to glbinding

std::string extensions = getString(GL_EXTENSIONS);
int numExtensions = getInteger(GL_NUM_EXTENSIONS);

if (isCoreProfile())
{
    return renderer(); // returns the GL_RENDERER string
}
```

##### Buffer

A buffer in means of OpenGL can be used for vertex attributes, indices, uniform data, atomic counters, texture data, and shader storage data.
```cpp
auto buffer = new Buffer();

// Using buffer data
buffer->setData({{ 0, 1, 2, 3, 4}}, GL_STATIC_DRAW);

// Using buffer storage
buffer->setStorage({{ 0, 1, 2, 3, 4}}, GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT);

buffer->setSubData({{ 4, 3, 2 }}, 0);
buffer->bindBase(GL_SHADER_STORAGE_BUFFER, 0);
```

##### Texture

Texture supports both traditional interfaces and bindless support.
```cpp
auto texture1 = new Texture(GL_TEXTURE_2D); // type has to be fix during lifetime
texture1->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
texture1->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
texture1->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
texture1->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

texture1->image2D(0, GL_RGBA8, glm::ivec2(512), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
texture1->clearImage(0, GL_RGBA, GL_UNSIGNED_BYTE, glm::ivec4(255, 255, 255, 255));
texture1->generateMipMap();

auto texture2 = Texture::createDefault(); // creates a default-configured 2D texture

auto handle = texture2->textureHandle(); // for bindless texturing
texture2->bindActive(0); // For traditional texturing
```

##### State

OpenGL state is wrapped as States, StateSettings and Capabilities, where the latter two are mainly used internally.
```cpp
auto currentState = State::currentState(); // full current state; usable for resetting

auto state1 = new State(State::ImmediateMode); // all changes are applied immediately
state1->enable(GL_RASTERIZER_DISCARD); // Configuring a Capability
state1->primitiveRestartIndex(static_cast<GLuint>(-1)); // Configuring a StateSetting

auto state2 = new State(State::DeferredMode); // changes has to be applied explicitly
state2->pointSize(10.0f);
state2->apply();

currentState->apply(); // Reset manipulated state
```

##### Error

```cpp
auto error = Error::get();

if (error)
{
    std::cout << "Error " << std::hex << error.code() << ": " << error.name() << std::endl;
}
```

##### Debug Message

Enable DebugMessages to get performance hints, warnings and errors from your OpenGL driver.
```cpp
DebugMessage::enable(); // enable automatic messages if KHR_debug is available

DebugMessage::setCallback([](const DebugMessage & message) {
    std::cout << message.message() << std::endl;
}); // if you want to handle messages by yourself
```

##### Framebuffer

Wraps a canvas with multiple render targets to render on.
```cpp
auto fbo = new Framebuffer();
fbo->attachTexture(GL_COLOR_ATTACHMENT0, texture1);
fbo->attachTexture(GL_COLOR_ATTACHMENT1, texture2);
fbo->attachRenderbuffer(GL_DEPTH_ATTACHMENT, depthRenderbuffer);
fbo->setDrawBuffers({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_NONE });
fbo->printStatus(true); // Print errors if fbo is not complete

fbo->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
fbo->clearBuffer(GL_COLOR, 0, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));

fbo->blit(GL_COLOR_ATTACHMENT0, {{ 0, 0, width, height }}, Framebuffer::defaultFBO(),
    GL_BACK_LEFT, {{ 0, 0, width, height }}, GL_COLOR_BUFFER_BIT, GL_NEAREST);
```

##### Named String

Register compile-time shader replacements for shader includes.
```cpp
// typically the only function call you'll need
auto namedString1 = new NamedString("/upNormal.glsl", "const vec3 up = vec3(0.0, 1.0, 0.0);");

// or reference an actual source file
auto namedString2 = new NamedString("/phong.glsl", new File("data/shaders/phong.glsl"));
```

##### Program

The Program object can represent both render programs and compute programs. Prior usage it automatically relinks upon shader changes.
```cpp
auto renderProgram = new Program();
renderProgram->attach(vertexShader, fragmentShader);
renderProgram->addUniform("viewProjection", glm::mat4(1.0));

renderProgram->use(); // compiles shaders, links and uses program

auto computeProgram = new Program();
computeProgram->attach(computeShader);

computeProgram->dispatchCompute(128, 1, 1);
```

##### Program Pipeline

```cpp
auto pipeline = new ProgramPipeline();
pipeline->useStages(vertexProgram, gl::GL_VERTEX_SHADER_BIT);
pipeline->useStages(fragmentProgram, gl::GL_FRAGMENT_SHADER_BIT);
pipeline->use(); // as Program interface
```

##### Query

Query and measure time and perform conditional rendering with passed samples.
```cpp
auto query = new Query();
query->begin(GL_TIME_ELAPSED);
// calls
query->end(GL_TIME_ELAPSED);

if (!query->resultsAvailable())
{
    query->wait();
}

auto elapsed = query->get(GL_QUERY_RESULT);
```

##### Renderbuffer

```cpp
auto renderBuffer = new Renderbuffer();
renderBuffer->storage(GL_RGBA32F, 512, 512);
```

##### Sampler

For temporary overrides of texture parameters. Note: a newly created sampler is not configured by default, and thus invalid.
```cpp
auto sampler = new Sampler();
sampler->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
sampler->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
sampler->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
sampler->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

sampler->bind(0); // override sampler state for texture at binding point 0
```
##### Shader

```cpp
auto shader1 = new Shader::fromFile(GL_VERTEX_SHADER, filename);
auto shader2 = new Shader::fromString(GL_FRAGMENT_SHADER, shaderSource);

Shader::globalReplace("#version 140", "#version 150"); // e.g., useful for OS X

shader1->setIncludePaths({ std::string("/data") });

shader2->compile();
std::cout << shader2->infoLog() << std::endl; // acess compile info log, although it's done automatically if there is a compile error
```

##### Sync

```cpp
auto sync = Sync::fence(GL_SYNC_GPU_COMMANDS_COMPLETE);

sync->clientWait(GL_SYNC_FLUSH_COMMANDS_BIT, 2000000000); // wait on GPU; 2 secs
sync->waitSync(1000000); // wait on CPU; 1 millisecond
```

##### Transform Feedback

Connect shader outputs to buffers and restart drawing.
```cpp
auto tf = new TransformFeedback();
tf->setVaryings(program, { { "next_position" } }, GL_INTERLEAVED_ATTRIBS);

tf->bind();
glEnable(GL_RASTERIZER_DISCARD);
program->use();
tf->begin(GL_TRIANGLES);
vao->drawArrays(GL_TRIANGLES, 0, 3);
tf->end();
glDisable(GL_RASTERIZER_DISCARD);
tf->draw(GL_TRIANGLE_STRIP);
```

##### Uniform

Uniforms attached to Programs are updated automatically, even after relinking.
```cpp
auto uniform1 = new Uniform<glm::vec3>("lightPos", glm::vec3(10.0f, 5.0f, 0.0f)); // name-based uniform binding
auto uniform2 = new Uniform<glm::mat4>(0, glm::mat4(1.0f)); // location-based uniform binding

program->addUniform(uniform1);
program->addUniform(uniform2);

program->use(); // uniform values are updated if required
```

##### Uniform Block

Use uniform blocks for large, often switched chunks of uniforms.
```cpp
auto block = program->uniformBlock("uniforms");
block->setBinding(0);
buffer->bindBase(GL_UNIFORM_BUFFER, 0);
```

##### Vertex Array

Use to configure vertex shader inputs and trigger render pipeline processes.
```cpp
auto vao = new VertexArray();
// configure bindings (see next section)

vao->enable(0);
vao->enable(1);

vao->drawArrays(GL_POINTS, 0, 10);
```

##### Vertex Attribute Binding

```cpp
// For attribute pointers
auto binding1 = vao->binding(0);
binding1->setBuffer(vertexBuffer, 0, sizeof(glm::vec3));
binding1->setFormat(3, GL_FLOAT, GL_FALSE, 0);

// For static attributes for each vertex
auto binding2 = vao->binding(0);
binding2->setValue<float>(1.0f);
```

## Additional Features

##### Reference Pointers

globjects uses the RAII (resource allocation is initialization) principle, meaning that created objects are also created on the GPU.
To effectively manage the dual-allocated memory, we use reference pointers.
We advise that every globjects ```Object``` pointer is stored in a ```ref_ptr```.
```cpp
{
    ref_ptr<Query> query = new Query(); // allocate on CPU and GPU
    
    // query is destructed and freed on GPU at the end of the block.
}
```
As the objects in globjects uses ```ref_ptr``` to store references, not using reference counting can lead to *accidentally* freed objects.
If you don't want to use smart pointers, you have to use the manual reference counting interface:
```cpp
program->ref(); // increase reference count
program->unref(); // decreare reference count; potentially free program pointer and GPU program
```

##### Shader Templates

The sources of Shaders (```ShaderSource```) can be configured and templated.
```cpp
auto template = new StringTemplate(new File("fragmentShader.frag"));
template->replace("REPLACE", "WITH THIS");

auto shader = new Shader(template);
```

##### Strategy Override

Although globjects tries to use most current OpenGL APIs, you can override this automatic process.
```cpp
// Enable CPU shader includes (although supported, some drivers have problems, so disable it)
globjects::init(Shader::IncludeImplementation::Fallback);

// Update strategy at run-time
Buffer::hintBindlessImplementation(Buffer::BindlessImplementation::Legacy);
```

##### Logging

globjects provides logging interfaces to its objects as well as glm objects.
```cpp
std::cout << Framebuffer::defaultFBO();
std::cout << glm::vec4(1.0, 0.0, 0.0, 1.0);
warning() << shader;
```
