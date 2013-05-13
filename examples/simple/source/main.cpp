#include <GL/glew.h>

#include <GL/freeglut.h>

#include <Window.h>

namespace {
	Window window;
	
	void paint() {
		window.paintGL();
		glutSwapBuffers();
	}
	
	void resize(int width, int height) {
		window.resizeGL(width, height);
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextFlags(GLUT_DEBUG);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1024,768);
	glutCreateWindow("Simple Example");
	
	window.initializeGL(argv[0]);
	
	glutDisplayFunc(paint);
	glutReshapeFunc(resize);
	
	glutMainLoop();
	
	return 0;
}
