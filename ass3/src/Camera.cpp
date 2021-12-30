//
// Created by pizza on 31/10/2021.
//

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

#include <ass3/Camera.hpp>

namespace {
    const float CAMERA_SPEED = 10.f;
    const float INVERT_X = -1.f;
    const float INVERT_Y = 1.f;
    bool cutscene = false;
    float fov = 68.47f;
    bool positive = true;
    float rotational_anglex = 0.0f;
    float rotational_angley = 0.0f;
    float rotational_anglez = 0.0f;
    bool initial_lclick = true;
    bool initial_rclick = true;
    bool initial_bclick = true;

    glm::vec3 sidev(float p, float y) {
        auto cy = std::cos(y);
        auto sy = std::sin(y);

        return glm::normalize(glm::vec3{ cy, 0, -sy });
    }

    glm::vec3 upv(float p, float y) {
        auto cp = std::cos(p);
        auto cy = std::cos(y);
        auto sp = std::sin(p);
        auto sy = std::sin(y);

        return glm::normalize(glm::vec3{ -sp * sy, cp, -cy * sp });
    }

    glm::vec3 frontv(float p, float y) {
        auto cp = std::cos(p);
        auto cy = std::cos(y);
        auto sp = std::sin(p);
        auto sy = std::sin(y);

        return glm::normalize(glm::vec3{ cp * sy, sp, cy * cp });
    }

    void update_angle(camera_t &cam, GLFWwindow *win, float dt, bool &mouse_button_click){
        glm::dvec2 ms_pos;
        glfwGetCursorPos(win, &ms_pos[0], &ms_pos[1]);

        static glm::vec2 prev_pos = ms_pos;
        glm::vec2 delta = glm::radians(glm::vec2(ms_pos) - prev_pos);
        if(mouse_button_click){
            prev_pos = ms_pos;
            mouse_button_click = false;
            std::cout << "BYEEE" << std::endl;
            return;
        }
        prev_pos = ms_pos;

        cam.yaw += INVERT_X * 1 / CAMERA_SPEED * delta.x;
        if((cam.pitch <= 3.141592654/2 && delta.y > 0) || ((cam.pitch >= -3.141592654/2 && delta.y < 0))) {
            cam.pitch += INVERT_Y * 1 / CAMERA_SPEED * delta.y;
        }
    }

    void update_cam_angles(camera_t &cam, GLFWwindow *win, float dt) {
        if(glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE && glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE){
            initial_bclick = true;
        }
        if(glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE){
            initial_lclick = true;
        }
        if(glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE){
            initial_rclick = true;
        }
        if(glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
            update_angle(cam, win, dt, initial_bclick);
        }
        if(glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
            update_angle(cam, win, dt, initial_lclick);
        }
        if(glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
            update_angle(cam, win, dt, initial_rclick);
        }

    }

    void update_cam_fov(camera_t &cam, GLFWwindow *win){
        cam.fov = fov;
    }

    void update_cam_pos(camera_t &cam, GLFWwindow *win, float dt) {
        auto front = -frontv(cam.pitch, cam.yaw);
        auto right = sidev(cam.pitch, cam.yaw);
        auto up = upv(cam.pitch, cam.yaw);
        float step = dt * CAMERA_SPEED;

        front[1] = 0;
        right[1] = 0;
        if(glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS){
            cam.rotational_anglex = 0.0f;
            cam.rotational_angley = 0.0f;
            cam.rotational_anglez = 0.0f;
            cam.cutscene = true;
        }
        if(glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            cam.cutscene = false;
        }
        if(cam.cutscene){
            cam.yaw += INVERT_X * 1 / CAMERA_SPEED * 0.12f;
            if(cam.positive) {
                cam.rotational_anglex += 0.004f;
                cam.pos += front * step * cam.rotational_anglex;
//                cam.pos += right * (step/2) * cam.rotational_anglex;
            }else{
                cam.rotational_anglex -= 0.004f;
                cam.pos -= front * step * cam.rotational_anglex;
//                cam.pos -= right * (step) * cam.rotational_anglex;
            }
            if(cam.rotational_anglex >= 0.4f){
                cam.positive = false;
            }
            if(cam.rotational_anglex <= 0.2f){
                cam.positive = true;
            }
            std::cout << cam.rotational_anglex << " - " << cam.positive << std::endl;
//            rotational_angley += 0.01f;
//            rotational_anglez += 0.01f;
        }
        if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS || (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)) {
            cam.pos += front * (step*10);
        }
        if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
            cam.pos -= front * (step*10);
        }
        if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
            cam.pos += right * (step*10);
        }
        if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
            cam.pos -= right * (step*10);
        }
        if (glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS) {
            cam.pos = {74.6409, 26.0623, 160.092};
            cam.pitch = 3.141592645/17;
            cam.yaw = 2.34;
        }
        if (glfwGetKey(win, GLFW_KEY_T) == GLFW_PRESS) {
            cam.pos = {-467.242, 156.734, -198.3};
            cam.pitch = 3.141592645/8;
            cam.yaw = 4.34;
        }
        if (glfwGetKey(win, GLFW_KEY_Y) == GLFW_PRESS) {
            cam.pos = {171.598, 163.855, -228.639};
            cam.pitch = 3.141592645/8;
            cam.yaw = 4.34;
        }
        if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS) {
            cam.pos += up * (step*10);
        }
        if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            cam.pos -= up * (step*10);
        }
    }

}

camera_t make_camera(glm::vec3 pos, glm::vec3 target) {
    glm::vec3 front = glm::normalize(target - pos);

    // calculate the yaw and pitch from the front vector
    // pretty sure there's a bug here
    float yaw = -std::atan2(front.x, -front.z);
    float pitch = -std::asin(front.y);

    return { pos, yaw, pitch , 68.47, false, 0.0f, 0.0f, 0.0f, true};
}

void update_FOV(camera_t &cam, float FOV){
    cam.fov = FOV;
}

void delete_camera(camera_t &c) {
    // nothing to do as this is a trivially destructible type
    (void)c;
}



void update_camera(camera_t &cam, GLFWwindow *window, float dt) {
    update_cam_angles(cam, window, dt);
    update_cam_pos(cam, window, dt);
    update_cam_fov(cam, window);
    std::cout << " X - " << cam.pos[0] << " Y - " << cam.pos[1] << " Z - " << cam.pos[2] << std::endl;
}

glm::mat4 get_view(const camera_t &cam) {
    auto s = sidev(cam.pitch, cam.yaw);
    auto u = upv(cam.pitch, cam.yaw);
    auto f = frontv(cam.pitch, cam.yaw);
    auto o = glm::vec3(-glm::dot(cam.pos, s), -glm::dot(cam.pos, u), -glm::dot(cam.pos, f));
    auto view = glm::mat4{
            s.x, u.x, f.x, 0,
            s.y, u.y, f.y, 0,
            s.z, u.z, f.z, 0,
            o.x, o.y, o.z, 1
    };

    return view;
}

void switchToFace(camera_t &cam, int face){
    switch(face){
        case 0:
            cam.pitch = 0;
            cam.yaw = 90;
            break;
        case 1:
            cam.pitch = 0;
            cam.yaw = -90;
            break;
        case 2:
            cam.pitch = -90;
            cam.yaw = 180;
            break;
        case 3:
            cam.pitch = 90;
            cam.yaw = 180;
            break;
        case 4:
            cam.pitch = 0;
            cam.yaw = 180;
            break;
        case 5:
            cam.pitch = 0;
            cam.yaw = 0;
            break;
    }

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    std::cout << "surprise" << std::endl;
    if (fov >= 1.0f && fov <= 100.0f) {
        fov -= glm::radians(static_cast<float>(yoffset))*20;
    }
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 100.0f)
        fov = 100.0f;
}