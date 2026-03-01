#pragma once

#include "Image.h"
#include <string>
#include <vector>

class PBMImage : public Image {
private:
    std::string filename;
    std::string format;    
    size_t width;
    size_t height;
    std::vector<std::vector<bool>> pixels;

public:
    PBMImage(const std::string& path);
    ~PBMImage() override = default;

    void load(const std::string& path) override;
    void save(const std::string& path) const override;

    void grayscale() override;
    void monochrome() override;
    void negative() override;
    void rotateLeft() override;
    void rotateRight() override;
    void flipHorizontal() override;
    void flipVertical() override;

    void paste(const Image& src, size_t posX, size_t posY) override;

    std::string getName() const override;
    std::string getFormat() const override;
};
