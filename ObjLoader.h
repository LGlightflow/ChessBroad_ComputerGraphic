
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

	ObjLoader(string filename);
    void Draw();
    void loadTextureImage(const char* texturePath);
    GLuint textureID;
private:
	vector<vector<GLfloat>> v;//存放顶点(x,y,z)坐标
	vector<vector<GLint>> f;//存放面的三个顶点索引
    std::vector<GLfloat> vertices;  // 顶点坐标
    std::vector<GLfloat> normals;   // 法线
    std::vector<GLfloat> texCoords; // 纹理坐标
    std::vector<std::vector<int>> faceIndices; // 顶点、纹理坐标和法线索引
};






// 读取BMP纹理图像数据
void ObjLoader::loadTextureImage(const char* texturePath) {
    // 打开BMP文件
    std::ifstream file(texturePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "无法打开BMP文件" << std::endl;
        return;
    }

    // 读取BMP文件头
    char header[54];
    file.read(header, 54);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int imageSize = *(int*)&header[34];

    if (imageSize == 0) {
        imageSize = width * height * 3;
    }

    // 分配存储图像数据的内存
    unsigned char* textureData = new unsigned char[imageSize];

    // 读取图像数据
    file.read(reinterpret_cast<char*>(textureData), imageSize);
    file.close();

    // 创建OpenGL纹理并传递图像数据
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, textureData);

    // 释放图像数据
    delete[] textureData;
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


void ObjLoader::Draw() {

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

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
    //glDisable(GL_TEXTURE_2D);
    //glDeleteTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_2D, 0);
}