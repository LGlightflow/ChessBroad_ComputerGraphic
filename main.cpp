#include<cstdio>
#include<cstdlib>
#include<cmath>
#include"ViewFrame.h"
#include"ObjLoader.h"
#include<gl\freeglut.h>

using namespace std;

#include <math.h>

#define PI 3.14159265


float window_height = 720;
float window_width = 1080;

// 立方体动画

GLfloat cube_angle = 45;

// 游戏模式
int score = 0; // 游戏得分
bool game_mode = false;
// plant的位置
float posx,posy;
float dstx,dsty;
float shiftx, shifty;
int shift_speed = 1;
int ang = 1;
// 坐标系显示
bool show_coordinate = true;

// 手电筒模式
bool light_mode = true;

// 棋子
bool chess_mode = true;


//棋子移动
bool isSelected = false;
bool isRised = false;
ViewFrame view_frame;

ObjLoader plant = ObjLoader("./obj/indoor plant_02.obj");
ObjLoader pawn = ObjLoader("./obj/pawn.obj");
ObjLoader rook = ObjLoader("./obj/rook.obj");
ObjLoader knight = ObjLoader("./obj/knight.obj");
ObjLoader bishop = ObjLoader("./obj/bishop.obj");
ObjLoader queen = ObjLoader("./obj/queen.obj");
ObjLoader king = ObjLoader("./obj/king.obj");


GLuint bgSkyTextureID, bgGrassTextureID;

GLuint loadBackgroundTexture(const char* texturePath, GLuint bgTextureID);



// 初始化
void init(void)
{
	// 模型初始位置
	posx = 6;
	posy = 4;
	dstx = rand() % 6;
	dsty = rand() % 8;
	static GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH); // 着色，使用顶点响应颜色进行着色

	// 光源的属性
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHTING); 
	glEnable(GL_LIGHT0); 
	glEnable(GL_DEPTH_TEST); // 深度缓冲区
	glEnable(GL_NORMALIZE); // 自动正则化
	glEnable(GL_CULL_FACE); // 剔除操作，避免不必要的渲染

	// 抗锯齿
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);



	// 初始化相机坐标系
	view_frame.P0 = Vector3D(500.0, 0.0, 150.0);
	view_frame.u = Vector3D(0.0, 1.0, 0.0);
	view_frame.v = Vector3D(0.0, 0.0, 1.0);
	view_frame.n = Vector3D(1.0, 0.0, 0.0);
	view_frame.step = 10;
	view_frame.turn_a = PI / 18;
	view_frame.pitch_a = PI / 18;
	view_frame.roll_a = PI / 18;
	
	

	bgSkyTextureID = loadBackgroundTexture("./tex/sky.bmp", bgSkyTextureID);
	bgGrassTextureID = loadBackgroundTexture("./tex/grass.bmp", bgGrassTextureID);
	plant.loadTextureImage("./tex/plant.bmp");
	pawn.loadTextureImage("./tex/pawn.bmp");
	rook.loadTextureImage("./tex/rook.bmp");
	knight.loadTextureImage("./tex/knight.bmp");
	bishop.loadTextureImage("./tex/bishop.bmp");
	queen.loadTextureImage("./tex/queen.bmp");
	king.loadTextureImage("./tex/king.bmp");
}


GLuint loadBackgroundTexture(const char* texturePath, GLuint bgTextureID) {

	std::ifstream file(texturePath, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "无法打开纹理文件" << std::endl;
		return 0;
	}

	// BMP文件头部
	char header[54];
	file.read(header, 54);

	// 从文件头中获取图像的宽度和高度
	int width = *(int*)&header[18];
	int height = *(int*)&header[22];
	int imageSize = *(int*)&header[34];

	if (imageSize == 0) {
		imageSize = width * height * 3;
	}

	unsigned char* textureData = new unsigned char[imageSize];
	file.read(reinterpret_cast<char*>(textureData), imageSize);
	file.close();


	// 创建纹理
	glGenTextures(1, &bgTextureID);
	glBindTexture(GL_TEXTURE_2D, bgTextureID);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, textureData);
	delete[] textureData;
	//glBindTexture(GL_TEXTURE_2D, 0);
	return bgTextureID;
}


void drawBackground() {
	// 设置背景纹理
	static GLfloat mat_color[] = { 1.0, 1.0, 1.0, 0.1 };


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
	//地面
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 2);
	glPushMatrix();
	glScalef(110, 110, 1);
	glTranslatef(0, 0, -25);

	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, 0.0);
	glTexCoord2d(1.0, 0.0); glVertex3d(+5.0, -5.0, 0.0);
	glTexCoord2d(1.0, 1.0); glVertex3d(+5.0, +5.0, 0.0);
	glTexCoord2d(0.0, 1.0); glVertex3d(-5.0, +5.0, 0.0);
	glEnd();
	glPopMatrix();

	//顶部
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 1 );
	glPushMatrix();
	glScalef(110, 110, 1);
	glTranslatef(0, 0, 475);
	glRotatef(180, 1, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, 5.0);
	glTexCoord2d(1.0, 0.0); glVertex3d(+5.0, -5.0, 5.0);
	glTexCoord2d(1.0, 1.0); glVertex3d(+5.0, +5.0, 5.0);
	glTexCoord2d(0.0, 1.0); glVertex3d(-5.0, +5.0, 5.0);
	glEnd();
	glPopMatrix();


	//背部
	glPushMatrix();
	glTranslatef(-500, 0, -25);
	glScalef(1, 110, 110);
	glBegin(GL_QUADS);

	glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, 0.0);
	glTexCoord2d(1.0, 0.0); glVertex3d(-5.0, +5.0, 0.0);
	glTexCoord2d(1.0, 1.0); glVertex3d(-5.0, +5.0, 5.0);
	glTexCoord2d(0.0, 1.0); glVertex3d(-5.0, -5.0, 5.0);
	glEnd();
	glPopMatrix();


	//前面
	glPushMatrix();
	glTranslatef(500, 0, -25);
	glScalef(1, 110, 110);
	glRotatef(180, 0, 0, 1);
	glBegin(GL_QUADS);

	glTexCoord2d(0.0, 0.0); glVertex3d(+5.0, -5.0, 0.0);
	glTexCoord2d(1.0, 0.0); glVertex3d(+5.0, +5.0, 0.0);
	glTexCoord2d(1.0, 1.0); glVertex3d(+5.0, +5.0, 5.0);
	glTexCoord2d(0.0, 1.0); glVertex3d(+5.0, -5.0, 5.0);
	glEnd();
	glPopMatrix();

	//右面
	glPushMatrix();
	glTranslatef(0, 500, -25);
	glScalef(110, 1, 110);
	glBegin(GL_QUADS);

	glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, +5.0, 0.0);
	glTexCoord2d(1.0, 0.0); glVertex3d(+5.0, +5.0, 0.0);
	glTexCoord2d(1.0, 1.0); glVertex3d(+5.0, +5.0, 5.0);
	glTexCoord2d(0.0, 1.0); glVertex3d(-5.0, +5.0, 5.0);
	glEnd();

	glPopMatrix();


	//左面
	glPushMatrix();
	glRotatef(180, 0, 0, 1);
	glTranslatef(0, 500, -25);
	glScalef(110, 1, 110);
	glBegin(GL_QUADS);

	glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, 0.0);
	glTexCoord2d(1.0, 0.0); glVertex3d(+5.0, -5.0, 0.0);
	glTexCoord2d(1.0, 1.0); glVertex3d(+5.0, -5.0, 5.0);
	glTexCoord2d(0.0, 1.0); glVertex3d(-5.0, -5.0, 5.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void drawChess(float sx, float sy, float sz, int nx, int ny) {
	float x, y, dx, dy;
	GLfloat mat_ambient[] = { 0.8,0.8, 0.8, 1.0 }; 
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; 
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 }; 

	dx = sx / (float)nx;
	dy = sy / (float)ny; 
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);

	glPushMatrix();
	glTranslatef(-2, 85, 20.0);
	glScalef(8, 8, 8);
	pawn.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 85-dy, 20.0);
	glScalef(8, 8, 8);
	pawn.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 85 - 2*dy, 20.0);
	glScalef(8, 8, 8);
	pawn.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 85 - 3*dy, 20.0);
	glScalef(8, 8, 8);
	pawn.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 85 - 4*dy, 20.0);
	glScalef(8, 8, 8);
	pawn.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 85 - 5*dy, 20.0);
	glScalef(8, 8, 8);
	pawn.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 85 - 6*dy, 20.0);
	glScalef(8, 8, 8);
	pawn.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 85 - 7*dy, 20.0);
	glScalef(8, 8, 8);
	pawn.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(46, 42, 20.0);
	glScalef(8, 8, 8);
	rook.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(46, 42 - 7*dy, 20.0);
	glScalef(8, 8, 8);
	rook.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(90, -8, 20.0);
	glScalef(8, 8, 8);
	knight.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(90, -8 - 5*dy, 20.0);
	glScalef(8, 8, 8);
	knight.Draw();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(130, -55, 20.0);
	glScalef(8, 8, 8);
	bishop.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(130, -55 - 3 * dy, 20.0);
	glScalef(8, 8, 8);
	bishop.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(200, 45 - 3 * dy, 20.0);
	glScalef(8, 8, 8);
	queen.Draw();
	glPopMatrix();	

	glPushMatrix();
	glTranslatef(160, 45 - 4 * dy, 20.0);
	glScalef(8, 8, 8);
	king.Draw();
	glPopMatrix();
}

void draw_chess_board(float sx, float sy, float sz, int nx, int ny)
//draw_chess_board(400.0, 400.0, 40.0, 4, 4);
// sx, sy, sz: 棋盘大小
// nx, ny: 网格数量
{
	static GLfloat mat1_color[] = { 0.8, 0.8, 0.8, 0.1 };
	static GLfloat mat2_color[] = { 0.2, 0.2, 0.2, 0.1 };
	int i, j, iflag, jflag;
	float x, y, dx, dy;
	float* pcolor;

	dx = sx / (float)nx;
	dy = sy / (float)ny;

	for (i = 0; i < nx; ++i)
	{
		iflag = i % 2;
		x = (i + 0.5) * dx - 0.5 * sx;
		for (j = 0; j < ny; ++j)
		{
			jflag = j % 2;
			y = (j + 0.5) * dy - 0.5 * sy;
			if (iflag == jflag) 
				pcolor = mat1_color;
			else pcolor = mat2_color;
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pcolor);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pcolor);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
			glPushMatrix();
			glTranslatef(x, y, 0.0);
			glScalef(dx, dy, sz);
			glutSolidCube(1.0);
			glPopMatrix();
		}
	}

}

void draw_coordinate() {
	static GLfloat mat_color[3][4] = {
		{ 1.0, 0.0, 0.0, 1.0 },
		{ 0.0, 1.0, 0.0, 1.0 },
		{ 1.0, 1.0, 0.0, 1.0 } };


	// Draw a cone
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
	glPushMatrix();
	glTranslatef(0, 0, 300);
	glutSolidCone(10, 50, 10, 5);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 0, 300);
	glutSolidCone(10, 50, 10, 5);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);

	glPushMatrix();
	glTranslatef(300, 0, 0);
	glRotatef(90, 0, 1, 0);
	glutSolidCone(10, 50, 10, 5);
	glPopMatrix();


	//z轴  红色
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[0]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
	glPushMatrix();
	glutWireCylinder(5, 300, 100, 100);
	glPopMatrix();

	//y轴  绿色
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[1]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[1]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glutWireCylinder(5, 300, 100, 100);
	glPopMatrix();

	//x轴  黄色
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glutWireCylinder(5, 300, 100, 100);
	glPopMatrix();
}


void draw_plant(float sx, float sy, float sz, int nx, int ny) {
	static GLfloat mat_color[5][4] = {
	{ 1.0, 0.0, 0.0, 1.0 },
	{ 0.0, 1.0, 0.0, 1.0 },
	{ 0.0, 0.0, 1.0, 1.0 },
	{ 1.0, 1.0, 0.0, 1.0 },
	{ 1.0, 1.0, 1.0, 1.0 }
	};
	float x, y, dx, dy, size;

	// 一个正方形格子的长宽(边长)
	dx = sx / (float)nx;
	dy = sy / (float)ny;
	if (dx < dy) size = dx;
	else size = dy;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[4]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[4]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);

	glPushMatrix();
	float x1 = (posx + 0.5) * dx - 0.5 * sx;
	float y1 = (posy + 0.5) * dy - 0.5 * sy;
	float x2 = (dstx + 0.5) * dx - 0.5 * sx;
	float y2 = (dsty + 0.5) * dy - 0.5 * sy;
	shiftx = x2 - x1;
	shifty = y2 - y1;

	if (isSelected && !isRised) {
		glTranslatef(0, 0, 110);
		isRised = true;
	}
	else if(!isSelected && isRised) {
		
		if ((int)shiftx != 0)
			posx = posx + shiftx / abs(shiftx) / dx * shift_speed;
		if ((int)shifty != 0)
			posy = posy + shifty / abs(shifty) / dy * shift_speed;

		if ((int)shiftx == 0 && (int)shifty == 0) {
			posx = dstx;
			posy = dsty;
			isRised = false;
			
		}
		else {
			glTranslatef(0, 0, 110);
		}

	}
	else if (isSelected && isRised) {
		glTranslatef(0, 0, 110);
	}



	if (game_mode) {

		//glTranslatef(shiftx-x2+x1, shifty-y2+y1, 0);
		if ((int)shiftx != 0)
			posx = posx + shiftx / abs(shiftx) / dx * shift_speed;
		if ((int)shifty != 0)
			posy = posy + shifty / abs(shifty) / dy * shift_speed;

		//if (abs(x1) >= abs(x2)-0.001 || abs(y1) >= abs(y2)-0.001) {
		if ((int)shiftx == 0 && (int)shifty == 0) {
			posx = dstx;
			posy = dsty;
			dstx = rand() % 6;
			dsty = rand() % 8;
		}

		//碰撞判断
		if (view_frame.P0.x<x1 + 40 && view_frame.P0.x>x1 - 40 && view_frame.P0.y<y1 + 40 && view_frame.P0.y>y1 - 40  && view_frame.P0.z<150) {
			posx = rand() % 6;
			posy = rand() % 8;
			dstx = rand() % 6;
			dsty = rand() % 8;
			score++;
		}
	}
	glTranslatef(x1, y1, 20);
	glScalef(20, 20, 20);
	glRotatef(90, 1, 0, 0);

	if (game_mode || isSelected || isRised) {
		if (score % 5 == 0 && score != 0) {
			shift_speed += 0.5;
		}
		glRotatef(ang, 0, 1, 0);
		ang++;
		if (ang == 360)
			ang = 1;
	}

	plant.Draw();
	glPopMatrix();
}

void draw_objects(float sx, float sy, float sz, int nx, int ny)

{
	static GLfloat mat_color[5][4] = {
		{ 1.0, 0.0, 0.0, 1.0 },
		{ 0.0, 1.0, 0.0, 1.0 },
		{ 0.0, 0.0, 1.0, 1.0 },
		{ 1.0, 1.0, 0.0, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 }
	};
	float x, y, dx, dy, size;

	// 一个正方形格子的长宽(边长)
	dx = sx / (float)nx;
	dy = sy / (float)ny;
	if (dx < dy) size = dx;
	else size = dy;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Draw a sphere
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[0]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[0]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
	x = (0 + 0.5) * dx - 0.5 * sx;
	y = (2 + 0.5) * dy - 0.5 * sy;
	glPushMatrix();
	glTranslatef(x, y, 0.5 * (sz + size));
	glutSolidSphere(0.5 * size, 10, 10);
	glPopMatrix();

	// 颜色不断变化的茶壶
	//float rand_color[] = {rand()%2,rand()%2,rand()%2};
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, rand_color);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, rand_color);
	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
	//x = (7 + 0.5) * dx - 0.5 * sx;   
	//y = (0 + 0.5) * dy - 0.5 * sy;
	//glPushMatrix();
	//glTranslatef(x, y, 0.5 * (sz + size));
	//glRotatef(90, 1, 0, 0);
	//glutSolidTeapot(20);
	//glPopMatrix();

	// Draw a cone
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[2]);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
	x = (2 + 0.5) * dx - 0.5 * sx;
	y = (6 + 0.5) * dy - 0.5 * sy;
	glPushMatrix();
	glTranslatef(x, y, 0.5 * sz);
	glutSolidCone(0.4 * size, size, 10, 5);
	glPopMatrix();

	// Draw a rectangular solid //乱跑的正方体
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[3]);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[3]);
	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
	//glMaterialf(GL_FRONT_AND_BACK, GL_EMISSION, 8.0);
	//x = (3 + 0.5) * dx - 0.5 * sx;
	//y = (3 + 0.5) * dy - 0.5 * sy;
	//
	//glPushMatrix();
	//glTranslatef(x+((int)dx)*(rand()%4-2), y+((int)dy)* (rand()%4 - 2), 0.5 * (sz + 0.9 * size));
	//glScalef(0.9 * dx, 0.9 * dy, 0.9 * size);
	////glRotatef(cube_angle, 1, 1, 1);
	//cube_angle = cube_angle + 45;
	//glutSolidCube(1);
	//glPopMatrix();

	//不好看的海马体
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[2]);
	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
	//x = (7 + 0.5) * dx - 0.5 * sx;
	//y = (6 + 0.5) * dy - 0.5 * sy;
	//double offset[] = {0,0,0};
	//glPushMatrix();
	//glTranslatef(x, y, 0.5 * (sz + 0.9 * size));
	//glutSolidSierpinskiSponge(1, offset, 30);
	//glPopMatrix();



}

// Display callback function
void display(void)
{
	static GLfloat light_pos[4] = { 200.0, 200.0, 400.0, 1.0 };

	// 清空缓冲区
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glPushMatrix(); //Save current ModelView matrix

	// Set viewing transformation matrix
	view_frame.look_at.x = view_frame.P0.x - view_frame.n.x;
	view_frame.look_at.y = view_frame.P0.y - view_frame.n.y;
	view_frame.look_at.z = view_frame.P0.z - view_frame.n.z;


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	Vector3D up;
	up = up.crossProduct(view_frame.n, view_frame.u);  // 避免移动左右斜向上的问题（但是俯仰角和旋转角的“震动仍会导致相机旋转”）

   gluLookAt(view_frame.P0.x, view_frame.P0.y, view_frame.P0.z,
   view_frame.look_at.x, view_frame.look_at.y, view_frame.look_at.z,
   up.x, up.y, up.z);
	//gluLookAt(view_frame.P0.x, view_frame.P0.y, view_frame.P0.z,
	//	view_frame.look_at.x, view_frame.look_at.y, view_frame.look_at.z,
	//	view_frame.v.x, view_frame.v.y, view_frame.v.z);

	string str;
	if (game_mode) {
		str = "score = " + std::to_string(score);
		if (score % 5 == 0 && score != 0)
			str = str + "\n speed up!";
	}
	
	else
		str = "game pause";
	const char* s = str.c_str();
	int n = strlen(s);
	//设置要在屏幕上显示字符的起始位置
	glRasterPos3i(0, 100, 300);
	//逐个显示字符串中的每个字符
	for (int i = 0; i < n; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *(s + i));



	// 实现缩放
	glScalef(view_frame.scale, view_frame.scale, view_frame.scale);

	// 光照
	if (light_mode == true)
		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);


	drawBackground();

	draw_chess_board(400.0, 400.0, 40.0, 8, 8);

	
	draw_plant(400.0, 400.0, 40.0, 8, 8);
	if (chess_mode == false) {
		;
		drawChess(400.0, 400.0, 40.0, 8, 8);
	}
	if (show_coordinate) {
		draw_coordinate();
		draw_objects(400.0, 400.0, 40.0, 8, 8);
	}
	glFlush();
	glutSwapBuffers();


}

void reshape(int w, int h)
{

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 10.0, 100000.0);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0); break;
	case 'w':
		view_frame.move_forward();
		glutPostRedisplay(); break;
	case 's':
		view_frame.move_backward();
		glutPostRedisplay(); break;
	case 'a':
		view_frame.move_left();
		glutPostRedisplay(); break;
	case 'd':
		view_frame.move_right();
		glutPostRedisplay(); break;
	case 'q':
		view_frame.rotate_left();
		glutPostRedisplay(); break;
	case 'e':
		view_frame.rotate_right();
		glutPostRedisplay(); break;
	case 'z':
		view_frame.roll_left();
		glutPostRedisplay(); break;
	case 'c':
		view_frame.roll_right();
		glutPostRedisplay(); break;
	case 'r':
		game_mode = !game_mode;
		break;
	case 't':
		if (show_coordinate == true)
			show_coordinate = false;
		else
			show_coordinate = true;
		break;
	case 'y':
		light_mode = !light_mode;
		break;
	case 'u':
		chess_mode = !chess_mode;
		break;
	}
}


void special_key(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		view_frame.turn_left();
		glutPostRedisplay(); break;
	case GLUT_KEY_RIGHT:
		view_frame.turn_right();
		glutPostRedisplay(); break;
	case GLUT_KEY_UP:
		view_frame.look_up();
		glutPostRedisplay(); break;
	case GLUT_KEY_DOWN:
		view_frame.look_down();
		glutPostRedisplay(); break;
	case GLUT_KEY_PAGE_UP:
		view_frame.move_up();
		glutPostRedisplay(); break;
	case GLUT_KEY_PAGE_DOWN:
		view_frame.move_down();
		glutPostRedisplay(); break;
	}
}


void PassiveMouseMotion(int x, int y) {

	view_frame.mouse_motion(x, y);

}

//判断所选位置是否有棋子，
bool chessSelect() {
	int dz = (40 - view_frame.P0.z) / (view_frame.look_at.z - view_frame.P0.z);
	int selectx = dz * (view_frame.look_at.x - view_frame.P0.x) + view_frame.P0.x;
	int selecty = dz * (view_frame.look_at.y - view_frame.P0.y) + view_frame.P0.y;
	printf("%d,%d\n", selectx, selecty);
	float x1 = (posx + 0.5) * 50 - 0.5 * 400;
	float y1 = (posy + 0.5) * 50 - 0.5 * 400;
	if (selectx<x1 + 25 && selectx>x1 - 25 && selecty<y1 + 25 && selecty>y1 - 25) {
		return true;
	}
	return false;
}

bool chessSet() {
	int dz = (40 - view_frame.P0.z) / (view_frame.look_at.z - view_frame.P0.z);
	int selectx = dz * (view_frame.look_at.x - view_frame.P0.x) + view_frame.P0.x;
	int selecty = dz * (view_frame.look_at.y - view_frame.P0.y) + view_frame.P0.y;
	if (selectx<200 && selectx>-200 && selecty<200 && selecty>-200) {
		dstx = int((selectx +0.5*400)/50 +0.5);
		dsty = int((selecty + 0.5 * 400) /50 + 0.5);
		return true;
	}
	return false;
}


void mouseClicked(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN)
			if (!isSelected) {
				if(chessSelect())
					isSelected = true;
				
			}
			else {
				if(chessSet())
					isSelected = false;
				
			}

	}
}

void mouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0)
	{
		view_frame.scale += 0.1;
		//printf("zoom in");
	}
	else
	{
		view_frame.scale -= 0.1;
		if (view_frame.scale <= 0.1)
			view_frame.scale = 0.1;
		//printf("zoom out");
	}
	//printf("scale:%f\n", view_frame.scale);
	//printf("*****%f   %f    %f*******\n", view_frame.v.x, view_frame.v.y, view_frame.v.z);
	//printf("x:%f   y:%f\t\t\t", (posx + 0.5) * 8 - 0.5 * 400, (posy + 0.5) * 8 - 0.5 * 400);
	//printf("p0x:%f   p0y:%f    p0z:%f\n", view_frame.P0.x, view_frame.P0.y, view_frame.P0.z);
	return;
}

void idleEvent()
{
	glutPostRedisplay(); //刷新函数
}

int main(int argc, char* argv[])
{
	srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(400, 300);
	glutCreateWindow("三维棋盘");
	init();

	glutDisplayFunc(display);
	//glutIdleFunc(display);
	glutIdleFunc(idleEvent);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special_key);
	glutPassiveMotionFunc(PassiveMouseMotion);
	glutMouseWheelFunc(mouseWheel);
	glutMouseFunc(mouseClicked);
	glutMainLoop();
	return 0;
}