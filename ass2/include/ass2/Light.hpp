//
// Created by pizza on 11/11/2021.
//

#ifndef COMP3421_ASSIGNMENT_2_LIGHT_HPP
#define COMP3421_ASSIGNMENT_2_LIGHT_HPP
#include <glm/glm.hpp>

struct directional_light_t {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct point_light_t {
    glm::vec3 pos;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

#endif //COMP3421_ASSIGNMENT_2_LIGHT_HPP
