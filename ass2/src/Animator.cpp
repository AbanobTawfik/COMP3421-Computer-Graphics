#include <ass2/animator.hpp>

#include <iostream>

#include <cmath>
#include <stack>
#include <vector>
#include <ass2/Scene.hpp>

// Repeat each animation every 10 seconds
const float PERIOD = 5.f;
float rotation_angle = 0.0f;
float vaporean_rotation = 90.0f;
float charizard_rotation = 50.0f;

/**
 * Following De Casteljau's algorithm, after linearly interpreting the 4 control points, we end up with this cubic eqn.
 * Easily derivable by hand.
 * The coefficients come from the binomial equation, but the nomenclature also calls these "Bernstein polynomials"
 * If you know Pascal's Triangle, it is easy to compute, though.
 *
 * @param p A vector of 4 control points
 * @param t The current interpolation value. Must be between 0 and 1.
 * @return The point along the Bezier curve at time t.
 */
glm::vec3 cubic(const std::vector<glm::vec3> &p, float t) {
    float s = 1 - t;
    auto pos = (s * s * s * p[0]) + (3 * t * s * s * p[1]) + (3 * t * t * s * p[2]) + (t * t * t * p[3]);
    return pos;
}

/**
 * Calculate a transformation matrix based upon a Bezier cubic.
 *
 * @param bspline The Bezier curve to use.
 * @param now The time in seconds
 * @return A transformation matrix
 */
glm::mat4 animate(const bspline_t &bspline, float now) {
    // clamps between 0 and 10.
    double cycle = std::fmod(now, PERIOD);

    // [0, 5) is the first segment, [5, 10) is the second
    const std::vector<glm::vec3> &segment = cycle < PERIOD / 2.f ? bspline.s1 : bspline.s2;

    // clamps between 0 and 1
    float t = 2 * std::fmod(cycle, PERIOD/2) / PERIOD;

    // get the translation value
    auto tr = cubic(segment, (float)t);

    return glm::translate(glm::mat4(1), tr);
}

/**
 * Calculate a transformation matrix using a parameterised lemniscate.
 * Here mainly for comparison with the corresponding BSpline.
 * @param lemni A tag type to represent a lemniscate
 * @param now The time now in seconds
 * @return A transformation matrix
 */
glm::mat4 animate(float now) {
    auto t = now;

    auto denom = 1 + std::sin(t) * std::sin(t);
    auto a = 7.5f;
    auto x = (a * a * std::cos(t)) / denom;
    auto y = 0;
    auto z = (a * a * std::sin(t) * std::cos(t)) / denom;

    return glm::translate(glm::mat4(1), glm::vec3{ x, y, z });
}


animator_t make_animator() {

    std::vector<glm::vec3> s1 = {
            {-0.002, 0.001, -0.0002},
            { 0,    0.003, 0.003 },
            { 0.001,    0.001, 0.003 },
            { 0.002,    0.001, 0 },
    };

    std::vector<glm::vec3> s2 = {
            { 0.002,    0.001, 0 },
            { 0.002,    0.001, -0.003 },
            { -0.001,    0.003, -0.0055 },
            { -0.002,    0.001, -0.0002 },
    };

    /*
     * This is a correction factor since the original control points assume a smaller curve
     * This is not the only way to do this; alternatively, one could calculate new control points.
     * For thr purposes of demonstration, this way is obviously quicker to show
     */
    float scale = 40.f;
    for (glm::vec3 &cp : s1) {
        cp *= scale;
    }

    for (glm::vec3 &cp : s2) {
        cp *= scale;
    }

    std::vector<glm::vec3> s3 = {
            {-0.002, 0.001, -0.0002},
            { 0,    0.003, 0.003 },
            { 0.001,    0.001, 0.003 },
            { 0.002,    0.001, 0 }
    };

    std::vector<glm::vec3> s4 = {
            { 0.002,    0.001, 0 },
            { 0.002,    0.001, -0.003 },
            { -0.001,    0.003, -0.0055 },
            { -0.002,    0.001, -0.0002 }
    };
    for (glm::vec3 &cp : s3) {
        cp *= scale*5;
    }

    for (glm::vec3 &cp : s4) {
        cp *= scale*5;
    }
    return animator_t{ bspline_t{ s1, s2 }, bspline_t {s3, s4}};
}

void animate(const animator_t &animator, scene_t &scene, float now) {
    std::stack<node_t *> animatables{{ &scene.root }};
    while (!animatables.empty()) {
        node_t *n = animatables.top();
        animatables.pop();

        rotation_angle = rotation_angle + 0.0005;
        auto x_rotation_vaporean = -28.0f*sin(rotation_angle);
        auto z_rotation_vaporean = -28.0f*cos(rotation_angle);

        auto x_rotation_charizard = 155.0f*sin(rotation_angle);
        auto z_rotation_charizard = 155.0f*cos(rotation_angle);

        auto x_rotation_pikachu = 16.46f*sin(rotation_angle + 7.3f);
        auto z_rotation_pikachu = 16.46f*cos(rotation_angle + 7.3f);

        auto x_rotation_bulb = 20.0f*sin(rotation_angle + 8.4f);
        auto z_rotation_bulb = 20.0f*cos(rotation_angle + 8.4f);

        auto x_rotation_jiggly = 800.0f*sin(rotation_angle + 4.4f);
        auto z_rotation_jiggly = 800.0f*cos(rotation_angle + 4.4f);

        auto x_rotation_snorlax = 1650.0f*sin(rotation_angle + 5.4f);
        auto z_rotation_snorlax = 1650.0f*cos(rotation_angle + 5.4f);

        switch (n->kind) {
        case node_t::WATER:
                n->transform = animate(animator.circle_spline, now);
                break;
        case node_t::LEAF:
                n->transform = animate(animator.rotational_spline, now);
                break;
        case node_t::VAPOREON:
                n->transform = glm::translate(glm::mat4(1), {x_rotation_vaporean, -2.9f, z_rotation_vaporean});
                n->rotation_angle = rotation_angle;
                break;
        case node_t::CHARIZARD:
                n->transform = glm::translate(glm::mat4(1), {x_rotation_charizard, -15.9f, z_rotation_charizard});
                n->rotation_angle = glm::radians(180.0f) + rotation_angle;
                break;
        case node_t::PIKACHU:
            n->transform = glm::translate(glm::mat4(1), {x_rotation_pikachu, -1.7f, z_rotation_pikachu});
            n->rotation_angle = glm::radians(250.0f) + rotation_angle;
            break;
        case node_t::BULB:
            n->transform = glm::translate(glm::mat4(1), {x_rotation_bulb, -1.7f, z_rotation_bulb});
            n->rotation_angle = glm::radians(300.0f) + rotation_angle;
            break;
        case node_t::JIGGLY:
            n->transform = glm::translate(glm::mat4(1), {x_rotation_jiggly, -80.0f, z_rotation_jiggly});
            n->rotation_angle = glm::radians(75.0f) + rotation_angle;
            break;
        case node_t::SNORLAX:
            n->transform = glm::translate(glm::mat4(1), {x_rotation_snorlax, -180.0f, z_rotation_snorlax});
            n->rotation_angle = glm::radians(135.0f) + rotation_angle;
            break;
        default:
            break;
        }

        for (node_t &c: n->children) {
            animatables.push(&c);
        }
    }
}
