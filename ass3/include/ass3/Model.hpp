//
// Created by pizza on 31/10/2021.
//

#ifndef COMP3421_ASSIGNMENT_2_MODEL_HPP
#define COMP3421_ASSIGNMENT_2_MODEL_HPP

#include <vector>
#include <string>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>


using index_t = GLuint;
const GLenum INDEX_TYPE = GL_UNSIGNED_INT;


/**
 * A struct that represents the raw geometry of a model.
 * Does not contain all the usual graphics things, yet.
 */


struct mesh_t {
    GLuint vao;
    GLuint vbo;
    GLuint nverts;
    int material_id;
};

struct no_obj_mesh_t {
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
    int nverts;
};


struct vertex_t {
    glm::vec3 pos;
    glm::vec3 nrm;
    glm::vec2 tc;
};


struct no_obj_material {
    GLuint texture;
};

/**
 * A struct that represents what a mesh is "made out of"
 * This can be colours, textures, and even more...
 */
struct material_t {
    GLuint texture;
    glm::vec4 color;
};

/**
 * A representation of a model loaded from an artist's tool
 */
struct model_t {
    std::vector<mesh_t> meshes;
    std::vector<material_t> materials;
};

struct no_obj_model_t {
    no_obj_mesh_t mesh;
    no_obj_material material;
};

struct sphere_model_t {
    std::vector<no_obj_mesh_t> meshes;
    std::vector<material_t> materials;
};

/**
 * Load a model from the .obj format
 *
 * @param path The path to the model's .obj file
 * @return A freshly initialised model, with everything uploaded to the GPU already
 */
model_t load_obj(const std::string &path);

/**
 * Destroy a model, freeing all of its GPU resources
 * @param m The model to destroy
 */
void destroy_obj(model_t &m);

struct material_param_t {
    std::string diff_map_path;
    std::vector<unsigned char> bytes{};
};

struct volume_param_t {
    float width = 1;
    float height = 1;
    float depth = 1;
    bool tex_cover = false;
};


model_t make_sphere();

no_obj_model_t make_skybox(const std::string &base_path, const std::string &extension = ".jpg");

/**
 * Creates a quad in NDC.
 *
 * A textured quad like this is good for applying post-processing effects, as we treat the scene as a simple texture
 *
 * @param scene_tex The texture of the scene that was rendered into.
 * @return A "model" representing the scene as a 2D quad.
 */
no_obj_model_t make_screen_quad(GLuint scene_tex);


#endif //COMP3421_ASSIGNMENT_2_MODEL_HPP
