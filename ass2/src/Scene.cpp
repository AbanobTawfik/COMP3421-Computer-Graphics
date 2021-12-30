//
// Created by pizza on 31/10/2021.
//

#include <ass2/Scene.hpp>

const char *BULB_PATH = "res/obj/Bulb/bulb.obj";
const char *CHARIZARD_PATH = "res/obj/Charizard/Charizard.obj";
const char *EEVEE_PATH = "res/obj/Eevee/Eevee.obj";
const char *LEAF_PATH = "res/obj/leaf/leaf.obj";
const char *JIGGLY_PATH = "res/obj/Jiggly/Jiggly.obj";
const char *PIKACHU_PATH = "res/obj/Pikachu/Pikachu.obj";
const char *SNORLAX_PATH = "res/obj/Snorlax/Snorlax.obj";
const char *VAPOREON_PATH = "res/obj/Vaporeon/Vaporeon.obj";
const char *WATER_PATH = "res/obj/water/water.obj";
const char *BACKGROUND_PATH = "res/obj/Background/background.obj";

scene_t make_scene() {
    camera_t cam = make_camera({ 0.4, 2.0, 5 }, { 0, 0, 0 });

    node_t leaf = {
            node_t::LEAF,
            load_obj(LEAF_PATH),
            glm::translate(glm::mat4(1), {0.5, 0.1f, 0}),
            glm::scale(glm::mat4(1), {4, 4, 4}),
            glm::identity<glm::mat4>(),
            0,
            glm::vec3(1,0,0),
            { }
    };

    node_t charizard = {
            node_t::CHARIZARD,
            load_obj(CHARIZARD_PATH),
            glm::translate(glm::mat4(1), {-100, -16.5f, 140}),
            glm::scale(glm::mat4(1), {0.05, 0.05, 0.05}),
            glm::identity<glm::mat4>(),
            glm::radians(150.0f),
            glm::vec3(0,1,0),
            { }
    };

    node_t water = {
            node_t::WATER,
            load_obj(WATER_PATH),
            glm::translate(glm::mat4(1), {0.08, 0.033f, 0}),
            glm::scale(glm::mat4(1), {30, 30, 30}),
            glm::identity<glm::mat4>(),
            0,
            glm::vec3(0,1,0),
            {}
    };

    node_t vaporeon = {
            node_t::VAPOREON,
            load_obj(VAPOREON_PATH),
            glm::translate(glm::mat4(1), {-29.0f, -2.9f, 0}),
            glm::scale(glm::mat4(1), {0.3, 0.3, 0.3}),
            glm::identity<glm::mat4>(),
            glm::radians(90.0f),
            glm::vec3(0,1,0),
            {water}

    };

    node_t jiggly = {
            node_t::JIGGLY,
            load_obj(JIGGLY_PATH),
            glm::translate(glm::mat4(1), {-500, -85.5f, -700}),
            glm::scale(glm::mat4(1), {0.01, 0.01, 0.01}),
            glm::identity<glm::mat4>(),
            glm::radians(50.0f),
            glm::vec3(0,1,0),
            { }
    };

    node_t pikachu = {
            node_t::PIKACHU,
            load_obj(PIKACHU_PATH),
            glm::translate(glm::mat4(1), {10, -1.70f, 12.46}),
            glm::scale(glm::mat4(1), {0.5, 0.5, 0.5}),
            glm::identity<glm::mat4>(),
            glm::radians(210.0f),
            glm::vec3(0,1,0),
            { }
    };

    node_t snorlax = {
            node_t::SNORLAX,
            load_obj(SNORLAX_PATH),
            glm::translate(glm::mat4(1), {660.0f, -174.5f, -1450.0f}),
            glm::scale(glm::mat4(1), {0.005, 0.005, 0.005}),
            glm::identity<glm::mat4>(),
            glm::radians(-30.0f),
            glm::vec3(0,1,0),
            { }
    };

    node_t bulb = {
            node_t::BULB,
            load_obj(BULB_PATH),
            glm::translate(glm::mat4(1), {19.5, -2.15f, 0}),
            glm::scale(glm::mat4(1), {0.4, 0.4, 0.4}),
            glm::identity<glm::mat4>(),
            glm::radians(270.0f),
            glm::vec3(0,1,0),
            { leaf }
    };

    node_t eevee = {
            node_t::EEVEE,
            load_obj(EEVEE_PATH),
            glm::translate(glm::mat4(1), {0.5, 0.65f, 0}),
            glm::scale(glm::mat4(1), {0.4, 0.4, 0.4}),
            glm::identity<glm::mat4>(),
            0,
            glm::vec3(1,0,0),
            {charizard, jiggly, vaporeon, pikachu, snorlax, bulb }
    };

    node_t background = {
            node_t::BCKG,
            load_obj(BACKGROUND_PATH),
            glm::mat4(1),
            glm::scale(glm::mat4(1), {1, 1, 1}),
            glm::identity<glm::mat4>(),
            0,
            glm::vec3(1,0,0),
            {  eevee }};

    point_light_t light_globe = {
            glm::vec3{ 0, 0.1, 0 },
            glm::vec3{ 0.6, 0.6, 0.6 },
            glm::vec3{ 0.2 },
            glm::vec3{ 0.2 },
    };

    directional_light_t sun = {
            glm::vec3{ 0.1 },
            glm::vec3{ 0.1 },
            glm::vec3{ 0.1 },
    };

    return scene_t{ cam, sun, light_globe,background};
}
