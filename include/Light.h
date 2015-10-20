#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light {
public:
	/* Constructors */
	Light();
	Light(glm::vec3 color, glm::vec3 position);
	~Light();

private:
	glm::vec3 color;
	glm::vec3 position;
};

#endif // LIGHT_H