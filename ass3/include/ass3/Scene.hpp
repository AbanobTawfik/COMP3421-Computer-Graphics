//
// Created by pizza on 31/10/2021.
//

#ifndef COMP3421_ASSIGNMENT_2_SCENE_HPP
#define COMP3421_ASSIGNMENT_2_SCENE_HPP

#include <glad/glad.h>

#include <glm/ext.hpp>

#include <ass3/camera.hpp>
#include <ass3/model.hpp>
#include <ass3/light.hpp>
#include <ass3/CubeMap.hpp>

#include <chicken3421/camera.hpp>


struct node_t {
    /**
     * The different types of models in our scene.
     */
    enum {
        BIRCH,
        TREE1,
        TREE2,
        SASKIADEADTREE,
        SASKIAALPINETREE,
        SASKIALEAFYTREE,
        HIDDENROOM,
        BACKGROUND,
        REFLECTIVE_ORB,
        REFLECTIVE_SKYBOX_ORB
    } kind;

    model_t obj;
    glm::mat4 transform;
    glm::mat4 scale;
    glm::mat4 rotation;
    float rotation_angle;
    glm::vec3 rotation_axis;
    std::vector<node_t> children;
    bool in_hidden_room;
    GLuint texture;
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
    camera_t cube_map_camera;
    directional_light_t sunlight;
    point_light_t globe;
    node_t root;
    no_obj_model_t skybox;
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

node_t make_saskia_dead_tree(glm::vec3 position, model_t &model, std::vector<node_t> children);
node_t make_saskia_alpine_tree(glm::vec3 position, model_t &model);
node_t make_saskia_leafy_tree(glm::vec3 position, model_t &model);

node_t make_reflective_sphere();
GLuint make_realtime_cubemap();
node_t make_reflective_sphere_skybox();
#endif //COMP3421_ASSIGNMENT_2_SCENE_HPP
