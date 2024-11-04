#include "texture.h"

Texture::Texture() {
}

Texture::Texture(const char *fileLocation) {
    mFileLocation = fileLocation;
}

Texture::~Texture() {
}

bool Texture::LoadTexture() {
    unsigned char* textureData = stbi_load(mFileLocation, &mWidth, &mHeight, nullptr, 0);
    if (!textureData) {
        printf("Failed to load texture from %s\n", mFileLocation);
        return false;
    }

    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(textureData);
    return true;
}

bool Texture::LoadTextureAlpha() {
    unsigned char* textureData = stbi_load(mFileLocation, &mWidth, &mHeight, nullptr, 0);
    if (!textureData) {
        printf("Failed to load texture from %s\n", mFileLocation);
        return false;
    }

    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(textureData);
    return true;
}

void Texture::UseTexture() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
}

void Texture::ClearTexture() {
    glDeleteTextures(1, &mTextureId);
    mTextureId = 0;
    mWidth = 0;
    mHeight = 0;
    mBitDepth = 0;
}
