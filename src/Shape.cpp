#include "Shape.h"

#include <algorithm> 

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

float Shape::BRDF(glm::vec3 &in, glm::vec3 &re, glm::vec3 &pos) {
	glm::vec3 normal = GetNormal(pos);
	float cosThetaIn = std::max(0.0f, glm::dot(-in, normal));
	float cosThetaRe = std::max(0.0f, glm::dot(re, normal));
	float cosThetaIn2 = cosThetaIn*cosThetaIn;
	float cosThetaRe2 = cosThetaRe*cosThetaRe;
	float sinThetaTanTheta = sqrt((1.0f - cosThetaIn2)*(1.0f - cosThetaRe2))/std::min(cosThetaIn, cosThetaRe); //not 100% about this one
	glm::vec3 inPlane = glm::normalize(-in - cosThetaIn*normal);
	glm::vec3 rePlane = glm::normalize(re - cosThetaRe*normal);
	float cosPhi = std::max(0.0f, glm::dot(inPlane, rePlane));

	float roughness2 = roughness*roughness;
	float A = 1.0f - 0.5f*roughness2/(roughness2 + 0.57f);
	float B = 1.0f - 0.45f*roughness2/(roughness2 + 0.09f);

	return reflectance/M_PI * cosThetaIn * (A + (B * std::max(0.0f, cosPhi) * sinThetaTanTheta));
}

glm::vec3 Shape::GetRandomDirection(glm::vec3 &pos) {
	return glm::vec3(0.0f, 1.0f, 0.0f);
}

float Shape::GetSamplingProbability(glm::vec3 &pos) {
	return 0.0f;
}