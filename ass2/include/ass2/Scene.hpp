//
// Created by pizza on 31/10/2021.
//

#ifndef COMP3421_ASSIGNMENT_2_SCENE_HPP
#define COMP3421_ASSIGNMENT_2_SCENE_HPP

#include <glad/glad.h>

#include <glm/ext.hpp>

#include <ass2/camera.hpp>
#include <ass2/model.hpp>
#include <ass2/light.hpp>


struct node_t {
    /**
     * The different types of models in our scene.
     */
    enum {
        BULB,
        CHARIZARD,
        EEVEE,
        LEAF,
        JIGGLY,
        PIKACHU,
        POKEBALL,
        SNORLAX,
        VAPOREON,
        WATER,
        BCKG
    } kind;
    model_t obj;
    glm::mat4 transform;
    glm::mat4 scale;
    glm::mat4 rotation;
    float rotation_angle;
    glm::vec3 rotation_axis;
    std::vector<node_t> children;
};

/**
 * Our scene consists of a simple camera and the graph of all objects.
 *
 * Note that every node is the same size because all the models are the same size.
 * This isn't necessarily true in the real world.
 * How might one deal with different node sizes?
 */
struct scene_t {
    camera_t cam;
    directional_light_t sunlight;
    point_light_t globe;
    node_t root;
};

/**
 * Create our scene in whatever fashion we desire.
 *
 * It is important to note that this function takes the role of a level loader.
 * Thus, we'll almost always be changing this when we want different things in-scene.
 *
 * @return Our scene
 */
scene_t make_scene();


#endif //COMP3421_ASSIGNMENT_2_SCENE_HPP
