//
// Created by pizza on 31/10/2021.
//

#include <ass2/renderer.hpp>

#include <stack>


#include <chicken3421/chicken3421.hpp>
#include <iostream>

const char *VERT_PATH = "res/shaders/shader.vert";
const char *FRAG_PATH = "res/shaders/shader.frag";

renderer_t make_renderer(const glm::mat4 &projection) {
    GLuint vs = chicken3421::make_shader(VERT_PATH, GL_VERTEX_SHADER);
    GLuint fs = chicken3421::make_shader(FRAG_PATH, GL_FRAGMENT_SHADER);

    GLuint render_program = chicken3421::make_program(vs, fs);

    chicken3421::delete_shader(vs);
    chicken3421::delete_shader(fs);

    GLint p_loc = chicken3421::get_uniform_location(render_program, "p");
    GLint v_loc = chicken3421::get_uniform_location(render_program, "v");
    GLint m_loc = chicken3421::get_uniform_location(render_program, "m");


    GLint tf_loc = chicken3421::get_uniform_location(render_program, "uTexFactor");
    GLint col_loc = chicken3421::get_uniform_location(render_program, "uColor");

    GLint sun_loc = chicken3421::get_uniform_location(render_program, "sun_pos");

    GLint sun_amb_loc = chicken3421::get_uniform_location(render_program, "u_sun_ambient");
    GLint sun_diff_loc = chicken3421::get_uniform_location(render_program, "u_sun_diffuse");
    GLint sun_spec_loc = chicken3421::get_uniform_location(render_program, "u_sun_specular");

    GLint p_pos_loc = chicken3421::get_uniform_location(render_program, "p_position");

    GLint p_amb_loc = chicken3421::get_uniform_location(render_program, "p_ambient");
    GLint p_diff_loc = chicken3421::get_uniform_location(render_program, "p_diffuse");
    GLint p_spec_loc = chicken3421::get_uniform_location(render_program, "p_specular");

    GLint shine_loc = chicken3421::get_uniform_location(render_program, "shininess");


    return renderer_t{projection,
                      render_program,
                      p_loc, v_loc, m_loc,
                      tf_loc, col_loc,
                      sun_loc, sun_amb_loc, sun_diff_loc, sun_spec_loc,
                      p_pos_loc, p_amb_loc, p_diff_loc, p_spec_loc,
                      shine_loc
//                      sun_loc, sun_amb_loc, sun_diff_loc, shine_loc
    };
}

void draw(const renderer_t &renderer, const glm::mat4 &p, const glm::mat4 &v, const glm::mat4 &m, const model_t &model) {
    for (const mesh_t &mesh: model.meshes) {

        if (mesh.material_id != -1) {
            const material_t &mtl = model.materials[mesh.material_id];
            glBindTexture(GL_TEXTURE_2D, mtl.texture);
            glUniform1f(renderer.tf_loc, mtl.texture == 0 ? 0.f : 1.f);
            glUniform4fv(renderer.col_loc, 1, glm::value_ptr(mtl.color));
        } else {
            // default material
            glUniform1f(renderer.tf_loc, 0.f);
            glUniform4f(renderer.col_loc, 1, 1, 1, 1);
        }

        // bind the shineiness scale for specular lighting
        glUniform1f(renderer.shine_loc, 15.0f);
        // bind projection, view and matrix
        glUniformMatrix4fv(renderer.p_loc, 1, GL_FALSE, glm::value_ptr(p));
        glUniformMatrix4fv(renderer.v_loc, 1, GL_FALSE, glm::value_ptr(v));
        glUniformMatrix4fv(renderer.m_loc, 1, GL_FALSE, glm::value_ptr(m));

        // bind directional lighting paramaters

        glBindVertexArray(mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glDrawArrays(GL_TRIANGLES, 0, mesh.nverts);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void render(const renderer_t &renderer, const scene_t &scene) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUniform1i(renderer.tf_loc, 0);
    glUniform1i(renderer.col_loc, 1);

    glUseProgram(renderer.program);

    glUniform3fv(renderer.sun_loc, 1, glm::value_ptr(scene.globe.pos));
    glUniform3fv(renderer.sun_amb_loc, 1, glm::value_ptr(scene.sunlight.ambient));
    glUniform3fv(renderer.sun_diff_loc, 1, glm::value_ptr(scene.sunlight.diffuse));
    glUniform3fv(renderer.sun_spec_loc, 1, glm::value_ptr(scene.sunlight.specular));

    glUniform3fv(renderer.p_position_loc, 1, glm::value_ptr(scene.globe.pos));
    glUniform3fv(renderer.p_ambient_loc, 1, glm::value_ptr(scene.globe.ambient));
    glUniform3fv(renderer.p_diffuse_loc, 1, glm::value_ptr(scene.globe.diffuse));
    glUniform3fv(renderer.p_specular_loc, 1, glm::value_ptr(scene.globe.specular));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);

    auto projection = glm::perspective(glm::radians(scene.cam.fov), 1920.0f / 1080.0f, 0.1f, 1000.0f);
    glm::mat4 p = glm::perspective(glm::radians(scene.cam.fov), 1920.0f / 1080.0f, 0.1f, 1000.0f);;
    glm::mat4 v = get_view(scene.cam);
    const glm::mat4 &vp = p * get_view(scene.cam);
    // an iterative approach allows for certain optimisations like tail-recursion not used here.
    // also, recursion of arbitrary depth is impossible, so this approach is more robust.
    std::stack<std::pair<const node_t &, glm::mat4>> renderables{{{scene.root, scene.root.transform}}};



    while (!renderables.empty()) {
        const auto &[node, parent_tf] = renderables.top();  // this is called a structured binding
        renderables.pop();
        auto rotate = glm::rotate(node.rotation, node.rotation_angle, node.rotation_axis);
        glm::mat4 xform = parent_tf * node.scale * node.transform * rotate;
        draw(renderer, p, v, xform, node.obj);

        for (const node_t &c: node.children) {
            renderables.push({c, xform});
        }
    }
}
