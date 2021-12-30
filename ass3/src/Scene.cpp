//
// Created by pizza on 31/10/2021.
//

#include <ass3/Scene.hpp>
#include <ass3/Model.hpp>
#include <ass3/Camera.hpp>

const char *SASKIA_ALPINE_TREE_PATH = "res/Models/SaskiaTrees/Alpine/spooky_alpine.obj";
const char *SASKIA_LEAFY_TREE_PATH = "res/Models/SaskiaTrees/Leafy/dark_leafy.obj";
const char *SASKIA_DEAD_TREE_PATH = "res/Models/SaskiaTrees/Dead/naked_tree.obj";
const char *HIDDEN_ROOM_PATH = "res/Models/Hidden Room/hiddenroom1.obj";
const char *TREE2_PATH = "res/Models/Tree 02/Tree.obj";
const char *TREE1_PATH = "res/Models/Tree/Tree.obj";
const char *BIRCH_PATH = "res/Models/Birch/birch_tree.obj";
const char *BACKGROUND_PATH = "res/Models/landscape/10450_Rectangular_Grass_Patch_v1_iterations-2.obj";
const char *SKYBOX_PATH = "res/Skybox/";

scene_t make_scene() {
    auto cam = make_camera({74.6409, 26.0623, 160.092}, {-456, 35, 19});
    auto cam2 = make_camera({500.4, 50, 3}, {0, 48, 0});

    auto skybox = make_skybox(SKYBOX_PATH);

    node_t hidden_room = {
            node_t::HIDDENROOM,
            load_obj(HIDDEN_ROOM_PATH),
            glm::mat4(1),
            glm::scale(glm::mat4(1), {1, 1, 1}),
            glm::identity<glm::mat4>(),
            0,
            glm::vec3(1, 0, 0),
            {}};

    auto dead_tree_mesh = load_obj(SASKIA_DEAD_TREE_PATH);
    auto alpine_tree_mesh = load_obj(SASKIA_ALPINE_TREE_PATH);
    auto leafy_mesh = load_obj(SASKIA_LEAFY_TREE_PATH);
    auto tree1_mesh = load_obj(TREE1_PATH);
    auto tree2_mesh = load_obj(TREE2_PATH);
    std::vector<node_t> saskia_dead_trees = {make_saskia_dead_tree({13.3, -13.04, 0}, dead_tree_mesh, {})};
    std::vector<node_t> saskia_alpine_trees = {};
    std::vector<node_t> saskia_leafy_trees = {};
    std::vector<node_t> tree1s = {};
    std::vector<node_t> tree2s = {};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            saskia_dead_trees.push_back(
                    make_saskia_dead_tree({6.3 + j * 2.1, -10.04 + i * 3.87, 0}, dead_tree_mesh, {}));

        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            saskia_alpine_trees.push_back(
                    make_saskia_alpine_tree({-11.9 + j * 2.1 * 2 * 7 / 5, -14.24 + i * 4.77 * 2, 0}, alpine_tree_mesh));
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 6; j++) {
            saskia_leafy_trees.push_back(make_saskia_leafy_tree({-13.5 + j * 4.9, 10.4 - i * 5.0, 0}, leafy_mesh));
        }
    }

    // row of trees
    saskia_leafy_trees.push_back(make_saskia_leafy_tree({-13.5, -5.4, 0}, leafy_mesh));
    saskia_dead_trees.push_back(make_saskia_dead_tree({-13.5, -2, 0}, dead_tree_mesh, {}));
    saskia_dead_trees.push_back(make_saskia_dead_tree({-8.5, -2, 0}, dead_tree_mesh, {}));
    saskia_leafy_trees.push_back(make_saskia_leafy_tree({-4.5, -2, 0}, leafy_mesh));

    // row of trees
    saskia_leafy_trees.push_back(make_saskia_leafy_tree({-10.5, -9.4, 0}, leafy_mesh));
    saskia_dead_trees.push_back(make_saskia_dead_tree({-7.4, -8.8, 0}, dead_tree_mesh, {}));
    saskia_dead_trees.push_back(make_saskia_dead_tree({-5.0, -10.8, 0}, dead_tree_mesh, {}));
    saskia_leafy_trees.push_back(make_saskia_leafy_tree({1.5, -7.4, 0}, leafy_mesh));
    saskia_dead_trees.push_back(make_saskia_dead_tree({-2.0, -6.0, 0}, dead_tree_mesh, {}));


    node_t birch = {
            node_t::BIRCH,
            load_obj(BIRCH_PATH),
            glm::translate(glm::mat4(1), {0, 0, 0}),
            glm::scale(glm::mat4(1), {30, 50, 50}),
            glm::identity<glm::mat4>(),
            glm::radians(90.0f),
            glm::vec3(1, 0, 0),
            {hidden_room}};

    node_t background = {
            node_t::BACKGROUND,
            load_obj(BACKGROUND_PATH),
            glm::mat4(1),
            glm::scale(glm::mat4(1), {6, 1, 6}),
            glm::identity<glm::mat4>(),
            glm::radians(-90.0f),
            glm::vec3(1, 0, 0),
            {hidden_room, birch, make_reflective_sphere(), make_reflective_sphere_skybox()}};

    for (auto node: saskia_dead_trees) {
        background.children.push_back(node);
    }
    for (auto node: saskia_alpine_trees) {
        background.children.push_back(node);
    }
    for (auto node: saskia_leafy_trees) {
        background.children.push_back(node);
    }
    for (auto node: tree1s) {
        background.children.push_back(node);
    }
    for (auto node: tree2s) {
        background.children.push_back(node);
    }
    point_light_t light_globe = {
            glm::vec3{33.8, 306.031, -16.432},
            glm::vec3{1.0},
            glm::vec3{1.0},
            glm::vec3{1.0},
    };

    directional_light_t sun = {
            glm::vec3{0.3},
            glm::vec3{0.2},
            glm::vec3{0.4},
    };

    return scene_t{cam, cam2,sun, light_globe, background, skybox};
}

node_t make_hidden_room() {
    return {
            node_t::HIDDENROOM,
            load_obj(HIDDEN_ROOM_PATH),
            glm::mat4(1),
            glm::scale(glm::mat4(1), {1, 1, 1}),
            glm::identity<glm::mat4>(),
            glm::radians(90.0f),
            glm::vec3(1, 0, 0), {}};
}

node_t make_reflective_sphere() {
    return {
            node_t::REFLECTIVE_ORB,
            make_sphere(),
            glm::translate(glm::mat4(1), {-5,1.8,3}),
            glm::scale(glm::mat4(1), {10, 10, 40}),
            glm::identity<glm::mat4>(),
            glm::radians(90.0f),
            glm::vec3(1, 0, 0), {},
            false,
            make_realtime_cubemap()
    };
}

node_t make_reflective_sphere_skybox() {
    return {
            node_t::REFLECTIVE_SKYBOX_ORB,
            make_sphere(),
            glm::translate(glm::mat4(1), {5,1.8,3}),
            glm::scale(glm::mat4(1), {10, 10, 40}),
            glm::identity<glm::mat4>(),
            glm::radians(90.0f),
            glm::vec3(1, 0, 0), {},
            false,
            make_cubemap(SKYBOX_PATH)
    };
}

node_t make_saskia_dead_tree(glm::vec3 position, model_t &model, std::vector<node_t> children = {}) {
    return {
            node_t::SASKIADEADTREE,
            model,
            glm::translate(glm::mat4(1), position),
            glm::scale(glm::mat4(1), {10, 10, 40}),
            glm::identity<glm::mat4>(),
            glm::radians(90.0f),
            glm::vec3(1, 0, 0),
            children};
}

node_t make_saskia_alpine_tree(glm::vec3 position, model_t &model) {
    return {
            node_t::SASKIAALPINETREE,
            model,
            glm::translate(glm::mat4(1), position),
            glm::scale(glm::mat4(1), {10, 10, 40}),
            glm::identity<glm::mat4>(),
            glm::radians(90.0f),
            glm::vec3(1, 0, 0), {}};
}

node_t make_saskia_leafy_tree(glm::vec3 position, model_t &model) {
    return {
            node_t::SASKIALEAFYTREE,
            model,
            glm::translate(glm::mat4(1), position),
            glm::scale(glm::mat4(1), {10, 10, 40}),
            glm::identity<glm::mat4>(),
            glm::radians(90.0f),
            glm::vec3(1, 0, 0), {}};
}