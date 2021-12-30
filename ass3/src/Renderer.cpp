//
// Created by pizza on 31/10/2021.
//

#include <ass3/Model.hpp>
#include <ass3/FrameBuffer.hpp>
#include <ass3/Renderer.hpp>
#include <ass3/Camera.hpp>
#include <glm/ext.hpp>

#include <stack>

#include <chicken3421/chicken3421.hpp>
#include <iostream>

const char *MODEL_VERT_PATH = "res/shaders/shader.vert";
const char *MODEL_FRAG_PATH = "res/shaders/shader.frag";

const char *SKYBOX_VERT_PATH = "res/shaders/skybox.vert";
const char *SKYBOX_FRAG_PATH = "res/shaders/skybox.frag";

const char *BlUR_VERT_PATH = "res/shaders/Blur.vert";
const char *BLUR_FRAG_PATH = "res/shaders/Blur.frag";

const char *HDR_VERT_PATH = "res/shaders/HDR.vert";
const char *HDR_FRAG_PATH = "res/shaders/HDR.frag";

const char *REFLECTIVE_VERT_PATH = "res/shaders/reflective.vert";
const char *REFLECTIVE_FRAG_PATH = "res/shaders/reflective.frag";

const char *GREYSCALE_VERT_PATH = "res/shaders/Greyscale.vert";
const char *GREYSCALE_FRAG_PATH = "res/shaders/Greyscale.frag";

const char *KERNEL_VERT_PATH = "res/shaders/Kernel.vert";
const char *KERNEL_FRAG_PATH = "res/shaders/Kernel.frag";

const char *INVERSE_VERT_PATH = "res/shaders/inverse.vert";
const char *INVERSE_FRAG_PATH = "res/shaders/inverse.frag";

bool not_rendered_cube_map_realtime = true;

GLuint load_program(const std::string &vs_path, const std::string &fs_path) {
    using namespace chicken3421;
    GLuint vs = make_shader(vs_path, GL_VERTEX_SHADER), fs = make_shader(fs_path, GL_FRAGMENT_SHADER);
    GLuint prog = make_program(vs, fs);
    delete_shader(vs), delete_shader(fs);
    return prog;
}

renderer_t make_renderer(const glm::mat4 &projection) {
    // set once, and never again because YOLO
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint prog_skybox = load_program(SKYBOX_VERT_PATH, SKYBOX_FRAG_PATH);
    GLuint prog_model = load_program(MODEL_VERT_PATH, MODEL_FRAG_PATH);
    GLuint prog_reflective = load_program(REFLECTIVE_VERT_PATH, REFLECTIVE_FRAG_PATH);
    GLuint prog_hdr = load_program(HDR_VERT_PATH, HDR_FRAG_PATH);
    GLuint prog_blur = load_program(BlUR_VERT_PATH, BLUR_FRAG_PATH);
    GLuint prog_greyscale = load_program(GREYSCALE_VERT_PATH, GREYSCALE_FRAG_PATH);
    GLuint prog_kernel = load_program(KERNEL_VERT_PATH, KERNEL_FRAG_PATH);
    GLuint prog_inverse = load_program(INVERSE_VERT_PATH, INVERSE_FRAG_PATH);

    return {projection, {}, prog_model, prog_skybox, prog_reflective, prog_hdr, prog_blur, prog_greyscale, prog_kernel,
            prog_inverse};
}

void draw_sky(const renderer_t &renderer, const glm::mat4 &p, const glm::mat4 &v, const no_obj_model_t &skybox) {
    glUseProgram(renderer.skybox);
    glFrontFace(GL_CW);
    glDepthMask(GL_FALSE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.material.texture);

    glBindVertexArray(skybox.mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, skybox.mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox.mesh.ibo);

    chicken3421::set_uniform(renderer.skybox, "u_v", v);
    chicken3421::set_uniform(renderer.skybox, "u_p", p);

    glDrawElements(GL_TRIANGLES, skybox.mesh.nverts, INDEX_TYPE, nullptr);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDepthMask(GL_TRUE);
    glFrontFace(GL_CCW);
    glUseProgram(0);
}

void draw_model(const renderer_t &renderer,
                const glm::mat4 &p, const glm::mat4 &v, const glm::mat4 &m, const model_t &model, const float shine,
                const directional_light_t dl, const point_light_t pl
) {
    glUseProgram(renderer.program);

    // bind the projection, view and matrix for shape
    chicken3421::set_uniform(renderer.program, "m", m);
    chicken3421::set_uniform(renderer.program, "v", v);
    chicken3421::set_uniform(renderer.program, "p", p);
    // bind the shineiness scale for specular lighting
    chicken3421::set_uniform(renderer.program, "shininess", shine);
    // bind directional lighting
    chicken3421::set_uniform(renderer.program, "sun_pos", pl.pos);
    chicken3421::set_uniform(renderer.program, "u_sun_ambient", dl.ambient);
    chicken3421::set_uniform(renderer.program, "u_sun_diffuse", dl.diffuse);
    chicken3421::set_uniform(renderer.program, "u_sun_specular", dl.specular);

    // bind point light
    chicken3421::set_uniform(renderer.program, "p_position", pl.pos);
    chicken3421::set_uniform(renderer.program, "p_ambient", pl.ambient);
    chicken3421::set_uniform(renderer.program, "p_diffuse", pl.diffuse);
    chicken3421::set_uniform(renderer.program, "p_specular", pl.specular);


    for (const mesh_t &mesh: model.meshes) {

        if (mesh.material_id != -1) {
            const material_t &mtl = model.materials[mesh.material_id];
            glBindTexture(GL_TEXTURE_2D, mtl.texture);
            chicken3421::set_uniform(renderer.program, "uTexFactor", mtl.texture == 0 ? 0.f : 1.f);
            chicken3421::set_uniform(renderer.program, "uColor", mtl.color);
        } else {
            // default material
            chicken3421::set_uniform(renderer.program, "uTexFactor", 0.f);
            chicken3421::set_uniform(renderer.program, "uColor", glm::identity<glm::mat4>());
        }


        glBindVertexArray(mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glDrawArrays(GL_TRIANGLES, 0, mesh.nverts);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}

void draw_reflective(renderer_t &renderer, const glm::mat4 &mv, const model_t &m, GLuint texture, scene_t &scene,
                     node_t &node) {
    // render to cube map for the first time
    if (not_rendered_cube_map_realtime) {
        auto cube_map_size = 1024;
        glViewport(0, 0, cube_map_size, cube_map_size);
        auto fb = make_framebuffer_cubemap(cube_map_size, cube_map_size);
        glBindTexture(GL_TEXTURE_CUBE_MAP, fb.texture);

        glBindFramebuffer(GL_FRAMEBUFFER, fb.fbo);
        glBindRenderbuffer(GL_RENDERBUFFER, fb.rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, cube_map_size, cube_map_size);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb.rbo);
        auto scene_copy = scene;
        auto cam_copy = make_camera({500.4, 50, 3}, {0, 48, 0});
        update_FOV(cam_copy, 90.0f);
        auto new_children = std::vector<node_t>{};
        for (auto children: scene_copy.root.children) {
            if (children.kind != node_t::REFLECTIVE_ORB) {
                new_children.push_back(children);
            }
        }
        scene_copy.root.children = new_children;
        scene_copy.cam = cam_copy;
        for (int i = 0; i < 6; i++) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, fb.texture,
                                   0);

            switchToFace(scene_copy.cam, (i % 6));

            render(renderer, scene_copy, true);
        }
        node.texture = fb.texture;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 1920, 1080);
        not_rendered_cube_map_realtime = false;
        delete_framebuffer(fb);

    }

    glUseProgram(renderer.reflective);
    glBindTexture(GL_TEXTURE_CUBE_MAP, node.texture);
    chicken3421::set_uniform(renderer.reflective, "u_projection", renderer.projection);
    chicken3421::set_uniform(renderer.reflective, "u_model_view", mv);
    chicken3421::set_uniform(renderer.reflective, "u_view", renderer.view);


    for (const mesh_t &mesh: m.meshes) {
        glBindVertexArray(mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glDrawArrays(GL_TRIANGLES, 0, mesh.nverts);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void draw_reflective_skybox(renderer_t &renderer, const glm::mat4 &mv, const model_t &m, GLuint texture, scene_t &scene,
                            node_t &node) {
    glUseProgram(renderer.reflective);
    glBindTexture(GL_TEXTURE_CUBE_MAP, node.texture);
    chicken3421::set_uniform(renderer.reflective, "u_projection", renderer.projection);
    chicken3421::set_uniform(renderer.reflective, "u_model_view", mv);
    chicken3421::set_uniform(renderer.reflective, "u_view", renderer.view);


    for (const mesh_t &mesh: m.meshes) {
        glBindVertexArray(mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glDrawArrays(GL_TRIANGLES, 0, mesh.nverts);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void render(renderer_t &renderer, scene_t &scene, bool render_to_cubemap) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);

    glm::mat4 view = get_view(scene.cam);
    renderer.view = view;
    draw_sky(renderer, renderer.projection, view, scene.skybox);

    std::stack<std::pair<node_t &, glm::mat4>> renderables{{{scene.root, glm::mat4(1)}}};
    while (!renderables.empty()) {
        auto &[node, parent_tf] = renderables.top();  // this is called a structured binding
        renderables.pop();

        auto rotate = glm::rotate(node.rotation, node.rotation_angle, node.rotation_axis);
        glm::mat4 xform = parent_tf * node.scale * node.transform * rotate;
        auto aspect_ratio = render_to_cubemap ? 1 : 1920.0f / 1080.0f;
        auto projection = glm::perspective(glm::radians(scene.cam.fov), aspect_ratio, 0.1f, 1000.0f);
        switch (node.kind) {
            case node_t::BACKGROUND:
            case node_t::TREE1:
            case node_t::TREE2:
            case node_t::BIRCH:
            case node_t::SASKIADEADTREE:
            case node_t::SASKIAALPINETREE:
            case node_t::SASKIALEAFYTREE:
                draw_model(renderer, projection, view, xform, node.obj, 15, scene.sunlight, scene.globe);
                break;
            case node_t::REFLECTIVE_ORB:
                draw_reflective(renderer, renderer.view * xform, node.obj, node.texture, scene, node);
                break;
            case node_t::REFLECTIVE_SKYBOX_ORB:
                draw_reflective_skybox(renderer, renderer.view * xform, node.obj, node.texture, scene, node);
                break;
        }

        for (node_t &c: node.children) {
            renderables.push({c, xform});
        }
    }
}

void render(renderer_t &renderer, scene_t &scene, framebuffer_t &source) {
    glBindFramebuffer(GL_FRAMEBUFFER, source.fbo);
    render(renderer, scene);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void post_process(GLuint &renderer_program, no_obj_model_t &screen_quad, framebuffer_t &target) {
    auto &[mesh, mat] = screen_quad;


    glBindFramebuffer(GL_FRAMEBUFFER, target.fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);
    glUseProgram(renderer_program);
    glBindTexture(GL_TEXTURE_2D, mat.texture);

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glDrawElements(GL_TRIANGLES, mesh.nverts, INDEX_TYPE, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void post_process(GLuint &renderer_program, no_obj_model_t &screen_quad) {
    auto &[mesh, mat] = screen_quad;


    // explicitly use the screen framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);

    glUseProgram(renderer_program);
    glBindTexture(GL_TEXTURE_2D, mat.texture);

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

    glDrawElements(GL_TRIANGLES, mesh.nverts, INDEX_TYPE, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}