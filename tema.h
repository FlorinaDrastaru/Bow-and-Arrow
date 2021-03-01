#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include "Arrow.h"

class tema : public SimpleScene
{
public:
	tema();
	~tema();

	void Init() override;
	void CreateArc(std::string name, float ballRadius, glm::vec3 color, bool fill);

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
	Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);



protected:
	glm::mat3 modelMatrix, modelMatrix1;
	float translateX = 0, translateY = 0, trY;
	float lineX, lineY = 0;
	float goArrX = 30, goArrY = 350;
	float scaleX, scaleY;
	float angularStep;
	float newX, newY;
	double timePressedClick;

	float firstGoX, firstGoY;

	float trLineArrX = 30, trLineArrY = 350;
	float power = 0;
	float initialAngle;
	int timeShuriken = -1;
	struct balloon {
		float pozX;
		float pozY;
	};
	double time = 0;
	double lastTime = 0, nowTime = 0;
	struct shuriken {
		float pozX;
		float pozY;
	};

	struct arcCircleCoord {
		float x;
		float y;
		float radius;
	};

	struct shurikenCircle {
		float x;
		float y;
		float radius;
	};

	int lifes = 3;

	std::vector<balloon> balloons;
	std::vector<shuriken> shurikens = {};
	int n = 0, m = 0;

	int nrRed = 0, nrYellow = 0;
	GLfloat centreX, centreY, angle, speed;
	float firstAngle;

	std::vector<int> shurikenX = {};
	std::vector<int> shurikenY = {};
	std::vector<bool> shurikenHit = {false};
	std::vector<bool> sHitByArrow = {false};
	bool startS = false;
	bool hit = false;
	bool startB = false;

	struct circleCoord {
		float x;
		float y;
		float radius;
	};

	int score = 0;

	std::vector<int> redX = {};
	std::vector<int> yellowX = {};
	std::vector<int> redY = {};
	std::vector<int> yellowY = {};
	std::vector<bool> rHitByArrow = {false};
	std::vector<bool> yHitByArrow = {false};
	bool changed = false;

	float arrowX = 160;
	float arrowY = 0;
	GLenum cullFace;
	GLenum polygonMode;
	
};
