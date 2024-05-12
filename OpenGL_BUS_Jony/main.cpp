#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle=0;
double angle_stop=0;

double posx = 200;
double posy = 200;
double posz = 40;

double lx = (-1/sqrt(2));
double ly = (-1/sqrt(2));
double lz = 0;

double rx = 1/sqrt(2);;
double ry = -1/sqrt(2);
double rz = 0;

double ux = 0;
double uy = 0;
double uz = 1;

double rr = 1;
double gr = 1;
double br = 1;

double rg = 1;
double gg = 1;
double bg = 1;

struct point
{
	double x,y,z;
};

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);

                glVertex3f(0, i*10, -90);
				glVertex3f(0, i*10, 90);



			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}
void drawCube(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,a);
		glVertex3f( a,-a,a);
		glVertex3f(-a,-a,a);
		glVertex3f(-a, a,a);

		glVertex3f(a,a,-a);
		glVertex3f(a,-a,-a);
		glVertex3f(-a,-a,-a);
		glVertex3f(-a,a,-a);

		glVertex3f(a,-a,a);
		glVertex3f(a,a,a);
		glVertex3f(a,a,-a);
		glVertex3f(a,-a,-a);


		glVertex3f(-a, -a,a);
		glVertex3f(-a,a,a);
		glVertex3f(-a,a,-a);
		glVertex3f(-a,-a,-a);


		glVertex3f( a,a,a);
		glVertex3f( a,a,-a);
		glVertex3f(-a,a,-a);
		glVertex3f(-a,a,a);

		glVertex3f(a,-a,a);
		glVertex3f(a,-a,-a);
		glVertex3f(-a,-a,-a);
		glVertex3f(-a,-a,a);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCylinder(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw quads using generated points
    for(i=0;i<segments;i++)
    {

        glBegin(GL_QUADS);
        {
			glVertex3f(points[i].x,points[i].y,height/2);
			glVertex3f(points[i+1].x,points[i+1].y,height/2);
			glVertex3f(points[i].x,points[i].y,-height/2);
			glVertex3f(points[i+1].x,points[i+1].y,-height/2);
        }
        glEnd();
    }
}

double tx=0;
double ty=0;
double c=0;
double d=0;

int i;

void drawSS()
{
   ///Main Land

    glPushMatrix();
    {
        glColor3f(0, 0.44, 0.086);
        glScalef(58,1,58);
        glTranslatef(0,-5,0);
        drawCube(2);
    }glPopMatrix();

    ///Rail Track
    glPushMatrix();
    {
        glColor3f(.8529, .720588, .720588);
        glTranslatef(0,4,-5);
        glScalef(111,.25,.25);
        drawCube(1);
    }glPopMatrix();

    glPushMatrix();
    {
        glColor3f(.8529, .720588, .720588);
        glTranslatef(0,4,5);
        glScalef(111,.25,.25);
        drawCube(1);
    }glPopMatrix();

    for(i=0; i<113; i+=6){
        glPushMatrix();
        {
            glColor3f(.8529, .720588, .720588);
            glTranslatef(i,4,0);
            glScalef(1, .25, 10);
            drawCube(1);
        }glPopMatrix();
    }

    for(i=-6; i>-113; i-=6){
        glPushMatrix();
        {
            glColor3f(.8529, .720588, .720588);
            glTranslatef(i,4,0);
            glScalef(1, .25, 10);
            drawCube(1);
        }glPopMatrix();
    }

    ///Main road
    glPushMatrix();
    {
        glColor3f(.5098, .5, .5);
        glTranslatef(0,2,0);
        glScalef(8,.25,116);
        drawCube(1);
    }glPopMatrix();

    ///Crossbar

    ///Stand

    glPushMatrix();
    {
        glColor3f(0, 0, 0);
        glTranslatef(10, 0 ,12);
        glScalef(.1,2,.1);
        drawCube(10);
    }glPopMatrix();

    ///Bar

    glPushMatrix();
    {
        glColor3f(0, .52, .78);
        glTranslatef(4,14,12);///Vertical
        glTranslatef(6,0,0);
        glRotatef(angle,0,0,1);
        glTranslatef(-6,0,0);
        glTranslatef(-6,-24,-12);
        glTranslatef(6, 24 ,12);///Horizontal
        glTranslatef(6,0,0);
        glRotatef(90,0,0,-1);
        glTranslatef(-6,0,0);
        glScalef(1.2, .1, .1);
        drawCube(10);
    }glPopMatrix();

    ///Traffic Light

    ///Traffic Light Stand

    glPushMatrix();
    {
        glColor3f(0.4, 0.239, 0.239);
        glTranslatef(-10, 0 ,12);
        glScalef(.1,3.8,.1);
        drawCube(10);
    }glPopMatrix();

    ///Light Stand

    glPushMatrix();
    {
        glColor3f(1, 1, 1);
        glTranslatef(0, 35, 12);
        glScalef(1, .1, .1);
        drawCube(10);
    }glPopMatrix();

    ///Red Lights

    glPushMatrix();
    {
        glColor3f(rr, gr, br);
        glTranslatef(6, 35, 13);
        glScalef(.1, .1, .1);
        drawCube(10);
    }glPopMatrix();

    ///Green Lights

    glPushMatrix();
    {
        glColor3f(rg, gg, bg);
        glTranslatef(0, 35, 13);
        glScalef(.1, .1, .1);
        drawCube(10);
    }glPopMatrix();



    ///Train

    ///First 3 wheel
    glPushMatrix();  ///Global Translation For Train
    {
       glTranslatef(97, 0 ,0);
        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(0, 5.25 ,5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(0, 5.25, -5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(2, 5.25 ,5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(2, 5.25, -5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(-2, 5.25 ,5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(-2, 5.25, -5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        ///First Compound
        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(0, 0, 1);
            glTranslatef(0, 11.25, 0);
            drawCube(5);
        }glPopMatrix();

        ///Second 3 wheel
        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(12, 5.25 ,5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(12, 5.25, -5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(14, 5.25 ,5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(14, 5.25, -5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(10, 5.25 ,5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(10, 5.25, -5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        ///Second Compound
        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(.89, .35, .117);
            glTranslatef(12, 11.25, 0);
            drawCube(5);
        }glPopMatrix();

        ///Third 3 wheel
        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(-12, 5.25 ,5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(-12, 5.25, -5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(-10, 5.25 ,5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(-10, 5.25, -5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(-14, 5.25 ,5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(1, 0, 0);
            glTranslatef(-14, 5.25, -5);
            drawCylinder(1, 1, 50);
        }glPopMatrix();

        ///Third Compound
        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(0, .87, 1);
            glTranslatef(-12, 11.25, 0);
            drawCube(5);
        }glPopMatrix();glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(0, 0, 1);
            glTranslatef(-12, 11.25, 0);
            drawCube(5);
        }glPopMatrix();

        ///Compound Joining
        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(0, 0, 0);
            glTranslatef(5.5, 6.25, 0);
            glScalef(1.5,.25,2);
            drawCube(1);
        }glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-tx, 0, 0);
            glColor3f(0, 0, 0);
            glTranslatef(-5.5, 6.25, 0);
            glScalef(1.5,.25,2);
            drawCube(1);
        }glPopMatrix();
    }glPopMatrix();

    ///Bus

    glPushMatrix();
    {
        glTranslatef(0, 0, 76); /// GLOBAL TRANSLATION FOR BUS

        ///Front Wheel 1
        glPushMatrix();
        {
            glTranslatef(0, 0, -ty);
            glColor3f(1, 0, 0);
            glTranslatef(2, 4.5, 40);
            glRotated(90, 0, 1, 0);
            drawCylinder(2, 2, 50);
        }glPopMatrix();

        ///Front Wheel 2
        glPushMatrix();
        {
            glTranslatef(0, 0, -ty);
            glColor3f(1, 0, 0);
            glTranslatef(-2, 4.5, 40);
            glRotated(90, 0, 1, 0);
            drawCylinder(2, 2, 50);
        }glPopMatrix();

        ///Back Wheel 1

        glPushMatrix();
        {
            glTranslatef(0, 0, -ty);
            glColor3f(1, 0, 0);
            glTranslatef(2, 4.5, 30);
            glRotated(90, 0, 1, 0);
            drawCylinder(2, 2, 50);
        }glPopMatrix();

        ///Back Wheel 2
        glPushMatrix();
        {
            glTranslatef(0, 0, -ty);
            glColor3f(1, 0, 0);
            glTranslatef(-2, 4.5, 30);
            glRotated(90, 0, 1, 0);
            drawCylinder(2, 2, 50);
        }glPopMatrix();

        ///Body
        glPushMatrix();
        {
            glTranslatef(0, 0, -ty);
            glColor3f(1, 1, 0);
            glTranslatef(0, 9, 35);
            glScalef(.35, .25, 1);
            drawCube(10);
        }glPopMatrix();
    }glPopMatrix();


}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case 'z':
			posx += lx;
			posy += ly;
			posz += lz;
			break;
        case 'x':
			posx -= lx;
			posy -= ly;
			posz -= lz;
			break;
        case 'c':
			posx += rx;
			posy += ry;
			posz += rz;
			break;
        case 'v':
			posx -= rx;
			posy -= ry;
			posz -= rz;
			break;
        case 'b':
			posx += ux;
			posy += uy;
			posz += uz;
			break;
        case 'n':
			posx -= ux;
			posy -= uy;
			posz -= uz;
			break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(100,100,-200,	0,0,0,	0,1,0);

	gluLookAt(posx,posy,posz,	posx+lx,posy+ly,posz+lz,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	//drawGrid();

    glColor3f(1,0,0);
    //drawSquare(10);
    //drawCube(20);

    drawSS();
    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){


    if(angle<90 && angle_stop == 0 && tx<110){
        angle += .5;
        rr = 1;
        gr = 0;
        br = 0;

        rg = 1;
        gg = 1;
        bg = 1;

    }

    if(angle<90 && angle_stop == 1){
        angle -= .5;
        if(angle<=0){
                angle_stop=0;
                angle += .5;

        }

        rg = 0;
        gg = 1;
        bg = 0;


        rr = 1;
        gr = 1;
        br = 1;

    }

    if(angle>=90 && tx>=100){

        angle_stop=1;
        angle -= .5;
        rg = 0;
        gg = 1;
        bg = 0;

        rr = 1;
        gr = 1;
        br = 1;

    }


    tx+=((2*pi*5*4)/360);
    if(tx>194)
        tx=0;

    if(tx<60 || tx>110){
            ty+=((2*pi*5*4*1.35)/360);
    }

    if(ty>194)
        ty=0;


	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
