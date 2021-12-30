#ifndef CHICKEN3421_GL_UTILS_HPP
#define CHICKEN3421_GL_UTILS_HPP

#include <string>
#include <vector>

#include <glad/glad.h>

namespace chicken3421 {
    /**
     *
     * Reads in the shader source code in `path` and creates a new shader object of type `shader-type`
     *
     * @param path: filesystem path to the shader
     * @param shader_type: one of GL_VERTEX_SHADER, GL_FRAGMENT_SHADER
     * @return A handle to the new shader object
     */
    GLuint make_shader(const std::string &path, GLenum shader_type);

    /**
     *
     * Schedules a shader for deletion with OpenGL.
     *
     * Typically, once a rendering program has been linked together and is valid,
     * the source shaders no longer need to exist. In case like those,
     * it is "environmentally friendly" to clean up after ourselves
     *
     * @param shader: the shader to delete.
     */
    void delete_shader(GLuint shader);

    /**
     *
     * Creates a new program comprised only of the vertex shader and fragment shader
     *
     * @param vs: the vertex shader
     * @param fs: the fragment shader
     * @return: a renderable program
     */
    GLuint make_program(GLuint vs, GLuint fs);

    /**
     *
     * Deletes a rendering program
     *
     * @param program
     */
    void delete_program(GLuint program);

    /**
     *
     * Creates a single new Vertex Array Object
     *
     * @return: The new vertex array
     */
    GLuint make_vao();

    /**
     *
     * Schedules the deletion of a Vertex Array Object
     *
     * @param vao: The VAO to delete.
     */
    void delete_vao(GLuint vao);

    /**
     *
     * Creates a new single generic Buffer Object
     *
     * @return: The new buffer
     */
    GLuint make_buffer();

    /**
     *
     * Schedules the deletion of the buffer object with OpenGL.
     *
     * @param buff: The buffer to delete.
     */
    void delete_buffer(GLuint buff);
}

#endif //CHICKEN3421_GL_UTILS_HPP
