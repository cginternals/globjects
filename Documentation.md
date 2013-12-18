#GLOW Introduction

##glowwindow::ContextFormat
A storage object for various [GLFW]( http://www.glfw.org/docs/latest/index.html "GLWF webpage." ) related settings like [window hints]( http://www.glfw.org/docs/latest/window.html#window_hints "GLFW window creations hints documentation"), and the [buffer swap interval]( http://www.glfw.org/docs/latest/window.html#window_swap "GLFW buffer swapping documentation" ). GLFW distinguishes between hard and soft constrained windows hints, which means that not all settings (the soft ones) must be matched exactly. One can use `glowwindow::ContextFormat::verify(format1, format2)` to check that two ContextFormats match (e.g. the requested and the one with the settings finally created by GLFW) and get warning messages for varying settings. 

##glowwindow::Window
Represents a windows of the OS specific windows handling system.
Provides Methods to control the apperance and behaviour of the window like fullscreen mode, size and event handling.

After creating a `ContextFormat` object with the desired settings a new `glowwindow::Window` object can be created with an optional title, width and height.

```cpp
glowwindow::ContextFormat profile;
profile.set...
...

bool success = window.create(format, "Title", 1024, 768)
```
Internally `glowwindow::Window` will first create a `glowwindow::Context` object that acts as an interface for the [OpenGL Context]( http://www.opengl.org/wiki/OpenGL_Context "Explains OpenGL Context." ) related methods of  `GLFW::Window`. `glowwindow::Context` initializes GLFW (`glfwInit`), creates a new `GLFW::Window` with the settings defined in the `glowwindow::ContextFormat` and initializes [GLEW]( http://glew.sourceforge.net/ "GLEW webpage.") inside the OpenGL Context of the `GLFW::Window`. After `glowwindow::Context` has sucessfully done its work `glowwindow::Window` initializes the event handling System. If an error occurs in any of the above steps `glowwindow::Window::create` will return false and should not be used.


An EventHandler can be set with 
```cpp
Window::setEventHandler(WindowEventHandler *eventHandler)
```

##glowwindow::WindowEventHandler
Describes an Interface that can be implemented by the user to react to the different events of an `glowwindow::Window`.
Each event handler method is executed in the [OpenGL Context]( http://www.opengl.org/wiki/OpenGL_Context "Explains OpenGL Context." ) of the `glowwindow::Window` to which the event handler was assigned. The follwing list describes the events that can be received when overiding the virtual methods of `glowwindow::WindowEventHandler`. Since most GLOW events map to a GLFW event the following descriptions where extracted from the [GLFW window handling documentation]( http://www.glfw.org/docs/latest/group__window.html "The GLWF Documentation for window handling.") 

* `resizeEvent(ResizeEvent & event)`: Called when the window is resized; event contains the new size, in screen coordinates, of the client area of the window.

* `framebufferResizeEvent(ResizeEvent & event)`: Called when the framebuffer of the window is resized; event contains the new size of the framebuffer in pixel.

* `moveEvent(MoveEvent & event)`: Called when the window is moved; event contains the screen position of the upper-left corner of the client area of the window.

* `paintEvent(PaintEvent & event)`: Called when the client area of the window needs to be redrawn, for example if the window has been exposed after having been covered by another window. On compositing window systems such as Aero, Compiz or Aqua, where the window contents are saved off-screen, this callback may be called only very infrequently or never at all. After the event handler has been executed the buffers of the window are swapped.

* `keyPressEvent(KeyEvent & event)`: Called when a key is pressed or repeated; `KeyEvent` provides the following methods to get information about the pressed key:

    * `int key()` Code of the key board key. Use the [GLFW Macros to identify the key]( http://www.glfw.org/docs/latest/group__keys.html "GLFW Keyboard Key macros." ). The macros are named after thier values in the standard US keyboard layout.

    * `int scanCode()` The system specific [Scancode]( http://en.wikipedia.org/wiki/Scancode "Wikipedia Scancode.") of the key.

    * `int action()` The action `GLFW_PRESS` or `GLFW_REPEAT`

    * `int modifiers()` Bitfield describing which [modifier keys]( http://www.glfw.org/docs/latest/group__mods.html "GLFW modifier keys." ) were held down

* `keyReleaseEvent(KeyEvent & event)`: Called when a key is released. The same methods of `KeyEvent` as in `keyPressEvent` can be used to get information about the released key, but `action()` will alwayd return `GLFW_RELEASE`.

* `mousePressEvent(MouseEvent & event)`: Called when a mouse button is pressed; `MouseEvent` provides the followinf methods to get information about the button:

	* `int button()` The [mouse button]( http://www.glfw.org/docs/latest/group__buttons.html "GLFW mouse button macros.") that was pressed.

	* `int modifiers()` Bitfield describing which [modifier keys]( http://www.glfw.org/docs/latest/group__mods.html "GLFW modifier keys." ) were held down

* `mouseReleaseEvent(MouseEvent & event)`: Called when a mouse button is released. When a window loses focus, it will generate synthetic mouse button release events for all pressed mouse buttons. You can tell these events from user-generated events by the fact that the synthetic ones are generated after an `FocusEvent` with `FocusEvent::hasFocus() == false`.

* `mouseReleaseEvent(MouseEvent & event)`: Called when a mouse button is released. The same methods of `MouseEvent` as in `mouseReleaseEvent` can be used to get information about the button.

* `mouseMoveEvent(MouseEvent & event)`: Called when the mouse cursor is moved; 'MouseEvent::x()' and 'MouseEvent::y()' provide the position, in screen coordinates rounded downwards, relative to the upper-left corner of the client area of the window.

* `scrollEvent(ScrollEvent & event)`: Called when a scrolling device is used, such as a mouse wheel or scrolling area of a touchpad.

    * `glm::vec2& ScrollEvent::offset()` The scroll offset along the x and y Axis

    * `glm::ivec2 & ScrollEvent::pos()` The mouse x and y position of the mouse.

* `focusEvent(FocusEvent & event)`: Called when the window gains or loses focus.

	* `FocusEvent::hasFocus()` True when the window has gained focus otherwise false.

* `iconifyEvent(IconifyEvent & event)`: Called when a window is iconified (e.g. minimized on Windows) or restored (e.g. Windows: clicking on the icon of the window in the taskbar)

	* `bool isIconified()`: True when the window was iconified, false when it was restored.

* `timerEvent(TimerEvent& event)`: Called when a timer created with `glowwindow::Window::addTimer` expires.

	* `int id()` The id of the timer. If the timer was set as single shot it is removed after the event handler returns. You can use the id to remove the timer: `event.window()->removeTimer(event.id())`

* `void initialize(Window & window)`: Called once when the window has been created (before `Window::create'' returns).

* `void finalize(Window & window)`: Called once before the window is destroyed.

* `void idle(Window & window)`: Called when in the current main loop iteration no event was in the event queue of the window. Usally at least the paintEvent is called every loop iteration, but for fixed repaint time this must not be the case.

Each event object can be marked as accepted (`WindowEvent::accept()`) or ignored (`WindowEvent::ignore()`). By default an event object is marked as ignored. After the event handler method returns ignored event objects are evaluated by the `glowwindow::Window` object to trigger default actions (like closing the window when ESC was pressed, see `glowwindow::Window::defaultEventAction`). If you do not want default actions to happen mark the event object as accepted.

#Internals: The event handling system

##glowwindow::WindowEventDispatcher
A Singleton that acts as a mediator between the GLFW events of multiple `GLFW::Window` objects and their corresponding `glowwindow::Window` object.
Therefore a `glowwindow::Window` object must be registered with the `glowwindow::WindowEventDispatcher` using
```cpp
glowwindow::WindowEventDispatcher::register(Window* window)
```
WindowEventDispatcher then sets its own methods as callbacks for the various `GLFW::Window` events. Incoming events are translated into `glowwindow::WindowEvent` objects (each event type has its own subclass of `glowwindow::WindowEvent` like `glowwindow::MouseEvent`) and append to the event queue of the corresponding `glowwindow::Window` object. Furthermore the event dispatcher can create Timer events for a `glowwindow::Window`.

##glowwindow::MainLoop
A Singleton that implements the main loop of the program. In the main loop the following things are done:

1. Process all events that have been retrieved since the last iteration (this will tigger the callbacks in `glowwindow::WindowEventDispatcher`)

2. For each `glowwindow::Window` object that has events in its queue call the user defined event handler methods. Before the user event handler methods are called the OpenGL context of the `glowwindow::Window` object is set as current.

`glowwindow::MainLoop::start` can be called to start the main loop and `glowwindow::MainLoop::exit` to exit the main loop.