#include "Plane.h"

Plane::Plane() {
    a = glm::vec3(0.2f, 0.9f, -0.4);
    b = glm::vec3(0.4f, 0.9f, -0.4);
    c = glm::vec3(0.2f, 0.9f, 0.2);

    calcNormal();
}

Plane::~Plane() {
	
}

glm::vec3 Plane::calcNormal() {
    glm::vec3 normal = glm::vec3(glm::normalize(glm::cross(b - a, c - a)));
    return normal;
}

bool Plane::Intersect(const Ray &ray, float &hitDist) {
    float denom = glm::dot(normal, ray.direction);
    if (glm::abs(denom) > 0.0001f) {
        float t = (-glm::dot(normal, ray.origin) + glm::dot(normal, a)) / denom;
        if (t >= 0)
            return true;
    }
    return false;
}

glm::vec3 Plane::GetColor(glm::vec3 &pos) {
    return color;
}

glm::vec3 Plane::GetNormal(glm::vec3 &intersection) {
    return calcNormal();
}

// TODO
glm::vec3 Plane::GetRandomPosition() {
    return glm::vec3(0.0f, 1.0f, 0.0f);
}

// TODO
glm::vec3 Plane::GetRandomDirection(glm::vec3 &pos) {
    return glm::vec3(1.0f);
}

// TODO
float Plane::GetSamplingProbability(glm::vec3 &pos) {
    return 2.0f;
}
