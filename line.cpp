#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<gl\glut.h>
using namespace std;

int m0, m1, n0, n1;//声明全局变量，起始坐标和终止坐标
//int winwidth = 1024, winheight = 720;//窗口长宽
int winwidth = 800, winheight = 500;//窗口长宽
void DrawLine(); //实际上画直线的函数
void DDALine(int, int, int, int);
void MidpointLine(int, int, int, int);
void BresenhamLine(int, int, int, int);


//数据交换
void swap(GLint& a, GLint& b) { GLint t = a; a = b; b = t; }


//在屏幕上画一个点
void setPixel(GLint x, GLint y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

//实际上画直线的函数
void DrawLine() {
	/*自己选择要用的方法*/
	//DDALine(m0, n0, m1, n1);  //DDA画线法
	//MidpointLine(m0,n0,m1,n1);//中点划线法
	BresenhamLine(m0, n0, m1, n1);//BresenhamLine算法
}

/*DDA画线法*/
void DDALine(int x0, int y0, int x1, int y1)
{
	glClear(GL_COLOR_BUFFER_BIT);//清除窗口显示内容
	int dx, dy, steps, i;
	float x, y, delta_x, delta_y;
	dx = x1 - x0;
	dy = y1 - y0;
	x = x0;
	y = y0;

	//解决斜率问题:选定（x2-x1)和(y2-y1)中较大者为步进方向。
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
		glColor3f(1.0f, 0.0f, 0.0f); //选择颜色
		glVertex2f(int(x + 0.5), int(y + 0.5));  //四舍五入
		//SetPixel(int(x + 0.5), int(y + 0.5),RED);  //四舍五入
		x += delta_x;
		y += delta_y;
	}
	glEnd();
	glFlush();
}

/*中心画线法*/
void MidpointLine(int x0, int y0, int x1, int y1)
{
	glClear(GL_COLOR_BUFFER_BIT);//清除窗口显示内容

	int a, b, d1, d2, d, x, y;
	a = y0 - y1, b = x1 - x0;
	x = x0, y = y0;

	int dx, dy;
	dx = abs(x1 - x0), dy = abs(y1 - y0);
	int flag = 0;  //斜率绝对值标志，0表示斜率绝对值小于1
	if (dy > dx)//斜率绝对值大于1
	{
		flag = 1;
	}
	//确定步长tx，ty，斜率为正或为负，且只考虑左/右上or左/右下，左/右边的情况不加步长即可
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
			if (d > 0) //注意
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

/*BresenhamLine算法*/
void BresenhamLine(int x0, int y0, int x1, int y1)
{
	glClear(GL_COLOR_BUFFER_BIT);//清除窗口显示内容

	int x, y, dx, dy, e;
	x = x0, y = y0;
	dx = x1 - x0, dy = y1 - y0;
	/*e = -dx;*/

	int flag = 0;  //斜率绝对值标志，0表示斜率绝对值小于1
	if (abs(dy) > abs(dx))//斜率绝对值大于1
	{
		flag = 1;
	}
	//确定步长tx，ty，斜率为正或为负，且只考虑左/右上or左/右下，左/右边的情况不加步长即可
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


//鼠标拖动
void dragmouse(int x, int y) {
	m1 = x;
	n1 = y;
	//printf("%d %d %d %d\n", m0, n0, m1, n1);//把两个坐标打印出来
	DrawLine();//画线
	glFlush();

}
int flag = 0; //flag为0时表示是起点

//鼠标监听，画点
void mymouse(int button, int state, int x, int y) {
	//flag = 0;

	if (flag == 0) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			m0 = x;
			n0 = y;
			setPixel(m0, n0);
			//glVertex2i(m0, n0);
			printf("起点：%d %d      ", m0, n0);//把两个坐标打印出来	
			flag = 1;
		}
	}
	else {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			m1 = x;
			n1 = y;
			setPixel(m1, n1);
			//glVertex2i(m1, n1);
			printf("终点：%d %d\n", m1, n1);//把两个坐标打印出来
			DrawLine();//画线
			glFlush();
			glEnd();
			flag = 0;
		}
	}
}

void init() {
	glClearColor(1, 1, 1, 1);//设置绘制窗口颜色为白色
	glClear(GL_COLOR_BUFFER_BIT);//清除窗口内容
	glPointSize(3.0f);//设置点的大小
	/*设置为投影类型模式和其他观察参数*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, winwidth, winheight, 0);
	glColor3f(1, 0, 0);//设置画点的颜色
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);//初始化
	//	scanf("%d%d",&m0,&n0);
	//	m1=m0,n1=n0;//让起始点与目标点重合

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//设置绘制模式
	glutInitWindowPosition(150, 100);//设置窗口出现的位置
	glutInitWindowSize(winwidth, winheight);//设置窗口大小
	glutCreateWindow("画直线");//创建窗口
	init();

	glutDisplayFunc(DrawLine);//绘制回调函数,glut机制，它觉得需要重新绘制的时候就会执行
	glutMouseFunc(mymouse);//鼠标监听回调函数
	glutMotionFunc(dragmouse);//鼠标拖动
	glutMainLoop();
}
