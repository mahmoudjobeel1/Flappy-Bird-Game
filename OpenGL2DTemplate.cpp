#include <stdio.h>
using namespace std;
#include <Windows.h>
#include <MMSystem.h>
#include <mciapi.h>

#include <iostream>
#include<string.h>
#include<string>

#include <math.h>
#include <random>
#include <glut.h>
#include <algorithm> 
#include <Mmsystem.h>
#include <mciapi.h>
#include <playsoundapi.h>


#pragma comment(lib,"winmm.lib")


using std::string;
#define min(a,b) (((a) > (b)) ? (b) : (a))
#define max(a,b) (((a) > (b)) ? (a) : (b))

void drawRect(int x, int y, int w, int h);
void drawCircle(int x, int y, float r);
void Key(unsigned char key, int x, int y);
void Timer(int value);
int random(int min, int max);
bool checkOverlap(int R, int Xc, int Yc, int X1, int Y1, int X2, int Y2);
bool checkIntersection();
void initBHeights();
void initXPositions();
void Display();
void drawBarrier(int x,int y,int w,int h,float R,float G,float B);
void drawBird(int x, int y, int r);
void drawPower(int x, int y);
void outText(int x, int y, string val, float R, float G, float B);
void outValue(int x, int y, int val, float R, float G, float B);
void randomPower();
bool circleIntersect();
bool checkBorders();
bool checkISWin();

int status = 2;

int ballY = 400,ballX=100;       // holds the value of the Y coordinate of the ball
int ballTimer = 20;

int score = 3;
int hearts = 3;
bool isPowered=false;
int pCount = 0;

int heights[24]; // The height of barriers
int xPositions[24]; //the x position of barriers 
int power[3];
int powerPositions[3][2];

void main(int argc, char** argr) {
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(50, 50);

	initBHeights();
	initXPositions();
	randomPower();

	glutCreateWindow("Flappy Bird");
	
	glutDisplayFunc(Display);
	glutKeyboardFunc(Key);      // sets the Keyboard handler function; called when a key is pressed
	glutTimerFunc(0, Timer, 0);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(0.7, 0.8, 0, 1);
	gluOrtho2D(0, 800, 0, 800);

	glutMainLoop();
}




// Keyboard handler function
//   key: the key pressed on the keyboard
//   x  : the X coordinate of the mouse cursor at the moment of pressing the key
//   y  : the Y coordinate of the mouse cursor at the moment of pressing the key
void Key(unsigned char key, int x, int y) {
    
    
    if (key == ' ') {
        ballY += 50;
        ballTimer = 40;
    }else  {
		initBHeights();
		initXPositions();
		randomPower();
		hearts = 3;
		score = 0;
		status = 2;
		ballY = 400, ballX = 100;
		pCount = -100;
		glutTimerFunc(0, Timer, 0);
	}

    // ask OpenGL to recall the display function to reflect the changes on the window
    glutPostRedisplay();
}


void Timer(int value) {

	

		circleIntersect();

		ballY -= 3;
		for (int i = 0;i < 24;i++) xPositions[i] -= 2;
	/*	if (pCount > 0) {
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.6, 1, 0, 1);
		}*/
		if (xPositions[0] % 20 == 0) {
			score++;
			pCount--;
		}

		if (checkISWin()) {
			sndPlaySound(TEXT("win.wav"), SND_ASYNC);
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.6, 1, 0, 1);

			status = 1;
		}
		else if (checkBorders()) {
			status = 1;
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.8, 0.2, 0.1, 1);
		}
		else if (!checkIntersection()) {
			hearts--;
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.8, 0.2, 0.1, 1);
			if (hearts == 2) {
				glutTimerFunc(2000, Timer, 0);
				sndPlaySound(TEXT("1.wav"), SND_ASYNC);

			}
			else if (hearts == 1) {
				glutTimerFunc(2000, Timer, 0);
				sndPlaySound(TEXT("2.wav"), SND_ASYNC);
			}
			else if (hearts == 0) {
				sndPlaySound(TEXT("3.wav"), SND_ASYNC);
				status = 1;
			}
		}
		else {
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(1, 1, 1, 0);
			glutTimerFunc(20, Timer, 0);
		}
	
	glutPostRedisplay();

}
bool checkISWin() {
	return score > 160;
}
bool checkIntersection() {
    if (pCount>0) return true;
    
	for (int i = 0;i < 24;i += 2) {
        if (ballX > xPositions[i] && ballX < (xPositions[i] + 100)) {
            if ((ballY > 0 && ballY < heights[i]) || (800 > ballY && ballY > (800 + heights[i + 1]))) {
                for (int i = 0;i < 24;i++) xPositions[i] += 50;
                ballY = heights[i] + (800 + heights[i + 1] - heights[i]) / 2;
                return false;
            }
        }
	}
	return true;
}

bool checkBorders(){
	return ballY > 790 || ballY < 10;
}

bool circleIntersect(){
    for (int i = 0;i < 3;i++) {
        int x1 = ballX, y1 = ballY, r1 = 15;
        int x2 = powerPositions[i][0], y2 = powerPositions[i][1],r2=15;
		int distSq = (x1 - x2) * (x1 - x2) +
			(y1 - y2) * (y1 - y2);
		int radSumSq = (r1 + r2) * (r1 + r2);
		if (distSq < radSumSq){
            pCount = 45;
            power[i] = 0;
			sndPlaySound(TEXT("powerup.wav"), SND_ASYNC);
            return true;
        }
    }
    return false;
}

void randomPower(){
	power[0] = power[1] = power[2] = 0;
    for (int i = 0;i < 3;i++) power[i] = random(0, 1);
}

//void GameOverDisplay() {

//}

void Display() {
	if (status == 2) {
		glClear(GL_COLOR_BUFFER_BIT);
		
		for (int i = 0;i < 24;i += 2) {
			
			if (i % 8 == 0) {
				if (power[i / 8] == 1) {
					drawPower(powerPositions[i / 8][0] = xPositions[i] + 50, powerPositions[i / 8][1] = heights[i] + (800 + heights[i + 1] - heights[i]) / 2);
				}
				else {
					powerPositions[i / 8][0] = 0;
					powerPositions[i / 8][1] = 0;
				}
			}
			if (pCount > 0) {
				drawBarrier(xPositions[i], 0, 100, heights[i], 0.7 ,0.6 , 1);
				drawBarrier(xPositions[i + 1], 800, 100, heights[i + 1], 0.7 , 0.6 , 1);
			}
			else {
				drawBarrier(xPositions[i], 0, 100, heights[i], 0.5, 0.9, 0);
				drawBarrier(xPositions[i + 1], 800, 100, heights[i + 1], 0.5, 0.9, 0);
			}
			

		}


		glColor3f(0, 0, 1);             //Circle
		drawBird(ballX, ballY, 15);



		outText(0, 770, "Score:",0,0,0);
		outValue(70, 770, score,0,0,0);
		outText(500, 770, "Hearts:",0,0,0);
		outValue(580, 770, hearts,0,0,0);
		glFlush();
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i <= 20; i++) {
			outText(200 + i * 20, 600, "*",0,0,0);
		}

		if(checkISWin()){
			outText(330, 500, "Congratulations!", 0, 0, 0);
		}
		else {
			outText(350, 500, "Game_Over", 0, 0, 0);
		}
		for (int i = 5; i <= 15; i++) {
			outText(200 + i * 20, 470, "*", 0, 0, 0);
		}
		outText(350, 450, "Your_Score", 0, 0, 0);
		outValue(400, 410, score, 1, 1, 1);
		for (int i = 5; i <= 15; i++) {
			outText(200 + i * 20, 380, "*", 0, 0, 0);
		}
		outText(260, 350, "Press_Enter_Any_Key_Restart", 0, 0, 0);
		for (int i = 1;i <= 20;i++) {
			outText(200, 200 + i * 20, "*", 0, 0, 0);
			outText(600, 200 + i * 20, "*", 0, 0, 0);
		}

		for (int i = 0; i <= 20; i++) {
			outText(200 + i * 20, 200, "*", 0, 0, 0);
		}
		glFlush();
	}
}



void initBHeights() {
    for (int i = 0;i < 24;i+=2) {
        heights[i] = random(200, 500);
        heights[i + 1] = -(700-heights[i]);
    }
}

void initXPositions() {
    int diff = 250;
    xPositions[0] = xPositions[1] = 300;
    for (int i = 2;i < 24;i += 2) {
        xPositions[i] = xPositions[i + 1] = xPositions[i - 1] + diff;
    }
}

int random(int min, int max) //range : [min, max]
{
    static bool first = true;
    if (first)
    {
        srand(time(NULL)); //seeding for the first time only!
        first = false;
    }
    return min + rand() % ((max + 1) - min);
}



// draws a circle using OpenGL's gluDisk, given (x,y) of its center and tis radius
void drawCircle(int x, int y, float r) {
	    glPushMatrix();
		glColor3f(0, 0, 1);
	    glTranslatef(x, y, 0);
	    GLUquadric* quadObj = gluNewQuadric();
	    gluDisk(quadObj, 0, r, 50, 50);
	    glPopMatrix();
}

void drawBird(int x,int y,int r) {
	glPushMatrix();
	glPushMatrix();
	glColor3f(0.5, 0.9, 0);
	glTranslatef(x, y, 0);
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
	glColor3f(0.9, 0.5, 0.1);
	glBegin(GL_TRIANGLES);
	glVertex2f(x + r, y - 8);
	glVertex2f(x + (r + 8), y);
	glVertex2f(x + r, y + 8);
	glEnd();
	glColor3f(0, 0, 0);
	drawRect(x, y, 5, 5);
	glPopMatrix();
}

void drawPower(int x,int y) {
    glPushMatrix();

    drawCircle(x, y, 15);

    glColor3f(1, 1, 0);
	glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x-10,y-10);
    glVertex2f(x-5 , y+2 );
    glVertex2f(x + 5, y - 8);
    glVertex2f(x + 9, y + 8);
    glEnd();

    glPopMatrix();
}

// draws rectangles using the (x,y) of the bottom left vertex, width (w) and height (h)
void drawRect(int x, int y, int w, int h) {
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();
}

void drawBarrier(int x,int y,int w,int h,float R,float G,float B) {
    glPushMatrix();

    glColor3f(R, G, B);
	    glBegin(GL_POLYGON);
	    glVertex2f(x, y);
	    glVertex2f(x + w, y);
	    glVertex2f(x + w, y + h);
	    glVertex2f(x, y + h);
	    glEnd();

        glColor3f(0.9, 0.5, 0.1);
		glBegin(GL_TRIANGLES);
		glVertex2f(x , y+h);
		glVertex2f(x + w/2, (y+h)-h/2);
		glVertex2f(x + w, y +h);
		glEnd();

    glPopMatrix();
    
}
void outValue(int x, int y, int val, float R, float G, float B) {
	glPushMatrix();
	glColor3f(R, G, B);
	glRasterPos2f(x, y); //define position on the screen
	//glScalef(5.0, 5.0, 5.0);
	string  s = std::to_string(val);
	char* cstr = new char[s.length() + 1];
	std::strcpy(cstr, s.c_str());
	char* string = std::strtok(cstr, " ");

	while (*string) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *string++);
	}
	glPopMatrix();
}


void outText(int x, int y, string val,float R,float G,float B) {
	glPushMatrix();
	glColor3f(R, G, B);
	glRasterPos2f(x, y); //define position on the screen

	char* cstr = new char[val.length() + 1];
	std::strcpy(cstr, val.c_str());
	char* string = std::strtok(cstr, " ");

	while (*string) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *string++);
	}
	glPopMatrix();
}

