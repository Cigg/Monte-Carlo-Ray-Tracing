#include "Shape.h"

bool Shape::Intersect(const Ray &ray, float &hitDist) {
	return false;
}

glm::vec3 Shape::GetColor(glm::vec3 &pos) {
	return glm::vec3(0.5f);
}

void Shape::SetPosition(glm::vec3 pos) {
	position = pos;
}

glm::vec3 Shape::GetPosition() {
	return position;
}

glm::vec3 Shape::GetRandomPosition() {
	//THIS IS WRONG!
	return glm::vec3(0.0f);
}