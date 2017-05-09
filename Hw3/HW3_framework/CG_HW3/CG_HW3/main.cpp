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

int perspective = 1;
float p_right = 1.0f, p_left = -1.0f, p_top = 1.0f, p_bottom = -1.0f, p_near = 1.0f, p_far = 10.0f;
float tx, ty, tz, orthox, orthoy, orthoz, A, B, C, D, perx, pery;

// Shader attributes
GLint iLocPosition;
GLint iLocNormal;
GLint iLocMVP, iLocMN, iLocR;

GLint iLocMDiffuse, iLocMAmbient, iLocMSpecular, iLocMShininess;

GLint iLocLDAmbient, iLocLDPosition, iLocLDDiffuse, iLocLDSpecular;
GLint iLocLPAmbient, iLocLPPosition, iLocLPDiffuse, iLocLPSpecular, iLocLPca, iLocLPla, iLocLPqa;
GLint iLocLSAmbient, iLocLSPosition, iLocLSDiffuse, iLocLSSpecular, iLocLSca, iLocLSla, iLocLSqa, iLocLSDir, iLocLSExp, iLocLSCut, iLocLSCuta;
GLint iLocLIAmbient, iLocLIPosition;

GLint iLocEye;

GLint iLocModeOne, iLocModeTwo, iLocModeThree, iLocModeA, iLocModeD, iLocModeS, iLocModeC;
int mode_one = 0;
int mode_two = 0;
int mode_three = 0;
int mode_a = 1; // ON
int mode_d = 1; // ON
int mode_s = 1; // ON
int mode_c = 0;
int mode_f = 0;

char filename[][50] = { "NormalModels/High/dragon10KN.obj", "NormalModels/Medium/cow3KN.obj",
						"NormalModels/Medium/laurana2KN.obj" };
int file_size = 3;
int image_now = 0;

GLMmodel* OBJ;
GLfloat* vertices;
GLfloat* normals;

Matrix4 N;

int dir = 0;
int rotate = 0;
float r_y = 0.0f;


struct LightSourceParameters {
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float position[4];
	float halfVector[4];
	float spotDirection[4];
	float spotExponent;
	float spotCutoff; // (range: [0.0,90.0], 180.0)
	float spotCosCutoff; // (range: [1.0,0.0],-1.0)
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
}typedef LightSource;
LightSource lightsource[4];

void traverseColorModel()
{
	int i;

	// number of triangles
	vertices = (GLfloat*)malloc(sizeof(GLfloat)*OBJ->numtriangles*9);
	normals = (GLfloat*)malloc(sizeof(GLfloat)*OBJ->numtriangles*9);

	float max_x = OBJ->vertices[3];
	float max_y = OBJ->vertices[4];
	float max_z = OBJ->vertices[5];
	float min_x = OBJ->vertices[3];
	float min_y = OBJ->vertices[4];
	float min_z = OBJ->vertices[5];

	for(i=0; i<(int)OBJ->numtriangles; i++)
	{
		// the index of each vertex
		int indv1 = OBJ->triangles[i].vindices[0];
		int indv2 = OBJ->triangles[i].vindices[1];
		int indv3 = OBJ->triangles[i].vindices[2];

		// the index of each normal
		int indc1 = OBJ->triangles[i].nindices[0];
		int indc2 = OBJ->triangles[i].nindices[1];
		int indc3 = OBJ->triangles[i].nindices[2];

		// vertices
		
		vertices[i*9 + 0] = OBJ->vertices[indv1*3+0];
		vertices[i*9 + 1] = OBJ->vertices[indv1*3+1];
		vertices[i*9 + 2] = OBJ->vertices[indv1*3+2];
		if(vertices[i*9 + 0] > max_x) max_x = vertices[i*9 + 0];
		if(vertices[i*9 + 1] > max_y) max_y = vertices[i*9 + 1]; 
		if(vertices[i*9 + 2] > max_z) max_z = vertices[i*9 + 2];
		if(vertices[i*9 + 0] < min_x) min_x = vertices[i*9 + 0];
		if(vertices[i*9 + 1] < min_y) min_y = vertices[i*9 + 1];
		if(vertices[i*9 + 2] < min_z) min_z = vertices[i*9 + 2];

		vertices[i*9 + 3] = OBJ->vertices[indv2*3+0];
		vertices[i*9 + 4] = OBJ->vertices[indv2*3+1];
		vertices[i*9 + 5] = OBJ->vertices[indv2*3+2];
		if(vertices[i*9 + 3] > max_x) max_x = vertices[i*9 + 3];
		if(vertices[i*9 + 4] > max_y) max_y = vertices[i*9 + 4];
		if(vertices[i*9 + 5] > max_z) max_z = vertices[i*9 + 5];
		if(vertices[i*9 + 3] < min_x) min_x = vertices[i*9 + 3];
		if(vertices[i*9 + 4] < min_y) min_y = vertices[i*9 + 4];
		if(vertices[i*9 + 5] < min_z) min_z = vertices[i*9 + 5];
	
		vertices[i*9 + 6] = OBJ->vertices[indv3*3+0];
		vertices[i*9 + 7] = OBJ->vertices[indv3*3+1];
		vertices[i*9 + 8] = OBJ->vertices[indv3*3+2];
		if(vertices[i*9 + 6] > max_x) max_x = vertices[i*9 + 6];
		if(vertices[i*9 + 7] > max_y) max_y = vertices[i*9 + 7];
		if(vertices[i*9 + 8] > max_z) max_z = vertices[i*9 + 8];
		if(vertices[i*9 + 6] < min_x) min_x = vertices[i*9 + 6];
		if(vertices[i*9 + 7] < min_y) min_y = vertices[i*9 + 7];
		if(vertices[i*9 + 8] < min_z) min_z = vertices[i*9 + 8];

		// normal

		normals[i*9 + 0] = OBJ->normals[indc1*3+0];
		normals[i*9 + 1] = OBJ->normals[indc1*3+1];
		normals[i*9 + 2] = OBJ->normals[indc1*3+2];

		normals[i*9 + 3] = OBJ->normals[indc2*3+0];
		normals[i*9 + 4] = OBJ->normals[indc2*3+1];
		normals[i*9 + 5] = OBJ->normals[indc2*3+2];

		normals[i*9 + 6] = OBJ->normals[indc3*3+0];
		normals[i*9 + 7] = OBJ->normals[indc3*3+1];
		normals[i*9 + 8] = OBJ->normals[indc3*3+2];
	}
	float normalize_scale = max(max(abs(max_x-min_x),abs(max_y-min_y)),abs(max_z-min_z));

	Matrix4 S,T;
	S.identity();
	T.identity();
	S[0] = 2/normalize_scale;
	S[5] = 2/normalize_scale;;
	S[10] = 2/normalize_scale;
	T[3] = -(min_x+max_x)/2;
	T[7] = -(min_y+max_y)/2;
	T[11] = -(min_z+max_z)/2;

	N = S*T;
}

void loadOBJModel()
{
	// read an obj model here
	if(OBJ != NULL){
		free(OBJ);
	}
	OBJ = glmReadOBJ(filename[image_now]);
	printf("%s\n", filename[image_now]);

	glmFacetNormals(OBJ);
	glmVertexNormals(OBJ, 90.0);

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
	glEnableVertexAttribArray(iLocNormal);


	//MVP
	Matrix4 T;
	Matrix4 S;
	Matrix4 R;
	Matrix4 RI;

	Matrix4 M;
	if (rotate == 0) {
		R = Matrix4(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);
	}
	else {
		r_y += 0.0015f;
		R = Matrix4(
				cos(r_y),		0.0f,	sin(r_y),	0.0f,
				0.0f,			1.0f,	0.0f,		0.0f,
				(-1)*sin(r_y),	0.0f,	cos(r_y),	0.0f,
				0.0f,			0.0f,	0.0f,		1.0f);
		RI = R;
	}
	M = T * S * R;
	
	//VIEW
	Vector3 eye = Vector3(0.0f, 0.0f, 2.0f); //P1
	float e[3];
	e[0] = eye.x;
	e[1] = eye.y;
	e[2] = eye.z;
	glUniform3fv(iLocEye, 1, e);
		
	Vector3 center = Vector3(0.0f, 0.0f, -5.0f); //P2
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
	if (perspective == 0) { //ORTHO
		P = Matrix4(
				orthox, 0.0f, 0.0f, tx,
				0.0f, orthoy, 0.0f, ty,
				0.0f, 0.0f, orthoz, tz,
				0.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (perspective == 1) { //PERSPECTIVE
		P = Matrix4(
				perx, 0.0f, A, 0.0f,
				0.0f, pery, B, 0.0f,
				0.0f, 0.0f, C, D,
				0.0f, 0.0f, -1.0, 0.0f);
	}

	Matrix4 MVP = P*V*M*N;

	GLfloat mvp[16];
	// row-major ---> column-major
	mvp[0] = MVP[0];  mvp[4] = MVP[1];   mvp[8]  = MVP[2];    mvp[12] = MVP[3];  
	mvp[1] = MVP[4];  mvp[5] = MVP[5];   mvp[9]  = MVP[6];    mvp[13] = MVP[7];  
	mvp[2] = MVP[8];  mvp[6] = MVP[9];   mvp[10] = MVP[10];   mvp[14] = MVP[11];  
	mvp[3] = MVP[12]; mvp[7] = MVP[13];  mvp[11] = MVP[14];   mvp[15] = MVP[15];

	Matrix4 MN = M*N;
	GLfloat mn[16];
	// row-major ---> column-major
	mn[0] = MN[0];  mn[4] = MN[1];   mn[8] = MN[2];    mn[12] = MN[3];
	mn[1] = MN[4];  mn[5] = MN[5];   mn[9] = MN[6];    mn[13] = MN[7];
	mn[2] = MN[8];  mn[6] = MN[9];   mn[10] = MN[10];  mn[14] = MN[11];
	mn[3] = MN[12]; mn[7] = MN[13];  mn[11] = MN[14];  mn[15] = MN[15];
	glUniformMatrix4fv(iLocMN, 1, GL_FALSE, mn);

	
	GLfloat r[16];
	// row-major ---> column-major
	r[0] = RI[0];   r[4] = RI[1];    r[8] = RI[2];     r[12] = RI[3];
	r[1] = RI[4];   r[5] = RI[5];    r[9] = RI[6];     r[13] = RI[7];
	r[2] = RI[8];   r[6] = RI[9];    r[10] = RI[10];   r[14] = RI[11];
	r[3] = RI[12];  r[7] = RI[13];   r[11] = RI[14];   r[15] = RI[15];
	glUniformMatrix4fv(iLocR, 1, GL_FALSE, r);

	// Mode control
	glUniform1i(iLocModeOne, mode_one);
	glUniform1i(iLocModeTwo, mode_two);
	glUniform1i(iLocModeThree, mode_three);
	glUniform1i(iLocModeA, mode_a);
	glUniform1i(iLocModeD, mode_d);
	glUniform1i(iLocModeS, mode_s);
	glUniform1i(iLocModeC, mode_c);

	//Get material data here
	glUniform4fv(iLocMAmbient, 1, OBJ->materials[1].ambient);
	glUniform4fv(iLocMDiffuse, 1, OBJ->materials[1].diffuse);
	glUniform4fv(iLocMSpecular, 1, OBJ->materials[1].specular);

	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(iLocNormal, 3, GL_FLOAT, GL_FALSE, 0, normals);

	glUniformMatrix4fv(iLocMVP, 1, GL_FALSE, mvp);
	glDrawArrays(GL_TRIANGLES, 0, (OBJ->numtriangles) * 3);

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

/**** VALUE ****/
void setLightingSource(){
	float PLRange = 100;
	// Initial
	lightsource[3].position[0] = 0;
	lightsource[3].position[1] = 0;
	lightsource[3].position[2] = 0;
	lightsource[3].position[3] = 1;
	lightsource[3].ambient[0] = 0.7;
	lightsource[3].ambient[1] = 0.7;
	lightsource[3].ambient[2] = 0.7;
	lightsource[3].ambient[3] = 1;

	// Directional Light
	lightsource[0].position[0] = 0;
	lightsource[0].position[1] = 1;
	lightsource[0].position[2] = 0;
	lightsource[0].position[3] = 1;
	lightsource[0].ambient[0] = 0.7;
	lightsource[0].ambient[1] = 0.7;
	lightsource[0].ambient[2] = 0.7;
	lightsource[0].ambient[3] = 1;
	lightsource[0].diffuse[0] = 1;
	lightsource[0].diffuse[1] = 1;
	lightsource[0].diffuse[2] = 1;
	lightsource[0].diffuse[3] = 1;
	lightsource[0].specular[0] = 1;
	lightsource[0].specular[1] = 1;
	lightsource[0].specular[2] = 1;
	lightsource[0].specular[3] = 1;
	lightsource[0].constantAttenuation = 1;
	lightsource[0].linearAttenuation = 4.5 / PLRange;
	lightsource[0].quadraticAttenuation = 75 / (PLRange*PLRange);

	// Point Light
	lightsource[1].position[0] = 0;
	lightsource[1].position[1] = -1;
	lightsource[1].position[2] = 0;
	lightsource[1].position[3] = 1;
	lightsource[1].ambient[0] = 0.7;
	lightsource[1].ambient[1] = 0.7;
	lightsource[1].ambient[2] = 0.7;
	lightsource[1].ambient[3] = 1;
	lightsource[1].diffuse[0] = 1;
	lightsource[1].diffuse[1] = 1;
	lightsource[1].diffuse[2] = 1;
	lightsource[1].diffuse[3] = 1;
	lightsource[1].specular[0] = 1;
	lightsource[1].specular[1] = 1;
	lightsource[1].specular[2] = 1;
	lightsource[1].specular[3] = 1;
	lightsource[1].constantAttenuation = 1;
	lightsource[1].linearAttenuation = 4.5 / PLRange;
	lightsource[1].quadraticAttenuation = 75 / (PLRange*PLRange);

	// Spot Light
	PLRange = 100;
	lightsource[2].position[0] = 0;
	lightsource[2].position[1] = 0;
	lightsource[2].position[2] = 1;
	lightsource[2].position[3] = 1;
	lightsource[2].ambient[0] = 0.7;
	lightsource[2].ambient[1] = 0.7;
	lightsource[2].ambient[2] = 0.7;
	lightsource[2].ambient[3] = 1;
	lightsource[2].diffuse[0] = 1;
	lightsource[2].diffuse[1] = 1;
	lightsource[2].diffuse[2] = 1;
	lightsource[2].diffuse[3] = 1;
	lightsource[2].specular[0] = 1;
	lightsource[2].specular[1] = 1;
	lightsource[2].specular[2] = 1;
	lightsource[2].specular[3] = 1;
	lightsource[2].spotDirection[0] = 0;
	lightsource[2].spotDirection[1] = 0;
	lightsource[2].spotDirection[2] = -2;
	lightsource[2].spotDirection[3] = 1;
	lightsource[2].constantAttenuation = 1;
	lightsource[2].linearAttenuation = 4.5 / PLRange;
	lightsource[2].quadraticAttenuation = 75 / (PLRange*PLRange);
	lightsource[2].spotExponent = 0.1;
	lightsource[2].spotCutoff = 45;
	lightsource[2].spotCosCutoff = 0.96592582628; // 1/12 pi
} 

void setShaders()
{
	GLuint v, f, p;
	char *vs = NULL;
	char *fs = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	if (mode_f == 1) {
		vs = textFileRead("shader.vert");
		fs = textFileRead("shader.frag");
	}
	else {
		vs = textFileRead("sample.vert");
		fs = textFileRead("sample.frag");
	}
	

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
	iLocNormal = glGetAttribLocation(p, "av3normal");
	iLocMVP		 = glGetUniformLocation(p, "mvp");
	iLocMN = glGetUniformLocation(p, "MN");
	iLocR = glGetUniformLocation(p, "R");

	iLocMDiffuse = glGetUniformLocation(p, "Material.diffuse");
	iLocMAmbient = glGetUniformLocation(p, "Material.ambient");
	iLocMSpecular = glGetUniformLocation(p, "Material.specular");
	iLocMShininess = glGetUniformLocation(p, "Material.shininess");

	iLocLDPosition = glGetUniformLocation(p, "LightSource[0].position");
	iLocLDAmbient = glGetUniformLocation(p, "LightSource[0].ambient");
	iLocLDDiffuse = glGetUniformLocation(p, "LightSource[0].diffuse");
	iLocLDSpecular = glGetUniformLocation(p, "LightSource[0].specular");

	iLocLPPosition = glGetUniformLocation(p, "LightSource[1].position");
	iLocLPAmbient = glGetUniformLocation(p, "LightSource[1].ambient");
	iLocLPDiffuse = glGetUniformLocation(p, "LightSource[1].diffuse");
	iLocLPSpecular = glGetUniformLocation(p, "LightSource[1].specular");
	iLocLPca = glGetUniformLocation(p, "LightSource[1].constantAttenuation");
	iLocLPla = glGetUniformLocation(p, "LightSource[1].linearAttenuation");
	iLocLPqa = glGetUniformLocation(p, "LightSource[1].quadraticAttenuation");

	iLocLSPosition = glGetUniformLocation(p, "LightSource[2].position");
	iLocLSAmbient = glGetUniformLocation(p, "LightSource[2].ambient");
	iLocLSDiffuse = glGetUniformLocation(p, "LightSource[2].diffuse");
	iLocLSSpecular = glGetUniformLocation(p, "LightSource[2].specular");
	iLocLSca = glGetUniformLocation(p, "LightSource[2].constantAttenuation");
	iLocLSla = glGetUniformLocation(p, "LightSource[2].linearAttenuation");
	iLocLSqa = glGetUniformLocation(p, "LightSource[2].quadraticAttenuation");
	iLocLSDir = glGetUniformLocation(p, "LightSource[2].spotDirection");
	iLocLSExp = glGetUniformLocation(p, "LightSource[2].spotExponent");
	iLocLSCut = glGetUniformLocation(p, "LightSource[2].spotCosCutoff");
	iLocLSCuta = glGetUniformLocation(p, "LightSource[2].spotCutoff");

	iLocLIAmbient = glGetUniformLocation(p, "LightSource[3].ambient");

	iLocModeOne = glGetUniformLocation(p, "modeONE"); 
	iLocModeTwo = glGetUniformLocation(p, "modeTWO");
	iLocModeThree = glGetUniformLocation(p, "modeTHREE");
	iLocModeA = glGetUniformLocation(p, "modeA");
	iLocModeD = glGetUniformLocation(p, "modeD");
	iLocModeS = glGetUniformLocation(p, "modeS");
	iLocModeC = glGetUniformLocation(p, "modeC");

	iLocEye = glGetUniformLocation(p, "eye");

	glUseProgram(p);

	// Initial LightSource value
	glUniform4fv(iLocLDPosition, 1, lightsource[0].position);
	glUniform4fv(iLocLDAmbient, 1, lightsource[0].ambient);
	glUniform4fv(iLocLDDiffuse, 1, lightsource[0].diffuse);
	glUniform4fv(iLocLDSpecular, 1, lightsource[0].specular);

	glUniform4fv(iLocLPPosition, 1, lightsource[1].position);
	glUniform4fv(iLocLPAmbient, 1, lightsource[1].ambient);
	glUniform4fv(iLocLPDiffuse, 1, lightsource[1].diffuse);
	glUniform4fv(iLocLPSpecular, 1, lightsource[1].specular);
	glUniform1f(iLocLPca, lightsource[1].constantAttenuation);
	glUniform1f(iLocLPla, lightsource[1].linearAttenuation);
	glUniform1f(iLocLPqa, lightsource[1].quadraticAttenuation);

	glUniform4fv(iLocLSPosition, 1, lightsource[2].position);
	glUniform4fv(iLocLSAmbient, 1, lightsource[2].ambient);
	glUniform4fv(iLocLSDiffuse, 1, lightsource[2].diffuse);
	glUniform4fv(iLocLSSpecular, 1, lightsource[2].specular);
	glUniform1f(iLocLSca, lightsource[2].constantAttenuation);
	glUniform1f(iLocLSla, lightsource[2].linearAttenuation);
	glUniform1f(iLocLSqa, lightsource[2].quadraticAttenuation);
	glUniform4fv(iLocLSDir, 1, lightsource[2].spotDirection);
	glUniform1f(iLocLSExp, lightsource[2].spotExponent);
	glUniform1f(iLocLSCuta, lightsource[2].spotCutoff);
	glUniform1f(iLocLSCut, lightsource[2].spotCosCutoff);

	glUniform4fv(iLocLIAmbient, 1, lightsource[3].ambient);
}


void onMouse(int who, int state, int x, int y)
{
	//printf("%18s(): (%d, %d) ", __FUNCTION__, x, y);

	switch(who)
	{
		case GLUT_LEFT_BUTTON:		
			lightsource[2].spotExponent += 5.0f;
			glUniform1f(iLocLSExp, lightsource[2].spotExponent);
			break;
		case GLUT_MIDDLE_BUTTON: 
			
			break;
		case GLUT_RIGHT_BUTTON:
			lightsource[2].spotExponent -= 5.0f;
			glUniform1f(iLocLSExp, lightsource[2].spotExponent);
			break; 
		case GLUT_WHEEL_UP:      
			lightsource[2].spotCosCutoff += 0.002f;
			glUniform1f(iLocLSCut, lightsource[2].spotCosCutoff); 
			break;
		case GLUT_WHEEL_DOWN:    
			lightsource[2].spotCosCutoff -= 0.002f;
			glUniform1f(iLocLSCut, lightsource[2].spotCosCutoff); 
			break;
		default:                 break;
	}

	switch(state)
	{
		case GLUT_DOWN: break;
		case GLUT_UP:   break;
	}

	printf("\n");
}

void onMouseMotion(int x, int y)
{
	if (mode_three == 1) {
		if (mode_c == 0) {
			lightsource[2].position[0] = 3 * ((float)(x - 400) / 800);
			lightsource[2].position[1] = 3 * ((-1)*((float)(y - 400) / 800));
			lightsource[2].position[2] = 1;
			lightsource[2].position[3] = 1;
			glUniform4fv(iLocLSPosition, 1, lightsource[2].position);
		}
		else if (mode_c == 1) {
			lightsource[0].position[0] = 3 * ((float)(x - 400) / 800);
			lightsource[0].position[1] = 3 * ((-1)*((float)(y - 400) / 800));
			lightsource[0].position[2] = 1;
			lightsource[0].position[3] = 1;
			glUniform4fv(iLocLDPosition, 1, lightsource[0].position);
		}
		else if (mode_c == 2) {
			lightsource[1].position[0] = 3 * ((float)(x - 400) / 800);
			lightsource[1].position[1] = 3 * ((-1)*((float)(y - 400) / 800));
			lightsource[1].position[2] = 1;
			lightsource[1].position[3] = 1;
			glUniform4fv(iLocLPPosition, 1, lightsource[1].position);
		}
	}
}

void onKeyboard(unsigned char key, int x, int y) 
{
	//printf("%18s(): (%d, %d) key: %c(0x%02X) ", __FUNCTION__, x, y, key, key);
	switch(key) 
	{
		case 'h': // Help
			printf("Press 'h'\n");
			printf("\n------------------ Help Menu ----------------------\n");
			printf("press 'q' 'w' 'e' to toggle the light source\n");
			printf("press 'r' to toggle auto rotation\n");
			printf("press 'a' 's' 'd' to toggle the light attribute");
			printf("press 'f' to toggle per-pixel rendering\n");
			printf("press 'z' 'x' to change model\n");
			printf("press 'c' to change the spot light into another type\n");
			printf("press 'v' 'b' to change the rotation speed\n");
			printf("use arrow button to move the point light\n");
			printf("hover mouse to move the spot light\n");
			printf("click mouse to tune EXP\n");
			printf("scroll mouse to tune CUT_OFF_ANGLE\n");
			printf("------------------ Help Menu ----------------------\n");
			break;
		//***********************************************************//
		case 'q': // Directional
			printf("Press 'q'\n");
			mode_one = 1;
			dir++;
			if (dir > 3) {
				mode_one = 0;
				dir = 0;
			}
			if (dir == 1) {
				printf("Directional Light (First)\n");
				lightsource[0].position[0] = 1;
				lightsource[0].position[1] = 3;
				lightsource[0].position[2] = 0;
				lightsource[0].position[3] = 1;
				glUniform4fv(iLocLDPosition, 1, lightsource[0].position);
			}
			else if (dir == 2) {
				printf("Directional Light (Second)\n");
				lightsource[0].position[0] = 0;
				lightsource[0].position[1] = 3;
				lightsource[0].position[2] = 0;
				lightsource[0].position[3] = 1;
				glUniform4fv(iLocLDPosition, 1, lightsource[0].position);
			}
			else if (dir == 3) {
				printf("Directional Light (Third)\n");
				lightsource[0].position[0] = -1;
				lightsource[0].position[1] = 3;
				lightsource[0].position[2] = 0;
				lightsource[0].position[3] = 1;
				glUniform4fv(iLocLDPosition, 1, lightsource[0].position);
			}

			printf("@@@ NOW: Directional Light ");
			if (mode_one == 1)	printf("ON, Point Light ");
			else printf("OFF, Point Light ");
			if (mode_two == 1) printf("ON, Spot Light ");
			else printf("OFF, Spot Light ");
			if (mode_three == 1) printf("ON @@@\n");
			else printf("OFF @@@\n");
			break;
		case 'w': // Point
			printf("Press 'w'\n");
			if (mode_two == 1) mode_two = 0;
			else mode_two = 1;

			printf("@@@ NOW: Directional Light ");
			if (mode_one == 1)	printf("ON, Point Light ");
			else printf("OFF, Point Light ");
			if (mode_two == 1) printf("ON, Spot Light ");
			else printf("OFF, Spot Light ");
			if (mode_three == 1) printf("ON @@@\n");
			else printf("OFF @@@\n");
			break;
		case 'e': // Spot
			printf("Press 'e'\n");
			if (mode_three == 1) mode_three = 0;
			else mode_three = 1;

			printf("@@@ NOW: Directional Light ");
			if (mode_one == 1)	printf("ON, Point Light ");
			else printf("OFF, Point Light ");
			if (mode_two == 1) printf("ON, Spot Light ");
			else printf("OFF, Spot Light ");
			if (mode_three == 1) printf("ON @@@\n");
			else printf("OFF @@@\n");
			break;
		case 'r': // Auto rotation
			printf("Press 'r'\n");
			rotate++;
			if (rotate > 1) {
				printf("Rotation OFF\n");
				rotate = 0;
				r_y = 0.0f;
			}
			if(rotate == 1) printf("Rotation ON\n");
			break;
		case 'a': // Ambient
			printf("Press 'a'\n");
			mode_a++;
			if (mode_a > 1) mode_a = 0;

			printf("=== NOW: Ambient ");
			if (mode_a == 1)	printf("ON, Diffuse ");
			else printf("OFF, Diffuse ");
			if (mode_d == 1) printf("ON, Specular ");
			else printf("OFF, Specular ");
			if (mode_s == 1) printf("ON ===\n");
			else printf("OFF ===\n");
			break;
		case 's': // Diffuse
			printf("Press 's'\n");
			mode_d++;
			if (mode_d > 1) mode_d = 0;

			printf("=== NOW: Ambient ");
			if (mode_a == 1)	printf("ON, Diffuse ");
			else printf("OFF, Diffuse ");
			if (mode_d == 1) printf("ON, Specular ");
			else printf("OFF, Specular ");
			if (mode_s == 1) printf("ON ===\n");
			else printf("OFF ===\n");
			break;
		case 'd': // Specular
			printf("Press 'd'\n");
			mode_s++;
			if (mode_s > 1)	mode_s = 0;
			
			printf("=== NOW: Ambient ");
			if (mode_a == 1)	printf("ON, Diffuse ");
			else printf("OFF, Diffuse ");
			if (mode_d == 1) printf("ON, Specular ");
			else printf("OFF, Specular ");
			if (mode_s == 1) printf("ON ===\n");
			else printf("OFF ===\n");
			break;
		case 'c':
			printf("Press 'c'\n");
			if (mode_three == 1) {
				mode_c++;
				if (mode_c > 2)	mode_c = 0;
				if (mode_c == 1) printf("Press 'c'\nDirectional Light\n");
				else if (mode_c == 2) printf("Press 'c'\nPoint Light\n");
				else printf("Press 'c'\nSpot Light\n");
			}
			break;
		case 'f':
			printf("Press 'f'\n");
			mode_f++;
			if (mode_f > 1)	mode_f = 0;
			if (mode_f == 1) printf("Per pixel mode\n");
			else printf("Normal pixel mode\n");
			setShaders();
			break;
		//***********************************************************//

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
		case 'o':
			printf("Press 'o'\n");
			printf("ORTHOGONAL mode\n");
			perspective = 0;
			break;
		case 'p':
			printf("Press 'p'\n");
			printf("PERSPECTIVE mode\n");
			perspective = 1;
			break;
		case GLUT_KEY_ESC: /* the Esc key */ 
			exit(0); 
			break;
	}
}

void onKeyboardSpecial(int key, int x, int y){
	switch(key)
	{
		case GLUT_KEY_LEFT:
			lightsource[1].position[0] -= 0.3f;
			glUniform4fv(iLocLPPosition, 1, lightsource[1].position);
			printf("### Light position: (%f, %f, %f) ###\n", lightsource[1].position[0], lightsource[1].position[1], lightsource[1].position[2]);
			break;
			
		case GLUT_KEY_RIGHT:
			lightsource[1].position[0] += 0.3f;
			glUniform4fv(iLocLPPosition, 1, lightsource[1].position);
			printf("### Light position: (%f, %f, %f) ###\n", lightsource[1].position[0], lightsource[1].position[1], lightsource[1].position[2]);
			break;

		case GLUT_KEY_UP:
			lightsource[1].position[2] -= 0.3f;
			glUniform4fv(iLocLPPosition, 1, lightsource[1].position);
			printf("### Light position: (%f, %f, %f) ###\n", lightsource[1].position[0], lightsource[1].position[1], lightsource[1].position[2]);
			break;

		case GLUT_KEY_DOWN:
			lightsource[1].position[2] += 0.3f;
			glUniform4fv(iLocLPPosition, 1, lightsource[1].position);
			printf("### Light position: (%f, %f, %f) ###\n", lightsource[1].position[0], lightsource[1].position[1], lightsource[1].position[2]);
			break;
		default:
			break;
	}
	
}


void onWindowReshape(int width, int height)
{
	//printf("%18s(): %dx%d\n", __FUNCTION__, width, height);
}

int main(int argc, char **argv) 
{
	// glut init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// create window
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("10420 CS550000 CG HW3 103062234");

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
	//glutMotionFunc  (onMouseMotion);
	glutReshapeFunc (onWindowReshape);
	glutPassiveMotionFunc(onMouseMotion);

	//set up lighting parameters
	setLightingSource();

	// set up shaders here
	setShaders();
	
	glEnable(GL_DEPTH_TEST);

	// main loop
	glutMainLoop();

	// free
	glmDelete(OBJ);

	return 0;
}