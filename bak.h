
#include <vector>
#include<fstream>
#include <gl/glut.h>
#include<stdlib.h>
#include <iostream>
#include<istream>
#include<sstream>
#include"ViewFrame.h"  // ����Vector3D����ʱ����
using namespace std;

class ObjLoader
{
public:

    ObjLoader(string filename);//��ȡ����
    void Draw_Tri();//���ƺ���
    void Draw();
    void ObjLoader_Tri(string filename);
private:
    vector<vector<GLfloat>> v;//��Ŷ���(x,y,z)����
    vector<vector<GLint>> f;//������������������
};


std::vector<GLfloat> vertices;  // ��������
std::vector<GLfloat> normals;   // ����
std::vector<GLfloat> texCoords; // ��������
std::vector<std::vector<int>> faceIndices; // ���㡢��������ͷ�������

void ObjLoader::ObjLoader_Tri(string filename) {
    //std::ifstream file(filename);
//if (!file.is_open()) {
//    // �����ļ�����ʧ��
//}

//std::string line;
//while (std::getline(file, line)) {
//    std::istringstream iss(line);
//    std::string token;
//    iss >> token;

//    if (token == "v") {
//        // ����������
//        GLfloat x, y, z;
//        iss >> x >> y >> z;
//        vertices.push_back(x);
//        vertices.push_back(y);
//        vertices.push_back(z);
//    }
//    else if (token == "vn") {
//        // ������
//        GLfloat nx, ny, nz;
//        iss >> nx >> ny >> nz;
//        normals.push_back(nx);
//        normals.push_back(ny);
//        normals.push_back(nz);
//    }
//    else if (token == "vt") {
//        // ����������
//        GLfloat u, v;
//        iss >> u >> v;
//        texCoords.push_back(u);
//        texCoords.push_back(v);
//    }
//    else if (token == "f") {
//        // ���У���ȡ���㡢��������ͷ�������
//        std::vector<int> indices;
//        while (iss >> token) {
//            int vIndex, tIndex, nIndex;
//            sscanf(token.c_str(), "%d/%d/%d", &vIndex, &tIndex, &nIndex);
//            indices.push_back(vIndex - 1);  // ��1����ΪOBJ������1��ʼ
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
        std::cerr << "�޷����ļ�" << std::endl;
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
                        std::cerr << "��Ч������" << token << std::endl;
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
    // ��Ⱦģ��
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
    // ��Ⱦģ��
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
            std::cerr << "��������������" << std::endl;
        }
    }

    glEnd();

}