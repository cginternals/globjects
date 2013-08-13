#include <GlutWindow.h>


GlutWindow::GlutWindow(const std::string& name, int x, int y, int w, int h)
: _destroyed(false)
, _id(0)
{
	setup(name, x, y, w, h);
}

GlutWindow::~GlutWindow()
{
	if (!_destroyed)
	{
		destroy();
	}
}

void GlutWindow::setup(const std::string& name, int x, int y, int w, int h)
{
	glutInitWindowPosition(x, y);
	glutInitWindowSize(w, h);

	_id = glutCreateWindow(name.c_str());

	glutSetWindowData(static_cast<void*>(this));

	glutDisplayFunc(GlutWindow::dispatchPaint);
	glutReshapeFunc(GlutWindow::dispatchResize);
	glutKeyboardFunc(GlutWindow::dispatchKeyboard);
	glutSpecialFunc(GlutWindow::dispatchSpecial);
	glutMouseFunc(GlutWindow::dispatchMouse);
	glutMotionFunc(GlutWindow::dispatchMotion);
	glutPassiveMotionFunc(GlutWindow::dispatchPassiveMotion);
	glutCloseFunc(GlutWindow::dispatchClose);
	glutIdleFunc(GlutWindow::dispatchIdle);
}

int GlutWindow::id() const
{
	return _id;
}

void GlutWindow::update()
{
	glutPostRedisplay();
}

void GlutWindow::destroy()
{
	glutDestroyWindow(_id);
}

void GlutWindow::initialize()
{
	glewExperimental = GL_TRUE;
	glewInit();
}

void GlutWindow::paint()
{
}

void GlutWindow::resizeEvent(int width, int height)
{
}

void GlutWindow::keyTypedEvent(unsigned char key, int x, int y)
{
}

void GlutWindow::specialKeyEvent(int key, int x, int y)
{
}

void GlutWindow::mouseDownEvent(int button, int x, int y)
{
}

void GlutWindow::mouseUpEvent(int button, int x, int y)
{
}

void GlutWindow::mouseDragEvent(int x, int y)
{
}

void GlutWindow::mouseMoveEvent(int x, int y)
{
}

void GlutWindow::mouseWheelEvent(int delta)
{
}

void GlutWindow::idle()
{
}


void GlutWindow::handlePaint()
{
	paint();
	glutSwapBuffers();
}

void GlutWindow::handleResize(int width, int height)
{
	resizeEvent(width, height);
}

void GlutWindow::handleKeyboard(unsigned char key, int x, int y)
{
	keyTypedEvent(key, x, y);
}

void GlutWindow::handleSpecial(int key, int x, int y)
{
	specialKeyEvent(key, x, y);
}

void GlutWindow::handleMouse(int button, int state, int x, int y)
{
	if (button<3)
	{
		if (state == GLUT_DOWN)
		{
			mouseDownEvent(button, x, y);
		}
		else if (state == GLUT_UP)
		{
			mouseUpEvent(button, x, y);
		}
	}
	else if (state == GLUT_DOWN)
	{
		if (state == GLUT_DOWN)
		{
			if (button == 3)
			{
				mouseWheelEvent(1); // up
			}
			else if (button == 4)
			{
				mouseWheelEvent(-1); // down
			}
		}
	}
}

void GlutWindow::handleMotion(int x, int y)
{
	mouseDragEvent(x, y);
}

void GlutWindow::handlePassiveMotion(int x, int y)
{
	mouseMoveEvent(x, y);
}

void GlutWindow::handleClose()
{
	_destroyed = true;
}

void GlutWindow::handleIdle()
{
	idle();
}

void GlutWindow::handleTimer(int value)
{
	if (_timers.find(value) == _timers.end()) return;

	Timer& timer = _timers[value];

	timer.callback();

	if (timer.repeat)
	{
		glutTimerFunc(timer.msecs, GlutWindow::dispatchTimer, value);
	}
	else
	{
		_timers.erase(value);
	}
}

int GlutWindow::registerTimer(unsigned msecs, std::function<void(void)> callback, bool repeat)
{
	int id = nextTimerId++;

	Timer timer = { callback, repeat, msecs };

	_timers[id] = timer;

	glutTimerFunc(msecs, GlutWindow::dispatchTimer, id);

	return id;
}

void GlutWindow::deregisterTimer(int id)
{
	_timers.erase(id);
}

// static dispatching

GlutWindow* GlutWindow::currentWindow()
{
	return static_cast<GlutWindow*>(glutGetWindowData());
}

void GlutWindow::dispatchPaint()
{
	if (GlutWindow* current = GlutWindow::currentWindow())
	{
		current->handlePaint();
	}
}

void GlutWindow::dispatchResize(int width, int height)
{
	if (GlutWindow* current = GlutWindow::currentWindow())
	{
		current->handleResize(width, height);
	}
}

void GlutWindow::dispatchKeyboard(unsigned char key, int x, int y)
{
	if (GlutWindow* current = GlutWindow::currentWindow())
	{
		current->handleKeyboard(key, x, y);
	}
}

void GlutWindow::dispatchSpecial(int key, int x, int y)
{
	if (GlutWindow* current = GlutWindow::currentWindow())
	{
		current->handleSpecial(key, x, y);
	}
}

void GlutWindow::dispatchMouse(int button, int state, int x, int y)
{
	if (GlutWindow* current = GlutWindow::currentWindow())
	{
		current->handleMouse(button, state, x, y);
	}
}

void GlutWindow::dispatchMotion(int x, int y)
{
	if (GlutWindow* current = GlutWindow::currentWindow())
	{
		current->handleMotion(x, y);
	}
}

void GlutWindow::dispatchPassiveMotion(int x, int y)
{
	if (GlutWindow* current = GlutWindow::currentWindow())
	{
		current->handlePassiveMotion(x, y);
	}
}

void GlutWindow::dispatchClose()
{
	if (GlutWindow* current = GlutWindow::currentWindow())
	{
		current->handleClose();
	}
}

void GlutWindow::dispatchIdle()
{
	if (GlutWindow* current = GlutWindow::currentWindow())
	{
		current->handleIdle();
	}
}

void GlutWindow::dispatchTimer(int value)
{
	if (GlutWindow* current = GlutWindow::currentWindow())
	{
		current->handleTimer(value);
	}
}
