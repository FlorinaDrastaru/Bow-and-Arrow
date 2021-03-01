#include "Object2D.h"

#include <math.h>
#include <Core/Engine.h>



// mesa pentru dreptunghi
Mesh* Object2D::CreateRectangle(std::string name, glm::vec3 color, bool fill)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(0, 50.f, 0), color),
		VertexFormat(glm::vec3(50.f, 50.f, 0), color),
		VertexFormat(glm::vec3(50.f, 0, 0), color),

	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		rectangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(1);
	}
	rectangle->InitFromData(vertices, indices);
	return rectangle;
}

// mesa pentru cerc/balon
Mesh* Object2D::CreateCircle(std::string nume, glm::vec3 center, glm::vec3 color, bool fill, float radius) {
	std::vector<VertexFormat> vertices;
	vertices.push_back(VertexFormat(center, color));
	for (int i = 0; i < 360; ++i) {
		vertices.push_back(VertexFormat(glm::vec3(center.x + sin(RADIANS(i)) * radius, 
										center.y + cos(RADIANS(i)) * radius, 0), color));
	}

	std::vector<unsigned short> indices;
	for (int i = 0; i < 360; ++i) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}
	indices.push_back(0);
	indices.push_back(359);
	indices.push_back(1);

	Mesh* circle = new Mesh(nume);
	circle->SetDrawMode(GL_LINE_LOOP);

	circle->InitFromData(vertices, indices);
	return circle;

}

// mesa pentru arc
Mesh* Object2D::CreateArc(std::string nume, glm::vec3 color, bool fill) {
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	int nr = 0;
	float radius = 80;
	for (int i = -90; i < 90; i++) {
		vertices.push_back(VertexFormat(glm::vec3(cos(RADIANS(i)) * radius, sin(RADIANS(i)) * radius, 0), color));
		nr++;
	}
	for (int i = 0; i < nr; i++) {
		indices.push_back(i);
	}
	Mesh* arc = new Mesh(nume);
	arc->SetDrawMode(GL_LINE_STRIP);
	arc->InitFromData(vertices, indices);
	return arc;

}

// mesa pentru triunghi
Mesh* Object2D::CreateTriangle(std::string name, glm::vec3 color, bool fill)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(10, -60, 0), color),
		VertexFormat(glm::vec3(-10, -60, 0), color),
		VertexFormat(glm::vec3(0, -50, 0), color),
	};
	std::vector<unsigned short> indices =
	{
		0, 1, 2
	};

	Mesh* triangle = new Mesh(name);
	if (!fill) {
		triangle->SetDrawMode(GL_LINE_LOOP);
	}
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(1);

	triangle->InitFromData(vertices, indices);
	return triangle;
}

// mesa pentru linia sagetii
Mesh* Object2D::CreateLineArrow(std::string name, glm::vec3 color, bool fill)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(130, 0, 0), color),
	};
	std::vector<unsigned short> indices =
	{
		0, 1
	};

	Mesh* lineArr = new Mesh(name);
	lineArr->SetDrawMode(GL_LINES);

	lineArr->InitFromData(vertices, indices);
	return lineArr;
}

// mesa pentru linia arcului
Mesh* Object2D::CreateLineArc(std::string name, glm::vec3 color, bool fill)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 80, 0), color),
		VertexFormat(glm::vec3(0, -80, 0), color),
	};
	std::vector<unsigned short> indices =
	{
		0, 1
	};

	Mesh* lineArc = new Mesh(name);
	lineArc->SetDrawMode(GL_LINES);

	lineArc->InitFromData(vertices, indices);
	return lineArc;
}

// mesa pentru varful sagetii
Mesh* Object2D::CreateArrow(std::string name, glm::vec3 color, bool fill) {
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(130.f, 5.f, 0.f), color),
		VertexFormat(glm::vec3(130.f, -5.f, 0.f), color),
		VertexFormat(glm::vec3(160.f, 0.f, 0.f), color),
	};
	std::vector<unsigned short> indices =
	{
		0, 1, 2
	};


	Mesh* arrow = new Mesh(name);
	if (!fill) {

		arrow->SetDrawMode(GL_LINE_LOOP);
	}
	arrow->InitFromData(vertices, indices);
	return arrow;
}

// mesa pentru shuriken
Mesh* Object2D::CreateMesh(std::string name, glm::vec3 color, bool fill)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(30, 0, 0), color),
		VertexFormat(glm::vec3(30, 30, 0), color),	
		VertexFormat(glm::vec3(0, 30, 0), color),
		VertexFormat(glm::vec3(-30, 30, 0), color),
		VertexFormat(glm::vec3(-30, 0, 0), color),
		VertexFormat(glm::vec3(-30, -30, 0), color),
		VertexFormat(glm::vec3(0, -30, 0), color),
		VertexFormat(glm::vec3(30, -30, 0), color),
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 2,
		0, 3, 4,
		0, 5, 6,
		0, 7, 8
	};


	Mesh* mesh = new Mesh(name);
	if (!fill) {
		mesh->SetDrawMode(GL_LINE_LOOP);
	}

	mesh->InitFromData(vertices, indices);
	return mesh;
}


// mesa pentru coada balonului
Mesh* Object2D::CreateLine1(std::string name, glm::vec3 color, bool fill)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, -60, 0), color),
		VertexFormat(glm::vec3(-10, -70, 0), color),
		VertexFormat(glm::vec3(10, -80, 0), color),
		VertexFormat(glm::vec3(0, -90, 0), color),
		VertexFormat(glm::vec3(10, -100, 0), color),
	
	};
	std::vector<unsigned short> indices =
	{
		0, 1, 2, 3, 4, 
	};

	Mesh* line1 = new Mesh(name);
	line1->SetDrawMode(GL_LINE_STRIP);
	line1->InitFromData(vertices, indices);
	return line1;
}


