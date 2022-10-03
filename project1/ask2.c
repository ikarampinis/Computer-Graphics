#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

GLint screenWidth= 500;
GLint screenHeight =500;

int menu; // for menu creation
int selection; // for menu choices
int sxhma; // for drawing line or rectangle
int numCorners = 0;
int x_c, y_c , x_1,y_1, x_2, y_2;

 typedef struct 
 { 
	 GLint x,y;
 }
 GLintPoint;

void myInit() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 500, 0, 500);
}

//draw pixel of Ellipse
void draw( int ax, int ay) {
	glBegin(GL_POINTS);
    glVertex2i(ax,ay);
	glEnd();
	glFlush();
}

//draw pixel for line
void draw_line_points( int ax, int ay, double e) {
    if(e>1){
        e=1;
    }
    if(e<0){
        e=0;
    }
	glBegin(GL_POINTS);
    //grayscale for antialising
    glColor3f(e, e, e);
    glVertex2i(ax,ay);
    //make the pixel color white again
    glColor3f(1,1,1);
	glEnd();
	glFlush();
}

void draw_line(int xL, int xH, int yL, int yH) {
	int dx, dy, k = 0, xval, yval, p, temp;
	double m , e;
    //if the second has smaller x than the first point we swap them
    if(xH < xL){
        temp = xH;
        xH = xL;
        xL = temp;
        
        temp = yH;
        yH = yL;
        yL = temp;
    }
	dx=xH - xL;
	dy=yH - yL;
	xval= xL;
	yval= yL;
    //slope of the line
	m = (double)dy/(double)dx;
	//we have four (4) cases depended on m
	if(m < -1){
		p=2*dx+dy;
        //init the e for antialising
        e = 0.5;
		draw_line_points(xval,yval, 1);
		while ( yval > yH){
			
			if ( p < 0 ){
				p = p+2*dx;
                yval--;
                e = e - (double)1/(double)m;
                printf("%lf - 1\n", e);
                //draw the pixel based on bresenham
				draw_line_points(xval ,yval, 1);
                //draw the other pixel for antialising
                draw_line_points(xval+1, yval,e);
			}
			else{
				p = p+2*dx+2*dy;
                xval++;
                yval--;
                e = e - 1 - (double)1/(double)m;
                printf("%lf - 1\n", e);
				draw_line_points(xval ,yval, 1);
                draw_line_points(xval-1, yval,e);
			}
			k++;
		}
	}else if(-1<=m && m<0){
		p=-2*dy-dx;
        e = 0.5;
		draw_line_points(xval,yval, 1);
		while ( xval < xH){
			
			if ( p < 0 ){
				p = p-2*dy;
                xval++;
                e = e - m;
                printf("%lf - 2\n", e);
				draw_line_points(xval ,yval, 1);
                draw_line_points(xval, yval-1,e);
			}
			else{
				p = p-2*dy-2*dx;
                xval++;
                yval--;
                e = e - 1 - m;
                printf("%lf - 2\n", e);
				draw_line_points(xval ,yval, 1);
                draw_line_points(xval, yval+1,e);
			}
			k++;
		}
	}else if(0<=m && m<=1){
		p=2*dy-dx;
        e = 0.5;
		draw_line_points(xval, yval, 1);
		while ( xval< xH){
			if (p<0 ){
				p = p+2*dy;
                xval++;
                e = e + m;
                printf("%lf\n", e);
				draw_line_points(xval, yval, 1);
                draw_line_points(xval, yval+1,e);
			}
			else{
				p = p+2*dy-2*dx;
                xval++;
                yval++;
                e = e -1 + m;
                printf("%lf\n", e);
                draw_line_points(xval, yval-1,e);
				draw_line_points(xval ,yval, 1);
			}
			k++;
		}
	}else if(m>1){
		p=2*dx-dy;
        e = 0.5;
		draw_line_points(xval,yval,1);
		while ( yval < yH){
			
			if ( p < 0 ){
				p = p+2*dx;
                yval++;
                e = e + (double)1/(double)m;
                printf("%lf\n", e);
                draw_line_points(xval ,yval, 1);
                draw_line_points(xval+1, yval,e);
			}
			else{
				p = p+2*dx-2*dy;
                xval++;
                yval++;
                e = e -1 + (double)1/(double)m;
                printf("%lf\n", e);
				draw_line_points(xval ,yval, 1);
                draw_line_points(xval-1, yval,e);
			}
			k++;
		}
	}
	
}

void draw_ellipse(int xc, int yc, int x_1, int y_1, int x_2, int y_2){ 
    float dx, dy, d1, d2, x, y;
    //calculate the distance between the center and the top point
    float ry = sqrt((x_2-xc)*(x_2-xc)+(y_2-yc)*(y_2-yc));
    //calculate the distance between the center and the right point
    float rx = sqrt((x_1-xc)*(x_1-xc)+(y_1-yc)*(y_1-yc));
    d1 = (ry * ry)- (rx * rx * ry)+ (0.25 * rx * rx);
    x = 0;
    y = ry;

    // Initial decision parameter of region 1
    dx = 2 * ry * ry * x;
    dy = 2 * rx * rx * y;

    // For region 1
    while (dx < dy) {
        //draw the pixe and its 3 symetric pixels
        draw( x + xc, y + yc);
        draw( -x + xc, y + yc);
        draw( x + xc, -y + yc);
        draw( -x + xc, -y + yc);
        
        if (d1 < 0) {
            x++;
            dx = dx + (2 * ry * ry);
            d1 = d1 + dx + (ry * ry);
        }
        else {
            x++;
            y--;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d1 = d1 + dx - dy + (ry * ry);
        }
    }
    
    d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5)))+ ((rx * rx) * ((y - 1) * (y - 1)))- (rx * rx * ry * ry);

    // Plotting points of region 2
    while (y >= 0) {
        //draw the pixel and its 3 symetric pixels
        draw( x + xc, y + yc);
        draw( -x + xc, y + yc);
        draw( x + xc, -y + yc);
        draw( -x + xc, -y + yc);
        
        if (d2 > 0) {
            y--;
            dy = dy - (2 * rx * rx);
            d2 = d2 + (rx * rx) - dy;
        }
        else {
            y--;
            x++;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d2 = d2 + dx - dy + (rx * rx);
        }
    }
}

void myMouse (int button, int state, int x1, int y1)
{
	static GLintPoint corner[3];
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{		
		corner[numCorners].x =x1;
		corner[numCorners].y =screenHeight - y1;
		numCorners++;
		if (numCorners == 2)
		{
			if (sxhma == 1)
			{
				draw_line(corner[0].x, corner[1].x, corner[0].y, corner[1].y);
                //we set the numCorners to zero so the user can draw another line
                numCorners =0;
			}
			
        }
        if (numCorners == 3) {
            
			if(sxhma == 2)
			{
				draw_ellipse(corner[0].x, corner[0].y, corner[1].x, corner[1].y, corner[2].x, corner[2].y);
                numCorners =0;
			}
        }
    }
	glFlush();
}

void myMenu(int selection)
{
	if(selection==1) // drawing  line
	{
		sxhma=1;
		numCorners = 0;
		glutPostRedisplay();
	}
	if(selection==2) // drawing Ellipse
	{	
		sxhma=2;
		numCorners = 0;
		glutPostRedisplay();
	}
	if(selection==3) // clear screen
	{
		glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
	}
	if(selection==0) // quit
	{
		exit(-1);	
	}
}

void myDisplay(void)
{
	glClearColor(0, 0, 0, 0);
	glFlush();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (screenWidth, screenHeight);
	glutInitWindowPosition(0,0);
	glutCreateWindow ("Dhmiourgia Menu");
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	menu = glutCreateMenu(myMenu);
	glutAddMenuEntry("Line",1);
	glutAddMenuEntry("Ellipse",2);
    glutAddMenuEntry("Clear",3);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	myInit();
	glutMainLoop();
	glutDestroyMenu(menu);
	return 0;
}
