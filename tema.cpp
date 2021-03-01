#include "tema.h"

#include <vector>
#include <iostream>
#include <math.h>
#include <random>
#include <chrono>
#include <thread>
#include <Core/Engine.h>
#include <ctime>
#include "Transform2D.h"
#include "Object2D.h"
#include "Arrow.h"

using namespace std;

bool goArrow = false;

bool clickedLeft = true;
glm::vec3 arrowPos;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

tema::tema()
{
}

tema::~tema()
{
}


void tema::Init()
{
	{
		cullFace = GL_BACK;
		polygonMode = GL_FILL;
		glm::ivec2 resolution = window->GetResolution();
		auto camera = GetSceneCamera();
		camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
		camera->SetPosition(glm::vec3(0, 0, 50));
		camera->SetRotation(glm::vec3(0, 0, 0));
		camera->Update();
		GetCameraInput()->SetActive(false);

		angularStep = 0;
		
		//sageata
		Mesh* arrow = Object2D::CreateArrow("arrow", glm::vec3(1, 1, 1), true);
		AddMeshToList(arrow);

		// linia verticala de la arc
		Mesh* lineArc = Object2D::CreateLineArc("lineArc", glm::vec3(1, 1, 1), true);
		AddMeshToList(lineArc);

		//balon rosu
		Mesh* redCircle = Object2D::CreateCircle("redCircle", glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), true, 50);
		AddMeshToList(redCircle);

		// balon galben
		Mesh* yellowCircle = Object2D::CreateCircle("yellowCircle", glm::vec3(0, 0, 0), glm::vec3(1, 1, 0), true, 50);
		AddMeshToList(yellowCircle);

		// arc
		Mesh* arc = Object2D::CreateArc("arc", glm::vec3(1, 1, 1), true);
		AddMeshToList(arc);

		//shuriken
		Mesh* mesh = Object2D::CreateMesh("mesh", glm::vec3(1, 0, 0), true);
		AddMeshToList(mesh);

		// reprezentari ale vietilor
		Mesh* life1 = Object2D::CreateCircle("life1", glm::vec3(100, resolution.y - 30, 0), glm::vec3(1, 0, 0), true, 15);
		AddMeshToList(life1);

		Mesh* life2 = Object2D::CreateCircle("life2", glm::vec3(135, resolution.y - 30, 0), glm::vec3(1, 0, 0), true, 15);
		AddMeshToList(life2);

		Mesh* life3 = Object2D::CreateCircle("life3", glm::vec3(170, resolution.y - 30, 0), glm::vec3(1, 0, 0), true, 15);
		AddMeshToList(life3);

		// linia de la sageata
		Mesh* lineArr = Object2D::CreateLineArrow("lineArr", glm::vec3(1, 1, 1), true);
		AddMeshToList(lineArr);

		// mesh pentru puterea de aruncare a sagetii
		Mesh* powRectangle = Object2D::CreateRectangle("powRectangle", glm::vec3(0, 1, 0), true);
		AddMeshToList(powRectangle);

		// triunghi rosu
		Mesh* redTriangle = Object2D::CreateTriangle("redTriangle", glm::vec3(1, 0, 0), true);
		AddMeshToList(redTriangle);

		// coada balonului rosu
		Mesh* redLine = Object2D::CreateLine1("redLine", glm::vec3(1, 0, 0), true);
		AddMeshToList(redLine);

		// triunghi galben
		Mesh* yellowTriangle = Object2D::CreateTriangle("yellowTriangle", glm::vec3(1, 1, 0), true);
		AddMeshToList(yellowTriangle);

		// coada balonului galben
		Mesh* yellowLine = Object2D::CreateLine1("yellowLine", glm::vec3(1, 1, 0), true);
		AddMeshToList(yellowLine);
	}
}

void tema::FrameStart()
{
	// curata bufferul de culori si adancimea
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// seteaza suprafata de ecran pe care se deseneaza
	glViewport(0, 0, resolution.x, resolution.y);
}

void tema::Update(float deltaTimeSeconds)
{	
	// jocul are loc atata timp cat avem macar o viata
	if (lifes != 0) {
		glm::ivec2 resolution = window->props.resolution;

		// desenarea sagetii, pozitionarea si rotirea
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(0, lineY);
		modelMatrix *= Transform2D::Translate(goArrX, goArrY);
		modelMatrix *= Transform2D::Rotate(angle);
		RenderMesh2D(meshes["lineArr"], shaders["VertexColor"], modelMatrix);
		RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrix);

		// desenarea arcului
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(trLineArrX, trLineArrY);
		modelMatrix *= Transform2D::Translate(0, lineY);
		modelMatrix *= Transform2D::Rotate(angle);
		RenderMesh2D(meshes["arc"], shaders["VertexColor"], modelMatrix);
		RenderMesh2D(meshes["lineArc"], shaders["VertexColor"], modelMatrix);


		// desenarea dreptunghiului pentru puterea de tragere
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(100, 100);
		modelMatrix *= Transform2D::Scale(power / 10, 1);
		RenderMesh2D(meshes["powRectangle"], shaders["VertexColor"], modelMatrix);

		arcCircleCoord arcCoord = {};
		arcCoord.radius = 80;
		arcCoord.x = trLineArrX;
		arcCoord.y = trLineArrY;


		modelMatrix = glm::mat3(1);
		scaleX = 1;
		scaleY = 1.5;
		trY += deltaTimeSeconds * 100.f;
		// desenarea baloanelor
		if (startB) {
			// baloanele rosii
			for (int i = 0; i < nrRed; i++) {
				// desenez balonul doar daca nu a fost lovit de sageata anterior
				if (rHitByArrow[i] == false) {
					modelMatrix = Transform2D::Translate(redX[i], redY[i]);
					modelMatrix *= Transform2D::Translate(0.f, trY);
					modelMatrix *= Transform2D::Scale(scaleX, scaleY);
					RenderMesh2D(meshes["redLine"], shaders["VertexColor"], modelMatrix);
					modelMatrix = Transform2D::Translate(redX[i], redY[i]);
					modelMatrix *= Transform2D::Translate(0.f, trY);
					modelMatrix *= Transform2D::Scale(scaleX, scaleY);
					RenderMesh2D(meshes["redTriangle"], shaders["VertexColor"], modelMatrix);
					modelMatrix = Transform2D::Translate(redX[i], redY[i]);
					modelMatrix *= Transform2D::Translate(0.f, trY);
					modelMatrix *= Transform2D::Scale(scaleX, scaleY);
					RenderMesh2D(meshes["redCircle"], shaders["VertexColor"], modelMatrix);

					// verific coliziunea dintre sageata si balonul rosu
					circleCoord circle = {};
					circle.radius = 60;
					circle.x = redX[i];
					circle.y = redY[i];
					int dx2 = (arrowX + goArrX) - circle.x;
					int dy2 = (arrowY + goArrY + lineY) - (circle.y + trY);
					float dist = sqrt(dx2 * dx2 + dy2 * dy2);
					
					if (dist < circle.radius && rHitByArrow[i] == false && (circle.y + circle.radius + trY) > 0) {
						rHitByArrow[i] = true;
						score++;
						cout << "score: " << score << endl;
					}
				}
			}

			// baloanele galbene
			for (int k = 0; k < nrYellow; k++) {
				// desenez balonul doar daca n-a fost lovit de sageata
				if (yHitByArrow[k] == false) {
					modelMatrix = Transform2D::Translate(yellowX[k], yellowY[k]);
					modelMatrix *= Transform2D::Translate(0.f, trY);
					modelMatrix *= Transform2D::Scale(scaleX, scaleY);
					RenderMesh2D(meshes["yellowLine"], shaders["VertexColor"], modelMatrix);
					modelMatrix = Transform2D::Translate(yellowX[k], yellowY[k]);
					modelMatrix *= Transform2D::Translate(0.f, trY);
					modelMatrix *= Transform2D::Scale(scaleX, scaleY);
					RenderMesh2D(meshes["yellowTriangle"], shaders["VertexColor"], modelMatrix);
					modelMatrix = Transform2D::Translate(yellowX[k], yellowY[k]);
					modelMatrix *= Transform2D::Translate(0.f, trY);
					modelMatrix *= Transform2D::Scale(scaleX, scaleY);
					RenderMesh2D(meshes["yellowCircle"], shaders["VertexColor"], modelMatrix);

					// verific coliziunea dintre sageata si balonul galben
					circleCoord circle = {};
					circle.radius = 60;
					circle.x = yellowX[k];
					circle.y = yellowY[k];
					int dx3 = (arrowX + goArrX) - circle.x;
					int dy3 = (arrowY + goArrY + lineY) - (circle.y + trY);
					float dist1 = sqrt(dx3 * dx3 + dy3 * dy3);
					if (dist1 < circle.radius && yHitByArrow[k] == false && (circle.y + circle.radius + trY) > 0) {
						yHitByArrow[k] = true;
						score--;
						cout << "score: " << score << endl;
					}
				}
			}
			

			bool finishB = true;
			// daca baloanele rosii nu au iesit din scena, atunci nu am terminat de desenat
			for (int i = 0; i < nrRed; i++) {
				if (redY[i] + trY < resolution.y) {
					finishB = false;
				}
			}
			// daca baloanele galbene nu au iesit din scena, atunci nu am terminat de desenat
			for (int k = 0; k < nrYellow; k++) {
				if (yellowY[k] + trY < resolution.y) {
					finishB = false;
				}
			}
			if (finishB) {
				startB = false;
				trY = 0;
			}
		}
		else { // daca nu desenez baloane, le generez pozitiile aleatoriu
			startB = true;
			nrRed = rand() % 100 + 20;
			nrYellow = rand() % 100 + 20;
			redX.resize(nrRed);
			redY.resize(nrRed);
			yellowX.resize(nrYellow);
			yellowY.resize(nrYellow);
			rHitByArrow.resize(nrRed);
			yHitByArrow.resize(nrYellow);
			for (int i = 0; i < nrRed; i++) {
				redX[i] = rand() % (resolution.x - 300) + 350;
				if (i > 0) {
					// baloanele vor veni de la un y mai mic decat precedentii lor
					// pentru a fi desenate la intervale de timp diferite
					redY[i] = redY[i - 1] - (rand() % 800 + 150);
				}
				else {
					redY[i] = - rand() % 600;
				}

			}

			for (int k = 0; k < nrYellow; k++) {
				yellowX[k] = rand() % (resolution.x - 300) + 350;
				if (k > 0) {
					yellowY[k] = yellowY[k - 1] - (rand() % 800 + 150);
				}
				else {
					yellowY[k] = - rand() % 600;
				}
			}
		}

		angularStep += deltaTimeSeconds;
		translateX -= deltaTimeSeconds * 100.f;

		modelMatrix = glm::mat3(1);
		if (startS) {
			// desenarea shurikenilor
			for (int j = 0; j < m; j++) {
				// desenez un shuriken doar daca n-a fost lovit de sageata
				if (sHitByArrow[j] == false) {
					modelMatrix = Transform2D::Translate(shurikenX[j], shurikenY[j]);
					modelMatrix *= Transform2D::Translate(translateX, 0.f);
					modelMatrix *= Transform2D::Rotate(angularStep);
					RenderMesh2D(meshes["mesh"], shaders["VertexColor"], modelMatrix);
					
					// verific coliziunea cerc la cerc dintre arc si shuriken
					shurikenCircle sCircle = {};
					sCircle.radius = 40;
					sCircle.x = shurikenX[j];
					sCircle.y = shurikenY[j];
					int dx = arcCoord.x - (sCircle.x + translateX);
					int dy = arcCoord.y + lineY - sCircle.y;
					float distance = sqrt(dx * dx + dy * dy);
					if (distance < (arcCoord.radius + sCircle.radius) && shurikenHit[j] == false && (sCircle.x + translateX) < resolution.x) {
						shurikenHit[j] = true;
						lifes--;
					}

					// verific coliziunea punct la cerc dintre sageata si shuriken
					int dx1 = (arrowX + goArrX) - (sCircle.x + translateX);
					int dy1 = (arrowY + goArrY + lineY) - sCircle.y;
					float distance1 = sqrt(dx1 * dx1 + dy1 * dy1);
					if (distance1 < sCircle.radius && (sCircle.x + translateX) < resolution.x) {
						sHitByArrow[j] = true;
						if (shurikenHit[j] == false) {
							// daca scorul se modifica, il afisez la consola
							score++;
							cout << "score: " << score << endl;
						}
						
					}
				}
				
			}
			bool finishS = true;

			// cat timp shurikenii sunt in scena, nu am terminat de desenat
			for (int j = 0; j < m; j++) {
				if (shurikenX[j] + translateX > 0) {
					finishS = false;
				}
			}
			if (finishS) {
				startS = false;
				translateX = 0;
			}
		}
		else { // generez pozitii aleatorii pentru shurikeni
			startS = true;
			m = rand() % 100 + 30;
			shurikenX.resize(m);
			shurikenY.resize(m);
			shurikenHit.resize(m);
			sHitByArrow.resize(m);
			for (int j = 0; j < m; j++) {
				shurikenY[j] = rand() % 600 + 30;
				if (j == 0) {
					shurikenX[j] = resolution.x + rand() % 500;
				}
				else {
					// fiecare shuriken va avea un x mai mare decat cei precedenti, 
					// pentru a fi afisati in scena la intervale diferite de timp
					shurikenX[j] = shurikenX[j - 1] + (rand() % 800 + 150);
				}
			}
		}

		// desenarea vietilor
		if (lifes == 3) {
			modelMatrix = glm::mat3(1);
			RenderMesh2D(meshes["life1"], shaders["VertexColor"], modelMatrix);
			modelMatrix = glm::mat3(1);
			RenderMesh2D(meshes["life2"], shaders["VertexColor"], modelMatrix);
			modelMatrix = glm::mat3(1);
			RenderMesh2D(meshes["life3"], shaders["VertexColor"], modelMatrix);
		}
		else if (lifes == 2) {
			modelMatrix = glm::mat3(1);
			RenderMesh2D(meshes["life1"], shaders["VertexColor"], modelMatrix);
			modelMatrix = glm::mat3(1);
			RenderMesh2D(meshes["life2"], shaders["VertexColor"], modelMatrix);
		}
		else if (lifes == 1) {
			modelMatrix = glm::mat3(1);
			RenderMesh2D(meshes["life1"], shaders["VertexColor"], modelMatrix);
		}
		else { // atunci cand nu mai sunt vieti, jocul se incheie
			Engine::Exit();
			std::exit(0);
		}
		
	} 

	
}

void tema::FrameEnd()
{

}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void tema::OnInputUpdate(float deltaTime, int mods)
{
	// se modifica pozitia pe verticala a arcului si a sagetii,
	// in functie de apasarea tastelor W si S
	if (window->KeyHold(GLFW_KEY_W))
	{
		lineY += deltaTime * 80.f;
	}

	if (window->KeyHold(GLFW_KEY_S))
	{
		lineY -= deltaTime * 80.f;
	}

	// in functie de apasarea pe butonul stang al mouse-ului, se modifica unghiul, 
	// pozitia sagetii si puterea
	int state = glfwGetMouseButton(window->GetGLFWWindow(), GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS && goArrow == true) {
		power = 0;
		goArrX = 30;
		goArrY = 350;
		firstGoX = 30;
		firstGoY = 350;
	}

	if (state == GLFW_PRESS) {
		power += 80 * deltaTime;
		goArrow = false;
	}
	else {
		goArrow = true;
	}
	if (goArrow) {
		goArrX += power * cos(angle);
		goArrY += power * sin(angle);
		firstGoX += power * cos(firstAngle);
		firstGoY += power * sin(firstAngle);

	}
	
	
};

void tema::OnKeyPress(int key, int mods)
{
	// add key press event
	
};

void tema::OnKeyRelease(int key, int mods)
{
	// add key release event
};


void tema::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	glm::ivec2 resolution = window->GetResolution();
	
	// unghiul in functie de care se misca sageata si arcul, dupa cursorul mouse-ului
	angle = atan((resolution.y - mouseY - trLineArrY - lineY) / (mouseX - trLineArrX));

	int state = glfwGetMouseButton(window->GetGLFWWindow(), GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS && changed == false) {
		firstAngle = angle;
		changed = true;
	}
	

	if (state == GLFW_PRESS) {
		changed = false;
	}

	//cout << "first: " << firstAngle << "new:" << angle << endl;
	
};

void tema::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void tema::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void tema::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void tema::OnWindowResize(int width, int height)
{
	// treat window resize event
}
