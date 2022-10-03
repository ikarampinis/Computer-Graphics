#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define DISABLE 0
#define ENABLE 1

typedef struct { 
  GLdouble x,y;
}GLdoublePoint;

struct node{
	GLdoublePoint point;
	struct node *next;
};

GLdouble screenWidth= 500;
GLdouble screenHeight =500;
GLint numOfVertexes = 0;
GLint polygon_state = DISABLE;
GLint clip_state = DISABLE;
GLint rectangle_state = DISABLE;
GLdouble rect_pos[4] = {0 ,0 ,0 ,0};
GLdoublePoint vertex[10];
struct node* clip_polygon = NULL;

void myInit (void)
{
	glClearColor(0,0,0,0);     
	glClear(GL_COLOR_BUFFER_BIT);		
	glColor3f(1.0, 1.0, 1.0);        
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 500, 0, 500);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glFlush();
}

void drawPoint( int ax, int ay)
{
    glColor3f(1,1,1);
	glBegin(GL_POINTS);
		glVertex3i(ax,ay, -1);
	glEnd();
    glColor3f(1,1,1);
	glFlush();
}

void draw_polygon(){
	int i;
    glColor3f(1,0,0);
	glBegin(GL_POLYGON);
        for(i=0; i<numOfVertexes; i++){
            glVertex2i(vertex[i].x, vertex[i].y);
        }
	glEnd();
    glColor3f(1,1,1);
	glFlush();
}

void draw_polygon_clipped(struct node **last){
	struct node* curr;
	GLdoublePoint firstPoint;
    glColor3f(1,0,0);
	glBegin(GL_POLYGON);
	curr = *last;
    curr = curr->next;
	firstPoint = curr->point;
	do{
		glVertex2i(curr->point.x, curr->point.y);
		curr = curr->next;
    }while(curr->point.x != firstPoint.x || curr->point.y != firstPoint.y);
	glVertex2i(firstPoint.x, firstPoint.y);
	glEnd();
    glColor3f(1,1,1);
	glFlush();
}

void draw_rect(){
	glColor4f(0, 0, 1, 0.5);
    glBegin(GL_QUADS);
	    glVertex2i(rect_pos[0], rect_pos[2]);
	    glVertex2i(rect_pos[0], rect_pos[3]);
	    glVertex2i(rect_pos[1], rect_pos[3]);
	    glVertex2i(rect_pos[1], rect_pos[2]);
    glEnd();
    glColor3f(1,1,1);
    glFlush();
}

void addlist(struct node **last, GLdoublePoint point){
	struct node* newNode;
    newNode = (struct node*)malloc(sizeof(struct node));
    struct node* curr;

    if(*last == NULL){
    	newNode->point = point;
    	newNode->next = newNode;
    	*last = newNode;
    }
    else{
    	curr = *last;
    	newNode->point = point;
    	newNode->next = curr->next;
    	curr->next = newNode;
    	*last = newNode;
    }
}
void removelist(struct node **last){
	struct node* temp;
	struct node* curr;

    if (*last != NULL){
    	curr = *last;
        temp = curr->next;
        curr->next = temp->next;
        if(temp->next == temp){
        	*last = NULL;
        }
        free(temp);
    }
}

void Sutherland_Hodgeman_clip(){
	int i, side=0;
	GLdoublePoint newPoints[2];
	GLdoublePoint firstPoint;
	int counterNewPoints = 0;
	struct node* curr = NULL;
    struct node* array[5] = {NULL, NULL, NULL, NULL, NULL};
    
	for(i=0; i<numOfVertexes; i++){
		addlist(&array[0], vertex[i]);
    }    

	for(side=0; side<4; side++){
        curr = array[side]->next;
        firstPoint = curr->point;
        
		do{
            //left
			if(side==0){
				//out-in
				if(curr->point.x < rect_pos[side] && curr->next->point.x > rect_pos[side]){
					newPoints[0].x = rect_pos[side];
					newPoints[0].y = curr->point.y + (curr->next->point.y-curr->point.y)*(rect_pos[side]-curr->point.x)/(curr->next->point.x-curr->point.x);
					newPoints[1].x = curr->next->point.x;
					newPoints[1].y = curr->next->point.y;
					counterNewPoints = 2;
				}
				//in-out
				if(curr->point.x > rect_pos[side] && curr->next->point.x < rect_pos[side]){
					newPoints[0].x = rect_pos[side];
					newPoints[0].y = curr->point.y + (curr->next->point.y-curr->point.y)*(rect_pos[side]-curr->point.x)/(curr->next->point.x-curr->point.x);
					counterNewPoints = 1;
				}
				//in-in
				if(curr->point.x > rect_pos[side] && curr->next->point.x > rect_pos[side]){
					newPoints[0].x = curr->next->point.x;
					newPoints[0].y = curr->next->point.y;
					counterNewPoints = 1;
				}
				//out-out
				if(curr->point.x < rect_pos[side] && curr->next->point.x < rect_pos[side]){
					counterNewPoints = 0;
				}
			}
			//right
			else if(side==1){
				//out-in
				if(curr->point.x > rect_pos[side] && curr->next->point.x < rect_pos[side]){
					newPoints[0].x = rect_pos[side];
					newPoints[0].y = curr->point.y + (curr->next->point.y-curr->point.y)*(rect_pos[side]-curr->point.x)/(curr->next->point.x-curr->point.x);
					newPoints[1].x = curr->next->point.x;
					newPoints[1].y = curr->next->point.y;
					counterNewPoints = 2;
				}
				//in-out
				if(curr->point.x < rect_pos[side] && curr->next->point.x > rect_pos[side]){
					newPoints[0].x = rect_pos[side];
					newPoints[0].y = curr->point.y + (curr->next->point.y-curr->point.y)*(rect_pos[side]-curr->point.x)/(curr->next->point.x-curr->point.x);
					counterNewPoints = 1;
				}
				//in-in
				if(curr->point.x < rect_pos[side] && curr->next->point.x < rect_pos[side]){
					newPoints[0].x = curr->next->point.x;
					newPoints[0].y = curr->next->point.y;
					counterNewPoints = 1;
				}
				//out-out
				if(curr->point.x > rect_pos[side] && curr->next->point.x > rect_pos[side]){
					counterNewPoints = 0;
				}
			}
			//bottom
			else if(side==2){
				//out-in
				if(curr->point.y < rect_pos[side] && curr->next->point.y > rect_pos[side]){
					newPoints[0].y = rect_pos[side];
					newPoints[0].x = curr->point.x + (curr->next->point.x-curr->point.x)*(rect_pos[side]-curr->point.y)/(curr->next->point.y-curr->point.y);
					newPoints[1].x = curr->next->point.x;
					newPoints[1].y = curr->next->point.y;
					counterNewPoints = 2;
				}
				//in-out
				if(curr->point.y > rect_pos[side] && curr->next->point.y < rect_pos[side]){
					newPoints[0].y = rect_pos[side];
					newPoints[0].x = curr->point.x + (curr->next->point.x-curr->point.x)*(rect_pos[side]-curr->point.y)/(curr->next->point.y-curr->point.y);
					counterNewPoints = 1;
				}
				//in-in
				if(curr->point.y > rect_pos[side] && curr->next->point.y > rect_pos[side]){
					newPoints[0].x = curr->next->point.x;
					newPoints[0].y = curr->next->point.y;
					counterNewPoints = 1;
				}
				//out-out
				if(curr->point.y < rect_pos[side] && curr->next->point.y < rect_pos[side]){
					counterNewPoints = 0;
				}
			}
			//top
			else if(side==3){
				//out-in
				if(curr->point.y > rect_pos[side] && curr->next->point.y < rect_pos[side]){
					newPoints[0].y = rect_pos[side];
					newPoints[0].x = curr->point.x + (curr->next->point.x-curr->point.x)*(rect_pos[side]-curr->point.y)/(curr->next->point.y-curr->point.y);
					newPoints[1].x = curr->next->point.x;
					newPoints[1].y = curr->next->point.y;
					counterNewPoints = 2;
				}
				//in-out
				if(curr->point.y < rect_pos[side] && curr->next->point.y > rect_pos[side]){
					newPoints[0].y = rect_pos[side];
					newPoints[0].x = curr->point.x + (curr->next->point.x-curr->point.x)*(rect_pos[side]-curr->point.y)/(curr->next->point.y-curr->point.y);
					counterNewPoints = 1;
				}
				//in-in
				if(curr->point.y < rect_pos[side] && curr->next->point.y < rect_pos[side]){
					newPoints[0].x = curr->next->point.x;
					newPoints[0].y = curr->next->point.y;
					counterNewPoints = 1;
				}
				//out-out
				if(curr->point.y > rect_pos[side] && curr->next->point.y > rect_pos[side]){
					counterNewPoints = 0;
				}
			}
			for(i=0; i<counterNewPoints; i++){
				addlist(&array[side+1], newPoints[i]);
			}
            curr = curr->next;
        }while(curr->point.x != firstPoint.x || curr->point.y != firstPoint.y);
        
        while(array[side] != NULL){
            removelist(&array[side]);
        }
	}
	clip_polygon = array[side];
	glutPostRedisplay();
}



void myMouse (int button, int state, int x1, int y1){
    int i;
    
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && polygon_state == DISABLE && rectangle_state == DISABLE && clip_state == DISABLE){
        if(numOfVertexes <10){ 
            vertex[numOfVertexes].x = x1;
            vertex[numOfVertexes].y = screenHeight-y1;
            numOfVertexes++;
            glutPostRedisplay();
        }
        else{
            printf("YOU CAN'T PUT MORE THAN 10 POINTS\n");
        }
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && polygon_state == ENABLE && rectangle_state == ENABLE && numOfVertexes > 2){
    	rect_pos[0] = x1;
    	rect_pos[1] = x1;
    	rect_pos[2] = screenHeight-y1;
    	rect_pos[3] = screenHeight-y1;
    	glutPostRedisplay();
    }

    if (button == GLUT_LEFT_BUTTON && state ==GLUT_UP && polygon_state == ENABLE && rectangle_state == ENABLE && numOfVertexes > 2){
    	rect_pos[1] = x1;
    	rect_pos[3] = screenHeight-y1;
    	glutPostRedisplay();
    }

    if (button == GLUT_RIGHT_BUTTON && state ==GLUT_DOWN ){
    	glClear(GL_COLOR_BUFFER_BIT);
        polygon_state = DISABLE;
        rectangle_state = DISABLE;
        clip_state = DISABLE;
    	numOfVertexes = 0;
        for(i=0; i<4; i++){
            rect_pos[i] = 0;
        }
    	glFlush();
    }
}

void Motion(GLint x, GLint y) {
    if(polygon_state == ENABLE && rectangle_state == ENABLE && numOfVertexes > 2){
        rect_pos[1] = x;
        rect_pos[3] = screenHeight-y;
        glutPostRedisplay ( );
    }
}

void keyEvent(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'C':
		case 'c':
        clip_state = ENABLE;
        polygon_state = DISABLE;
		Sutherland_Hodgeman_clip();
		break;
		case 'f':
        case 'F':
		polygon_state = ENABLE;
        rectangle_state = ENABLE;
        glutPostRedisplay();
		break;
		case 'k':
        case 'K':
        if(rectangle_state == ENABLE){
            rectangle_state = DISABLE;
        }
        else{
            rectangle_state = ENABLE;
        }
        glutPostRedisplay();
		break; 
		default:
		break;
	}
}

void myDisplay(){
    int i;
    glClear(GL_COLOR_BUFFER_BIT);
    if(numOfVertexes>0 && polygon_state == DISABLE && rectangle_state == DISABLE && clip_state == DISABLE){
        for(i=0; i<numOfVertexes; i++){
            drawPoint(vertex[i].x,vertex[i].y);
        }
    }
    if(polygon_state == ENABLE && clip_state == DISABLE){
        draw_polygon();
    }
    if(clip_state == ENABLE){
        draw_polygon_clipped(&clip_polygon);
    }
    if(rectangle_state == ENABLE && rect_pos[0]!=0 && rect_pos[1]!=0 && rect_pos[2]!=0 && rect_pos[3]!=0){
        draw_rect();
    }
    glFlush();
    glutSwapBuffers();
}


int main(int argc, char **argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (screenWidth, screenHeight);
	glutInitWindowPosition(0,0);
	glutCreateWindow ("Sutherland Hodgeman clipping");
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
    glutMotionFunc(Motion);
	glutKeyboardFunc(keyEvent);
	myInit();
	glutMainLoop();

	return 0;
}
