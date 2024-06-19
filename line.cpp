#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<gl\glut.h>
using namespace std;

int m0, m1, n0, n1;//����ȫ�ֱ�������ʼ�������ֹ����
//int winwidth = 1024, winheight = 720;//���ڳ���
int winwidth = 800, winheight = 500;//���ڳ���
void DrawLine(); //ʵ���ϻ�ֱ�ߵĺ���
void DDALine(int, int, int, int);
void MidpointLine(int, int, int, int);
void BresenhamLine(int, int, int, int);


//���ݽ���
void swap(GLint& a, GLint& b) { GLint t = a; a = b; b = t; }


//����Ļ�ϻ�һ����
void setPixel(GLint x, GLint y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

//ʵ���ϻ�ֱ�ߵĺ���
void DrawLine() {
	/*�Լ�ѡ��Ҫ�õķ���*/
	//DDALine(m0, n0, m1, n1);  //DDA���߷�
	//MidpointLine(m0,n0,m1,n1);//�е㻮�߷�
	BresenhamLine(m0, n0, m1, n1);//BresenhamLine�㷨
}

/*DDA���߷�*/
void DDALine(int x0, int y0, int x1, int y1)
{
	glClear(GL_COLOR_BUFFER_BIT);//���������ʾ����
	int dx, dy, steps, i;
	float x, y, delta_x, delta_y;
	dx = x1 - x0;
	dy = y1 - y0;
	x = x0;
	y = y0;

	//���б������:ѡ����x2-x1)��(y2-y1)�нϴ���Ϊ��������
	if (abs(dx) > abs(dy))
		steps = abs(dx);
	else
		steps = abs(dy);
	delta_x = (float)dx / (float)steps;
	delta_y = (float)dy / (float)steps;
	//glPointSize(2);
	glBegin(GL_POINTS);
	for (i = 0; i <= steps; i++)
	{
		glColor3f(1.0f, 0.0f, 0.0f); //ѡ����ɫ
		glVertex2f(int(x + 0.5), int(y + 0.5));  //��������
		//SetPixel(int(x + 0.5), int(y + 0.5),RED);  //��������
		x += delta_x;
		y += delta_y;
	}
	glEnd();
	glFlush();
}

/*���Ļ��߷�*/
void MidpointLine(int x0, int y0, int x1, int y1)
{
	glClear(GL_COLOR_BUFFER_BIT);//���������ʾ����

	int a, b, d1, d2, d, x, y;
	a = y0 - y1, b = x1 - x0;
	x = x0, y = y0;

	int dx, dy;
	dx = abs(x1 - x0), dy = abs(y1 - y0);
	int flag = 0;  //б�ʾ���ֵ��־��0��ʾб�ʾ���ֵС��1
	if (dy > dx)//б�ʾ���ֵ����1
	{
		flag = 1;
	}
	//ȷ������tx��ty��б��Ϊ����Ϊ������ֻ������/����or��/���£���/�ұߵ�������Ӳ�������
	int tx = (x1 > x0 ? 1 : (b = -b, -1));
	int ty = (y1 > y0 ? 1 : (a = -a, -1));

	glBegin(GL_POINTS);
	glVertex2i(x, y);
	if (flag == 0) {
		d = 2 * a + b, d1 = 2 * a, d2 = 2 * (a + b);
		while (x != x1)
		{
			if (d < 0)
			{
				y += ty;
				d += d2;
			}
			else
			{
				d += d1;
			}
			x += tx;
			glVertex2i(x, y);
			//setPixel(x, y);
		}
	}
	else {
		d = 2 * b + a, d1 = 2 * b, d2 = 2 * (a + b);
		while (y != y1)
		{
			if (d > 0) //ע��
			{
				x += tx;
				d += d2;
			}
			else
			{
				d += d1;
			}
			y += ty;
			glVertex2i(x, y);
			//setPixel(x, y);
		}
	}
	glEnd();
	glFlush();

}

/*BresenhamLine�㷨*/
void BresenhamLine(int x0, int y0, int x1, int y1)
{
	glClear(GL_COLOR_BUFFER_BIT);//���������ʾ����

	int x, y, dx, dy, e;
	x = x0, y = y0;
	dx = x1 - x0, dy = y1 - y0;
	/*e = -dx;*/

	int flag = 0;  //б�ʾ���ֵ��־��0��ʾб�ʾ���ֵС��1
	if (abs(dy) > abs(dx))//б�ʾ���ֵ����1
	{
		flag = 1;
	}
	//ȷ������tx��ty��б��Ϊ����Ϊ������ֻ������/����or��/���£���/�ұߵ�������Ӳ�������
	int tx = (x1 > x0 ? 1 : (dx = -dx, -1));
	int ty = (y1 > y0 ? 1 : (dy = -dy, -1));

	glBegin(GL_POINTS);
	glVertex2i(x, y);
	if (flag == 0) {
		e = -dx;
		while (x != x1)
		{
			x += tx;
			e += 2 * dy;
			if (e >= 0)
			{
				y += ty;
				e -= 2 * dx;
			}
			glVertex2i(x, y);
			//setPixel(x, y);
		}
	}
	else {
		e = -dy;
		while (y != y1)
		{
			y += ty;
			e += 2 * dx;
			if (e >= 0)
			{
				x += tx;
				e -= 2 * dy;
			}
			glVertex2i(x, y);
			//setPixel(x, y);
		}
	}
	glEnd();
	glFlush();
}


//����϶�
void dragmouse(int x, int y) {
	m1 = x;
	n1 = y;
	//printf("%d %d %d %d\n", m0, n0, m1, n1);//�����������ӡ����
	DrawLine();//����
	glFlush();

}
int flag = 0; //flagΪ0ʱ��ʾ�����

//������������
void mymouse(int button, int state, int x, int y) {
	//flag = 0;

	if (flag == 0) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			m0 = x;
			n0 = y;
			setPixel(m0, n0);
			//glVertex2i(m0, n0);
			printf("��㣺%d %d      ", m0, n0);//�����������ӡ����	
			flag = 1;
		}
	}
	else {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			m1 = x;
			n1 = y;
			setPixel(m1, n1);
			//glVertex2i(m1, n1);
			printf("�յ㣺%d %d\n", m1, n1);//�����������ӡ����
			DrawLine();//����
			glFlush();
			glEnd();
			flag = 0;
		}
	}
}

void init() {
	glClearColor(1, 1, 1, 1);//���û��ƴ�����ɫΪ��ɫ
	glClear(GL_COLOR_BUFFER_BIT);//�����������
	glPointSize(3.0f);//���õ�Ĵ�С
	/*����ΪͶӰ����ģʽ�������۲����*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winwidth, winheight, 0);
	glColor3f(1, 0, 0);//���û������ɫ
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);//��ʼ��
	//	scanf("%d%d",&m0,&n0);
	//	m1=m0,n1=n0;//����ʼ����Ŀ����غ�

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//���û���ģʽ
	glutInitWindowPosition(150, 100);//���ô��ڳ��ֵ�λ��
	glutInitWindowSize(winwidth, winheight);//���ô��ڴ�С
	glutCreateWindow("��ֱ��");//��������
	init();

	glutDisplayFunc(DrawLine);//���ƻص�����,glut���ƣ���������Ҫ���»��Ƶ�ʱ��ͻ�ִ��
	glutMouseFunc(mymouse);//�������ص�����
	glutMotionFunc(dragmouse);//����϶�
	glutMainLoop();
}
