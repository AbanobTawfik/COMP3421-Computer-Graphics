// C++17 doesn't have std::pi
#define _USE_MATH_DEFINES

#include <cmath>

#include <cstdlib>
#include <chrono>

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <GLFW/glfw3.h>

#include <chicken3421/chicken3421.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

struct image_t {
    int width;
    int height;
    int n_channels;
    void *data;
};

image_t load_image(const std::string &filename) {
    image_t img;

    stbi_set_flip_vertically_on_load(true);
    img.data = stbi_load(filename.data(), &img.width, &img.height, &img.n_channels, 0);
    chicken3421::expect(img.data, "Could not read " + filename);

    return img;
}


void delete_image(image_t &img) {
    stbi_image_free(img.data);

    img.data = nullptr;
    img.width = 0;
    img.height = 0;
    img.n_channels = 0;
}


GLuint make_texture(const std::string &path1) {
    // load the texture into cpu memory first
    auto img = load_image(path1);
    // load into the VRAM (GPU Memory)
    auto format = img.n_channels == 4 ? GL_RGBA : GL_RGB;
    GLuint tex;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);


    // this is the line that puts image into gpu memory
    glTexImage2D(GL_TEXTURE_2D, 0, format, img.width, img.height, 0, format, GL_UNSIGNED_BYTE, img.data);
    // clean up, removes the global variable tex
    delete_image(img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    return tex;
}


struct vertex_t {
    glm::vec4 pos;
    glm::vec2 tex_coords; // a.k.a 'UV coords or ST coords'
};

struct rect_t {
    GLuint vao;
    GLuint vbo;
    std::vector<vertex_t> vertices;

};

void activate_texture(GLuint texture) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void render(const rect_t &mesh, glm::mat3 matrix, GLuint program) {
    glUseProgram(program);

    auto loc_u_matrix = glGetUniformLocation(program, "u_matrix");

    glUniformMatrix3fv(loc_u_matrix, 1, GL_FALSE, glm::value_ptr(matrix));

    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLint>(mesh.vertices.size()));
    glBindVertexArray(0);
}

rect_t make_shape(std::vector<vertex_t> verts) {
    // top-left
    GLuint vao;
    glGenVertexArrays(1, &vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, (GLintptr) (sizeof(vertex_t) * verts.size()), verts.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *) (0 + offsetof(vertex_t, pos)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *) (0 + offsetof(vertex_t, tex_coords)));

    return {vao, vbo, verts};
}

void preload_textures() {
    std::cout << "PRELOADING GIF 1/7 ....." << std::endl;
    for (int i = 0; i < 173; i++) {
        auto tmp = load_image("res/img/car_shearer/" + std::to_string(i) + ".png");
    }
    std::cout << "PRELOADING GIF 2/7 ....." << std::endl;
    for (int i = 0; i < 158; i++) {
        auto tmp = load_image("res/img/catjam/" + std::to_string(i) + ".png");
    }
    std::cout << "PRELOADING GIF 3/7 ....." << std::endl;
    for (int i = 0; i < 174; i++) {
        auto tmp = load_image("res/img/coffin_dance/" + std::to_string(i) + ".png");
    }
    std::cout << "PRELOADING GIF 4/7 ....." << std::endl;
    for (int i = 0; i < 125; i++) {
        auto tmp = load_image("res/img/dance/" + std::to_string(i) + ".png");
    }
    std::cout << "PRELOADING GIF 5/7 ....." << std::endl;
    for (int i = 0; i < 173; i++) {
        auto tmp = load_image("res/img/drake/" + std::to_string(i) + ".png");
    }
    std::cout << "PRELOADING GIF 6/7 ....." << std::endl;
    for (int i = 0; i < 61; i++) {
        auto tmp = load_image("res/img/froge/" + std::to_string(i) + ".png");
    }
    std::cout << "PRELOADING GIF 7/7 ....." << std::endl;
    for (int i = 0; i < 100; i++) {
        auto tmp = load_image("res/img/orange_justice/" + std::to_string(i) + ".png");
    }

    auto tmp = load_image("res/img/disco2.png");
    tmp = load_image("res/img/spotlight.png");
    tmp = load_image("res/img/spotlight2.png");
    std::cout << "ALL GIFS PRELOADED ....." << std::endl;
}

int main() {
    std::cout << "PRELOADING TEXTURES PLEASE WAIT 5-10 SECONDS" << std::endl;
    preload_textures();
    auto win = chicken3421::make_opengl_window(1920, 1080, "Disco Disco");
    auto vs = chicken3421::make_shader("res/shaders/vert.glsl", GL_VERTEX_SHADER);
    auto fs = chicken3421::make_shader("res/shaders/frag.glsl", GL_FRAGMENT_SHADER);

    auto render_prog = chicken3421::make_program(vs, fs);

    chicken3421::delete_shader(vs);
    chicken3421::delete_shader(fs);

    glUseProgram(render_prog);

    auto before_car_shearer = std::chrono::high_resolution_clock::now();
    auto before_coffin_dance = std::chrono::high_resolution_clock::now();
    auto before_dance = std::chrono::high_resolution_clock::now();
    auto before_drake = std::chrono::high_resolution_clock::now();
    auto before_orange_justice = std::chrono::high_resolution_clock::now();
    auto before_catjam = std::chrono::high_resolution_clock::now();
    auto before_froge = std::chrono::high_resolution_clock::now();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto count_car_shearer = 126;
    auto count_coffin_dance = 0;
    auto count_dance = 0;
    auto count_drake = 0;
    auto count_orange_justice = 0;
    auto count_catjam = 0;
    auto count_froge = 0;

    auto background_width = 1.0;
    auto background_height = 1.0;
    std::vector<vertex_t> background_vertex = std::vector<vertex_t>{
            {{background_width,  background_height,  0, 1}, {1, 1}},             // top-right
            {{background_width,  -background_height, 0, 1}, {1, 0}},             // bottom-right
            {{-background_width, -background_height, 0, 1}, {0, 0}},             // bottom-left

            {{background_width,  background_height,  0, 1}, {1, 1}},             // top-right
            {{-background_width, -background_height, 0, 1}, {0, 0}},             // bottom-left
            {{-background_width, background_height,  0, 1}, {0, 1}},             // top-left
    };

    auto car_shearer_width = 0.15;
    auto car_shearer_height = 0.35;
    std::vector<vertex_t> car_shearer_vertex = std::vector<vertex_t>{
            {{car_shearer_width,  car_shearer_height,  0, 1}, {1, 1}},           // top-right
            {{car_shearer_width,  -car_shearer_height, 0, 1}, {1, 0}},           // bottom-right
            {{-car_shearer_width, -car_shearer_height, 0, 1}, {0, 0}},           // bottom-left

            {{car_shearer_width,  car_shearer_height,  0, 1}, {1, 1}},           // top-right
            {{-car_shearer_width, -car_shearer_height, 0, 1}, {0, 0}},           // bottom-left
            {{-car_shearer_width, car_shearer_height,  0, 1}, {0, 1}},           // top-left
    };

    auto coffin_dance_width = 0.35;
    auto coffin_dance_height = 0.40;
    std::vector<vertex_t> coffin_dance_vertex = std::vector<vertex_t>{
            {{coffin_dance_width,  coffin_dance_height,  0, 1}, {1, 1}},           // top-right
            {{coffin_dance_width,  -coffin_dance_height, 0, 1}, {1, 0}},           // bottom-right
            {{-coffin_dance_width, -coffin_dance_height, 0, 1}, {0, 0}},           // bottom-left

            {{coffin_dance_width,  coffin_dance_height,  0, 1}, {1, 1}},           // top-right
            {{-coffin_dance_width, -coffin_dance_height, 0, 1}, {0, 0}},           // bottom-left
            {{-coffin_dance_width, coffin_dance_height,  0, 1}, {0, 1}},           // top-left
    };

    auto dance_width = 0.26;
    auto dance_height = 0.40;
    std::vector<vertex_t> dance_vertex = std::vector<vertex_t>{
            {{dance_width,  dance_height,  0, 1}, {1, 1}},           // top-right
            {{dance_width,  -dance_height, 0, 1}, {1, 0}},           // bottom-right
            {{-dance_width, -dance_height, 0, 1}, {0, 0}},           // bottom-left

            {{dance_width,  dance_height,  0, 1}, {1, 1}},           // top-right
            {{-dance_width, -dance_height, 0, 1}, {0, 0}},           // bottom-left
            {{-dance_width, dance_height,  0, 1}, {0, 1}},           // top-left
    };

    auto drake_width = 0.26;
    auto drake_height = 0.50;
    std::vector<vertex_t> drake_vertex = std::vector<vertex_t>{
            {{drake_width,  drake_height,  0, 1}, {1, 1}},           // top-right
            {{drake_width,  -drake_height, 0, 1}, {1, 0}},           // bottom-right
            {{-drake_width, -drake_height, 0, 1}, {0, 0}},           // bottom-left

            {{drake_width,  drake_height,  0, 1}, {1, 1}},           // top-right
            {{-drake_width, -drake_height, 0, 1}, {0, 0}},           // bottom-left
            {{-drake_width, drake_height,  0, 1}, {0, 1}},           // top-left
    };

    auto orange_justice_width = 0.20;
    auto orange_justice_height = 0.45;
    std::vector<vertex_t> orange_justice_vertex = std::vector<vertex_t>{
            {{orange_justice_width,  orange_justice_height,  0, 1}, {1, 1}},           // top-right
            {{orange_justice_width,  -orange_justice_height, 0, 1}, {1, 0}},           // bottom-right
            {{-orange_justice_width, -orange_justice_height, 0, 1}, {0, 0}},           // bottom-left

            {{orange_justice_width,  orange_justice_height,  0, 1}, {1, 1}},           // top-right
            {{-orange_justice_width, -orange_justice_height, 0, 1}, {0, 0}},           // bottom-left
            {{-orange_justice_width, orange_justice_height,  0, 1}, {0, 1}},           // top-left
    };

    auto catjam_width = 0.20;
    auto catjam_height = 0.45;
    std::vector<vertex_t> catjam_vertex = std::vector<vertex_t>{
            {{catjam_width,  catjam_height,  0, 1}, {1, 1}},           // top-right
            {{catjam_width,  -catjam_height, 0, 1}, {1, 0}},           // bottom-right
            {{-catjam_width, -catjam_height, 0, 1}, {0, 0}},           // bottom-left

            {{catjam_width,  catjam_height,  0, 1}, {1, 1}},           // top-right
            {{-catjam_width, -catjam_height, 0, 1}, {0, 0}},           // bottom-left
            {{-catjam_width, catjam_height,  0, 1}, {0, 1}},           // top-left
    };
    auto froge_width = 0.20;
    auto froge_height = 0.45;
    std::vector<vertex_t> froge_vertex = std::vector<vertex_t>{
            {{froge_width,  froge_height,  0, 1}, {1, 1}},           // top-right
            {{froge_width,  -froge_height, 0, 1}, {1, 0}},           // bottom-right
            {{-froge_width, -froge_height, 0, 1}, {0, 0}},           // bottom-left

            {{froge_width,  froge_height,  0, 1}, {1, 1}},           // top-right
            {{-froge_width, -froge_height, 0, 1}, {0, 0}},           // bottom-left
            {{-froge_width, froge_height,  0, 1}, {0, 1}},           // top-left
    };

    auto spotlight_width = 0.20;
    auto spotlight_height = 0.45;
    std::vector<vertex_t> spotlight_vertex = std::vector<vertex_t>{
            {{spotlight_width,  spotlight_height,  0, 1}, {1, 1}},           // top-right
            {{spotlight_width,  -spotlight_height, 0, 1}, {1, 0}},           // bottom-right
            {{-spotlight_width, -spotlight_height, 0, 1}, {0, 0}},           // bottom-left

            {{spotlight_width,  spotlight_height,  0, 1}, {1, 1}},           // top-right
            {{-spotlight_width, -spotlight_height, 0, 1}, {0, 0}},           // bottom-left
            {{-spotlight_width, spotlight_height,  0, 1}, {0, 1}},           // top-left
    };

    auto spotlight2_width = 0.3;
    auto spotlight2_height = 0.6;
    std::vector<vertex_t> spotlight2_vertex = std::vector<vertex_t>{
            {{spotlight2_width,  spotlight2_height,  0, 1}, {1, 1}},           // top-right
            {{spotlight2_width,  -spotlight2_height, 0, 1}, {1, 0}},           // bottom-right
            {{-spotlight2_width, -spotlight2_height, 0, 1}, {0, 0}},           // bottom-left

            {{spotlight2_width,  spotlight2_height,  0, 1}, {1, 1}},           // top-right
            {{-spotlight2_width, -spotlight2_height, 0, 1}, {0, 0}},           // bottom-left
            {{-spotlight2_width, spotlight2_height,  0, 1}, {0, 1}},           // top-left
    };

    auto background = make_shape(background_vertex);
    auto spotlight = make_shape(spotlight_vertex);
    auto spotlight2 = make_shape(spotlight_vertex);
    auto car_shearer = make_shape(car_shearer_vertex);
    auto coffin_dance = make_shape(coffin_dance_vertex);
    auto dance = make_shape(dance_vertex);
    auto drake = make_shape(drake_vertex);
    auto orange_justice = make_shape(orange_justice_vertex);
    auto catjam = make_shape(catjam_vertex);
    auto froge = make_shape(froge_vertex);

    auto disco_tex = make_texture("res/img/disco2.png");
    auto spotlight_tex = make_texture("res/img/spotlight.png");
    auto spotlight2_tex = make_texture("res/img/spotlight2.png");

    auto coffin_dance_flip = false;
    auto dance_flip = false;
    auto drake_reverse = false;
    auto drake_radius = 0.3;
    auto drake_angle = 0;
    auto drake_rotation_timer = std::chrono::high_resolution_clock::now();
    auto catjam_radius = 0.2;
    auto catjam_angle = 0;
    auto catjam_rotation_timer = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        // background image
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.8, 0.8, 1, 1);
        glm::mat3 disco_matrix = {
                {1.0f, 0,    0},
                {0,    1.0f, 0},
                {0.0f, 0.0f, 1.0f}
        };
        activate_texture(disco_tex);
        render(background, disco_matrix, render_prog);

        glm::mat3 spotlight_matrix = {
                {1.0f, 0,      0},
                {0,    1.0f,   0},
                {0.0f, -0.65f, 1.0f}
        };
        activate_texture(spotlight_tex);
        render(spotlight, spotlight_matrix, render_prog);

        glm::mat3 spotlight2_matrix = {
                {2.0f,  0,     0},
                {0,     2.0f,  0},
                {0.01f, 0.65f, 1.0f}
        };
        activate_texture(spotlight2_tex);
        render(spotlight2, spotlight2_matrix, render_prog);

        glm::mat3 car_shearer_matrix = {
                {0.8f, 0,     0},
                {0,    0.8f,  0},
                {0.0f, 0.65f, 1.0f}
        };
        auto car_shearer_tex = make_texture("res/img/car_shearer/" + std::to_string(count_car_shearer) + ".png");
        activate_texture(car_shearer_tex);
        render(car_shearer, car_shearer_matrix, render_prog);

        auto coffin_dance_x_translation = coffin_dance_flip ? -0.3f +
                                                              (((-0.9 + 0.3) / 174) * ((174 - count_coffin_dance))) :
                                          -0.3f + ((-0.9 + 0.3) / 174) * count_coffin_dance;
        auto coffin_dance_y_translation = coffin_dance_flip ? 0.3f +
                                                              (((0.79 - 0.3) / 174) * ((174 - count_coffin_dance))) :
                                          0.3f + ((0.79 - 0.3) / 174) * count_coffin_dance;
        glm::mat3 coffin_dance_matrix = {
                {0.5f,                       0,                          0},
                {0,                          0.5f,                       0},
                {coffin_dance_x_translation, coffin_dance_y_translation, 1.0f}
        };
        auto coffin_dance_tex = make_texture("res/img/coffin_dance/" + std::to_string(count_coffin_dance) + ".png");
        activate_texture(coffin_dance_tex);
        render(coffin_dance, coffin_dance_matrix, render_prog);

        auto dance_x_translation = dance_flip ? 0.3f + (((0.92 - 0.3) / 124) * ((124 - count_dance))) : 0.3f +
                                                                                                        ((0.92 - 0.3) /
                                                                                                         124) *
                                                                                                        count_dance;
        auto dance_y_translation = dance_flip ? -0.2f + (((-0.8 + 0.2) / 124) * ((124 - count_dance))) : -0.2f +
                                                                                                         ((-0.8 + 0.2) /
                                                                                                          124) *
                                                                                                         count_dance;
        glm::mat3 dance_matrix = {
                {0.5f,                0,                   0},
                {0,                   0.5f,                0},
                {dance_x_translation, dance_y_translation, 1.0f}
        };
        auto dance_tex = make_texture("res/img/dance/" + std::to_string(count_dance) + ".png");
        activate_texture(dance_tex);
        render(dance, dance_matrix, render_prog);

        auto drake_x_translation = drake_reverse ? drake_radius * cosf(-1 * drake_angle / (3.1415 * 3)) - 0.6 :
                                   drake_radius * cosf(drake_angle / (3.1415 * 3)) - 0.6;
        auto drake_y_translation = drake_reverse ? drake_radius * sinf(-1 * drake_angle / (3.1415 * 3)) - 0.4 :
                                   drake_radius * sinf(drake_angle / (3.1415 * 3)) - 0.4;
        glm::mat3 drake_matrix = {
                {1.5f,                0,                   0},
                {0,                   0.6f,                0},
                {drake_x_translation, drake_y_translation, 1.0f}
        };
        auto drake_tex = make_texture("res/img/drake/" + std::to_string(count_drake) + ".png");
        activate_texture(drake_tex);
        render(drake, drake_matrix, render_prog);

        glm::mat3 orange_justice_matrix = {
                {0.5f, 0,      0},
                {0,    0.5f,   0},
                {0.0f, -0.65f, 0.0f}
        };
        auto orange_justice_tex = make_texture(
                "res/img/orange_justice/" + std::to_string(count_orange_justice) + ".png");
        activate_texture(orange_justice_tex);
        render(orange_justice, orange_justice_matrix, render_prog);

        auto catjam_x_translation = catjam_radius * cosf(catjam_angle * 3.14592 / 180) + 0.6;
        auto catjam_y_translation = catjam_radius * sinf(catjam_angle * 3.14592 / 180) + 0.45;
        glm::mat3 catjam_matrix = {
                {0.6f,                 0,                    0},
                {0,                    0.6f,                 0},
                {catjam_x_translation, catjam_y_translation, 0.0f}
        };
        auto catjam_tex = make_texture("res/img/catjam/" + std::to_string(count_catjam) + ".png");
        activate_texture(catjam_tex);
        render(catjam, catjam_matrix, render_prog);

        glm::mat3 froge_matrix = {
                {0.5f, 0.0f, 0},
                {0.0f, 0.5f, 0},
                {0.0f, 0.0f, 0.0f}
        };

        auto froge_tex = make_texture("res/img/froge/" + std::to_string(count_froge) + ".png");
        activate_texture(froge_tex);
        render(froge, froge_matrix, render_prog);


        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - before_car_shearer).count() >= 25) {
            count_car_shearer = (count_car_shearer == 172) ? 0 : count_car_shearer + 1;
            before_car_shearer = std::chrono::high_resolution_clock::now();
        }

        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - before_coffin_dance).count() >= 24) {
            count_coffin_dance = (count_coffin_dance == 173) ? 0 : count_coffin_dance + 1;
            coffin_dance_flip = (count_coffin_dance == 0) ? !coffin_dance_flip : coffin_dance_flip;
            before_coffin_dance = std::chrono::high_resolution_clock::now();
        }

        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - before_dance).count() >= 80) {
            count_dance = (count_dance == 124) ? 0 : count_dance + 1;
            dance_flip = (count_dance == 0) ? !dance_flip : dance_flip;
            before_dance = std::chrono::high_resolution_clock::now();
        }

        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - before_drake).count() >= 22) {
            count_drake = (count_drake == 213) ? 0 : count_drake + 1;
            before_drake = std::chrono::high_resolution_clock::now();
        }

        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - before_orange_justice).count() >= 25) {
            // different frame counters
            count_orange_justice = (count_orange_justice == 99) ? 0 : count_orange_justice + 1;
            before_orange_justice = std::chrono::high_resolution_clock::now();
        }

        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - before_catjam).count() >= 40) {
            // different frame counters
            count_catjam = (count_catjam == 157) ? 0 : count_catjam + 1;
            before_catjam = std::chrono::high_resolution_clock::now();
        }

        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - before_froge).count() >= 25) {
            // different frame counters
            count_froge = (count_froge == 60) ? 0 : count_froge + 1;
            before_froge = std::chrono::high_resolution_clock::now();
        }

        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - drake_rotation_timer).count() >= 100) {
            // different frame counters
            drake_angle = (drake_angle + 1) % 360;
            drake_reverse = drake_angle % 5 == 0 ? !drake_reverse : drake_reverse;
            drake_rotation_timer = std::chrono::high_resolution_clock::now();
        }

        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - drake_rotation_timer).count() >= 16.66) {
            // different frame counters
            catjam_angle = (catjam_angle + 1) % 360;
            catjam_rotation_timer = std::chrono::high_resolution_clock::now();
        }

        glDeleteTextures(1, &car_shearer_tex);
        glDeleteTextures(1, &coffin_dance_tex);
        glDeleteTextures(1, &dance_tex);
        glDeleteTextures(1, &drake_tex);
        glDeleteTextures(1, &orange_justice_tex);
        glDeleteTextures(1, &catjam_tex);
        glDeleteTextures(1, &froge_tex);

        glfwSwapBuffers(win);
    }


    return EXIT_SUCCESS;
}