#ifndef CHICKEN3421_WINDOW_UTILS_HPP
#define CHICKEN3421_WINDOW_UTILS_HPP

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace chicken3421 {

    /**
     *
     * Creates a new window with the given width, height, and title.
     *
     * @param width: Screen width in pixels
     * @param height: Screen height in pixels
     * @param title: The title for the window
     * @return A valid GLFWwindow pointer or null if window creation failed
     */
    GLFWwindow *make_window(int width, int height, const std::string &title);

    /**
     *
     * Given a valid GLFWwindow pointer, makes this window's OpenGL context current and loads the Glad function pointers into it
     *
     * @param win: A valid GLFWwindow pointer
     * @return True if there was no error, false if there was.
     */
    bool make_opengl_context(GLFWwindow *win);

    /**
     * Convenience function that combines make_window() and make_opengl_context().
     * Throws an exception on error.
     *
     * @see make_window
     * @see make_opengl_context
     *
     * @param width: initial width of the window
     * @param heigh: initial height of the window
     * @param title: initial title of the window
     * @return: a valid window ready for rendering into
     */
    GLFWwindow *make_opengl_window(int width, int height, const std::string &title);
}


#endif //CHICKEN3421_WINDOW_UTILS_HPP
