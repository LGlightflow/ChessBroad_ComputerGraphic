#pragma once
#define _USE_MATH_DEFINES
#include<cmath>

class Camera
{
public:
    Camera();

    // �����z��(-z)Ϊ����y�ᴹֱ����(Ťͷ),x�ḩ��
    float camera_x, camera_y, camera_z;  // �����λ������
    float lookat_x, lookat_y, lookat_z;  // �������������

    void YawCamera(float angle);  
    void PitchCamera(float angle);  
    void WalkStraight(float speed);  // ǰ���ƶ�����
    void WalkTransverse(float speed); // �����ƶ�����

    float angle;  // ÿ����ת�Ƕ�
    float speed;  // ÿ���ƶ�����
    float sight;  // ��Ұ
    float rad_yz, rad_xz;  // �����������yOzƽ�棬xOzƽ�����ɻ���
    float rotate_yz, rotate_xz;  //�����������yOzƽ�棬xOzƽ�����ɽǶ�

};

Camera::Camera()  
{

    angle = 1;
    speed = 0.3;
    sight = 100; //��Ұ �ӵ�������ľ���

    rotate_yz = 0.0f;
    rotate_xz = -90.0f;
    rad_yz = rotate_yz * M_PI / 180.0f;
    rad_xz = rotate_xz * M_PI / 180.0f;

    camera_x = 0.0f;
    camera_y = -2.0f;
    camera_z = 10.0f;

    lookat_x = camera_x + sight * cos(rad_yz) * cos(rad_xz);
    lookat_y = camera_y + sight * sin(rad_yz);
    lookat_z = camera_z + sight * cos(rad_yz) * sin(rad_xz);
}

// 
// ƫ���� Χ��y����ת
void Camera::YawCamera(float angle)  
{
    rotate_xz = (int)(rotate_xz + angle) % 360;
    rad_xz = rotate_xz * M_PI / 180.0f;

    lookat_x = camera_x + sight * cos(rad_yz) * cos(rad_xz);
    lookat_y = camera_y + sight * sin(rad_yz);
    lookat_z = camera_z + sight * cos(rad_yz) * sin(rad_xz);
}

//������ Χ��X����ת
void Camera::PitchCamera(float angle)
{
    rotate_yz = (int)(rotate_yz + angle) % 360;
    rad_yz = rotate_yz * M_PI / 180.0f;

    lookat_x = camera_x + sight * cos(rad_yz) * cos(rad_xz);
    lookat_y = camera_y + sight * sin(rad_yz);
    lookat_z = camera_z + sight * cos(rad_yz) * sin(rad_xz);
}

//ǰ���ƶ�
void Camera::WalkStraight(float speed)  
{
    camera_x += speed * cos(rad_yz) * cos(rad_xz);
    camera_y += speed * sin(rad_yz);
    camera_z += speed * cos(rad_yz) * sin(rad_xz);

    lookat_x = camera_x + sight * cos(rad_yz) * cos(rad_xz);
    lookat_y = camera_y + sight * sin(rad_yz);
    lookat_z = camera_z + sight * cos(rad_yz) * sin(rad_xz);
}


// �����ƶ�
void Camera::WalkTransverse(float speed)  
{
    camera_x += speed * cos(rad_yz) * sin(rad_xz);
    camera_z -= speed * cos(rad_yz) * cos(rad_xz);

    lookat_x = camera_x + sight * cos(rad_yz) * cos(rad_xz);
    lookat_y = camera_y + sight * sin(rad_yz);
    lookat_z = camera_z + sight * cos(rad_yz) * sin(rad_xz);
}