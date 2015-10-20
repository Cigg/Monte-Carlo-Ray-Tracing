#include "Light.h"

Light::Light() {
	color = glm::vec3(1.0f, 1.0f, 0.0f);
	position = glm::vec3(1.0, 0.0, 0.0);
}

Light::Light(glm::vec3 c, glm::vec3 p) {
	color = c;
	position = p;
	// intesity?
}

Light::~Light() {}