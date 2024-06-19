#pragma once
#define _USE_MATH_DEFINES
#include<cmath>

class Camera
{
public:
    Camera();

    // 摄像机z轴(-z)为朝向，y轴垂直向上(扭头),x轴俯仰
    float camera_x, camera_y, camera_z;  // 摄像机位置坐标
    float lookat_x, lookat_y, lookat_z;  // 摄像机朝向坐标

    void YawCamera(float angle);  
    void PitchCamera(float angle);  
    void WalkStraight(float speed);  // 前后移动方法
    void WalkTransverse(float speed); // 左右移动方法

    float angle;  // 每次旋转角度
    float speed;  // 每次移动距离
    float sight;  // 视野
    float rad_yz, rad_xz;  // 摄像机朝向与yOz平面，xOz平面所成弧度
    float rotate_yz, rotate_xz;  //摄像机朝向与yOz平面，xOz平面所成角度

};

Camera::Camera()  
{

    angle = 1;
    speed = 0.3;
    sight = 100; //视野 视点离相机的距离

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
// 偏航角 围绕y轴旋转
void Camera::YawCamera(float angle)  
{
    rotate_xz = (int)(rotate_xz + angle) % 360;
    rad_xz = rotate_xz * M_PI / 180.0f;

    lookat_x = camera_x + sight * cos(rad_yz) * cos(rad_xz);
    lookat_y = camera_y + sight * sin(rad_yz);
    lookat_z = camera_z + sight * cos(rad_yz) * sin(rad_xz);
}

//俯仰角 围绕X轴旋转
void Camera::PitchCamera(float angle)
{
    rotate_yz = (int)(rotate_yz + angle) % 360;
    rad_yz = rotate_yz * M_PI / 180.0f;

    lookat_x = camera_x + sight * cos(rad_yz) * cos(rad_xz);
    lookat_y = camera_y + sight * sin(rad_yz);
    lookat_z = camera_z + sight * cos(rad_yz) * sin(rad_xz);
}

//前后移动
void Camera::WalkStraight(float speed)  
{
    camera_x += speed * cos(rad_yz) * cos(rad_xz);
    camera_y += speed * sin(rad_yz);
    camera_z += speed * cos(rad_yz) * sin(rad_xz);

    lookat_x = camera_x + sight * cos(rad_yz) * cos(rad_xz);
    lookat_y = camera_y + sight * sin(rad_yz);
    lookat_z = camera_z + sight * cos(rad_yz) * sin(rad_xz);
}


// 左右移动
void Camera::WalkTransverse(float speed)  
{
    camera_x += speed * cos(rad_yz) * sin(rad_xz);
    camera_z -= speed * cos(rad_yz) * cos(rad_xz);

    lookat_x = camera_x + sight * cos(rad_yz) * cos(rad_xz);
    lookat_y = camera_y + sight * sin(rad_yz);
    lookat_z = camera_z + sight * cos(rad_yz) * sin(rad_xz);
}