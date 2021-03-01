#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

	Mesh* CreateRectangle(std::string name, glm::vec3 color, bool fill = false);

	Mesh* CreateCircle(std::string nume, glm::vec3 center, glm::vec3 color, bool fill, float radius);

	Mesh* CreateTriangle(std::string name, glm::vec3 color, bool fill);

	Mesh* CreateArc(std::string nume, glm::vec3 color, bool fill);

	Mesh* CreateLine1(std::string name, glm::vec3 color, bool fill);

	Mesh* CreateMesh(std::string name, glm::vec3 color, bool fill);

	Mesh* CreateArrow(std::string name, glm::vec3 color, bool fill);

	Mesh* CreateLineArrow(std::string name, glm::vec3 color, bool fill);

	Mesh* CreateLineArc(std::string name, glm::vec3 color, bool fill);
}

