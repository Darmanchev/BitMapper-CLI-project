#pragma once

#include <string>

class Image {
public:
    virtual ~Image() = default;

    virtual void load(const std::string& path) = 0;
    virtual void save(const std::string& path) const = 0;

    virtual void grayscale() = 0;
    virtual void monochrome() = 0;
    virtual void negative() = 0;
    virtual void rotateLeft() = 0;
    virtual void rotateRight() = 0;
    virtual void flipHorizontal() = 0;
    virtual void flipVertical() = 0;

    virtual void paste(const Image& src, size_t posX, size_t posY) = 0;

    virtual std::string getName() const = 0;
    virtual std::string getFormat() const = 0;
};
