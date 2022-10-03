#include <GL/glut.h>
#include <stdio.h>
#include<stdlib.h>

int x_1, y_1, x_2, y_2;

void myInit() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 500, 0, 500);
}

//draw the pixel
void draw(int x, int y, double* color) {
	glBegin(GL_POINTS);
    glColor3f(color[0], color[1], color[2]);
	glVertex2i(x, y);
	glEnd();
}

//draw the line
void draw_line(int xL, int xH, int yL, int yH) {
	int dx, dy, k = 0, xval, yval, p, i, temp;
    double c0[3] = {1, 0, 0} , c1[3] = {0,0,1}, *t, color[3];
	double m, temp_color;
    //if the second has smaller x than the first point we swap them
    if(xH < xL){
        temp = xH;
        xH = xL;
        xL = temp;
        
        temp = yH;
        yH = yL;
        yL = temp;
        
        //we change also the color direction
        for(i=0;i<3;i++){
            temp_color = c0[i];
            c0[i] = c1[i];
            c1[i] = temp_color;
        }
    }
    
    //calculate horizontal distance between the two points
	dx=xH - xL;
    //calulate the vertical distance
	dy=yH - yL;
    //we start from the left point
	xval= xL;
	yval= yL;
    //calculate the line's slope
	m = (double)dy/(double)dx;
    
    //we have four (4) cases depended on m
	if(m < -1){
        t = (double*)malloc((abs(dy))*sizeof(double));
        //we separate the line into pixels
        //each pixel has different color (something between red and blue)
        for(i=0; i<=abs(dy);i++){
            t[i] = i *((double) (1/(double) (abs(dy))));
        }
        //calculate the color for the first pixel
        color[0] = c0[0] * (1-t[0])+c1[0]*t[0]; //red
        color[1] = c0[1] * (1-t[0])+c1[1]*t[0]; //green
        color[2] = c0[2] * (1-t[0])+c1[2]*t[0]; //blue
		p=2*dx+dy;
		draw(xval,yval, color);
		while ( yval > yH){
			if ( p < 0 ){
				p = p+2*dx;
                yval--;
                //color for new pixel
                color[0] = c0[0] * (1-t[yL-yval])+c1[0]*t[yL-yval]; //red
                color[1] = c0[1] * (1-t[yL-yval])+c1[1]*t[yL-yval]; //green
                color[2] = c0[2] * (1-t[yL-yval])+c1[2]*t[yL-yval]; //blue
				draw(xval ,yval, color);
			}
			else{
				p = p+2*dx+2*dy;
                xval++;
                yval--;
                //color for new pixel
                color[0] = c0[0] * (1-t[yL-yval])+c1[0]*t[yL-yval]; //red
                color[1] = c0[1] * (1-t[yL-yval])+c1[1]*t[yL-yval]; //green
                color[2] = c0[2] * (1-t[yL-yval])+c1[2]*t[yL-yval]; //blue
				draw(xval ,yval, color);
			}
			k++;
		}
	}else if(-1<=m && m<0){
        t = (double*)malloc((abs(dx))*sizeof(double));
        for(i=0; i<=dx;i++){
            t[i] = i *((double) (1/(double) (dx)));
        }
        color[0] = c0[0] * (1-t[0])+c1[0]*t[0]; //red
        color[1] = c0[1] * (1-t[0])+c1[1]*t[0]; //green
        color[2] = c0[2] * (1-t[0])+c1[2]*t[0]; //blue
		p=-2*dy-dx;
		draw(xval,yval, color);
		while ( xval < xH){
			if ( p < 0 ){
				p = p-2*dy;
                xval++;
                color[0] = c0[0] * (1-t[xval-xL])+c1[0]*t[xval-xL]; //red
                color[1] = c0[1] * (1-t[xval-xL])+c1[1]*t[xval-xL]; //green
                color[2] = c0[2] * (1-t[xval-xL])+c1[2]*t[xval-xL]; //blue
				draw(xval ,yval, color);
			}
			else{
				p = p-2*dy-2*dx;
                xval++;
                yval--;
                color[0] = c0[0] * (1-t[xval-xL])+c1[0]*t[xval-xL]; //red
                color[1] = c0[1] * (1-t[xval-xL])+c1[1]*t[xval-xL]; //green
                color[2] = c0[2] * (1-t[xval-xL])+c1[2]*t[xval-xL]; //blue
				draw(xval ,yval, color);
			}
			k++;
		}
	}else if(0<=m && m<=1){
        t = (double*)malloc((abs(dx))*sizeof(double));
        for(i=0; i<=dx;i++){
            t[i] = i *((double) (1/(double) (dx)));
        }
        color[0] = c0[0] * (1-t[0])+c1[0]*t[0]; //red
        color[1] = c0[1] * (1-t[0])+c1[1]*t[0]; //green
        color[2] = c0[2] * (1-t[0])+c1[2]*t[0]; //blue
		p=2*dy-dx;
		draw(xval, yval, color);
		while ( xval< xH){
			if ( p < 0 ){
				p = p+2*dy;
                xval++;
                color[0] = c0[0] * (1-t[xval-xL])+c1[0]*t[xval-xL]; //red
                color[1] = c0[1] * (1-t[xval-xL])+c1[1]*t[xval-xL]; //green
                color[2] = c0[2] * (1-t[xval-xL])+c1[2]*t[xval-xL]; //blue
				draw(xval, yval, color);
			}
			else{
				p = p+2*dy-2*dx;
                xval++;
                yval++;
                color[0] = c0[0] * (1-t[xval-xL])+c1[0]*t[xval-xL]; //red
                color[1] = c0[1] * (1-t[xval-xL])+c1[1]*t[xval-xL]; //green
                color[2] = c0[2] * (1-t[xval-xL])+c1[2]*t[xval-xL]; //blue
				draw(xval ,yval, color);
			}
			k++;
		}
	}else if(m>1){
        t = (double*)malloc((abs(dy))*sizeof(double));
		for(i=0; i<=abs(dy);i++){
            t[i] = i *((double) (1/(double) (abs(dy))));
        }
        color[0] = c0[0] * (1-t[0])+c1[0]*t[0]; //red
        color[1] = c0[1] * (1-t[0])+c1[1]*t[0]; //green
        color[2] = c0[2] * (1-t[0])+c1[2]*t[0]; //blue
		p=2*dx-dy;
		draw(xval,yval, color);
		while ( yval < yH){
			if ( p < 0 ){
				p = p+2*dx;
                yval++;
                color[0] = c0[0] * (1-t[yval- yL])+c1[0]*t[yval- yL]; //red
                color[1] = c0[1] * (1-t[yval- yL])+c1[1]*t[yval- yL]; //green
                color[2] = c0[2] * (1-t[yval- yL])+c1[2]*t[yval- yL]; //blue
				draw(xval ,yval, color);
			}
			else{
				p = p+2*dx-2*dy;
                xval++;
                yval++;
                color[0] = c0[0] * (1-t[yval- yL])+c1[0]*t[yval- yL]; //red
                color[1] = c0[1] * (1-t[yval- yL])+c1[1]*t[yval- yL]; //green
                color[2] = c0[2] * (1-t[yval- yL])+c1[2]*t[yval- yL]; //blue
				draw(xval ,yval, color);
			}
			k++;
		}
	}
	free(t);
}

void myDisplay() {
	draw_line(x_1, x_2, y_1, y_2);
	glFlush();
}

int main(int argc, char **argv) {
    //select the start-end of the line
	printf( "Enter (x_1, y_1, x_2, y_2)\n");
	scanf("%d %d %d %d", &x_1, &y_1, &x_2, &y_2);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Bresenham's Line Drawing");
	myInit();
	glutDisplayFunc(myDisplay);
	glutMainLoop();
    return (0);
}
