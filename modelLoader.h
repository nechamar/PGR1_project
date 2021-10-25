//
// Created by nechamar on 30.3.18.
//
//----------------------------------------------------------------------------------------
/**
 * \file       parameters.h
 * \author     Marek Nechansky
 * \date       10.3.2018
 * \brief      Contains all constant variables.
*/
//----------------------------------------------------------------------------------------

#ifndef PGR_MODELLOADER_H
#define PGR_MODELLOADER_H



#include <pgr.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "meshGeometry.h"
#include <streambuf>

namespace nechamar {

    using namespace std;

    struct vector3f {
        float x;
        float y;
        float z;
    };

    struct vector2f {
        float x;
        float y;
    };

    struct vector3i {
        unsigned int pos;
        unsigned int norm;
        unsigned int tex;
    };

    struct vertex {
        vector3f pos;
        vector3f norm;
        vector2f tex;
    };


    struct triangle {
        vector3i x;
        vector3i y;
        vector3i z;
    };

    bool compareVertices(vertex v1, vector<float> v2, int startIndex) {
        return v1.pos.x == v2[startIndex + 0] &&
               v1.pos.y == v2[startIndex + 1] &&
               v1.pos.z == v2[startIndex + 2] &&
               v1.norm.x == v2[startIndex + 3] &&
               v1.norm.y == v2[startIndex + 4] &&
               v1.norm.z == v2[startIndex + 5] &&
               v1.tex.x == v2[startIndex + 6] &&
               v1.tex.y == v2[startIndex + 7];
    }

    void setMaterials(material **mat) {
        (*mat) = new material;
        (*mat)->ambient = glm::vec3(0, 0, 0);
        (*mat)->diffuse = glm::vec3(0, 0, 0);
        (*mat)->specular = glm::vec3(0, 0, 0);
        (*mat)->shininess = 1;
        (*mat)->useDiffuseTexture = false;
        (*mat)->useEmissionTexture = false;
        (*mat)->diffuseTexture= 0;
        (*mat)->emissionTexture= 0;
        (*mat)->ambientTexture =0;
        (*mat)->useAmbientTexture = false;

    }

    void loadMaterials(string name, std::vector<material> *materials, std::vector<string> *names) {

        std::ifstream myFile(name);
        if (!myFile.good()) {
            return;
        }

        std::string line;
        char *parts;
        std::string tmp;
        material *mat = new material;
        float a, b, c;

        while (getline(myFile, line)) {
            parts = strtok((char *) line.data(), " \n");
            if (parts == nullptr) continue;
            tmp = parts;

            if (tmp == "newmtl") {
                materials->push_back(*mat);
                mat = new material;
                tmp = strtok(NULL, "\n");
                setMaterials(&mat);
                names->push_back(tmp);
            } else if (tmp == "Ns") {
                tmp = strtok(NULL, "\n");
                mat->shininess = stof(tmp);
            } else if (tmp == "Ka") {
                tmp = strtok(NULL, " ");
                a = stof(tmp);
                tmp = strtok(NULL, " ");
                b = stof(tmp);
                tmp = strtok(NULL, "\n");
                c = stof(tmp);
                mat->ambient = glm::vec3(a, b, c);
            } else if (tmp == "Kd") {
                tmp = strtok(NULL, " ");
                a = stof(tmp);
                tmp = strtok(NULL, " ");
                b = stof(tmp);
                tmp = strtok(NULL, "\n");
                c = stof(tmp);
                mat->diffuse = glm::vec3(a, b, c);
            } else if (tmp == "Ks") {
                tmp = strtok(NULL, " ");
                a = stof(tmp);
                tmp = strtok(NULL, " ");
                b = stof(tmp);
                tmp = strtok(NULL, "\n");
                c = stof(tmp);
                mat->specular = glm::vec3(a, b, c);
            } else if (tmp == "Ke") {
                tmp = strtok(NULL, " ");
                a = stof(tmp);
                tmp = strtok(NULL, " ");
                b = stof(tmp);
                tmp = strtok(NULL, "\n");
                c = stof(tmp);
                mat->emission = glm::vec3(a, b, c);
            } else if (tmp == "map_Kd"){
                tmp = strtok(NULL, "\r\n");
                mat->useDiffuseTexture = true;
                string s("./res/");
                s.append(tmp);
                mat->diffuseTexture = pgr::createTexture(s);
            }else if(tmp == "map_Ke"){
                tmp = strtok(NULL, "\r\n");
                mat->useEmissionTexture = true;
                string s("./res/");
                s.append(tmp);
                mat->emissionTexture = pgr::createTexture(s);
            }else if(tmp == "map_Ka"){
                tmp = strtok(NULL, "\r\n");
                mat->useAmbientTexture = true;
                string s("./res/");
                s.append(tmp);
                mat->ambientTexture = pgr::createTexture(s);
            }
        }
        materials->push_back(*mat);
        materials->erase(materials->begin());

    }

    void loadbuffers(modelGeometry **mog, vector<float> vertices, vector<unsigned int> indices, int * numberOfIndicesForMaterial, int numberOfMaterials,
                     int numberOfDataInVertex, std::vector<material> materials) {

        (*mog)->parts = new meshGeometry[numberOfMaterials];

        for (int i = 0; i < numberOfMaterials; ++i) {
            (*mog)->parts[i].numTriangles = numberOfIndicesForMaterial[i];
            (*mog)->parts[i].meshMaterial = materials[i];

        }

        (*mog)->numberOfParts = numberOfMaterials;

        glBindVertexArray(0);

        glGenVertexArrays(1, &((*mog)->vertexArrayObject));
        glBindVertexArray((*mog)->vertexArrayObject);




        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

        glGenBuffers(1, &((*mog)->vertexBufferObject));
        glBindBuffer(GL_ARRAY_BUFFER, (*mog)->vertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);


        glGenBuffers(1, &((*mog)->elementBufferObject));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*mog)->elementBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(),
                     GL_STATIC_DRAW);

        glEnableVertexAttribArray(handler.position);
        glVertexAttribPointer(handler.position, 3, GL_FLOAT, GL_FALSE, numberOfDataInVertex * sizeof(GL_FLOAT), (void *) (0));

        glEnableVertexAttribArray(handler.normal);
        glVertexAttribPointer(handler.normal, 3, GL_FLOAT, GL_FALSE, numberOfDataInVertex * sizeof(GL_FLOAT), (void *) (3 *
                sizeof(float)));

        glEnableVertexAttribArray(handler.texCords);
        glVertexAttribPointer(handler.texCords, 2, GL_FLOAT, GL_FALSE, numberOfDataInVertex * sizeof(GL_FLOAT), (void *) (6 *
                                                                                                                        sizeof(float)));
        glBindVertexArray(0);
    }

    void createBtobj(vector<float> vertices, vector<unsigned int> indices, char *name) {
        string s(name);
        s.append(".btobj");
        ofstream mf(s);
        for (int i = 0; i < vertices.size(); ++i) {
            mf << vertices[i] << "\n";
        }
        mf << "indices\n";
        for (int i = 0; i < indices.size(); ++i) {
            mf << indices[i] << "\n";
        }
    }

    int getIndexOfMaterial(string name, vector<string> names) {
        for (int i = 0; i < names.size(); ++i) {
            if (name == names[i]) {
                return i;
            }
        }
        return -1;
    }

    inline void addVertex(vertex vec, std::vector<float> * vertices) {
        vertices->push_back(vec.pos.x);
        vertices->push_back(vec.pos.y);
        vertices->push_back(vec.pos.z);

        vertices->push_back(vec.norm.x);
        vertices->push_back(vec.norm.y);
        vertices->push_back(vec.norm.z);

        vertices->push_back(vec.tex.x);
        vertices->push_back(vec.tex.y);
    }

    void loadFromOBJ(char *name, modelGeometry **mg) {

        string string1(name);

        string mtl(name);
        mtl.append(".mtl");

        std::vector<material> materials;

        std::vector<string> namesOfMaterials;

        loadMaterials(mtl, &materials, &namesOfMaterials);

        vector<float> vertices;
        vector<unsigned int> indices;
        int * indexCountForMaterial = new int[namesOfMaterials.size()];
        for (int j = 0; j < namesOfMaterials.size(); ++j) {
            indexCountForMaterial[j]=0;
        }

        string line;
        string1.append(".btobj");

        int currentMaterialIndex = 0;

        ifstream mf(string1);
        if (mf.good()) {
            currentMaterialIndex = 0;
            while (getline(mf, line)) {
                if (line == "indices") {
                    break;
                }
                vertices.push_back(stof(line));
            }
            while (getline(mf, line)) {
                if(line == "next"){
                    currentMaterialIndex++;
                    continue;
                }
                indices.push_back(stoul(line));
                indexCountForMaterial[currentMaterialIndex]++;

            }
            loadbuffers(mg, vertices, indices, indexCountForMaterial, namesOfMaterials.size(), 8, materials);
            return;
        }


        vector<vector3f> poss;
        vector<vector3f> norms;
        vector<vector2f> texCords;
        vector<triangle> triangles;
        //vector<triangle> * triangles = new vector<triangle>[namesOfMaterials.size()];

        string sd(name);
        sd.append(".obj");

        ifstream myFile(sd);
        string tmp;

        while (getline(myFile, line)) {
            stringstream s(line);
            getline(s, tmp, ' ');

            if (tmp == "v") {

                vector3f vec;
                getline(s, tmp, ' ');
                while (tmp.empty()) {
                    getline(s, tmp, ' ');
                }
                vec.x = stof(tmp);
                getline(s, tmp, ' ');
                vec.y = stof(tmp);
                getline(s, tmp);
                vec.z = stof(tmp);
                poss.push_back(vec);
            } else if (tmp == "vn") {
                vector3f vec;
                getline(s, tmp, ' ');
                vec.x = stof(tmp);
                getline(s, tmp, ' ');
                vec.y = stof(tmp);
                getline(s, tmp);
                vec.z = stof(tmp);
                norms.push_back(vec);
            } else if (tmp == "vt") {
                vector2f vec;
                getline(s, tmp, ' ');
                vec.x = stof(tmp);
                getline(s, tmp);
                vec.y = stof(tmp);
                texCords.push_back(vec);
            } else if (tmp == "f") {

                triangle tr;
                getline(s, tmp);
                bool norms = true;
                if (tmp.find("//") != string::npos) {norms = false; cout << "to snad ne" << endl;}
                char *str = (char *) tmp.data();    // prekopirovat radsi data nemenit const
                char *parts = strtok(str, " /\n");
                tr.x.pos = stoul(parts);
                if (norms) {
                    parts = strtok(NULL, " / \n");
                    tr.x.tex = stoul(parts);
                } else {
                    tr.x.tex = UINT_MAX;
                }
                parts = strtok(NULL, " / \n");
                tr.x.norm = stoul(parts);

                parts = strtok(NULL, " / \n");
                tr.y.pos = stoul(parts);
                if (norms) {
                    parts = strtok(NULL, " / \n");
                    tr.y.tex = stoul(parts);
                } else {
                    tr.y.tex = UINT_MAX;
                }
                parts = strtok(NULL, " / \n");
                tr.y.norm = stoul(parts);

                parts = strtok(NULL, " / \n");
                tr.z.pos = stoul(parts);
                if (norms) {
                    parts = strtok(NULL, " / \n");
                    tr.z.tex = stoul(parts);
                } else {
                    tr.z.tex = UINT_MAX;
                }
                parts = strtok(NULL, " / \n");
                tr.z.norm = stoul(parts);

                triangles.push_back(tr);
                try {
                    triangle tr2;
                    tr2.x = tr.x;
                    tr2.y = tr.z;
                    parts = strtok(NULL, " / \n");
                    tr2.z.pos = stoul(parts);
                    if (norms) {
                        parts = strtok(NULL, " / \n");
                        tr2.z.tex = stoul(parts);
                    } else {
                        tr2.z.tex = UINT_MAX;
                    }
                    parts = strtok(NULL, " / \n");
                    tr2.z.norm = stoul(parts);

                    triangles.push_back(tr2);
                } catch (exception e) {
                }
            } else if (tmp == "usemtl") {
                getline(s, tmp);
                currentMaterialIndex = getIndexOfMaterial(tmp, namesOfMaterials);
            }
        }

        cout << triangles.size() << endl;


        for (int index = 0; index < triangles.size(); index++) {
            bool found;
            vertex vec;
            found = false;
            vec.pos = poss.at(triangles.at(index).x.pos - 1);
            vec.norm = norms.at(triangles.at(index).x.norm - 1);
            if (triangles.at(index).x.tex == UINT_MAX) {
                vec.tex = vector2f{0, 0};
            } else {
                vec.tex = texCords.at(triangles.at(index).x.tex - 1);
            }


            for (int i = 0; i < vertices.size(); i += 8) {
                if (compareVertices(vec, vertices, i)) {
                    indices.push_back(i / 8);
                    found = true;
                }
            }
            if (!found) {
                addVertex(vec, &vertices);

                indices.push_back(vertices.size() / 8 - 1);
            }

            found = false;
            vec.pos = poss.at(triangles.at(index).y.pos - 1);
            vec.norm = norms.at(triangles.at(index).y.norm - 1);
            if (triangles.at(index).y.tex == UINT_MAX) {
                vec.tex = vector2f{0, 0};
            } else {
                vec.tex = texCords.at(triangles.at(index).y.tex - 1);
            }

            for (int i = 0; i < vertices.size(); i += 8) {
                if (compareVertices(vec, vertices, i)) {
                    indices.push_back(i / 8);
                    found = true;
                }
            }
            if (!found) {
                addVertex(vec, &vertices);

                indices.push_back(vertices.size() / 8 - 1);
            }

            found = false;
            vec.pos = poss.at(triangles.at(index).z.pos - 1);
            vec.norm = norms.at(triangles.at(index).z.norm - 1);
            if (triangles.at(index).z.tex == UINT_MAX) {
                vec.tex = vector2f{0, 0};
            } else {
                vec.tex = texCords.at(triangles.at(index).z.tex - 1);
            }

            for (int i = 0; i < vertices.size(); i += 8) {
                if (compareVertices(vec, vertices, i)) {
                    indices.push_back(i / 8);
                    found = true;
                }
            }
            if (!found) {
                addVertex(vec, &vertices);

                indices.push_back(vertices.size() / 8 - 1);

            }
        }

        cout << vertices.size() << ", " << indices.size() << endl;

        createBtobj(vertices, indices, name);

        loadbuffers(mg, vertices, indices, indexCountForMaterial, namesOfMaterials.size(), 8, materials);

    }

}

#endif //PGR_MODELLOADER_H
