#include <thread>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <chicken3421/chicken3421.hpp>

#include <ass3/memes.hpp>
#include <ass3/Model.hpp>
#include <ass3/Scene.hpp>
#include <ass3/Renderer.hpp>
#include <ass3/Camera.hpp>
#include <ass3/framebuffer.hpp>
#include <iostream>

bool hdr_on = false;
bool kernel_on = false;
bool blur_on = false;
bool inverse_on = false;
bool greyscale_on = false;

/**
 * Returns the difference in time between when this function was previously called and this call.
 * @return A float representing the difference between function calls in seconds.
 */
float time_delta();

/**
 * Returns the current time in seconds.
 * @return Returns the current time in seconds.
 */
float time_now();

std::pair<int, int> get_framebuffer_size(GLFWwindow *win);

/**
 * Flips a boolean.
 * Useful for turning filtering on and off...
 *
 * @param win The window this event is associated with
 * @param key What key was pressed
 * @param action Was it released, pressed, or held?
 */
void update_filter_on_off_cb(GLFWwindow *win, int key, int, int action, int) {

    if (key == GLFW_KEY_H && action == GLFW_PRESS) {
        hdr_on = !hdr_on;
    }
    if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        kernel_on = !kernel_on;
    }
    if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        inverse_on = !inverse_on;
    }
    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        blur_on = !blur_on;
    }
    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        greyscale_on = !greyscale_on;
    }
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        hdr_on = false;
        kernel_on = false;
        inverse_on = false;
        blur_on = false;
        greyscale_on = false;
    }
}

void update_sunlight(scene_t &scene, GLFWwindow *win, float dt) {
    using namespace glm;

    glm::vec3 &sun = scene.globe.pos;

    // in degrees
    static glm::vec2 EXTENT = {170, 10};
    static float INCREMENT = 25;
    static float curr_angle = 90;

    float change = 0;
    if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) {
        change = curr_angle >= EXTENT.x ? change : INCREMENT * dt;
    } else if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) {
        change = curr_angle <= EXTENT.y ? change : -INCREMENT * dt;
    }

    curr_angle += change;

    sun = rotate(identity<mat4>(), radians(change), {0, 0, 1}) * vec4(sun, 1);
}


int main() {
    GLFWwindow *win = marcify(chicken3421::make_opengl_window(1920, 1080, "Assignment 3"));
//    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    auto scene = make_scene();
    auto renderer = make_renderer(glm::perspective(glm::pi<double>() / 4, 1920.0 / 1080, 0.1, 1000.0));

    framebuffer_t screen_buffer = make_framebuffer(1920, 1080);
    auto screen_quad = make_screen_quad(screen_buffer.texture);

    auto hdr_framebuffer = make_framebuffer(1920, 1080);
    auto hdr_quad = make_screen_quad(hdr_framebuffer.texture);

    auto kernel_framebuffer = make_framebuffer(1920, 1080);
    auto kernel_quad = make_screen_quad(kernel_framebuffer.texture);

    auto blur_framebuffer = make_framebuffer(1920, 1080);
    auto blur_quad = make_screen_quad(blur_framebuffer.texture);

    auto inverse_framebuffer = make_framebuffer(1920, 1080);
    auto inverse_quad = make_screen_quad(inverse_framebuffer.texture);

    auto greyscale_framebuffer = make_framebuffer(1920, 1080);
    auto greyscale_quad = make_screen_quad(greyscale_framebuffer.texture);

    glfwSetScrollCallback(win, scroll_callback);
    glfwSetKeyCallback(win, update_filter_on_off_cb);

    while (!glfwWindowShouldClose(win)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.f);
        float dt = time_delta();
        update_camera(scene.cam, win, dt);
        std::vector<std::pair<framebuffer_t, no_obj_model_t>> post_effects = {};
        std::vector<GLuint> programs = {};
        if (inverse_on) {
            post_effects.push_back(std::pair<framebuffer_t, no_obj_model_t>(inverse_framebuffer, inverse_quad));
            programs.push_back(renderer.inverse);
        }
        if (greyscale_on) {
            post_effects.push_back(std::pair<framebuffer_t, no_obj_model_t>(greyscale_framebuffer, greyscale_quad));
            programs.push_back(renderer.greyscale);
        }
        if (blur_on) {
            post_effects.push_back(std::pair<framebuffer_t, no_obj_model_t>(blur_framebuffer, blur_quad));
            programs.push_back(renderer.blur);
        }
        if (kernel_on) {
            post_effects.push_back(std::pair<framebuffer_t, no_obj_model_t>(kernel_framebuffer, kernel_quad));
            programs.push_back(renderer.kernel);
        }
        if (hdr_on) {
            post_effects.push_back(std::pair<framebuffer_t, no_obj_model_t>(hdr_framebuffer, hdr_quad));
            programs.push_back(renderer.hdr);
        }
        if (post_effects.empty()) {
            render(renderer, scene);
        } else {
            render(renderer, scene, post_effects[0].first);

            for (size_t i = 0; i < post_effects.size() - 1; i++) {
                post_process(programs[i], post_effects[i].second, post_effects[i + 1].first);
                // want to perform post processing binding output to the next post effect, until your about to reach the end
            }
            post_process(programs[post_effects.size() - 1], post_effects[post_effects.size() - 1].second);
        }


        glfwSwapBuffers(win);
        glfwPollEvents();
        // not entirely correct as a frame limiter, but close enough
        // it would be more correct if we knew how much time this frame took to render
        // and calculated the distance to the next "ideal" time to render and only slept that long
        // the current way just always sleeps for 16.67ms, so in theory we'd drop frames
        std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(1000.f / 60));
    }
//
//    // deleting the whole window also removes the opengl context, freeing all our memory in one fell swoop.
//    chicken3421::delete_opengl_window(win);

    return EXIT_SUCCESS;
}


float time_delta() {
    static float then = time_now();
    float now = time_now();
    float dt = now - then;
    then = now;
    return dt;
}

float time_now() {
    return (float) glfwGetTime();
}