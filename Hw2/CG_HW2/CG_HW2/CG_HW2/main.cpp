#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <freeglut/glut.h>
#include "textfile.h"
#include "glm.h"

#include "Matrices.h"

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
GLuint vao[2], vbo[2];
GLint iLocPosition;
GLint iLocColor;
GLint iLocMVP;

char filename[][50] = { "ColorModels/al7KC.obj", "ColorModels/blitzcrank_incognito.obj",
						"ColorModels/texturedknot11KC.obj", "ColorModels/elephant16KC.obj",
						"ColorModels/bunny5KC.obj",  "ColorModels/dragon10KC.obj",
						"ColorModels/igea17KC.obj",  "ColorModels/Dino20KC.obj",
						"ColorModels/lion12KC.obj",  "ColorModels/lucy25KC.obj",
						"ColorModels/happy10KC.obj",  "ColorModels/ziggs.obj" };
char modename[][50] = { "ObjectTranslae", "ObjectScale", "ObjectRotate",
						"EyeTranslate", "LookAtTranslate", "ProjectionMode" };
int file_size = 12;
int image_now = 0;
int callback = 0;
int judge = 0;
int color = 0;
GLint myLoc;

GLMmodel* OBJ;
GLfloat* vertices;
GLfloat* colors;
GLfloat* cube_vertices;
GLfloat* cube_colors;
float mid_x, mid_y, mid_z, scale;

int load = 0;

int mode = 0;
int mode_p = 0;

int mouse_mode = 0;
float t_x = 0.0f, t_y = 0.0f, t_z = 0.0f; //Translate
float s_x = 1.0f, s_y = 1.0f, s_z = 1.0f; //Scale
float r_x = 0.0f, r_y = 0.0f, r_z = 0.0f; //Rotate
float e_x = 0.0f, e_y = 0.0f, e_z = 0.0f; //View
float l_x = 0.0f, l_y = 0.0f, l_z = 0.0f; //Look at
float p_x = 0.0f, p_y = 0.0f; //Projection
float mouse_x, mouse_y;
int mm = 0;

float d = 0.0f;
float p_right = 1.0f, p_left = -1.0f, p_top = 1.0f, p_bottom = -1.0f, p_near = 1.0f, p_far = 5.0f;
float tx, ty, tz, orthox, orthoy, orthoz, A, B, C, D, perx, pery;

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

	if (load == 0) {
		cube_vertices = new float[(OBJ->numtriangles) * 9];
		cube_colors = new float[(OBJ->numtriangles) * 9];
	}

	int now_v = 0;
	int now_c = 0;

	if (load == 0) {
		for (i = 0; i < (int)OBJ->numtriangles; i++)
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
			vx = OBJ->vertices[indv1 * 3 + 0];
			vy = OBJ->vertices[indv1 * 3 + 1];
			vz = OBJ->vertices[indv1 * 3 + 2];

			cube_vertices[now_v++] = vx;
			cube_vertices[now_v++] = vy;
			cube_vertices[now_v++] = vz;

			vx = OBJ->vertices[indv2 * 3 + 0];
			vy = OBJ->vertices[indv2 * 3 + 1];
			vz = OBJ->vertices[indv2 * 3 + 2];

			cube_vertices[now_v++] = vx;
			cube_vertices[now_v++] = vy;
			cube_vertices[now_v++] = vz;

			vx = OBJ->vertices[indv3 * 3 + 0];
			vy = OBJ->vertices[indv3 * 3 + 1];
			vz = OBJ->vertices[indv3 * 3 + 2];

			cube_vertices[now_v++] = vx;
			cube_vertices[now_v++] = vy;
			cube_vertices[now_v++] = vz;

			// colors
			GLfloat c1, c2, c3;
			c1 = OBJ->colors[indv1 * 3 + 0];
			c2 = OBJ->colors[indv1 * 3 + 1];
			c3 = OBJ->colors[indv1 * 3 + 2];

			// ** add to float array
			cube_colors[now_c++] = c1;
			cube_colors[now_c++] = c2;
			cube_colors[now_c++] = c3;

			c1 = OBJ->colors[indv2 * 3 + 0];
			c2 = OBJ->colors[indv2 * 3 + 1];
			c3 = OBJ->colors[indv2 * 3 + 2];

			// ** add to float array
			cube_colors[now_c++] = c1;
			cube_colors[now_c++] = c2;
			cube_colors[now_c++] = c3;

			c1 = OBJ->colors[indv3 * 3 + 0];
			c2 = OBJ->colors[indv3 * 3 + 1];
			c3 = OBJ->colors[indv3 * 3 + 2];

			// ** add to float array
			cube_colors[now_c++] = c1;
			cube_colors[now_c++] = c2;
			cube_colors[now_c++] = c3;
		}
	}
	else if (load == 1) {
		for (i = 0; i<(int)OBJ->numtriangles; i++)
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
			vx = OBJ->vertices[indv1 * 3 + 0];
			vy = OBJ->vertices[indv1 * 3 + 1];
			vz = OBJ->vertices[indv1 * 3 + 2];

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

			vx = OBJ->vertices[indv2 * 3 + 0];
			vy = OBJ->vertices[indv2 * 3 + 1];
			vz = OBJ->vertices[indv2 * 3 + 2];

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

			vx = OBJ->vertices[indv3 * 3 + 0];
			vy = OBJ->vertices[indv3 * 3 + 1];
			vz = OBJ->vertices[indv3 * 3 + 2];

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
			c1 = OBJ->colors[indv1 * 3 + 0];
			c2 = OBJ->colors[indv1 * 3 + 1];
			c3 = OBJ->colors[indv1 * 3 + 2];

			// ** add to float array
			colors[now_c++] = c1;
			colors[now_c++] = c2;
			colors[now_c++] = c3;

			c1 = OBJ->colors[indv2 * 3 + 0];
			c2 = OBJ->colors[indv2 * 3 + 1];
			c3 = OBJ->colors[indv2 * 3 + 2];

			// ** add to float array
			colors[now_c++] = c1;
			colors[now_c++] = c2;
			colors[now_c++] = c3;

			c1 = OBJ->colors[indv3 * 3 + 0];
			c2 = OBJ->colors[indv3 * 3 + 1];
			c3 = OBJ->colors[indv3 * 3 + 2];

			// ** add to float array
			colors[now_c++] = c1;
			colors[now_c++] = c2;
			colors[now_c++] = c3;
		}

		mid_x = (maxVal[0] + minVal[0]) / 2;
		mid_y = (maxVal[1] + minVal[1]) / 2;
		mid_z = (maxVal[2] + minVal[2]) / 2;
		//printf("\nMidpoints: ");
		//printf("[%f, %f, %f]\n", mid_x, mid_y, mid_z);

		float max_edge = max(max(maxVal[0] - minVal[0], maxVal[1] - minVal[1]), maxVal[2] - minVal[2]);
		scale = 2 / max_edge;
	}
}

void loadOBJModel()
{
	// read an obj model here
	if (OBJ != NULL) {
		free(OBJ);
	}

	if (load == 0) {
		OBJ = glmReadOBJ("ColorModels/boxC.obj");
	}
	else if (load == 1) {
		OBJ = glmReadOBJ(filename[image_now]);
	}

	// traverse the color model
	traverseColorModel();
}

void onIdle()
{
	glutPostRedisplay();
}

void onDisplay(void)
{
	if (load == 0) {
		load = 1;
		loadOBJModel();
	}
	// clear canvas
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);

	glEnableVertexAttribArray(iLocPosition);
	glEnableVertexAttribArray(iLocColor);

	// TODO:
	//// Please define the model transformation matrix, viewing transformation matrix, 
	//// projection transformation matrix

	//MVP
	Matrix4 T = Matrix4(
						1.0f, 0.0f, 0.0f, t_x,
						0.0f, 1.0f, 0.0f, t_y,
						0.0f, 0.0f, 1.0f, t_z,
						0.0f, 0.0f, 0.0f, 1.0f);
	Matrix4 S = Matrix4(
						s_x, 0.0f, 0.0f, 0.0f,
						0.0f, s_y, 0.0f, 0.0f,
						0.0f, 0.0f, s_z, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);
	
	
	Matrix4 R_X = Matrix4(
						1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, cos(r_x), (-1)*sin(r_x), 0.0f,
						0.0f, sin(r_x), cos(r_x), 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f); 
	Matrix4 R_Y = Matrix4(
						cos(r_y), 0.0f, sin(r_y), 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						(-1)*sin(r_y), 0.0f, cos(r_y), 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);
	Matrix4 R_Z = Matrix4(
						cos(r_z), (-1)*sin(r_z), 0.0f, 0.0f,
						sin(r_z), cos(r_z), 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);
	Matrix4 R = R_X * R_Y * R_Z;
	Matrix4 N_S = Matrix4(
						scale, 0.0f, 0.0f, 0.0f,
						0.0f, scale, 0.0f, 0.0f,
						0.0f, 0.0f, scale, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);
	Matrix4 N_T = Matrix4(
						1.0f, 0.0f, 0.0f, (-1)*mid_x,
						0.0f, 1.0f, 0.0f, (-1)*mid_y,
						0.0f, 0.0f, 1.0f, (-1)*mid_z,
						0.0f, 0.0f, 0.0f, 1.0f);
	Matrix4 N = N_S * N_T;
	Matrix4 M = T * S * R * N;
	
	//VARIABLE
	tx = (-1) * (p_right + p_left) / (p_right - p_left);
	ty = (-1) * (p_top + p_bottom) / (p_top - p_bottom);
	tz = (-1) * (p_far + p_near) / (p_far - p_near);
	orthox = 2 / (p_right - p_left);
	orthoy = 2 / (p_top - p_bottom);
	orthoz = (-2) / (p_far - p_near);
	A = (p_right + p_left) / (p_right - p_left);
	B = (p_top + p_bottom) / (p_top - p_bottom);
	C = (-1) * (p_far + p_near) / (p_far - p_near);
	D = (-1) * 2 * p_far * p_near / (p_far - p_near);
	perx = 2 * p_near / (p_right - p_left);
	pery = 2 * p_near / (p_top - p_bottom);

	//VIEW
	Vector3 eye = Vector3(0.0f + e_x, 0.0f + e_y, 2.0f + e_z); //P1
	Vector3 center = Vector3(0.0f + l_x, 0.0f + l_y, -5.0f + l_z); //P2
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f); //P3
	
	Vector3 forward = center - eye;
	Vector3 right = forward.cross(up);
	Vector3 up_vector = right.cross(forward);
	up = eye + up_vector;
		
	Vector3 Rz = (center - eye) / (center - eye).length();
	Vector3 Rx = (center - eye).cross(up - eye) / ((center - eye).cross(up - eye)).length();
	Vector3 Ry = Rx.cross(Rz);

	Matrix4 V_R;
	V_R.setRow(0, Rx);
	V_R.setRow(1, Ry);
	V_R.setRow(2, -Rz);
	V_R.setRow(3, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	
	Matrix4 V_T = Matrix4(
						1.0f, 0.0f, 0.0f, (-1)*eye.x,
						0.0f, 1.0f, 0.0f, (-1)*eye.y,
						0.0f, 0.0f, 1.0f, (-1)*eye.z,
						0.0f, 0.0f, 0.0f, 1.0f);

	Matrix4 V = V_R * V_T;

	//PROJECTION
	Matrix4 P;
	if (mode_p == 0) { //ORTHO
		 P = Matrix4(
					orthox, 0.0f, 0.0f, tx,
					0.0f, orthoy, 0.0f, ty,
					0.0f, 0.0f, orthoz, tz,
					0.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (mode_p == 1) { //PERSPECTIVE
		P = Matrix4(
					perx, 0.0f, A, 0.0f,
					0.0f, pery, B, 0.0f,
					0.0f, 0.0f, C, D,
					0.0f, 0.0f, -1.0, 0.0f);
	}

	Matrix4 MVP = P*V*M;

	GLfloat mvp[16];
	// row-major ---> column-major
	mvp[0] = MVP[0];  mvp[4] = MVP[1];   mvp[8]  = MVP[2];    mvp[12] = MVP[3];  
	mvp[1] = MVP[4];  mvp[5] = MVP[5];   mvp[9]  = MVP[6];    mvp[13] = MVP[7];  
	mvp[2] = MVP[8];  mvp[6] = MVP[9];   mvp[10] = MVP[10];   mvp[14] = MVP[11];  
	mvp[3] = MVP[12]; mvp[7] = MVP[13];  mvp[11] = MVP[14];   mvp[15] = MVP[15];
	

	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(iLocColor, 3, GL_FLOAT, GL_FALSE, 0, colors);

	glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, mvp);
	glDrawArrays(GL_TRIANGLES, 0, (OBJ->numtriangles) * 3);
	
	// CUBE
	Matrix4 S2 = Matrix4(
						3.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.1f, 0.0f, 0.0f,
						0.0f, 0.0f, 3.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);
	Matrix4 T2 = Matrix4(
						1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, -1.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);
	
	Matrix4 M2 = T2 * S2;
	Matrix4 MVP2 = P * V * M2;
	GLfloat mvp2[16];
	mvp2[0] = MVP2[0];  mvp2[4] = MVP2[1];   mvp2[8] = MVP2[2];    mvp2[12] = MVP2[3];
	mvp2[1] = MVP2[4];  mvp2[5] = MVP2[5];   mvp2[9] = MVP2[6];    mvp2[13] = MVP2[7];
	mvp2[2] = MVP2[8];  mvp2[6] = MVP2[9];   mvp2[10] = MVP2[10];   mvp2[14] = MVP2[11];
	mvp2[3] = MVP2[12]; mvp2[7] = MVP2[13];  mvp2[11] = MVP2[14];   mvp2[15] = MVP2[15];
	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, cube_vertices);
	glVertexAttribPointer(iLocColor, 3, GL_FLOAT, GL_FALSE, 0, cube_colors);

	glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, mvp2);
	glDrawArrays(GL_TRIANGLES, 0, 12*3);

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
	iLocMVP		 = glGetUniformLocation(p, "mvp");
	myLoc		 = glGetUniformLocation(p, "RGB");

	glUseProgram(p);
}

void onMouse(int who, int state, int x, int y)
{
	if (callback == 1) {
		//printf("%18s(): (%d, %d) ", __FUNCTION__, x, y);
		switch (who)
		{
		case GLUT_LEFT_BUTTON:    break;
		case GLUT_MIDDLE_BUTTON:  break;
		case GLUT_RIGHT_BUTTON:   break;
		case GLUT_WHEEL_UP:      break;
		case GLUT_WHEEL_DOWN:    break;
		default:      break;
		}
		switch (state)
		{
		case GLUT_DOWN:  break;
		case GLUT_UP:    break;
		}
		printf("\n");
	}

	switch (who)
	{
	case GLUT_LEFT_BUTTON:   mm = 0;  mouse_mode = 1; mouse_x = (float)x; mouse_y = (float)y; break;
	case GLUT_MIDDLE_BUTTON:  break;
	case GLUT_RIGHT_BUTTON:  mm = 1; mouse_mode = 1; mouse_x = (float)x; mouse_y = (float)y; break;
	case GLUT_WHEEL_UP:
		if (mode == 0) t_z += 0.2f;
		else if (mode == 1) s_z += 0.02f;
		else if (mode == 2) r_z += 0.1f;
		else if (mode == 3) e_z += 0.2f;
		else if (mode == 4) l_z += 0.2f;
		break;
	case GLUT_WHEEL_DOWN:
		if (mode == 0) t_z -= 0.2f;
		else if (mode == 1) {
			 if(s_z > 0.05f) s_z -= 0.02f;
		}
		else if (mode == 2) r_z -= 0.1f;
		else if (mode == 3) e_z -= 0.2f;
		else if (mode == 4) l_z -= 0.2f;
		break;
	default: break;
	}

	switch (state)
	{
	case GLUT_DOWN:  break;
	case GLUT_UP:  mouse_mode = 0; break;
	}
}

void onMouseMotion(int x, int y)
{
	if (callback == 1)	printf("%18s(): (%d, %d) mouse move\n", __FUNCTION__, x, y);
	if (mouse_mode == 1) {
		if (mode == 0) { //Transalte
			t_x += ((float)x - mouse_x) / 500.0f;
			t_y += (mouse_y - (float)y) / 500.0f;
			//printf("## [t_x, t_y] = [%f, %f] ##\n", t_x, t_y);
		}
		else if (mode == 1) { //Scale
			s_x += ((float)x - mouse_x) / 500.0f;
			s_y += (mouse_y - (float)y) / 500.0f;
		}
		else if (mode == 2) { //Rotate
			r_x += (mouse_y - (float)y) / 100.0f;
			r_y += ((float)x - mouse_x) / 100.0f; 
		}
		else if (mode == 3) { //Eye
			e_x -= ((float)x - mouse_x) / 200.0f;
			e_y -= (mouse_y - (float)y) / 200.0f;
		}
		else if (mode == 4) { //Look at
			l_x += ((float)x - mouse_x) / 200.0f;
			l_y += (mouse_y - (float)y) / 200.0f;
		}
		else if(mode == 5) { //Projection
			p_x = ((float)x - mouse_x) / 200.0f;
			p_y = (mouse_y - (float)y) / 200.0f;
			if (mm == 0) {
				p_right += p_x;
				p_left -= p_x;
				p_top += p_y;
				p_bottom -= p_y;
			}
			else if (mm == 1) {
				p_near += p_x;
				p_far += p_y;
			}
		}
		mouse_x = (float)x;
		mouse_y = (float)y;
	}
}

void onKeyboard(unsigned char key, int x, int y)
{
	//printf("%18s(): (%d, %d) key: %c(0x%02X) \n", __FUNCTION__, x, y, key, key);

	switch (key)
	{
	case 'h': // ** Help menu
		printf("\n------------------ Help menu ------------------\n");
		printf("*** keyboard control ***\n");
		printf("h: show help menu\n");
		printf("w: switch between solid / wired rendering mode\n");
		printf("z: move to previous model\n");
		printf("x: move to next model\n");
		printf("v: show author's information\n");
		printf("c: switch color filter\n\n");
		printf("m: turn ON/OFF callback message\n");

		printf("f: switch rotation mode\n");
		printf("(rotate from current state / rotate from original angle by X-->Y-->Z\n");
		printf("# note that the rotation value will be set to 0 in all direction upon switching rotation mode\n\n");
		printf("o: switch between orthogonal / perspective projection\n");
		printf("i: show current model name and current control mode\n\n");

		printf("MODE SWITCHING\n");
		printf("t: go to OBJECT translate mode\n");
		printf("s: go to OBJECT scale mode\n");
		printf("r: go to OBJECT rotate mode\n");
		printf("e: go to EYE translate mode\n");
		printf("l: go to CENTER(look at) translate mode\n");
		printf("p: go to PROJECTION mode\n");
		printf("*** end of keyboard control ***\n\n");

		printf("*** mouse control ***\n");
		printf("VALUE CONTROL\n");
		printf("# note that minimum scaling is to 0.05x of the original size");
		printf("mouse drag LEFT: decrease the value on X axis\n");
		printf("mouse drag RIGHT: increase the value on X axis\n");
		printf("mouse drag DOWN: decrease the value on Y axis\n");
		printf("mouse drag UP: increase the value on Y axis\n");
		printf("mouse wheel DOWN: decrease the value on Z axis\n");
		printf("mouse wheel UP: increase the value on Z axis\n");
		printf("*** end of mouse control ***\n");
		printf("------------------------------------------------\n");
		break;
	case 'w': // ** mode
		if (judge == 0)	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		judge++;
		if (judge > 1) judge = 0;
		break;
	case 'z': // ** previous
		if (image_now != 0)	image_now--;
		else image_now = file_size - 1;
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
			color++;
			if (color == 1)	glUniform1i(myLoc, 1);
			else if (color == 2)	glUniform1i(myLoc, 2);
			else if (color == 3)	glUniform1i(myLoc, 3);
			else {
				glUniform1i(myLoc, 0);
				color = 0;
			}
		}
		printf("%d", (int)myLoc);
		break;
	case 'v': // ** author
		printf("\n************** Author Information **************\n");
		printf("                103062234  Arc Chang\n");
		printf("************************************************\n");
		break;

	case 't':
		mode = 0;
		printf("mode: object translate");
		break;
	case 's':
		mode = 1;
		printf("mode: object scaling");
		break;
	case 'r':
		mode = 2;
		printf("mode: object rotation");
		break;

	case 'e':
		mode = 3;
		printf("mode: eye translate");
		break;
	case 'l':
		mode = 4;
		printf("mode: \"look at\" translate");
		break;
	case 'p':
		mode = 5;
		printf("mode: projection mode");
		break;

	case 'o':
		mode_p++;
		d = 0.8f;
		if (mode_p > 1) {
			mode_p = 0;
			d = 0;
		}
		printf("switch projection mode");
		break;
	case 'i':
		printf("model name: %s\n", filename[image_now]);
		printf("actived action mode: %s\n", modename[mode]);
		break;
	case 'f':
		printf("switch rotation mode\n");
		r_x = 0.0f;
		r_y = 0.0f;
		r_z = 0.0f;
		break;
	case GLUT_KEY_ESC: // ** the Esc key
		exit(0);
		break;
	}
	printf("\n");
}

void onKeyboardSpecial(int key, int x, int y){
	//printf("%18s(): (%d, %d) ", __FUNCTION__, x, y);
	switch(key)
	{
		case GLUT_KEY_LEFT:
			//printf("key: LEFT ARROW");
			break;
			
		case GLUT_KEY_RIGHT:
			//printf("key: RIGHT ARROW");
			break;

		default:
			//printf("key: 0x%02X      ", key);
			break;
	}
	//printf("\n");
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
	glutCreateWindow("10420 CS550000 CG HW2");

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

