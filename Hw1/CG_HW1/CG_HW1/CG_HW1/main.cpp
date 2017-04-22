#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <freeglut/glut.h>
#include "textfile.h"
#include "glm.h"

#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "freeglut.lib")

#ifndef GLUT_WHEEL_UP
# define GLUT_WHEEL_UP   0x0003
# define GLUT_WHEEL_DOWN 0x0004
#endif

#ifndef GLUT_KEY_ESC
# define GLUT_KEY_ESC 0x001B
#endif

#ifndef max
# define max(a,b) (((a)>(b))?(a):(b))
# define min(a,b) (((a)<(b))?(a):(b))
#endif

// Shader attributes
GLint iLocPosition;
GLint iLocColor;

char filename[][50] = { "ColorModels/al7KC.obj", "ColorModels/blitzcrank_incognito.obj", 
						"ColorModels/texturedknot11KC.obj", "ColorModels/elephant16KC.obj",
						"ColorModels/frog2KC.obj",  "ColorModels/dragon10KC.obj",
						"ColorModels/igea17KC.obj",  "ColorModels/Dino20KC.obj",
						"ColorModels/lion12KC.obj",  "ColorModels/lucy25KC.obj",
						"ColorModels/happy10KC.obj",  "ColorModels/ziggs.obj" };
int file_size = 12;
int image_now = 0;
int judge = 0;
int color = 0;
GLint myLoc;

GLMmodel* OBJ;
GLfloat* vertices;
GLfloat* colors;

void traverseColorModel()
{
	int i;

	GLfloat maxVal[3];
	GLfloat minVal[3];

	// TODO:
	//// You should traverse the vertices and the colors of each triangle, and 
	//// then normalize the model to unit size. i.e. Each vertex should be 
	//// bounded in [-1, 1], which will fit the camera clipping window.

	// number of triangles
	OBJ->numtriangles;

	// number of vertices
	OBJ->numvertices;

	// The center position of the model 
	OBJ->position[0] = 0;
	OBJ->position[1] = 0;
	OBJ->position[2] = 0;

	vertices = new float[(OBJ->numtriangles) * 9];
	colors = new float[(OBJ->numtriangles) * 9];

	int now_v = 0;
	int now_c = 0;

	for(i=0; i<(int)OBJ->numtriangles; i++)
	{
		// the index of each vertex
		int indv1 = OBJ->triangles[i].vindices[0];
		int indv2 = OBJ->triangles[i].vindices[1];
		int indv3 = OBJ->triangles[i].vindices[2];

		// the index of each color
		int indc1 = indv1;
		int indc2 = indv2;
		int indc3 = indv3;

		// vertices
		GLfloat vx, vy, vz;
		vx = OBJ->vertices[indv1*3+0];
		vy = OBJ->vertices[indv1*3+1];
		vz = OBJ->vertices[indv1*3+2];

		if (i == 0) {
			maxVal[0] = vx;
			maxVal[1] = vy;
			maxVal[2] = vz;
			minVal[0] = vx;
			minVal[1] = vy;
			minVal[2] = vz;
		}
		else {
			maxVal[0] = max(maxVal[0], vx);
			maxVal[1] = max(maxVal[1], vy);
			maxVal[2] = max(maxVal[2], vz);
			minVal[0] = min(minVal[0], vx);
			minVal[1] = min(minVal[1], vy);
			minVal[2] = min(minVal[2], vz);
		}

		// ** add to float array
		vertices[now_v++] = vx;
		vertices[now_v++] = vy;
		vertices[now_v++] = vz;

		vx = OBJ->vertices[indv2*3+0];
		vy = OBJ->vertices[indv2*3+1];
		vz = OBJ->vertices[indv2*3+2];

		maxVal[0] = max(maxVal[0], vx);
		maxVal[1] = max(maxVal[1], vy);
		maxVal[2] = max(maxVal[2], vz);
		minVal[0] = min(minVal[0], vx);
		minVal[1] = min(minVal[1], vy);
		minVal[2] = min(minVal[2], vz);

		// ** add to float array
		vertices[now_v++] = vx;
		vertices[now_v++] = vy;
		vertices[now_v++] = vz;

		vx = OBJ->vertices[indv3*3+0];
		vy = OBJ->vertices[indv3*3+1];
		vz = OBJ->vertices[indv3*3+2];

		maxVal[0] = max(maxVal[0], vx);
		maxVal[1] = max(maxVal[1], vy);
		maxVal[2] = max(maxVal[2], vz);
		minVal[0] = min(minVal[0], vx);
		minVal[1] = min(minVal[1], vy);
		minVal[2] = min(minVal[2], vz);

		// ** add to float array
		vertices[now_v++] = vx;
		vertices[now_v++] = vy;
		vertices[now_v++] = vz;

		// colors
		GLfloat c1, c2, c3;
		c1 = OBJ->colors[indv1*3+0];
		c2 = OBJ->colors[indv1*3+1];
		c3 = OBJ->colors[indv1*3+2];

		// ** add to float array
		colors[now_c++] = c1;
		colors[now_c++] = c2;
		colors[now_c++] = c3;

		c1 = OBJ->colors[indv2*3+0];
		c2 = OBJ->colors[indv2*3+1];
		c3 = OBJ->colors[indv2*3+2];

		// ** add to float array
		colors[now_c++] = c1;
		colors[now_c++] = c2;
		colors[now_c++] = c3;

		c1 = OBJ->colors[indv3*3+0];
		c2 = OBJ->colors[indv3*3+1];
		c3 = OBJ->colors[indv3*3+2];

		// ** add to float array
		colors[now_c++] = c1;
		colors[now_c++] = c2;
		colors[now_c++] = c3;
	}


	float mid_x = (maxVal[0] + minVal[0]) / 2;
	float mid_y = (maxVal[1] + minVal[1]) / 2;
	float mid_z = (maxVal[2] + minVal[2]) / 2;
	printf("\nMidpoints: ");
	printf("[%f, %f, %f]\n", mid_x, mid_y, mid_z);

	for (i = 0; i < now_v; i ++)
	{
		vertices[i++] -= mid_x;
		vertices[i++] -= mid_y;
		vertices[i] -= mid_z;
	}

	float max_edge = max(max(maxVal[0] - minVal[0], maxVal[1] - minVal[1]), maxVal[2] - minVal[2]);
	float scale = 2 / max_edge;
	for (i = 0; i < now_v; i++)
	{
		vertices[i] *= scale;
	}

}

void loadOBJModel()
{
	// read an obj model here
	if(OBJ != NULL){
		free(OBJ);
	}
	OBJ = glmReadOBJ(filename[image_now]);
	printf("#### %s ####\n", filename[image_now]);

	// traverse the color model
	traverseColorModel();
}

void onIdle()
{
	glutPostRedisplay();
}

void onDisplay(void)
{
	// clear canvas
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(iLocPosition);
	glEnableVertexAttribArray(iLocColor);

	// organize the arrays
	/*static GLfloat triangle_color[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};*/

	/*static GLfloat triangle_vertex[] = {
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f
	};*/

	// bind array pointers to shader
	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(   iLocColor, 3, GL_FLOAT, GL_FALSE, 0, colors);

	// draw the array we just bound
	glDrawArrays(GL_TRIANGLES, 0, (OBJ->numtriangles)*3);

	glutSwapBuffers();
}

void showShaderCompileStatus(GLuint shader, GLint *shaderCompiled)
{
	glGetShaderiv(shader, GL_COMPILE_STATUS, shaderCompiled);
	if(GL_FALSE == (*shaderCompiled))
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character.
		GLchar *errorLog = (GLchar*) malloc(sizeof(GLchar) * maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		fprintf(stderr, "%s", errorLog);

		glDeleteShader(shader);
		free(errorLog);
	}
}

void setShaders()
{
	GLuint v, f, p;
	char *vs = NULL;
	char *fs = NULL;
	
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("shader.vert");
	fs = textFileRead("shader.frag");

	glShaderSource(v, 1, (const GLchar**)&vs, NULL);
	glShaderSource(f, 1, (const GLchar**)&fs, NULL);

	free(vs);
	free(fs);

	// compile vertex shader
	glCompileShader(v);
	GLint vShaderCompiled;
	showShaderCompileStatus(v, &vShaderCompiled);
	if(!vShaderCompiled) system("pause"), exit(123);

	// compile fragment shader
	glCompileShader(f);
	GLint fShaderCompiled;
	showShaderCompileStatus(f, &fShaderCompiled);
	if(!fShaderCompiled) system("pause"), exit(456);

	p = glCreateProgram();

	// bind shader
	glAttachShader(p, f);
	glAttachShader(p, v);

	// link program
	glLinkProgram(p);

	iLocPosition = glGetAttribLocation (p, "av4position");
	iLocColor    = glGetAttribLocation (p, "av3color");
	myLoc = glGetUniformLocation(p, "RGB");
	

	glUseProgram(p);
}

int callback = 1;
void onMouse(int who, int state, int x, int y)
{
	if (callback == 1) {
		printf("%18s(): (%d, %d) ", __FUNCTION__, x, y);

		switch (who)
		{
		case GLUT_LEFT_BUTTON:   printf("left button   "); break;
		case GLUT_MIDDLE_BUTTON: printf("middle button "); break;
		case GLUT_RIGHT_BUTTON:  printf("right button  "); break;
		case GLUT_WHEEL_UP:      printf("wheel up      "); break;
		case GLUT_WHEEL_DOWN:    printf("wheel down    "); break;
		default:                 printf("0x%02X          ", who); break;
		}

		switch (state)
		{
		case GLUT_DOWN: printf("start "); break;
		case GLUT_UP:   printf("end   "); break;
		}

		printf("\n");
	}
}

void onMouseMotion(int x, int y)
{
	if (callback == 1)	printf("%18s(): (%d, %d) mouse move\n", __FUNCTION__, x, y);
}

void onKeyboard(unsigned char key, int x, int y) 
{
	printf("%18s(): (%d, %d) key: %c(0x%02X) \n", __FUNCTION__, x, y, key, key);
	
	switch(key) 
	{
		case 'h': // ** Help menu
			printf("\n------------------ Help menu ------------------\n");
			printf("h: show help menu\n");
			printf("w: switch betwwen solid / wired rendering mode\n");
			printf("z: move to previous model\n");
			printf("x: move to next model\n");
			printf("m: open / close mouse callback messages\n");
			printf("s: show author's information\n");
			printf("------------------------------------------------\n");
			break;
		case 'w': // ** mode
			if(judge == 0)	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			judge++;
			if (judge > 1) judge = 0;
			break;
		case 'z': // ** previous
			if(image_now != 0)	image_now--;
			else image_now = file_size-1;
			loadOBJModel();
			break;
		case 'x': // ** next
			image_now++;
			if (image_now >= file_size) image_now = 0;
			loadOBJModel();
			break;
		case 'm': // ** mouse
			callback++;
			if (callback > 1) {
				callback = 0;
				printf("\nClose mouse callback!\n");
			}
			else {
				printf("\nOpen mouse callback!\n");
			}
			break;
		case 'c': // ** R, G, B channel 
			if (myLoc != -1)
			{
				color ++;
				if(color == 1)	glUniform1i(myLoc, 1);
				else if(color == 2)	glUniform1i(myLoc, 2);
				else if(color == 3)	glUniform1i(myLoc, 3);
				else {
					glUniform1i(myLoc, 0);
					color = 0;
				}
			}
			printf("%d", (int)myLoc);
			break;
		case 's': // ** author
			printf("\n************** Author Information **************\n");
			printf("                103062234  Arc Chang\n");
			printf("************************************************\n");
			break;
		case GLUT_KEY_ESC: // ** the Esc key
			exit(0); 
			break;
	}
	printf("\n");
}

void onKeyboardSpecial(int key, int x, int y){
	printf("%18s(): (%d, %d) ", __FUNCTION__, x, y);
	switch(key)
	{
		case GLUT_KEY_LEFT:
			printf("key: LEFT ARROW");
			break;
			
		case GLUT_KEY_RIGHT:
			printf("key: RIGHT ARROW");
			break;

		default:
			printf("key: 0x%02X      ", key);
			break;
	}
	printf("\n");
}


void onWindowReshape(int width, int height)
{
	printf("%18s(): %dx%d\n", __FUNCTION__, width, height);
}

int main(int argc, char **argv) 
{
	// glut init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// create window
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("10420 CS550000 CG HW1");

	glewInit();
	if(glewIsSupported("GL_VERSION_2_0")){
		printf("Ready for OpenGL 2.0\n");
	}else{
		printf("OpenGL 2.0 not supported\n");
		system("pause");
		exit(1);
	}

	// load obj models through glm
	loadOBJModel();

	// register glut callback functions
	glutDisplayFunc (onDisplay);
	glutIdleFunc    (onIdle);
	glutKeyboardFunc(onKeyboard);
	glutSpecialFunc (onKeyboardSpecial);
	glutMouseFunc   (onMouse);
	glutMotionFunc  (onMouseMotion);
	glutReshapeFunc (onWindowReshape);

	// set up shaders here
	setShaders();
	
	glEnable(GL_DEPTH_TEST);

	// main loop
	glutMainLoop();

	// free
	glmDelete(OBJ);

	return 0;
}

