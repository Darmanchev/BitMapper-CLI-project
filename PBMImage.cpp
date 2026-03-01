// PBMImage.cpp
#include "PBMImage.h"
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <cstdlib>

PBMImage::PBMImage(const std::string& path) {
    load(path);
}

void PBMImage::load(const std::string& path) {
    filename = path;
    std::ifstream in(path.c_str());
    if (!in.is_open())
        throw std::runtime_error("Cannot open PBM file: " + path);

    in >> format;
    if (format != "P1")
        throw std::runtime_error("Unsupported PBM format (only P1 supported): " + format);

    in >> std::ws;
    while (in.peek() == '#') {
        std::string dummy;
        std::getline(in, dummy);
        in >> std::ws;
    }

    in >> width >> height;
    if (!in || width == 0 || height == 0)
        throw std::runtime_error("Invalid PBM dimensions.");

    pixels.assign(height, std::vector<bool>(width, false));
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            int val;
            in >> val;
            if (!in || (val != 0 && val != 1))
                throw std::runtime_error("Invalid pixel value in PBM file.");
            pixels[i][j] = (val == 1);
        }
    }
}

void PBMImage::save(const std::string& path) const {
    std::ofstream out(path.c_str());
    if (!out)
        throw std::runtime_error("Cannot write to file: " + path);

    out << "P1\n";
    out << width << " " << height << "\n";
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            out << (pixels[i][j] ? "1 " : "0 ");
        }
        out << "\n";
    }
}

void PBMImage::grayscale() {
}

void PBMImage::monochrome() {
}

void PBMImage::negative() {
    for (auto& row : pixels) {
        for (size_t j = 0; j < row.size(); ++j) {
            row[j] = !row[j];
        }
    }
}

void PBMImage::rotateLeft() {
    std::vector<std::vector<bool>> rotated(width, std::vector<bool>(height, false));
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            rotated[width - j - 1][i] = pixels[i][j];
        }
    }
    pixels = rotated;
    std::swap(width, height);
}

void PBMImage::rotateRight() {
    std::vector<std::vector<bool>> rotated(width, std::vector<bool>(height, false));
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            rotated[j][height - i - 1] = pixels[i][j];
        }
    }
    pixels = rotated;
    std::swap(width, height);
}

void PBMImage::flipHorizontal() {
    for (auto& row : pixels) {
        size_t left = 0;
        size_t right = row.size() > 0 ? row.size() - 1 : 0;
        while (left < right) {
            bool tmp = row[left];
            row[left] = row[right];
            row[right] = tmp;
            ++left;
            --right;
        }
    }
}

void PBMImage::flipVertical() {
    size_t top = 0;
    size_t bottom = pixels.size() > 0 ? pixels.size() - 1 : 0;
    while (top < bottom) {
        pixels[top].swap(pixels[bottom]);
        ++top;
        --bottom;
    }
}

void PBMImage::paste(const Image& srcImg, size_t posX, size_t posY) {
    const PBMImage* src = dynamic_cast<const PBMImage*>(&srcImg);
    if (!src) {
        throw std::runtime_error("PBMImage::paste: source image is not PBM format");
    }

    size_t newWidth = (posX + src->width) > width ? (posX + src->width) : width;
    size_t newHeight = (posY + src->height) > height ? (posY + src->height) : height;

    if (newWidth > width || newHeight > height) {
        std::vector<std::vector<bool>> extended(newHeight, std::vector<bool>(newWidth, false));
        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                extended[i][j] = pixels[i][j];
            }
        }
        pixels = extended;
        width = newWidth;
        height = newHeight;
    }

    for (size_t i = 0; i < src->height; ++i) {
        for (size_t j = 0; j < src->width; ++j) {
            pixels[posY + i][posX + j] = src->pixels[i][j];
        }
    }
}

std::string PBMImage::getName() const {
    return filename;
}

std::string PBMImage::getFormat() const {
    return "PBM";
}
