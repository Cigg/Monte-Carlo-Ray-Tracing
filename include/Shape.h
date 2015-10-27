#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>

#include "Ray.h"

class Shape {
public:
	void SetPosition(glm::vec3 pos);
	glm::vec3 GetPosition();
	
	virtual bool Intersect(const Ray &ray, float &hitDist) = 0;
	virtual glm::vec3 GetColor(glm::vec3 &pos);
	virtual void SetColor(glm::vec3 col);
	virtual glm::vec3 GetNormal(glm::vec3 &intersection);
	virtual glm::vec3 GetRandomPosition();
	virtual glm::vec3 GetRandomDirectionTowardsShape(glm::vec3 &pos);
	virtual float OrenNayarBRDF(glm::vec3 &in, glm::vec3 &re, glm::vec3 &pos);
	virtual float LambertianBRDF();
	virtual float GetSamplingProbability(glm::vec3 &pos);

	bool isLight;
	bool isTrans;
	Shape() { color = glm::vec3(1.0f); isLight = false; isTrans = false; reflectance = 0.9f; roughness = 0.0f; }

	float reflectance;
	float roughness;

protected:
	glm::vec3 position;
	glm::vec3 color;
};

#endif // SHAPE_H