#include <ass3/framebuffer.hpp>

#include <chicken3421/chicken3421.hpp>

framebuffer_t make_framebuffer(int width, int height) {
    framebuffer_t fbuff{};

    // create framebuffer
    fbuff.fbo = chicken3421::make_framebuffer();
    glBindFramebuffer(GL_FRAMEBUFFER, fbuff.fbo);

    // create texture for framebuffer color
    fbuff.texture = chicken3421::make_texture();
    glBindTexture(GL_TEXTURE_2D, fbuff.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // create renderbuffer for framebuffer depth/stencil
    fbuff.rbo = chicken3421::make_renderbuffer();
    glBindRenderbuffer(GL_RENDERBUFFER, fbuff.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    // attach texture and renderbuffer to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbuff.texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbuff.rbo);

    // reset state
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    return fbuff;
}

framebuffer_t make_framebuffer_cubemap(int width, int height) {
    framebuffer_t fbuff{};

    // create framebuffer
    fbuff.fbo = chicken3421::make_framebuffer();
    glBindFramebuffer(GL_FRAMEBUFFER, fbuff.fbo);

    // create texture for framebuffer color
    fbuff.texture = chicken3421::make_texture();
    auto cube_map_size = 1024;
    auto fmt = GL_RGBA;

    glBindTexture(GL_TEXTURE_CUBE_MAP, fbuff.texture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //NULL means reserve texture memory, but texels are undefined
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+0, 0, fmt, cube_map_size, cube_map_size, 0, fmt, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+1, 0, fmt, cube_map_size, cube_map_size, 0, fmt, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+2, 0, fmt, cube_map_size, cube_map_size, 0, fmt, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+3, 0, fmt, cube_map_size, cube_map_size, 0, fmt, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+4, 0, fmt, cube_map_size, cube_map_size, 0, fmt, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+5, 0, fmt, cube_map_size, cube_map_size, 0, fmt, GL_UNSIGNED_BYTE, NULL);

    // create renderbuffer for framebuffer depth/stencil
    fbuff.rbo = chicken3421::make_renderbuffer();
    glBindRenderbuffer(GL_RENDERBUFFER, fbuff.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    // attach texture and renderbuffer to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbuff.texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbuff.rbo);

    // reset state
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    return fbuff;
}

void delete_framebuffer(framebuffer_t &fbuff) {
    chicken3421::delete_framebuffer(fbuff.fbo);
    chicken3421::delete_renderbuffer(fbuff.rbo);
//    chicken3421::delete_texture(fbuff.texture);
}
