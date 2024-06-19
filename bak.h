
#include <vector>
#include<fstream>
#include <gl/glut.h>
#include<stdlib.h>
#include <iostream>
#include<istream>
#include<sstream>
#include"ViewFrame.h"  // 导入Vector3D，暂时不用
using namespace std;

class ObjLoader
{
public:

    ObjLoader(string filename);//读取函数
    void Draw_Tri();//绘制函数
    void Draw();
    void ObjLoader_Tri(string filename);
private:
    vector<vector<GLfloat>> v;//存放顶点(x,y,z)坐标
    vector<vector<GLint>> f;//存放面的三个顶点索引
};


std::vector<GLfloat> vertices;  // 顶点坐标
std::vector<GLfloat> normals;   // 法线
std::vector<GLfloat> texCoords; // 纹理坐标
std::vector<std::vector<int>> faceIndices; // 顶点、纹理坐标和法线索引

void ObjLoader::ObjLoader_Tri(string filename) {
    //std::ifstream file(filename);
//if (!file.is_open()) {
//    // 处理文件加载失败
//}

//std::string line;
//while (std::getline(file, line)) {
//    std::istringstream iss(line);
//    std::string token;
//    iss >> token;

//    if (token == "v") {
//        // 顶点坐标行
//        GLfloat x, y, z;
//        iss >> x >> y >> z;
//        vertices.push_back(x);
//        vertices.push_back(y);
//        vertices.push_back(z);
//    }
//    else if (token == "vn") {
//        // 法线行
//        GLfloat nx, ny, nz;
//        iss >> nx >> ny >> nz;
//        normals.push_back(nx);
//        normals.push_back(ny);
//        normals.push_back(nz);
//    }
//    else if (token == "vt") {
//        // 纹理坐标行
//        GLfloat u, v;
//        iss >> u >> v;
//        texCoords.push_back(u);
//        texCoords.push_back(v);
//    }
//    else if (token == "f") {
//        // 面行，提取顶点、纹理坐标和法线索引
//        std::vector<int> indices;
//        while (iss >> token) {
//            int vIndex, tIndex, nIndex;
//            sscanf(token.c_str(), "%d/%d/%d", &vIndex, &tIndex, &nIndex);
//            indices.push_back(vIndex - 1);  // 减1是因为OBJ索引从1开始
//            indices.push_back(tIndex - 1);
//            indices.push_back(nIndex - 1);
//        }
//        faceIndices.push_back(indices);
//    }
//}

//file.close();
}


bool isIndexValid(int index, int arraySize) {
    return (index >= 0 && index < arraySize);
}

ObjLoader::ObjLoader(string filename)
{

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "v") {
            GLfloat x, y, z;
            iss >> x >> y >> z;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
        else if (token == "vn") {
            GLfloat nx, ny, nz;
            iss >> nx >> ny >> nz;
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);
        }
        else if (token == "vt") {
            GLfloat u, v;
            iss >> u >> v;
            texCoords.push_back(u);
            texCoords.push_back(v);
        }
        else if (token == "f") {
            std::vector<int> indices;
            while (iss >> token) {
                int vIndex, tIndex, nIndex;
                if (sscanf(token.c_str(), "%d/%d/%d", &vIndex, &tIndex, &nIndex) == 3) {
                    if (isIndexValid(vIndex - 1, vertices.size() / 3) &&
                        isIndexValid(tIndex - 1, texCoords.size() / 2) &&
                        isIndexValid(nIndex - 1, normals.size() / 3)) {
                        indices.push_back(vIndex - 1);
                        indices.push_back(tIndex - 1);
                        indices.push_back(nIndex - 1);
                    }
                    else {
                        std::cerr << "无效索引：" << token << std::endl;
                    }
                }
            }
            faceIndices.push_back(indices);
        }
    }

    file.close();

}

void ObjLoader::Draw_Tri()
{
    // 渲染模型
    /*glBegin(GL_TRIANGLES);
    for (int i = 0; i < vertices.size(); i += 3) {
        if (i + 2 < vertices.size() && i + 2 < normals.size()) {
            glNormal3f(normals[i], normals[i + 1], normals[i + 2]);
            glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
        }
    }
    glEnd();*/

    //glutSwapBuffers();

    glPushMatrix();
    // 渲染模型
    glBegin(GL_TRIANGLES); //GL_QUADS

    for (const auto& face : faceIndices) {
        for (int i = 0; i < 3; i++) {
            int vIndex = face[i * 3];
            int tIndex = face[i * 3 + 1];
            int nIndex = face[i * 3 + 2];

            if (vIndex >= 0 && vIndex < vertices.size() / 3 &&
                tIndex >= 0 && tIndex < texCoords.size() / 2 &&
                nIndex >= 0 && nIndex < normals.size() / 3) {
                glNormal3f(normals[nIndex * 3], normals[nIndex * 3 + 1], normals[nIndex * 3 + 2]);
                glTexCoord2f(texCoords[tIndex * 2], texCoords[tIndex * 2 + 1]);
                glVertex3f(vertices[vIndex * 3], vertices[vIndex * 3 + 1], vertices[vIndex * 3 + 2]);
            }
        }
    }
    glEnd();
    glPopMatrix();

}

void ObjLoader::Draw() {
    glBegin(GL_QUADS);

    for (const auto& face : faceIndices) {
        if (face.size() == 12) {
            for (int i = 0; i < 4; i++) {
                int vIndex = face[i * 3];
                int tIndex = face[i * 3 + 1];
                int nIndex = face[i * 3 + 2];

                glVertex3f(vertices[vIndex * 3], vertices[vIndex * 3 + 1], vertices[vIndex * 3 + 2]);
                glNormal3f(normals[nIndex * 3], normals[nIndex * 3 + 1], normals[nIndex * 3 + 2]);
                glTexCoord2f(texCoords[tIndex * 2], texCoords[tIndex * 2 + 1]);
            }
        }
        else {
            std::cerr << "面行索引数不足" << std::endl;
        }
    }

    glEnd();

}