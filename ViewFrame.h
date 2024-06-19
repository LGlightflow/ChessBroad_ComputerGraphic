#pragma once
#include<GL/freeglut.h>
// ���㼯
class Vector3D
{
public:
	float x, y, z;

	// Constructors
	Vector3D(void) { x = 0.0; y = 0.0; z = 0.0; }
	Vector3D(float x0, float y0, float z0)
	{
		x = x0; y = y0; z = z0;
	}

	Vector3D crossProduct(Vector3D a, Vector3D b) {
		Vector3D ret;
		ret.x = a.y * b.z - a.z * b.y;
		ret.y = a.z * b.x - a.x * b.z;
		ret.z = a.x * b.y - a.y * b.x;
		return ret;
	}
};

// View reference frame class
class ViewFrame
{
public:
	float step; // step size
	float turn_a; // ��ת��
	float pitch_a; // ������
	float roll_a; // ��ת��
	float scale = 1.0f;
	bool WarpingMouse = false;

	Vector3D P0; // �ӵ�

	// �������
	Vector3D u; 
	Vector3D v; 
	Vector3D n;
	Vector3D look_at;

	/*
	N - ��������Ŀ�곯��ķ�����������һЩ3d������Ҳ������look at�����������������Z���Ӧ��
	V - �����������ָ������ֱվ��ʱ��ͷ������յķ��������дһ��ģ��ķɻ���ô�ɻ���ת���������ǡ��ָ���ء����������Y���Ӧ��
	U - �������ָ��������Ҳ࣬��X�����Ӧip
		gluLookAt(view_frame.P0.x, view_frame.P0.y, view_frame.P0.z,
		look_at.x, look_at.y, look_at.z,
		view_frame.v.x, view_frame.v.y, view_frame.v.z);

		look_at = view_frame.P0 - view_frame.n;
	// �����ӵ�,�������λ�úͷ���ǰ��������(P0��xyz)��ʾ�ӵ�����λ�ã�look_at��xyz��ʾĿ���λ�ã�view_frame��ʾ������Ϸ���
	// ����������Ϸ�����(0,0,1)
	*/

	void move_up(void)
	{
		P0.x = P0.x + step * v.x;
		P0.y = P0.y + step * v.y;
		P0.z = P0.z + step * v.z;
	}

	void move_down(void)
	{
		P0.x = P0.x - step * v.x;
		P0.y = P0.y - step * v.y;
		P0.z = P0.z - step * v.z;
	}

	void move_left(void)
	{
		P0.x = P0.x - step * u.x;
		P0.y = P0.y - step * u.y;
		P0.z = P0.z - step * u.z;
	}

	void move_right(void)
	{
		P0.x = P0.x + step * u.x;
		P0.y = P0.y + step * u.y;
		P0.z = P0.z + step * u.z;
	}

	void move_forward(void)
	{
		P0.x = P0.x - step * n.x;
		P0.y = P0.y - step * n.y;
		P0.z = P0.z - step * n.z;

	}

	void move_backward(void)
	{
		P0.x = P0.x + step * n.x;
		P0.y = P0.y + step * n.y;
		P0.z = P0.z + step * n.z;
	}

	void turn_left(void)
	{
		u.x = u.x * cos(turn_a) - n.x * sin(turn_a);
		u.y = u.y * cos(turn_a) - n.y * sin(turn_a);
		u.z = u.z * cos(turn_a) - n.z * sin(turn_a);
		n.x = u.x * sin(turn_a) + n.x * cos(turn_a);
		n.y = u.y * sin(turn_a) + n.y * cos(turn_a);
		n.z = u.z * sin(turn_a) + n.z * cos(turn_a);
	}

	void turn_right(void)
	{
		u.x = u.x * cos(turn_a) + n.x * sin(turn_a);
		u.y = u.y * cos(turn_a) + n.y * sin(turn_a);
		u.z = u.z * cos(turn_a) + n.z * sin(turn_a);
		n.x = -u.x * sin(turn_a) + n.x * cos(turn_a);
		n.y = -u.y * sin(turn_a) + n.y * cos(turn_a);
		n.z = -u.z * sin(turn_a) + n.z * cos(turn_a);

	}

	void look_up(void)
	{
		v.x = v.x * cos(pitch_a) + n.x * sin(pitch_a);
		v.y = v.y * cos(pitch_a) + n.y * sin(pitch_a);
		v.z = v.z * cos(pitch_a) + n.z * sin(pitch_a);
		n.x = -v.x * sin(pitch_a) + n.x * cos(pitch_a);
		n.y = -v.y * sin(pitch_a) + n.y * cos(pitch_a);
		n.z = -v.z * sin(pitch_a) + n.z * cos(pitch_a);
	}

	void look_down(void)
	{
		v.x = v.x * cos(pitch_a) - n.x * sin(pitch_a);
		v.y = v.y * cos(pitch_a) - n.y * sin(pitch_a);
		v.z = v.z * cos(pitch_a) - n.z * sin(pitch_a);
		n.x = v.x * sin(pitch_a) + n.x * cos(pitch_a);
		n.y = v.y * sin(pitch_a) + n.y * cos(pitch_a);
		n.z = v.z * sin(pitch_a) + n.z * cos(pitch_a);
	}

	void rotate_right(void)
	{
		u.x = u.x * cos(roll_a) + n.x * sin(roll_a);
		u.y = u.y * cos(roll_a) + n.y * sin(roll_a);
		u.z = u.z * cos(roll_a) + n.z * sin(roll_a);
		n.x = -u.x * sin(roll_a) + n.x * cos(roll_a);
		n.y = -u.y * sin(roll_a) + n.y * cos(roll_a);
		n.z = -u.z * sin(roll_a) + n.z * cos(roll_a);

	}

	void rotate_left(void)
	{
		u.x = u.x * cos(roll_a) - n.x * sin(roll_a);
		u.y = u.y * cos(roll_a) - n.y * sin(roll_a);
		u.z = u.z * cos(roll_a) - n.z * sin(roll_a);
		n.x = u.x * sin(roll_a) + n.x * cos(roll_a);
		n.y = u.y * sin(roll_a) + n.y * cos(roll_a);
		n.z = u.z * sin(roll_a) + n.z * cos(roll_a);
	}

	void roll_left(void)
	{
		u.x = u.x * cos(roll_a) + v.x * sin(roll_a);
		u.y = u.y * cos(roll_a) + v.y * sin(roll_a);
		u.z = u.z * cos(roll_a) + v.z * sin(roll_a);
		v.x = -u.x * sin(roll_a) + v.x * cos(roll_a);
		v.y = -u.y * sin(roll_a) + v.y * cos(roll_a);
		v.z = -u.z * sin(roll_a) + v.z * cos(roll_a);
	}

	void roll_right(void)
	{
		u.x = u.x * cos(roll_a) - v.x * sin(roll_a);
		u.y = u.y * cos(roll_a) - v.y * sin(roll_a);
		u.z = u.z * cos(roll_a) - v.z * sin(roll_a);
		v.x = u.x * sin(roll_a) + v.x * cos(roll_a);
		v.y = u.y * sin(roll_a) + v.y * cos(roll_a);
		v.z = u.z * sin(roll_a) + v.z * cos(roll_a);
	}

	void mouse_motion(int x, int y) {
		int CenterX = glutGet(GLUT_WINDOW_WIDTH) / 2;
		int CenterY = glutGet(GLUT_WINDOW_HEIGHT) / 2;


		float dx = x - CenterX;
		float dy = CenterY - y;
		if (!WarpingMouse) {

			WarpingMouse = true;



			//printf("%f    %f        %f\n", P0.x, P0.y, P0.z);

			glutWarpPointer(CenterX, CenterY);
			if (dx < 0) {
				u.x = u.x * cos(turn_a * dx / (-CenterX)) - n.x * sin(turn_a * dx / (-CenterX));
				u.y = u.y * cos(turn_a * dx / (-CenterX)) - n.y * sin(turn_a * dx / (-CenterX));
				u.z = u.z * cos(turn_a * dx / (-CenterX)) - n.z * sin(turn_a * dx / (-CenterX));
				n.x = u.x * sin(turn_a * dx / (-CenterX)) + n.x * cos(turn_a * dx / (-CenterX));
				n.y = u.y * sin(turn_a * dx / (-CenterX)) + n.y * cos(turn_a * dx / (-CenterX));
				n.z = u.z * sin(turn_a * dx / (-CenterX)) + n.z * cos(turn_a * dx / (-CenterX));
			}
			else if (dx > 0) {
				u.x = u.x * cos(turn_a * dx / CenterX) + n.x * sin(turn_a * dx / CenterX);
				u.y = u.y * cos(turn_a * dx / CenterX) + n.y * sin(turn_a * dx / CenterX);
				u.z = u.z * cos(turn_a * dx / CenterX) + n.z * sin(turn_a * dx / CenterX);
				n.x = -u.x * sin(turn_a * dx / CenterX) + n.x * cos(turn_a * dx / CenterX);
				n.y = -u.y * sin(turn_a * dx / CenterX) + n.y * cos(turn_a * dx / CenterX);
				n.z = -u.z * sin(turn_a * dx / CenterX) + n.z * cos(turn_a * dx / CenterX);
			}

			if (dy < 0) {
				v.x = v.x * cos(pitch_a * dy / (-CenterY)) - n.x * sin(pitch_a * dy / (-CenterY));
				v.y = v.y * cos(pitch_a * dy / (-CenterY)) - n.y * sin(pitch_a * dy / (-CenterY));
				v.z = v.z * cos(pitch_a * dy / (-CenterY)) - n.z * sin(pitch_a * dy / (-CenterY));
				n.x = v.x * sin(pitch_a * dy / (-CenterY)) + n.x * cos(pitch_a * dy / (-CenterY));
				n.y = v.y * sin(pitch_a * dy / (-CenterY)) + n.y * cos(pitch_a * dy / (-CenterY));
				n.z = v.z * sin(pitch_a * dy / (-CenterY)) + n.z * cos(pitch_a * dy / (-CenterY));

			}
			else if (dy > 0) {
				v.x = v.x * cos(pitch_a * dy / CenterY) + n.x * sin(pitch_a * dy / CenterY);
				v.y = v.y * cos(pitch_a * dy / CenterY) + n.y * sin(pitch_a * dy / CenterY);
				v.z = v.z * cos(pitch_a * dy / CenterY) + n.z * sin(pitch_a * dy / CenterY);
				n.x = -v.x * sin(pitch_a * dy / CenterY) + n.x * cos(pitch_a * dy / CenterY);
				n.y = -v.y * sin(pitch_a * dy / CenterY) + n.y * cos(pitch_a * dy / CenterY);
				n.z = -v.z * sin(pitch_a * dy / CenterY) + n.z * cos(pitch_a * dy / CenterY);
			}
		}
		else {
			WarpingMouse = false;
		}
	}


};
