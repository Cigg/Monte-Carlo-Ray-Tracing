#include "Shape.h"

bool Shape::Intersect(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, float &hitDist) {
	return false;
}

void Shape::SetPosition(glm::vec3 pos) {
	position = pos;
}

glm::vec3 Shape::GetPosition() {
	return position;
}