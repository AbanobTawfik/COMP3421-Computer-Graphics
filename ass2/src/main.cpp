#include <thread>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <chicken3421/chicken3421.hpp>

#include <ass2/memes.hpp>
#include <ass2/Model.hpp>
#include <ass2/Scene.hpp>
#include <ass2/Renderer.hpp>
#include <ass2/Camera.hpp>
#include <iostream>
#include <ass2/Animator.hpp>

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

void update_sunlight(scene_t &scene, GLFWwindow *win, float dt) {
    using namespace glm;

    glm::vec3 &sun = scene.globe.pos;

    // in degrees
    static glm::vec2 EXTENT = { 170, 10 };
    static float INCREMENT = 25;
    static float curr_angle = 90;

    float change = 0;
    if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) {
        change = curr_angle >= EXTENT.x ? change : INCREMENT * dt;
    } else if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) {
        change = curr_angle <= EXTENT.y ? change : -INCREMENT * dt;
    }

    curr_angle += change;

    sun = rotate(identity<mat4>(), radians(change), { 0, 0, 1 }) * vec4(sun, 1);
}


int main() {
    GLFWwindow *win = marcify(chicken3421::make_opengl_window(1920, 1080, "Assignment 2"));

    // TODO - turn this on or off?
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    auto scene = make_scene();
    auto animator = make_animator();
    auto renderer = make_renderer(glm::perspective(glm::pi<double>() / 4, 1920.0 / 1080, 0.1, 1000.0));


    glfwSetScrollCallback(win,scroll_callback);

    while (!glfwWindowShouldClose(win)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.f);
        float dt = time_delta();
        animate(animator, scene, time_now());
        update_camera(scene.cam, win, dt);
        update_sunlight(scene, win, dt);
        render(renderer, scene);

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
    return (float)glfwGetTime();
}