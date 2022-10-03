#ifdef _APPLE_CC_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#define GREEN_COLOR 3
#define BLUE_COLOR 4
#define RED_COLOR 2

GLint screenWidth= 500;
GLint screenHeight =500;

int menu; // for menu creation
int selected; // for menu choices
int part; // for drawing line or rectangle
int numCorners = 0;
int level = 0;

 typedef struct 
 { 
	 GLint x,y;
 }
 GLintPoint;
 
 struct Node
 { 
	 GLint ymax, xmax, ymin;
     double slope, xmin;
     struct Node *next;
 };
 
 typedef struct Node SideInfo;

 //draw pixel
void drawPoint( int ax, int ay)
{
	glBegin(GL_POINTS);
		glVertex2i(ax,ay);
	glEnd();
	glFlush();
}

//draw corners of polygon and store info for the edges
SideInfo* drawPolygon(GLintPoint corner[], SideInfo lines[])
{
    int i, size = numCorners;
	glBegin(GL_LINES);
        for(i=0; i<size-1; i++){
            glVertex2i(corner[i].x,corner[i].y);
            glVertex2i(corner[i+1].x,corner[i+1].y);
            //find the characteristics of every edge and store them
            if(corner[i].y >= corner[i+1].y){
                lines[i].ymax = corner[i].y;
                lines[i].xmax = corner[i].x;
                lines[i].xmin = corner[i+1].x;
                lines[i].ymin = corner[i+1].y;
            }
            else{
                lines[i].ymax = corner[i+1].y;
                lines[i].xmax = corner[i+1].x;
                lines[i].xmin = corner[i].x;
                lines[i].ymin = corner[i].y;
            }
            //calculate the slope of the edge
            if(corner[i].x < corner[i+1].x){
                lines[i].slope = (double)(corner[i+1].y-corner[i].y) / (double)(corner[i+1].x - corner[i].x);
            }
            else if(corner[i].x > corner[i+1].x){
                lines[i].slope = (double)(corner[i].y-corner[i+1].y) / (double)(corner[i].x - corner[i+1].x);
            }
            else{
                lines[i].slope = 0;
            }
            lines[i].next = NULL;
        }
        //do all the steps again for the last edge of the polygon
        glVertex2i(corner[i].x,corner[i].y);
        glVertex2i(corner[0].x,corner[0].y);
        if(corner[i].y >= corner[0].y){
            lines[i].ymax = corner[i].y;
            lines[i].xmax = corner[i].x;
            lines[i].xmin = corner[0].x;
            lines[i].ymin = corner[0].y;
        }
        else{
            lines[i].ymax = corner[0].y;
            lines[i].xmax = corner[0].x;
            lines[i].xmin = corner[i].x;
            lines[i].ymin = corner[i].y;
        }
        if(corner[i].x < corner[0].x){
            lines[i].slope = (double)(corner[0].y-corner[i].y) / (double)(corner[0].x - corner[i].x);
        }
        else if(corner[i].x > corner[0].x){
            lines[i].slope = (double)(corner[i].y-corner[0].y) / (double)(corner[i].x - corner[0].x);
        }
        else{
            lines[i].slope = 0;
        }
        lines[i].next = NULL;
	glEnd();
	glFlush();
    return lines;
}

//add an edge to the active edge list
void add_to_list(SideInfo **head, SideInfo *new_node){
    SideInfo* node = (SideInfo*)malloc(sizeof(SideInfo));
    node->ymax = new_node->ymax;
    node->xmax = new_node->xmax;
    node->xmin = new_node->xmin;
    node->slope = new_node->slope;
    node->ymin = new_node->ymin;
    node->next = NULL;
    
    //insert at the begging if the list is empty
    if(*head == NULL){
        *head = node;
    }else{
        //new head
        SideInfo* curr = *head;
        if(curr->xmin > node->xmin){
            node->next = curr;
            *head = node;
            return;
        }
        //new head
        if((curr->xmin == node->xmin) && (curr->xmax > node->xmax)){
            node->next = curr;
            *head = node;
            return;
        }
        //insert inside the list
        while(curr -> next != NULL){
            if(curr->next->xmin > node->xmin){
                break;
            }
            if((curr->next->xmin == node->xmin) && (curr->next->xmax > node->xmax)){
                break;
            }
            else{
                curr = curr->next;
            }
        }
        node->next = curr->next;
        curr->next = node;
    }
}

//remove an edge from the active edge list
void remove_from_list(SideInfo **head, int scan_row){
    SideInfo* temp;
    int delete_counter = 1;
    SideInfo* curr = NULL;
    
    //repeat until you delete all the edges with ymax == scan_row
    while(delete_counter != 0){
        delete_counter = 0;
        //if the list is empty end the loop
        if(*head == NULL){
            break;
        }
        //if the head must be deleted
        if((*head)->ymax == scan_row){
            temp = *head;
            *head = (*head)->next;
            free(temp);
            delete_counter = 1;
        }else{
            //search inside the list to delete an edge
            curr = *head;
            while(curr->next != NULL){
                if(curr->next->ymax == scan_row){
                    temp = curr->next;  
                    curr->next = curr->next->next;
                    free(temp);
                    delete_counter = 1;
                    break;
                }
                else{
                    curr = curr->next;
                }
            }
        }
    }
}

//fill the polygon with the selected color
void Fill_Polygon(SideInfo *lines){
    int i, k,j, size = numCorners, min_y, counter;
    SideInfo* y_bucket[501];
    SideInfo table[size];;
    SideInfo temp;
    SideInfo* prev;
    SideInfo* current = NULL;
    SideInfo* node = NULL;
    SideInfo* head = NULL;
    
    //initialize y_bucket
    for(i=0; i<501; i++){
        y_bucket[i] = NULL;
    }
    
    //repeat to choose all the edges
    for(j=0; j<size;j++){
        min_y = screenHeight+1;
        counter = 0;
        //choose the edges with the smaller ymin
        for(i=0; i<size; i++){
            //equals with smaller so we keep it
            if(lines[i].ymin == min_y){
                if(lines[i].ymin == lines[i].ymax){
                    continue;
                }
                counter++;
                table[counter] = lines[i];
            }
            //new smaller
            if(lines[i].ymin < min_y){
                if(lines[i].ymin == lines[i].ymax){
                    continue;
                }
                counter = 0;
                min_y = lines[i].ymin;
                table[counter] = lines[i];
            }
        }
        //change the min_y of the edges we found
        //so we cannot select them again
        for(i=0; i<size; i++){
            if(lines[i].ymin == min_y){
                lines[i].ymin = screenHeight+2;
            }
        }
        
        //sort the edges based on xmin first and xmax second (if it is necessary)
        for(k=0; k<counter;k++){
            for(i=0; i<counter;i++){
                if(table[i].xmin > table[i+1].xmin){
                    temp = table[i+1];
                    table[i+1] = table[i];
                    table[i] = temp;
                }
                if((table[i].xmin == table[i+1].xmin)&&(table[i].xmax > table[i+1].xmax)){
                    temp = table[i+1];
                    table[i+1] = table[i];
                    table[i] = temp;
                }
            }
        }
        
        //add the new edges in the y_bucket at ymin position
        if(min_y != screenHeight+1){
            y_bucket[min_y] = (SideInfo*)malloc(sizeof(SideInfo));
            y_bucket[min_y]->ymax = table[0].ymax;
            y_bucket[min_y]->xmax = table[0].xmax;
            y_bucket[min_y]->ymin = table[0].ymin;
            y_bucket[min_y]->slope = table[0].slope;
            y_bucket[min_y]->xmin = table[0].xmin;
            current = y_bucket[min_y];
            //if we have more than one new edges
            for(i=1; i<counter+1; i++){
                node = (SideInfo*)malloc(sizeof(SideInfo));
                current->next = node;
                node->ymax = table[i].ymax;
                node->xmax = table[i].xmax;
                node->ymin = table[i].ymin;
                node->slope = table[i].slope;
                node->xmin = table[i].xmin;
                current = node;
            }
            current->next = NULL;
        }
    }
    
    //check the y_bucket
    for(i=0;i<screenHeight+1; i++){
        if(y_bucket[i] != NULL){            
            current = y_bucket[i];
            printf("[%d] --> [%d] [%lf] [%lf]", i, current->ymax, current->xmin, current->slope);            
            while(current->next != NULL){
                current = current->next;
                printf(" --> [%d] [%lf] [%lf]", current->ymax, current->xmin, current->slope);
            }
            printf("\n");
        }
    }
    
    //add edges to the active edge list
    for(i=0; i< screenHeight+1; i++){
        if(y_bucket[i]!=NULL){
            current = y_bucket[i];
            add_to_list(&head, current);
            //if we add more than one
            while(current->next != NULL){
                current = current->next;
                add_to_list(&head, current);
            }
        }
        
        //check if any edge from the list has reached at the ymax and remove it
        if(head != NULL){
            remove_from_list(&head, i);
        }
        
        //draw points inside the polygon
        if(head != NULL){
            for(current = head; current!=NULL; current = current->next->next){
                //+1 at the begging to avoid painting the polygon
                for(k = (int)(current->xmin)+1; k < (int)(current->next->xmin); k++){
                    drawPoint(k,i);
                }
            }
        }
        
        //recalculate the xmin at every active edge
        if(head != NULL){
            for(current = head; current!=NULL; current = current->next){
                if(current->slope == 0 && (int)(current->xmin) == current->xmax){
                    //do nothing in xmin
                }else{
                    current->xmin = current->xmin + (double)1/(double)current->slope;
                }
            }
        }
    }
    
    //free y_bucket
    for(i=0; i< screenHeight+1; i++){
        if(y_bucket[i]!=NULL){
            current = y_bucket[i];
            if(current->next != NULL){
                while(current->next != NULL){
                    prev = current;
                    current = current->next;
                    free(prev);
                }
            }
            else{
                free(y_bucket[i]);
            }
        }
    }
    printf("\n-----------------------------------------------------\n");
}

void myInit (void)
{
	glClearColor(0,0,0,0);     
	glClear(GL_COLOR_BUFFER_BIT);		
	glColor3f(1.0, 1.0, 1.0);        
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 500, 0, 500);
	glFlush();
}
										

void myDisplay(void)
{
	glClearColor(0,0,0,0);   
	glFlush();						
}

void myMouse (int button, int state, int x1, int y1)
{
	static GLintPoint corner[10];
    
	if (button == GLUT_LEFT_BUTTON && state ==GLUT_DOWN )
	{
        //draw the corners part
        if(part == 1){
            if(numCorners <10){ 
                corner[numCorners].x =x1;
                corner[numCorners].y = screenHeight-y1;
                drawPoint(corner[numCorners].x,corner[numCorners].y);
                numCorners++;
            }
            else{
                printf("YOU CAN'T PUT MORE THAN 10 POINTS\n");
            }
        }
        //draw polygon part
        if (numCorners >= 3)
        {
            if (part == 2)
            {
                glColor3f(1.0, 1.0, 1.0); 
                SideInfo lines[numCorners];
                drawPolygon(corner, lines);
                if(selected == RED_COLOR){
                    glColor3f(1, 0, 0); 
                }
                else if(selected == GREEN_COLOR){
                    glColor3f(0, 1, 0); 
                }
                else if(selected == BLUE_COLOR){
                    glColor3f(0, 0, 1);
                }
                Fill_Polygon(lines);
                glColor3f(1.0, 1.0, 1.0);
                numCorners = 0;
                level++;
                part = 0;
            }
        }
        glFlush();
    }
}

// Menu choices
void myMenu(int selection)
{
	if(selection==1)
	{
        part = 1;
		glutPostRedisplay();
	}
	if(selection==RED_COLOR || selection==GREEN_COLOR || selection==BLUE_COLOR)
	{	
        part=2;
		glutPostRedisplay();
        selected = selection;
        myMouse(GLUT_LEFT_BUTTON,GLUT_DOWN,0,0);
        selected = 0;
	}
	if(selection==5) // clear screen
	{
        numCorners =0;
		glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
	}
	if(selection==0) // quit
	{
		exit(-1);	
	}
}



int main(int argc, char **argv)
{

	int menu, ColorSubmenu;
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (screenWidth, screenHeight);
	glutInitWindowPosition(0,0);
	glutCreateWindow ("Pop-up menu");
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	ColorSubmenu = glutCreateMenu(myMenu);
	glutAddMenuEntry("Red", RED_COLOR);
	glutAddMenuEntry("Green", GREEN_COLOR);
	glutAddMenuEntry("Blue", BLUE_COLOR);
	menu = glutCreateMenu(myMenu);
	glutAddMenuEntry("Choose Points", 1);
	glutAddSubMenu("Draw and Color", ColorSubmenu);
	glutAddMenuEntry("Clear", 5);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	myInit();
	glutMainLoop();
	glutDestroyMenu(ColorSubmenu);
	glutDestroyMenu(menu);

	return 0;
}
