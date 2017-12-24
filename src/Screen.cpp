#include "Screen.h"
#include "GravityGame.h"
#include "absl\strings\str_cat.h"
#include <algorithm>
namespace GameLib
{

int Screen::DEFAULT_WIDTH;
int Screen::DEFAULT_HEIGHT;
glm::mat4 Screen::projection = glm::mat4{1.0f};

Screen::Screen(const int w, const int h, std::string title)
{
	DEFAULT_WIDTH = glutGet(GLUT_SCREEN_WIDTH);
	DEFAULT_HEIGHT = glutGet(GLUT_SCREEN_HEIGHT);
	static const std::string lightString1 = "lightPos[";
	static const std::string lightString2 = "]";
	for (int i = 0; i < 20; ++i) {
		lightPositionStrings[i] = absl::StrCat(lightString1, i, lightString2);
	}
}

Screen::~Screen()
{
}

void Screen::initializeShaderContext(const Shape& s) {
	program->useProgram();
	glBufferData(GL_ARRAY_BUFFER, s.getVertices().size() * sizeof(Vector3d), s.getVertices().data(), GL_STATIC_DRAW);
	program->setInt("numberLights", std::min(static_cast<size_t>(20), GravityGame::lightPositions.size()));
	program->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	program->setVec3("ambient", 1.0f, 1.0f, 1.0f);
	program->setVec3("diffuse", 1.0f, 1.0f, 1.0f);
	program->setVec3("specular", 1.0f, 1.0f, 1.0f);
	program->setFloat("constant", 1.0f);
	program->setFloat("linear", 0.09f);
	program->setFloat("quadratic", 0.032f);
	program->setMat4("projection", projection);
}

void Screen::render(SpaceObject& spaceObject)
{
	const Vector3d& location = spaceObject.getPhysicsComponent().location;
	const auto& render = spaceObject.getRenderComponent();
	const Shape& s = render.getShape();
	const auto& color = render.getColor();
	program->useProgram();
	program->setInt("numberLights", std::min(static_cast<size_t>(20), GravityGame::lightPositions.size()));
	program->setVec3("lightColor", color[0], color[1], color[2]);
	//program->setVec3("ambient", color[0], color[1], color[2]);
	//program->setVec3("diffuse", color[0], color[1], color[2]);
	//program->setVec3("specular", color[0], color[1], color[2]);
	program->setFloat("constant", 0.01f);
	program->setFloat("linear", 0.001f);
	program->setFloat("quadratic", 0.001f);
	program->setMat4("projection", projection);
	glm::mat4 model{ 1.0f };
	model = glm::translate(model, glm::vec3{ location.x, location.y, location.z });
	model = glm::scale(model, glm::vec3(s.getRadius()));


	size_t lightpositions = std::min(static_cast<size_t>(20), GravityGame::lightPositions.size());
	for (size_t i = 0; i < lightpositions; ++i) {
		const auto & spo = GravityGame::spaceObjects[GravityGame::lightPositions[i]];
		const auto temp = spo.getPhysicsComponent().location;
		program->setVec3(lightPositionStrings[i], temp.x, temp.y, temp.z);
		if (&spo == &spaceObject) {
			program->setFloat("constant", 0.0f);
		}
	}

	program->setMat4("view", view);
	program->setMat4("model", model);
	program->setVec3("viewPos", position);
	program->setVec3("objectColor", color[0], color[1], color[2]);

	glBindVertexArray(s.glVertexArrayID);
	glDrawArrays(GL_TRIANGLES, 0, s.getVertices().size() * 3);
}

}
