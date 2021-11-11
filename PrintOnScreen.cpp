//#include <stdio.h>
//#include <string.h>
//#include <glut.h>
//
//
//
//
////this is the method used to print text in OpenGL
////there are three parameters,
////the first two are the coordinates where the text is display,
////the third coordinate is the string containing the text to display
//void print(int x, int y, char* string)
//{
//	int len, i;
//
//	//set the position of the text in the window using the x and y coordinates
//	glRasterPos2f(x, y);
//
//	//get the length of the string to display
//	len = (int)strlen(string);
//
//	//loop to display character by character
//	for (i = 0; i < len; i++)
//	{
//		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
//	}
//
//
//}
//
//
//
//void Display() {
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glColor3f(1, 0, 0);
//	char* p0s[20];
//	sprintf((char*)p0s, "Score: %d", 50);
//	print(500, 300, (char*)p0s);
//
//
//	glFlush();
//}
//
//
//void main(int argc, char** argr) {
//	glutInit(&argc, argr);
//
//	glutInitWindowSize(1000, 600);
//
//	glutCreateWindow("OpenGL - 2D Template");
//	glutDisplayFunc(Display);
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//	gluOrtho2D(0.0, 1000, 0.0, 600);
//
//	glutMainLoop();
//}
