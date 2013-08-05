#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <string>
#include <unordered_map>
#include <functional>

class GlutWindow
{
public:
	GlutWindow(const std::string& name, int x, int y, int w, int h);
	virtual ~GlutWindow();

	int id() const;

	void update();

	void destroy();
	bool isDestroyed() const;

	virtual void initialize();

	virtual void paint();
	virtual void resizeEvent(int width, int height);
	virtual void keyTypedEvent(unsigned char key, int x, int y);
	virtual void specialKeyEvent(int key, int x, int y);
	virtual void mouseDownEvent(int button, int x, int y);
	virtual void mouseUpEvent(int button, int x, int y);
	virtual void mouseDragEvent(int x, int y);
	virtual void mouseMoveEvent(int x, int y);
	virtual void mouseWheelEvent(int delta);
	virtual void idle();
protected:
	int registerTimer(unsigned msecs, std::function<void(void)> callback, bool repeat = false);
	void deregisterTimer(int id);
private:
	int _id;
	bool _destroyed;
	struct Timer
	{
		 std::function<void(void)> callback;
		 bool repeat;
		 unsigned msecs;
	};
	std::unordered_map<int, Timer> _timers;
	int nextTimerId;

	void setup(const std::string& name, int x, int y, int w, int h);

	void handlePaint();
	void handleResize(int width, int height);
	void handleKeyboard(unsigned char key, int x, int y);
	void handleSpecial(int key, int x, int y);
	void handleMouse(int button, int state, int x, int y);
	void handleMotion(int x, int y);
	void handlePassiveMotion(int x, int y);
	void handleClose();
	void handleIdle();
	void handleTimer(int value);
private:
	static GlutWindow* currentWindow();

	static void dispatchPaint();
	static void dispatchResize(int width, int height);
	static void dispatchKeyboard(unsigned char key, int x, int y);
	static void dispatchSpecial(int key, int x, int y);
	static void dispatchMouse(int button, int state, int x, int y);
	static void dispatchMotion(int x, int y);
	static void dispatchPassiveMotion(int x, int y);
	static void dispatchClose();
	static void dispatchIdle();
	static void dispatchTimer(int value);
};
