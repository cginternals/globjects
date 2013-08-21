#include <GL/glew.h>
#include <GL/freeglut.h>

#include <SSBOWindow.h>
#include <glow/query.h>
#include <glow/Error.h>

#include <iostream>

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(4, 2);
	glutInitContextFlags(GLUT_DEBUG | GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	SSBOWindow window("Shader Storage Buffer Objects Example", 100, 100, 1024, 768);
	window.initialize();

	std::cout << "Created OpenGL " <<  glow::query::version().toString()  << " context (" << glow::query::versionString() << ")" << std::endl;

	glutMainLoop();

	return 0;
}
