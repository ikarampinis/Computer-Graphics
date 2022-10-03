#include <GL/glut.h>
#include <stdio.h>
#include<math.h>

int x_c, y_c , x_1,y_1, x_2, y_2;

void myInit() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 500, 0, 500);
}

void draw(int x, int y) {
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void draw_line(int xc, int yc, int x_1, int y_1, int x_2, int y_2){ 
    float dx, dy, d1, d2, x, y;
    float ry = sqrt((x_2-xc)*(x_2-xc)+(y_2-yc)*(y_2-yc));
    float rx = sqrt((x_1-xc)*(x_1-xc)+(y_1-yc)*(y_1-yc));
    d1 = (ry * ry)- (rx * rx * ry)+ (0.25 * rx * rx);
    x = 0;
    y = ry;

    // Initial decision parameter of region 1
    dx = 2 * ry * ry * x;
    dy = 2 * rx * rx * y;

    // For region 1
    while (dx < dy) {
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

void myDisplay() {
	draw_line(x_c, y_c , x_1, y_1, x_2, y_2);
	glFlush();
}

int main(int argc, char **argv) {

	printf( "Enter (x_c, y_c , a, b)\n");
	scanf("%d %d %d %d %d %d", &x_c, &y_c, &x_1, &y_1, &x_2, &y_2);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Bresenham's Line Drawing");
	myInit();
	glutDisplayFunc(myDisplay);
	glutMainLoop();
    return 0;
}
