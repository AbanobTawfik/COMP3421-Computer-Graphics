//
// Created by pizza on 31/10/2021.
//

#include <ass3/Model.hpp>
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

    bool did_load = tinyobj::LoadObj(&attrib, &shapes, &materials, &str_warn, &str_err, path.c_str(),
                                     mtl_base.c_str());
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

no_obj_model_t make_screen_quad(GLuint scene_tex) {
    glm::vec4 positions[] = {
            {-1, -1, 0, 1},
            {1,  -1, 0, 1},
            {1,  1,  0, 1},
            {-1, 1,  0, 1}
    };

    glm::vec2 texcoords[] = {
            {0, 0},
            {1, 0},
            {1, 1},
            {0, 1}
    };

    const auto n_verts = 6;
    index_t indices[] = {
            0, 1, 2,
            0, 2, 3
    };

    GLuint vao = chicken3421::make_vao();
    glBindVertexArray(vao);

    GLuint vbo = chicken3421::make_buffer();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(texcoords), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(texcoords), texcoords);

    GLuint ibo = chicken3421::make_buffer();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *) sizeof(positions));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return {{vao, vbo, ibo, n_verts},
            {scene_tex}};
}

GLuint make_cubemap(const std::string &base_path, const std::string &extension) {
    static const char *suffixes[] = {"right", "left", "top", "bottom", "front", "back"};

    GLuint cubemap = chicken3421::make_texture();
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

    for (int side = 0; side < 6; side++) {
        std::string path = base_path;
        path += suffixes[side];
        path += extension; // a very C++ sort of thing
        chicken3421::image_t image = chicken3421::load_image(path, false);
        GLenum fmt = image.n_channels == 3 ? GL_RGB : GL_RGBA;

        GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + side;
        glTexImage2D(face, 0, fmt, image.width, image.height, 0, fmt, GL_UNSIGNED_BYTE, image.data);
        chicken3421::delete_image(image);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return cubemap;
}

GLuint make_realtime_cubemap(){
    GLuint cubemap = chicken3421::make_texture();
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    for(int i = 0; i < 6; i++){
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, 1024, 1024, 0,GL_RGBA, cubemap, nullptr);
    }
    // we might need to change 0 -> cubemap to bind texture properly?
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return cubemap;

}


no_obj_model_t make_skybox(const std::string &base_path, const std::string &extension) {
    const static glm::vec4 verts[] = {
            {-1, -1, 1,  1}, // LBF: 0
            {1,  -1, 1,  1}, // RBF: 1
            {1,  1,  1,  1}, // RTF: 2
            {-1, 1,  1,  1}, // LTF: 3

            {1,  -1, -1, 1}, // RBB: 4
            {-1, -1, -1, 1}, // LBB: 5
            {-1, 1,  -1, 1}, // LTB: 6
            {1,  1,  -1, 1}, // RTB: 7
    };

    const static index_t indices[] = {
            0, 1, 2,
            0, 2, 3,

            1, 4, 7,
            1, 7, 2,

            4, 5, 6,
            4, 6, 7,

            5, 0, 3,
            5, 3, 6,

            3, 2, 7,
            3, 7, 6,

            5, 4, 1,
            5, 1, 0
    };

    GLuint vao = chicken3421::make_vao();
    glBindVertexArray(vao);

    GLuint vbo = chicken3421::make_buffer();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    GLuint ibo = chicken3421::make_buffer();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    no_obj_mesh_t mesh = {vao, vbo, ibo, 36};
    no_obj_material mat = {make_cubemap(base_path, extension)};

    return {mesh, mat};
}


model_t make_sphere() {
    int stacks = 16;
    int slices = 32;

    struct sphere_vertex_t {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 texco;
    };

    std::vector<sphere_vertex_t> sverts;
    for (int i = 0; i <= stacks; ++i) {
        double phi = glm::pi<double>() * i / (double) stacks;

        for (int j = 0; j <= slices; ++j) {
            double theta = 2 * glm::pi<double>() * j / (double) slices;

            glm::vec3 p = { std::cos(theta) * std::sin(phi), std::cos(phi), std::sin(theta) * std::sin(phi) };
            glm::vec3 normal = glm::normalize(p);
            glm::vec2 tc = { (float) -j / (float) slices, (float) i / (float) stacks };

            sverts.push_back(sphere_vertex_t { p, normal, tc });
        }
    }

    std::vector<glm::vec4> positions;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec3> normals;

    // sorry
    auto push_vert = [&](int i) {
        const auto& v = sverts[i];
        positions.push_back(glm::vec4(v.pos,1));
        texcoords.push_back(v.texco);
        normals.push_back(v.normal);
    };
    for (int i = 0; i < slices * stacks + stacks; ++i) {
        push_vert(i);
        push_vert(i + slices + 1);
        push_vert(i + slices);

        push_vert(i + slices + 1);
        push_vert(i);
        push_vert(i + 1);
    }

    std::vector<mesh_t> meshes;
    meshes.push_back(make_mesh(positions, normals, texcoords, 0));

    std::vector<material_t> materials;
    materials.push_back(material_t { 0, glm::vec4(1, 1, 1, 1) });

    return { meshes, materials };
}
