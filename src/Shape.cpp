#include "Shape.h"

bool Shape::Intersect(const Ray &ray, float &hitDist) {
	return false;
}

glm::vec3 Shape::GetColor(glm::vec3 &pos) {
	return glm::vec3(0.5f);
}

void Shape::SetColor(glm::vec3 col) {
	color = col;
}

void Shape::SetPosition(glm::vec3 pos) {
	position = pos;
}

glm::vec3 Shape::GetPosition() {
	return position;
}

glm::vec3 Shape::GetNormal(glm::vec3 &intersection) {
	return glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 Shape::GetRandomPosition() {
	//THIS IS WRONG!
	return glm::vec3(0.0f);
}

float Shape::GetBRDF(glm::vec3 &in, glm::vec3 &re, glm::vec3 &pos) {
	//thetaIn
	//return reflectance/M_PI * cos(thetaIn) * (A + (B * std::max(0, cos(phiIn - phiRe)) * sin(maxTheta) * tan(maxTheta);
	return 0.0f;
}

glm::vec3 Shape::GetRandomDirection(glm::vec3 &pos) {
	return glm::vec3(0.0f, 1.0f, 0.0f);
}

float Shape::GetSamplingProbability(glm::vec3 &pos) {
	return 0.0f;
}