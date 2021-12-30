//
// Created by pizza on 31/10/2021.
//

#ifndef COMP3421_ASSIGNMENT_2_RENDERER_HPP
#define COMP3421_ASSIGNMENT_2_RENDERER_HPP

#include <glad/glad.h>
#include <glm/ext.hpp>

#include <ass3/Model.hpp>
#include <ass3/Scene.hpp>
#include <ass3/FrameBuffer.hpp>

/**
 * A simple bag of all the OpenGL rendering bits we've used up until now.
 */
struct renderer_t {
    glm::mat4 projection;
    glm::mat4 view;

    GLuint program;
    GLuint skybox;
    GLuint reflective;
    GLuint hdr;
    GLuint blur;
    GLuint greyscale;
    GLuint kernel;
    GLuint inverse;
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

void draw_sky(const renderer_t &renderer, const glm::mat4 &p, const glm::mat4 &v, const model_t &skybox);
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
void render(renderer_t &renderer, scene_t &scene, bool render_to_cubemap = false);
void render(renderer_t &renderer, scene_t &scene, framebuffer_t &target);
/**
 * Applies a black and white filter to the texture inside of screen_quad.
 *
 * The shaders that do this are in bw.vert, bw.frag.
 *
 * @param renderer The state of our renderer
 * @param screen_quad A textured quad that represents the screen as simple geometry and the scene as a texture
 */
void post_process(GLuint &renderer_program, no_obj_model_t &screen_quad,framebuffer_t &target);
void post_process(GLuint &renderer_program, no_obj_model_t &screen_quad);

#endif //COMP3421_ASSIGNMENT_2_RENDERER_HPP
