#pragma once

#ifndef __OBJECT_2D__
#define __OBJECT_2D__

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

#define RIGHT_ANGLE_TRIANGLE 0
#define EQUILATERAL_TRIANGLE 1

namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateTriangle(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false, int type = RIGHT_ANGLE_TRIANGLE);
    Mesh* CreateCircle(const std::string& name, glm::vec3 leftBottomCorner, float radius, int numberOfTriangles, glm::vec3 color, bool fill = false);
    Mesh* CreateHeart(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateBullet(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

    void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices, 
        std::unordered_map<std::string, Mesh*>& meshes);
    
}

#endif
