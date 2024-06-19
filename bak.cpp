#include <stdlib.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/freeglut.h>
#include<math.h>
#include"Camera.h"
#include<iostream>

int window_width = 640;
int window_height = 480;
Camera camera;
bool warping_mouse = false;
float scale = 1;

void setProjection()
{
    if (0 == window_height)
    {
        window_height = 1;
    }
    double ratio = 1.0 * window_width / window_height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();//恢复初始坐标系
    gluPerspective(45, ratio, 0.01, 100);
    glViewport(0, 0, window_width, window_height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /*
    第一组eyex, eyey,eyez 相机在世界坐标的位置
    第二组centerx,centery,centerz 相机镜头对准的物体在世界坐标的位置
    第三组upx,upy,upz 相机向上的方向在世界坐标中的方向
    */
    gluLookAt(camera.camera_x, camera.camera_y, camera.camera_z, camera.lookat_x, camera.lookat_y, camera.lookat_z, 0.0f, 1.0f, 0.0f);
}

void setLight()
{
    GLfloat ambient[] = { 1.0,1.0,1.0,1.0 };
    GLfloat diffuse[] = { 1.0,1.0,1.0,1.0 };
    GLfloat position[] = { 10.0,10.0,0.0,1.0 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);//模糊
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);//漫反射
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHTING); // Enable lighting
    glEnable(GL_LIGHT0); // Enable light source #0


}

void init(void)
{

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH); // Set shading model
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST); // Enable depth buffer test
    glEnable(GL_NORMALIZE); // 根据函数glNormal的设置条件，启用法向量
    glEnable(GL_CULL_FACE); // 启用隐藏图形材料的面 如物体的内部的面
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    setProjection();
    setLight();
}

void draw()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glutSolidTeapot(1);
    ;
    //static GLfloat mat1_color[] = { 0.8, 0.8, 0.8, 1.0 };
    //static GLfloat mat2_color[] = { 0.2, 0.2, 0.2, 1.0 };
    //int i, j, iflag, jflag;
    //float x, y, dx, dy;
    //float* pcolor;

    //dx = 5 / (float)4;
    //dy = 5 / (float)4;

    //for (i = 0; i < 4; ++i)
    //{
    //    iflag = i % 2;
    //    x = (i + 0.5) * dx - 0.5 * 5;
    //    for (j = 0; j < 4; ++j)
    //    {
    //        jflag = j % 2;
    //        y = (j + 0.5) * dy - 0.5 * 5;
    //        if (iflag == jflag) pcolor = mat1_color;
    //        else pcolor = mat2_color;
    //        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pcolor);
    //        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pcolor);
    //        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
    //        glPushMatrix();
    //        glTranslatef(x, y, 0.0);
    //        glScalef(dx, dy, 1);
    //        glutSolidCube(1.0);
    //        glPopMatrix();
    //    }
    //}
    glFlush();
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPushMatrix();
    init();
    glScalef(scale, scale, scale);
    setProjection();
    draw();
    glPopMatrix();
    glutSwapBuffers();

}




void reshape(int w, int h)
{


    // Set viewport as the entire program window
    glViewport(0, 0, w, h);

    // Set symmetric perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / (float)h, 10.0, 100000.0);


    // Reset modelview transformation matrix to identity
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SpecialKeys(int key, int x, int y)
{


    if (key == GLUT_KEY_UP)
        camera.PitchCamera(camera.angle);

    if (key == GLUT_KEY_DOWN)
        camera.PitchCamera(-camera.angle);

    if (key == GLUT_KEY_LEFT)
        camera.YawCamera(-camera.angle);

    if (key == GLUT_KEY_RIGHT)
        camera.YawCamera(camera.angle);

    glutPostRedisplay();
}

void KeyboardKeys(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);

    if (key == 'w')
        camera.WalkStraight(camera.speed);

    if (key == 's')
        camera.WalkStraight(-camera.speed);

    if (key == 'a')
        camera.WalkTransverse(camera.speed);

    if (key == 'd')
        camera.WalkTransverse(-camera.speed);

    glutPostRedisplay();
}

void PassiveMouseMotion(int x, int y) {

    int center_x = glutGet(GLUT_WINDOW_WIDTH) / 2;
    int center_y = glutGet(GLUT_WINDOW_HEIGHT) / 2;
    glutWarpPointer(center_x, center_y);
    float dx = x - center_x;
    float dy = center_y - y;
    //printf("%f  %f\n", dx, dy);
    printf("%f,%f,%f\n", camera.camera_x, camera.camera_y, camera.camera_z);
    if (!warping_mouse) {

        warping_mouse = true;
        if (dx < 0) {

            camera.YawCamera(-camera.angle);
        }
        else if (dx > 0) {

            camera.YawCamera(camera.angle);
        }

        if (dy < 0) {
            camera.PitchCamera(-camera.angle);

        }
        else if (dy > 0) {
            camera.PitchCamera(camera.angle);
        }
    }
    else {
        warping_mouse = false;

    }
    glutPostRedisplay();

}

void mouseWheel(int button, int dir, int x, int y) {
    if (dir > 0)
    {
        scale += 0.1;
        printf("zoom in");
    }
    else
    {
        scale -= 0.1;
        if (scale <= 0.1)
            scale = 0.1;
        printf("zoom out");
    }
    printf("%f \n", scale);
}

void main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("Teapot");
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(KeyboardKeys);
    glutPassiveMotionFunc(PassiveMouseMotion);
    glutMouseWheelFunc(mouseWheel);
    glutMainLoop();
}

//#include<cstdio>
//#include<cstdlib>
//#include<cmath>
//#include"ViewFrame.h"
//#include"ObjLoader.h"
//#include<gl\freeglut.h>
//
//using namespace std;
//
//#include <math.h>
//
//#define PI 3.14159265
//
//
//float window_height = 480;
//float window_width = 640;
//
//// 立方体动画
//int posx;
//int posy;
//GLfloat cube_angle = 45;
//
//// 游戏模式
//int score = 0; // 游戏得分
//bool game_mode = false;
//
//// 坐标系显示
//bool show_coordinate = true;
//
//// 手电筒模式
//bool light_mode = true;
//
//ViewFrame view_frame;
//
//ObjLoader plant = ObjLoader("./obj/indoor plant_02.obj");
////ObjLoader pawn = ObjLoader("./obj/pawn.obj");
////ObjLoader rook = ObjLoader("./obj/rook.obj");
////ObjLoader knight = ObjLoader("./obj/knight.obj");
////ObjLoader bishop = ObjLoader("./obj/bishop.obj");
////ObjLoader queen = ObjLoader("./obj/queen.obj");
////ObjLoader king = ObjLoader("./obj/king.obj");
//
//
//GLuint bgSkyTextureID, bgGrassTextureID;
//
//GLuint loadBackgroundTexture(const char* texturePath, GLuint bgTextureID);
//
//
//
//// 初始化
//void init(void)
//{
//	// 模型初始位置
//	posx = 6;
//	posy = 4;
//
//	static GLfloat light_ambient[] = { 0.01, 0.01, 0.01, 1.0 };
//	static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
//	static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
//
//	glClearColor(0.0, 0.0, 0.0, 0.0);
//	glShadeModel(GL_SMOOTH); // 着色，使用顶点响应颜色进行着色
//
//	// Set light source properties for light source #0
//	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
//	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
//
//	glEnable(GL_LIGHTING); // Enable lighting
//	glEnable(GL_LIGHT0); // Enable light source #0
//	glEnable(GL_DEPTH_TEST); // Enable depth buffer test
//	glEnable(GL_NORMALIZE); // Enable auto normalization
//	glEnable(GL_CULL_FACE); // Enable face culling
//
//	// 抗锯齿
//	glEnable(GL_LINE_SMOOTH);
//	glEnable(GL_BLEND);
//
//
//
//	// Set initial properties for view reference frame
//	view_frame.P0 = Vector3D(500.0, 0.0, 150.0);
//	view_frame.u = Vector3D(0.0, 1.0, 0.0);
//	view_frame.v = Vector3D(0.0, 0.0, 1.0);
//	view_frame.n = Vector3D(1.0, 0.0, 0.0);
//	view_frame.step = 10;
//	view_frame.turn_a = PI / 18;
//	view_frame.pitch_a = PI / 18;
//	view_frame.roll_a = PI / 18;
//	plant.textureID = 1;
//	plant.loadTextureImage("./tex/plant.bmp");
//	//pawn.loadTextureImage("./tex/pawn.bmp");
//	//rook.loadTextureImage("./tex/rook.bmp");
//	//knight.loadTextureImage("./tex/knight.bmp")
//	bgSkyTextureID = loadBackgroundTexture("./tex/sky.bmp", bgSkyTextureID);
//	bgGrassTextureID = loadBackgroundTexture("./tex/grass.bmp", bgGrassTextureID);
//}
//
//
//GLuint loadBackgroundTexture(const char* texturePath, GLuint bgTextureID) {
//
//	std::ifstream file(texturePath, std::ios::binary);
//	if (!file.is_open()) {
//		std::cerr << "无法打开BMP文件" << std::endl;
//		return 0;
//	}
//
//	// BMP文件头部
//	char header[54];
//	file.read(header, 54);
//
//	// 从文件头中获取图像的宽度和高度
//	int width = *(int*)&header[18];
//	int height = *(int*)&header[22];
//	int imageSize = *(int*)&header[34];
//
//	if (imageSize == 0) {
//		imageSize = width * height * 3;
//	}
//
//	unsigned char* textureData = new unsigned char[imageSize];
//	file.read(reinterpret_cast<char*>(textureData), imageSize);
//	file.close();
//
//
//	// 创建纹理
//	glGenTextures(1, &bgTextureID);
//	glBindTexture(GL_TEXTURE_2D, bgTextureID);
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, textureData);
//	free(textureData);
//	//glBindTexture(GL_TEXTURE_2D, 0);
//	return bgTextureID;
//}
//
//
//void drawBackground() {
//	// 设置背景纹理
//	static GLfloat mat_color[] = { 1.0, 1.0, 1.0, 0.1 };
//
//
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color);
//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
//	//地面
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, 3);
//	glPushMatrix();
//	glScalef(110, 110, 1);
//	glTranslatef(0, 0, -25);
//
//	glBegin(GL_QUADS);
//	glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, 0.0);
//	glTexCoord2d(1.0, 0.0); glVertex3d(+5.0, -5.0, 0.0);
//	glTexCoord2d(1.0, 1.0); glVertex3d(+5.0, +5.0, 0.0);
//	glTexCoord2d(0.0, 1.0); glVertex3d(-5.0, +5.0, 0.0);
//	glEnd();
//	glPopMatrix();
//
//	//顶部
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, 2);
//	glPushMatrix();
//	glScalef(110, 110, 1);
//	glTranslatef(0, 0, 475);
//	glRotatef(180, 1, 1, 0);
//	glBegin(GL_QUADS);
//	glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, 5.0);
//	glTexCoord2d(1.0, 0.0); glVertex3d(+5.0, -5.0, 5.0);
//	glTexCoord2d(1.0, 1.0); glVertex3d(+5.0, +5.0, 5.0);
//	glTexCoord2d(0.0, 1.0); glVertex3d(-5.0, +5.0, 5.0);
//	glEnd();
//	glPopMatrix();
//
//
//	//背部
//	glPushMatrix();
//	glTranslatef(-500, 0, -25);
//	glScalef(1, 110, 110);
//	glBegin(GL_QUADS);
//
//	glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, 0.0);
//	glTexCoord2d(1.0, 0.0); glVertex3d(-5.0, +5.0, 0.0);
//	glTexCoord2d(1.0, 1.0); glVertex3d(-5.0, +5.0, 5.0);
//	glTexCoord2d(0.0, 1.0); glVertex3d(-5.0, -5.0, 5.0);
//	glEnd();
//	glPopMatrix();
//
//
//	//前面
//	glPushMatrix();
//	glTranslatef(500, 0, -25);
//	glScalef(1, 110, 110);
//	glRotatef(180, 0, 0, 1);
//	glBegin(GL_QUADS);
//
//	glTexCoord2d(0.0, 0.0); glVertex3d(+5.0, -5.0, 0.0);
//	glTexCoord2d(1.0, 0.0); glVertex3d(+5.0, +5.0, 0.0);
//	glTexCoord2d(1.0, 1.0); glVertex3d(+5.0, +5.0, 5.0);
//	glTexCoord2d(0.0, 1.0); glVertex3d(+5.0, -5.0, 5.0);
//	glEnd();
//	glPopMatrix();
//
//	//右面
//	glPushMatrix();
//	glTranslatef(0, 500, -25);
//	glScalef(110, 1, 110);
//	glBegin(GL_QUADS);
//
//	glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, +5.0, 0.0);
//	glTexCoord2d(1.0, 0.0); glVertex3d(+5.0, +5.0, 0.0);
//	glTexCoord2d(1.0, 1.0); glVertex3d(+5.0, +5.0, 5.0);
//	glTexCoord2d(0.0, 1.0); glVertex3d(-5.0, +5.0, 5.0);
//	glEnd();
//
//	glPopMatrix();
//
//
//	//左面
//	glPushMatrix();
//	glRotatef(180, 0, 0, 1);
//	glTranslatef(0, 500, -25);
//	glScalef(110, 1, 110);
//	glBegin(GL_QUADS);
//
//	glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -5.0, 0.0);
//	glTexCoord2d(1.0, 0.0); glVertex3d(+5.0, -5.0, 0.0);
//	glTexCoord2d(1.0, 1.0); glVertex3d(+5.0, -5.0, 5.0);
//	glTexCoord2d(0.0, 1.0); glVertex3d(-5.0, -5.0, 5.0);
//	glEnd();
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glDisable(GL_TEXTURE_2D);
//	glPopMatrix();
//}
//
//void drawChess(float sx, float sy, float sz, int nx, int ny) {
//	float x, y, dx, dy;
//
//
//	dx = sx / (float)nx;
//	dy = sy / (float)ny;
//
//	x = (0 + 0.5) * dx - 0.5 * sx;
//	y = (2 + 0.5) * dy - 0.5 * sy;
//}
//
//void draw_chess_board(float sx, float sy, float sz, int nx, int ny)
////draw_chess_board(400.0, 400.0, 40.0, 4, 4);
//// sx, sy, sz: size of the chess board
//// nx, ny: Number of chess grids in x and y direction
//{
//	static GLfloat mat1_color[] = { 0.8, 0.8, 0.8, 0.1 };
//	static GLfloat mat2_color[] = { 0.2, 0.2, 0.2, 0.1 };
//	int i, j, iflag, jflag;
//	float x, y, dx, dy;
//	float* pcolor;
//
//	dx = sx / (float)nx;
//	dy = sy / (float)ny;
//
//	for (i = 0; i < nx; ++i)
//	{
//		iflag = i % 2;
//		x = (i + 0.5) * dx - 0.5 * sx;
//		for (j = 0; j < ny; ++j)
//		{
//			jflag = j % 2;
//			y = (j + 0.5) * dy - 0.5 * sy;
//			if (iflag == jflag) pcolor = mat1_color;
//			else pcolor = mat2_color;
//			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pcolor);
//			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pcolor);
//			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
//			glPushMatrix();
//			glTranslatef(x, y, 0.0);
//			glScalef(dx, dy, sz);
//			glutSolidCube(1.0);
//			glPopMatrix();
//		}
//	}
//
//}
//
//void draw_coordinate() {
//	static GLfloat mat_color[3][4] = {
//		{ 1.0, 0.0, 0.0, 1.0 },
//		{ 0.0, 1.0, 0.0, 1.0 },
//		{ 1.0, 1.0, 0.0, 1.0 } };
//
//
//	// Draw a cone
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[0]);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[2]);
//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
//	glPushMatrix();
//	glTranslatef(0, 0, 300);
//	glutSolidCone(10, 50, 10, 5);
//	glPopMatrix();
//
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[1]);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[2]);
//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
//	glPushMatrix();
//	glRotatef(-90, 1, 0, 0);
//	glTranslatef(0, 0, 300);
//	glutSolidCone(10, 50, 10, 5);
//	glPopMatrix();
//
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[2]);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[2]);
//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
//
//	glPushMatrix();
//	glTranslatef(300, 0, 0);
//	glRotatef(90, 0, 1, 0);
//	glutSolidCone(10, 50, 10, 5);
//	glPopMatrix();
//
//
//	//z轴  红色
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[0]);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[0]);
//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
//	glPushMatrix();
//	glutWireCylinder(5, 300, 100, 100);
//	glPopMatrix();
//
//	//y轴  绿色
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[1]);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[1]);
//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
//	glPushMatrix();
//	glRotatef(-90, 1, 0, 0);
//	glutWireCylinder(5, 300, 100, 100);
//	glPopMatrix();
//
//	//x轴  黄色
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[2]);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[2]);
//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
//	glPushMatrix();
//	glRotatef(90, 0, 1, 0);
//	glutWireCylinder(5, 300, 100, 100);
//	glPopMatrix();
//
//}
//
//void draw_objects(float sx, float sy, float sz, int nx, int ny)
//// sx, sy, sz: size of the chess board
//// nx, ny: Number of chess grids in x and y direction
//{
//	static GLfloat mat_color[4][4] = {
//		{ 1.0, 0.0, 0.0, 1.0 },
//		{ 0.0, 1.0, 0.0, 1.0 },
//		{ 0.0, 0.0, 1.0, 1.0 },
//		{ 1.0, 1.0, 0.0, 1.0 } };
//	float x, y, dx, dy, size;
//
//	// 一个正方形格子的长宽(边长)
//	dx = sx / (float)nx;
//	dy = sy / (float)ny;
//	if (dx < dy) size = dx;
//	else size = dy;
//
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//	// Draw a sphere
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[0]);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[0]);
//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
//	x = (0 + 0.5) * dx - 0.5 * sx;
//	y = (2 + 0.5) * dy - 0.5 * sy;
//	glPushMatrix();
//	glTranslatef(x, y, 0.5 * (sz + size));
//	glutSolidSphere(0.5 * size, 10, 10);
//	glPopMatrix();
//
//	// 颜色不断变化的茶壶
//	//float rand_color[] = {rand()%2,rand()%2,rand()%2};
//	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, rand_color);
//	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, rand_color);
//	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
//	//x = (7 + 0.5) * dx - 0.5 * sx;   
//	//y = (0 + 0.5) * dy - 0.5 * sy;
//	//glPushMatrix();
//	//glTranslatef(x, y, 0.5 * (sz + size));
//	//glRotatef(90, 1, 0, 0);
//	//glutSolidTeapot(20);
//	//glPopMatrix();
//
//	// Draw a cone
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[2]);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[2]);
//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
//	x = (2 + 0.5) * dx - 0.5 * sx;
//	y = (6 + 0.5) * dy - 0.5 * sy;
//	glPushMatrix();
//	glTranslatef(x, y, 0.5 * sz);
//	glutSolidCone(0.4 * size, size, 10, 5);
//	glPopMatrix();
//
//	// Draw a rectangular solid //乱跑的正方体
//	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color[3]);
//	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[3]);
//	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
//	//glMaterialf(GL_FRONT_AND_BACK, GL_EMISSION, 8.0);
//	//x = (3 + 0.5) * dx - 0.5 * sx;
//	//y = (3 + 0.5) * dy - 0.5 * sy;
//	//
//	//glPushMatrix();
//	//glTranslatef(x+((int)dx)*(rand()%4-2), y+((int)dy)* (rand()%4 - 2), 0.5 * (sz + 0.9 * size));
//	//glScalef(0.9 * dx, 0.9 * dy, 0.9 * size);
//	////glRotatef(cube_angle, 1, 1, 1);
//	//cube_angle = cube_angle + 45;
//	//glutSolidCube(1);
//	//glPopMatrix();
//
//	//不好看的海马体
//	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_color[2]);
//	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
//	//x = (7 + 0.5) * dx - 0.5 * sx;
//	//y = (6 + 0.5) * dy - 0.5 * sy;
//	//double offset[] = {0,0,0};
//	//glPushMatrix();
//	//glTranslatef(x, y, 0.5 * (sz + 0.9 * size));
//	//glutSolidSierpinskiSponge(1, offset, 30);
//	//glPopMatrix();
//
//	x = (posx + 0.5) * dx - 0.5 * sx;
//	y = (posy + 0.5) * dy - 0.5 * sy;
//
//	if (view_frame.P0.x<x + 40 && view_frame.P0.x>x - 40 && view_frame.P0.y<y + 40 && view_frame.P0.y>y - 40 && game_mode) {
//		posx = rand() % 8;
//		posy = rand() % 8;
//		score++;
//	}
//
//	glPushMatrix();
//	glTranslatef(x, y, 20);
//	glScalef(20, 20, 20);
//	glRotatef(90, 1, 0, 0);
//	plant.Draw();
//	glPopMatrix();
//
//}
//
//// Display callback function
//void display(void)
//{
//	static GLfloat light_pos[4] = { 200.0, 200.0, 400.0, 1.0 };
//
//	// 清空缓冲区
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	//glPushMatrix(); //Save current ModelView matrix
//
//	// Set viewing transformation matrix
//	view_frame.look_at.x = view_frame.P0.x - view_frame.n.x;
//	view_frame.look_at.y = view_frame.P0.y - view_frame.n.y;
//	view_frame.look_at.z = view_frame.P0.z - view_frame.n.z;
//
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//
//	//Vector3D up;
//	//up = up.crossProduct(view_frame.n, view_frame.u);  // 避免移动左右斜向上的问题（但是俯仰角和旋转角的“震动仍会导致相机旋转”）
//
//   //gluLookAt(view_frame.P0.x, view_frame.P0.y, view_frame.P0.z,
//   //	view_frame.look_at.x, view_frame.look_at.y, view_frame.look_at.z,
//   //	up.x, up.y, up.z);
//	gluLookAt(view_frame.P0.x, view_frame.P0.y, view_frame.P0.z,
//		view_frame.look_at.x, view_frame.look_at.y, view_frame.look_at.z,
//		view_frame.v.x, view_frame.v.y, view_frame.v.z);
//
//	string str;
//	if (game_mode)
//		str = "score = " + std::to_string(score);
//	else
//		str = "game pause";
//	const char* s = str.c_str();
//	int n = strlen(s);
//	//设置要在屏幕上显示字符的起始位置
//	glRasterPos3i(0, 100, 300);
//	//逐个显示字符串中的每个字符
//	for (int i = 0; i < n; i++)
//		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *(s + i));
//
//
//
//	// 实现缩放
//	glScalef(view_frame.scale, view_frame.scale, view_frame.scale);
//
//	// 光照
//	if (light_mode == true)
//		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
//
//
//	drawBackground();
//
//	draw_chess_board(400.0, 400.0, 40.0, 8, 8);
//
//	draw_objects(400.0, 400.0, 40.0, 8, 8);
//
//	drawChess(400.0, 400.0, 40.0, 8, 8);
//	if (show_coordinate)
//		draw_coordinate();
//
//
//
//	glutSwapBuffers();
//
//
//}
//
//// Reshape callback function
//void reshape(int w, int h)
//{
//
//	glViewport(0, 0, w, h);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(60.0, (float)w / (float)h, 10.0, 100000.0);
//
//	glMatrixMode(GL_MODELVIEW);
//
//	glLoadIdentity();
//}
//
//
//// Keyboard callback function
//void keyboard(unsigned char key, int x, int y)
//{
//	switch (key)
//	{
//	case 27:
//		exit(0); break;
//	case 'w':
//		view_frame.move_forward();
//		glutPostRedisplay(); break;
//	case 's':
//		view_frame.move_backward();
//		glutPostRedisplay(); break;
//	case 'a':
//		view_frame.move_left();
//		glutPostRedisplay(); break;
//	case 'd':
//		view_frame.move_right();
//		glutPostRedisplay(); break;
//	case 'q':
//		view_frame.rotate_left();
//		glutPostRedisplay(); break;
//	case 'e':
//		view_frame.rotate_right();
//		glutPostRedisplay(); break;
//	case 'z':
//		view_frame.roll_left();
//		glutPostRedisplay(); break;
//	case 'c':
//		view_frame.roll_right();
//		glutPostRedisplay(); break;
//	case 'r':
//		game_mode = !game_mode;
//		break;
//	case 't':
//		if (show_coordinate == true)
//			show_coordinate = false;
//		else
//			show_coordinate = true;
//	case 'y':
//		light_mode = !light_mode;
//	}
//}
//
//// Special key callback function
//void special_key(int key, int x, int y)
//{
//	switch (key)
//	{
//	case GLUT_KEY_LEFT:
//		view_frame.turn_left();
//		glutPostRedisplay(); break;
//	case GLUT_KEY_RIGHT://turn right
//		view_frame.turn_right();
//		glutPostRedisplay(); break;
//	case GLUT_KEY_UP://look up
//		view_frame.look_up();
//		glutPostRedisplay(); break;
//	case GLUT_KEY_DOWN://look down
//		view_frame.look_down();
//		glutPostRedisplay(); break;
//	case GLUT_KEY_PAGE_UP://move up
//		view_frame.move_up();
//		glutPostRedisplay(); break;
//	case GLUT_KEY_PAGE_DOWN://move down
//		view_frame.move_down();
//		glutPostRedisplay(); break;
//	}
//}
//
//
//void PassiveMouseMotion(int x, int y) {
//
//	view_frame.mouse_motion(x, y);
//
//}
//
//void mouseWheel(int button, int dir, int x, int y)
//{
//	if (dir > 0)
//	{
//		view_frame.scale += 0.1;
//		//printf("zoom in");
//	}
//	else
//	{
//		view_frame.scale -= 0.1;
//		if (view_frame.scale <= 0.1)
//			view_frame.scale = 0.1;
//		//printf("zoom out");
//	}
//	//printf("scale:%f\n", view_frame.scale);
//	//printf("*****%f   %f    %f*******\n", view_frame.v.x, view_frame.v.y, view_frame.v.z);
//	printf("x:%f   y:%f\t\t\t", (posx + 0.5) * 8 - 0.5 * 400, (posy + 0.5) * 8 - 0.5 * 400);
//	printf("p0x:%f   p0y:%f    p0z:%f\n", view_frame.P0.x, view_frame.P0.y, view_frame.P0.z);
//	return;
//}
//
//
//int main(int argc, char* argv[])
//{
//	srand(time(0));
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//	glutInitWindowSize(window_width, window_height);
//	glutInitWindowPosition(250, 250);
//	glutCreateWindow("3D Space");
//	init();
//
//	glutDisplayFunc(display);
//	glutIdleFunc(display);
//	glutReshapeFunc(reshape);
//	glutKeyboardFunc(keyboard);
//	glutSpecialFunc(special_key);
//	glutPassiveMotionFunc(PassiveMouseMotion);
//	glutMouseWheelFunc(mouseWheel);
//	glutMainLoop();
//	return 0;
//}