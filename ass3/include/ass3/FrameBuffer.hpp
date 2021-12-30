//
// Created by pizza on 31/10/2021.
//

#ifndef COMP3421_ASSIGNMENT_2_FRAMEBUFFER_HPP
#define COMP3421_ASSIGNMENT_2_FRAMEBUFFER_HPP

#include <glad/glad.h>

struct framebuffer_t {
    GLuint fbo;
    GLuint texture;
    GLuint rbo;
};

/**
 * Create a framebuffer
 * @returns struct containing handle to FBO and the resulting texture
 */
framebuffer_t make_framebuffer(int width, int height);
framebuffer_t make_framebuffer_cubemap(int width, int height);
/**
 * Destroy a framebuffer, releasing GPU resources
 * @param framebuffer The framebuffer to destroy
 */
void delete_framebuffer(framebuffer_t &framebuffer);

#endif //COMP3421_ASSIGNMENT_2_FRAMEBUFFER_HPP
