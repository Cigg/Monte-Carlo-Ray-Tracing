#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <vector>
#include "Shape.h"

class Scene
{
public:
    Scene();
    
    void addObject(Shape *sh);
    void addLight(Shape *lg);
    
    std::vector<Shape*> *shapes;
    std::vector<Shape*> *lights;    
};

#endif // SCENE_H
