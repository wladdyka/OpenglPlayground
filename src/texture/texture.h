#pragma once

#include <GL/glew.h>
#include <stb_image.h>

class Texture {
public:
    Texture();
    explicit Texture(const char* fileLocation);
    ~Texture();

    bool LoadTexture();
    bool LoadTextureAlpha();

    void UseTexture();
    void ClearTexture();

private:
    GLuint mTextureId{};
    int mWidth{}, mHeight{}, mBitDepth{};
    const char* mFileLocation{};
};
