//
// Created by pizza on 31/10/2021.
//

#ifndef COMP3421_ASSIGNMENT_2_RENDERER_HPP
#define COMP3421_ASSIGNMENT_2_RENDERER_HPP

#include <glad/glad.h>
#include <glm/ext.hpp>

#include <ass2/model.hpp>
#include <ass2/scene.hpp>

/**
 * A simple bag of all the OpenGL rendering bits we've used up until now.
 */
struct renderer_t {
    glm::mat4 projection;

    GLuint program;

    // vs uniforms
    GLint p_loc;
    GLint v_loc;
    GLint m_loc;

    GLint sun_loc;

    // fs uniforms
    GLint tf_loc;
    GLint col_loc;

    GLint sun_amb_loc;
    GLint sun_diff_loc;
    GLint sun_spec_loc;

    GLint p_position_loc;
    GLint p_ambient_loc;
    GLint p_diffuse_loc;
    GLint p_specular_loc;

    GLint shine_loc;
};

/**
 * Initialise our renderer.
 *
 * Inside of this we can load shaders, grab uniform locations, etc.
 * Of course, if we have different shaders, it is conceivable we would have different capabilities enabled/disabled too.
 * Whatever doesn't change can be initialised here, then.
 *
 * @param projection The projection matrix to use (almost always a perspective transform)
 * @return The renderer.
 */
renderer_t make_renderer(const glm::mat4 &projection);

/**
 * Draws the current node "m".
 *
 * @param renderer The rendering context gotten from make_renderer()
 * @param mvp The model-view-projection matrix.
 * @param m The model to draw.
 */
void draw(const renderer_t &renderer, const glm::mat4 &p, const glm::mat4 &v, const glm::mat4 &m, const model_t &model);
/**
 * Renders the current scene specified by scene.
 *
 * Starting at the root of the scenegraph, this function will "recursively" descend the scenegraph and draw all nodes.
 * Nothing is modified except the OpenGL context.
 *
 * @param renderer the rendering context gotten from make_renderer()
 * @param scene the scene to draw.
 */
void render(const renderer_t &renderer, const scene_t &scene);



#endif //COMP3421_ASSIGNMENT_2_RENDERER_HPP
