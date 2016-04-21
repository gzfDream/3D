#include <windows.h>	// Windows的头文件

#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL\glut.h>		// OpenGL32库的头文件
#include <GL\GL.H>
#include <GL\glu.h>		// GLu32库的头文件
#include <GL\glaux.h>	// GLaux库的头文件

#include <iostream>


#pragma comment( lib, "winmm.lib")
#pragma comment( lib, "opengl32.lib")	// OpenGL32连接库
#pragma comment( lib, "glu32.lib")		// GLu32连接库
#pragma comment( lib, "glaux.lib")		// GLaux连接库

#define MAP		40
#define PI      3.1415926
GLdouble	g_eye[3];		//
GLdouble	g_look[3];		//
float		rad_xz;			//角度
float		g_Angle;		//左右转
float		g_elev;			//仰俯角
int glx;
int gly;
using namespace std;


void init(){
	g_eye[0] = MAP;//
	g_eye[2] = -MAP;//
	g_Angle = -10;//方位角
	g_elev = 0;//俯仰角
}

void mouse(int key,int x,int y){

	float speed = 0.2f;							//步长
	if (key==GLUT_ACTIVE_SHIFT)  speed = speed * 4;//按SHIFT时的加速
	if (key==GLUT_KEY_LEFT)   g_Angle -= speed * 2;//左转
	if (key == GLUT_KEY_RIGHT)  g_Angle += speed * 2;//右转
	rad_xz = float(3.13149* g_Angle / 180.0f);	//计算左右旋转角度
	if (key == GLUT_KEY_PAGE_UP){ g_elev += 0.2f; cout << "pg" << g_elev; }	//Page UP  键
	if (key == GLUT_KEY_PAGE_DOWN) g_elev -= 0.2f;	//Page Down键
	if (g_elev < -100)	g_elev = -100;	//仰俯角
	if (g_elev > 100)	g_elev = 100;	//仰俯角
	if (key == GLUT_KEY_UP)				//前进
	{
		g_eye[2] += sin(rad_xz)*speed;
		g_eye[0] += cos(rad_xz)*speed;
		cout << "up"<<g_eye[2];
	}
	if (key == GLUT_KEY_DOWN)			//后退
	{
		g_eye[2] -= sin(rad_xz)*speed;
		g_eye[0] -= cos(rad_xz)*speed;
	}
	//控制到摄像机不离开地面
	 
	if (g_eye[0] < -(MAP * 2 - 20))	g_eye[0] = -(MAP * 2 - 20); //视点的X分量限制
	if (g_eye[0] > (MAP * 2 - 20))	g_eye[0] = (MAP * 2 - 20);
	if (g_eye[2] < -(MAP * 2 - 20))	g_eye[2] = -(MAP * 2 - 20); //视点的Z分量限制
	if (g_eye[2] > (MAP * 2 - 20))	g_eye[2] = (MAP * 2 - 20);
	g_eye[1] = 1.8;//设置摄像机对地位置高
	//摄像机的方向
	g_look[0] = float(g_eye[0] + 100 * cos(rad_xz));
	g_look[2] = float(g_eye[2] + 100 * sin(rad_xz));
	g_look[1] = g_eye[1];
	//建立modelview矩阵方向
	//gluLookAt(g_eye[0], g_eye[1], g_eye[2], g_look[0], g_look[1] + g_elev, g_look[2], 0.0, 1.0, 0.0);
	glutPostRedisplay();
}

int X = 0;
int Z = 0;

void keyboard(unsigned char key,int x,int y){
	switch (key)
	{
	case 'd':
		Z=Z + 1;
		if (Z > 100) Z = 100;
		cout << Z;
		glutPostRedisplay();
		break;
	case 'a':
		Z = Z - 1;
		if (Z < -100) Z = -100;
		glutPostRedisplay();
		break;
	case 's':
		X = X - 1;
		if (X < -100) X = -100;
		glutPostRedisplay();
		break;
	case 'w':
		X = X + 1;
		if (X > 100) X = 100;
		cout << X;
		glutPostRedisplay();
		break;

	default:
		break;
	}
}

//球心坐标为（x，y，z），球的半径为radius，M，N分别表示球体的横纵向被分成多少份  
void drawSphere(GLfloat xx, GLfloat yy, GLfloat zz, GLfloat radius, GLfloat M, GLfloat N)
{
	float step_z = PI / M;
	float step_xy = 2 * PI / N;
	float x[4], y[4], z[4];

	float angle_z = 0.0;
	float angle_xy = 0.0;
	int i = 0, j = 0;
	glBegin(GL_QUADS);
	for (i = 0; i<M; i++)
	{
		angle_z = i * step_z;

		for (j = 0; j<N; j++)
		{
			angle_xy = j * step_xy;

			x[0] = radius * sin(angle_z) * cos(angle_xy);
			y[0] = radius * sin(angle_z) * sin(angle_xy);
			z[0] = radius * cos(angle_z);

			x[1] = radius * sin(angle_z + step_z) * cos(angle_xy);
			y[1] = radius * sin(angle_z + step_z) * sin(angle_xy);
			z[1] = radius * cos(angle_z + step_z);

			x[2] = radius*sin(angle_z + step_z)*cos(angle_xy + step_xy);
			y[2] = radius*sin(angle_z + step_z)*sin(angle_xy + step_xy);
			z[2] = radius*cos(angle_z + step_z);

			x[3] = radius * sin(angle_z) * cos(angle_xy + step_xy);
			y[3] = radius * sin(angle_z) * sin(angle_xy + step_xy);
			z[3] = radius * cos(angle_z);

			for (int k = 0; k<4; k++)
			{
				glVertex3f(xx + x[k], yy + y[k], zz + z[k]);
			}
		}
	}
	glEnd();

	glPopMatrix();
}

void drawground(){
	glPushAttrib(GL_CURRENT_BIT);//保存现有颜色属实性
	glEnable(GL_BLEND);//使用纹理
	glPushMatrix();
	glColor3f(0.5f, 0.7f, 1.0f);//设置蓝色
	glTranslatef(0, 0.0f, 0);		//平台的定位
	int size0 = (int)(MAP * 2);
	glBegin(GL_LINES);//划一界线
	for (int x = -size0; x < size0; x += 4)
	{
		glVertex3i(x, 0, -size0); glVertex3i(x, 0, size0);
	}
	for (int z = -size0; z < size0; z += 4)
	{
		glVertex3i(-size0, 0, z); glVertex3i(size0, 0, z);
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glPopAttrib();//恢复前一属性
}

void drawS(float x,float y,float z){
	glPushAttrib(GL_CURRENT_BIT);//保存现有颜色属实性
	glPushMatrix(); 
	//glTranslatef(50 + X, 0, -40 + Z);		//平台的定位
	glColor3f(1.0f, 1.0f, 0.2f);		//绿色
	drawSphere(x + X, y, z + Z, 1, 20, 20);
	glPopMatrix();
}

GLfloat r = 0;
void picter(float x, float y, float z)//组合图形
{
	glPushAttrib(GL_CURRENT_BIT);//保存现有颜色属实性
	glPushMatrix();//平台==============================
	glTranslatef(x, y + 0.5f, z);		//平台的定位
	glColor3f(0.0f, 1.0f, 0.2f);		//绿色
	auxSolidCube(1);				//方台(边长)
	glTranslatef(0.0f, 0.8f, 0.0f);	//架的位置调整,上升0.8
	glColor3f(0.0f, 0.0f, 1.0f);		//蓝色
	auxSolidBox(.2f, 1.3f, .2f);		//长方架(宽、高、长)
	glPopMatrix();
	glPushMatrix();//雷达==============================
	glTranslatef(x, y + 2.5f, z);		//雷达的定位1
	glRotatef(r - 90, 0.0, 1.0, 0.0);	//雷达旋转2
	//=======================================
	glColor3f(1.0f, 1.0f, 1.0f);		//白色
	glRotatef(45, 1.0, 0.0, 0.0);	//盘的角度调整，仰30度
	auxWireCone(1.5, 0.6f);			//线园锥盘(底半径、高)
	//=======================================
	glRotatef(180, 1.0, 0.0, 0.0);	//杆的角度调整,反方向转
	glTranslatef(0.0f, 0.0f, -0.7f);  //杆的位置调整,缩进一点
	auxWireCone(0.2f, 2.0f);			//园锥杆(底半径、高)
	glColor3f(125, 0, 0);			//随机红色
	glTranslatef(0.0f, 0.0f, 2.0f);	//杆的位置调整,缩进一点
	auxSolidSphere(0.1f);			//园(半径)
	glPopMatrix();
	glPopAttrib();//恢复前一属性
	r += 0.5f; if (r > 360) r = 0;

}

void ss(){
	r += 5; if (r > 360) r = 0;
	//picter(MAP + 10, 0, -MAP);
}

void display(){
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);			 // 设置刷新背景色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// 刷新背景
	glLoadIdentity();								 // 重置当前的模型观察矩阵

	gluLookAt(g_eye[0], g_eye[1], g_eye[2], g_look[0], g_look[1] + g_elev, g_look[2], 0.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	drawground();
	picter(MAP + 10, 0, -MAP);
	drawS(MAP + 10, 0, -MAP);
	
	//glFlush();
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h){
	glViewport(0, 0, w, h);			// 设置OpenGL视口大小。	
	glMatrixMode(GL_PROJECTION);			// 设置当前矩阵为投影矩阵。
	glLoadIdentity();						// 重置当前指定的矩阵为单位矩阵
	gluPerspective							// 设置透视图
		(54.0f,							// 透视角设置为 45 度
		(GLfloat)w / (GLfloat)h,	// 窗口的宽与高比
		0.1f,								// 视野透视深度:近点1.0f
		3000.0f							// 视野透视深度:始点0.1f远点1000.0f
		);
	// 这和照象机很类似，第一个参数设置镜头广角度，第二个参数是长宽比，后面是远近剪切。
	glMatrixMode(GL_MODELVIEW);				// 设置当前矩阵为模型视图矩阵
	glLoadIdentity();						// 重置当前指定的矩阵为单位矩阵

	gluLookAt(g_eye[0], g_eye[1], g_eye[2], g_look[0], g_look[1] + g_elev, g_look[2], 0.0, 1.0, 0.0);
}


int main(int argc, char* argv[]){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(300, 300);
	glutCreateWindow("sss");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(mouse);
	//glutIdleFunc(ss);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;

}