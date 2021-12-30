//
// Created by pizza on 31/10/2021.
//

#include <ass2/Model.hpp>
#include <tiny_obj_loader.h>

#include <chicken3421/chicken3421.hpp>
#include <iostream>

mesh_t make_mesh(
        const std::vector<glm::vec4> &positions,
        const std::vector<glm::vec3> &normals,
        const std::vector<glm::vec2> &texcoords,
        int material_id
) {
    GLuint vao = chicken3421::make_vao();
    glBindVertexArray(vao);

    GLuint vbo = chicken3421::make_buffer();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLsizeiptr pos_sz = positions.size() * sizeof(glm::vec4);
    GLsizeiptr norm_size = normals.size() * sizeof(glm::vec3);
    GLsizeiptr tc_sz = texcoords.size() * sizeof(glm::vec2);
    glBufferData(
            GL_ARRAY_BUFFER,
            pos_sz + norm_size + tc_sz,
            nullptr,
            GL_STATIC_DRAW
    );
    glBufferSubData(GL_ARRAY_BUFFER, 0, pos_sz, positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, pos_sz, norm_size, normals.data());
    glBufferSubData(GL_ARRAY_BUFFER, pos_sz + norm_size, tc_sz, texcoords.data());

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void *) nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) pos_sz);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *) (pos_sz + norm_size));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return mesh_t{vao, vbo, (GLuint) positions.size(), material_id};
}

GLuint load_texture(const std::string &path) {
    chicken3421::image_t teximg = chicken3421::load_image(path);
    GLint fmt = teximg.n_channels == 3 ? GL_RGB : GL_RGBA;

    GLuint tex = chicken3421::make_texture();
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, fmt, teximg.width, teximg.height, 0, fmt, GL_UNSIGNED_BYTE, teximg.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    chicken3421::delete_image(teximg);

    return tex;
}

model_t load_obj(const std::string &path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string str_warn, str_err;
    std::string mtl_base = path.substr(0, path.find_last_of('/') + 1);

    bool did_load = tinyobj::LoadObj(&attrib, &shapes, &materials, &str_warn, &str_err, path.c_str(), mtl_base.c_str());
    chicken3421::expect(did_load && str_warn.empty() && str_err.empty(), str_warn + str_err);

    // load the meshes
    std::vector<mesh_t> meshes;
    for (const tinyobj::shape_t &shape: shapes) {
        // every 3 indices makes a face, quads etc.
        // In .obj files, the vertices are triangulated by default
        std::vector<glm::vec4> positions;
        std::vector<glm::vec2> texcoords;
        std::vector<glm::vec3> normals;
        std::cout << std::size(shape.mesh.indices) << std::endl;
        for (const tinyobj::index_t &index: shape.mesh.indices) {
            float *pos = &attrib.vertices[3 * index.vertex_index];

            float *norms = &attrib.normals[3 * index.normal_index];
            positions.emplace_back(pos[0], pos[1], pos[2], 1);
            if (index.texcoord_index >= 0) {
                float *tc = &attrib.texcoords[2 * index.texcoord_index];
                texcoords.emplace_back(tc[0], tc[1]);
            }
            if (index.normal_index == -1) {
                normals.emplace_back(0, 0, 0);
            } else {
                normals.emplace_back(norms[0], norms[1], norms[2]);
            }
        }

        int material = shape.mesh.material_ids.empty() ? -1 : shape.mesh.material_ids[0];

        meshes.emplace_back(make_mesh(positions, normals, texcoords, material));
    }

    // load the materials
    std::vector<material_t> mats;
    for (const tinyobj::material_t &mat: materials) {

        GLuint tex = mat.diffuse_texname.empty() || mat.diffuse_texname == "." ? 0 : load_texture(
                mtl_base + mat.diffuse_texname);
        glm::vec4 col = {mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 1};
        mats.emplace_back(material_t{tex, col});
    }

    return model_t{meshes, mats};
}

void destroy_obj(model_t &m) {
    for (mesh_t &mesh: m.meshes) {
        chicken3421::delete_vao(mesh.vao);
        chicken3421::delete_buffer(mesh.vbo);

        mesh.vbo = mesh.vao = 0;
        mesh.nverts = 0;
    }

    for (material_t &mat: m.materials) {
        chicken3421::delete_texture(mat.texture);
        mat.texture = 0;
    }
}