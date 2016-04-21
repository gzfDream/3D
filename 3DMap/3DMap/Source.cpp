#include <windows.h>	// Windows��ͷ�ļ�

#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL\glut.h>		// OpenGL32���ͷ�ļ�
#include <GL\GL.H>
#include <GL\glu.h>		// GLu32���ͷ�ļ�
#include <GL\glaux.h>	// GLaux���ͷ�ļ�

#include <iostream>


#pragma comment( lib, "winmm.lib")
#pragma comment( lib, "opengl32.lib")	// OpenGL32���ӿ�
#pragma comment( lib, "glu32.lib")		// GLu32���ӿ�
#pragma comment( lib, "glaux.lib")		// GLaux���ӿ�

#define MAP		40
#define PI      3.1415926
GLdouble	g_eye[3];		//
GLdouble	g_look[3];		//
float		rad_xz;			//�Ƕ�
float		g_Angle;		//����ת
float		g_elev;			//������
int glx;
int gly;
using namespace std;


void init(){
	g_eye[0] = MAP;//
	g_eye[2] = -MAP;//
	g_Angle = -10;//��λ��
	g_elev = 0;//������
}

void mouse(int key,int x,int y){

	float speed = 0.2f;							//����
	if (key==GLUT_ACTIVE_SHIFT)  speed = speed * 4;//��SHIFTʱ�ļ���
	if (key==GLUT_KEY_LEFT)   g_Angle -= speed * 2;//��ת
	if (key == GLUT_KEY_RIGHT)  g_Angle += speed * 2;//��ת
	rad_xz = float(3.13149* g_Angle / 180.0f);	//����������ת�Ƕ�
	if (key == GLUT_KEY_PAGE_UP){ g_elev += 0.2f; cout << "pg" << g_elev; }	//Page UP  ��
	if (key == GLUT_KEY_PAGE_DOWN) g_elev -= 0.2f;	//Page Down��
	if (g_elev < -100)	g_elev = -100;	//������
	if (g_elev > 100)	g_elev = 100;	//������
	if (key == GLUT_KEY_UP)				//ǰ��
	{
		g_eye[2] += sin(rad_xz)*speed;
		g_eye[0] += cos(rad_xz)*speed;
		cout << "up"<<g_eye[2];
	}
	if (key == GLUT_KEY_DOWN)			//����
	{
		g_eye[2] -= sin(rad_xz)*speed;
		g_eye[0] -= cos(rad_xz)*speed;
	}
	//���Ƶ���������뿪����
	 
	if (g_eye[0] < -(MAP * 2 - 20))	g_eye[0] = -(MAP * 2 - 20); //�ӵ��X��������
	if (g_eye[0] > (MAP * 2 - 20))	g_eye[0] = (MAP * 2 - 20);
	if (g_eye[2] < -(MAP * 2 - 20))	g_eye[2] = -(MAP * 2 - 20); //�ӵ��Z��������
	if (g_eye[2] > (MAP * 2 - 20))	g_eye[2] = (MAP * 2 - 20);
	g_eye[1] = 1.8;//����������Ե�λ�ø�
	//������ķ���
	g_look[0] = float(g_eye[0] + 100 * cos(rad_xz));
	g_look[2] = float(g_eye[2] + 100 * sin(rad_xz));
	g_look[1] = g_eye[1];
	//����modelview������
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

//��������Ϊ��x��y��z������İ뾶Ϊradius��M��N�ֱ��ʾ����ĺ����򱻷ֳɶ��ٷ�  
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
	glPushAttrib(GL_CURRENT_BIT);//����������ɫ��ʵ��
	glEnable(GL_BLEND);//ʹ������
	glPushMatrix();
	glColor3f(0.5f, 0.7f, 1.0f);//������ɫ
	glTranslatef(0, 0.0f, 0);		//ƽ̨�Ķ�λ
	int size0 = (int)(MAP * 2);
	glBegin(GL_LINES);//��һ����
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
	glPopAttrib();//�ָ�ǰһ����
}

void drawS(float x,float y,float z){
	glPushAttrib(GL_CURRENT_BIT);//����������ɫ��ʵ��
	glPushMatrix(); 
	//glTranslatef(50 + X, 0, -40 + Z);		//ƽ̨�Ķ�λ
	glColor3f(1.0f, 1.0f, 0.2f);		//��ɫ
	drawSphere(x + X, y, z + Z, 1, 20, 20);
	glPopMatrix();
}

GLfloat r = 0;
void picter(float x, float y, float z)//���ͼ��
{
	glPushAttrib(GL_CURRENT_BIT);//����������ɫ��ʵ��
	glPushMatrix();//ƽ̨==============================
	glTranslatef(x, y + 0.5f, z);		//ƽ̨�Ķ�λ
	glColor3f(0.0f, 1.0f, 0.2f);		//��ɫ
	auxSolidCube(1);				//��̨(�߳�)
	glTranslatef(0.0f, 0.8f, 0.0f);	//�ܵ�λ�õ���,����0.8
	glColor3f(0.0f, 0.0f, 1.0f);		//��ɫ
	auxSolidBox(.2f, 1.3f, .2f);		//������(���ߡ���)
	glPopMatrix();
	glPushMatrix();//�״�==============================
	glTranslatef(x, y + 2.5f, z);		//�״�Ķ�λ1
	glRotatef(r - 90, 0.0, 1.0, 0.0);	//�״���ת2
	//=======================================
	glColor3f(1.0f, 1.0f, 1.0f);		//��ɫ
	glRotatef(45, 1.0, 0.0, 0.0);	//�̵ĽǶȵ�������30��
	auxWireCone(1.5, 0.6f);			//��԰׶��(�װ뾶����)
	//=======================================
	glRotatef(180, 1.0, 0.0, 0.0);	//�˵ĽǶȵ���,������ת
	glTranslatef(0.0f, 0.0f, -0.7f);  //�˵�λ�õ���,����һ��
	auxWireCone(0.2f, 2.0f);			//԰׶��(�װ뾶����)
	glColor3f(125, 0, 0);			//�����ɫ
	glTranslatef(0.0f, 0.0f, 2.0f);	//�˵�λ�õ���,����һ��
	auxSolidSphere(0.1f);			//԰(�뾶)
	glPopMatrix();
	glPopAttrib();//�ָ�ǰһ����
	r += 0.5f; if (r > 360) r = 0;

}

void ss(){
	r += 5; if (r > 360) r = 0;
	//picter(MAP + 10, 0, -MAP);
}

void display(){
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);			 // ����ˢ�±���ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// ˢ�±���
	glLoadIdentity();								 // ���õ�ǰ��ģ�͹۲����

	gluLookAt(g_eye[0], g_eye[1], g_eye[2], g_look[0], g_look[1] + g_elev, g_look[2], 0.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	drawground();
	picter(MAP + 10, 0, -MAP);
	drawS(MAP + 10, 0, -MAP);
	
	//glFlush();
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h){
	glViewport(0, 0, w, h);			// ����OpenGL�ӿڴ�С��	
	glMatrixMode(GL_PROJECTION);			// ���õ�ǰ����ΪͶӰ����
	glLoadIdentity();						// ���õ�ǰָ���ľ���Ϊ��λ����
	gluPerspective							// ����͸��ͼ
		(54.0f,							// ͸�ӽ�����Ϊ 45 ��
		(GLfloat)w / (GLfloat)h,	// ���ڵĿ���߱�
		0.1f,								// ��Ұ͸�����:����1.0f
		3000.0f							// ��Ұ͸�����:ʼ��0.1fԶ��1000.0f
		);
	// �������������ƣ���һ���������þ�ͷ��Ƕȣ��ڶ��������ǳ���ȣ�������Զ�����С�
	glMatrixMode(GL_MODELVIEW);				// ���õ�ǰ����Ϊģ����ͼ����
	glLoadIdentity();						// ���õ�ǰָ���ľ���Ϊ��λ����

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