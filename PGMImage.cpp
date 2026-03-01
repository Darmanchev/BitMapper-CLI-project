#include "PGMImage.h"
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <cstdlib>   

PGMImage::PGMImage(const std::string& path) {
    load(path);
}

void PGMImage::load(const std::string& path) {
    filename = path;
    std::ifstream in(path.c_str());
    if (!in.is_open())
        throw std::runtime_error("Cannot open PGM file: " + path);

    in >> format;
    if (format != "P2")
        throw std::runtime_error("Unsupported PGM format (only P2 supported): " + format);

    in >> std::ws;
    while (in.peek() == '#') {
        std::string comment;
        std::getline(in, comment);
        in >> std::ws;
    }

    in >> width >> height >> maxGray;
    if (!in || width == 0 || height == 0 || maxGray <= 0)
        throw std::runtime_error("Invalid PGM header.");

    pixels.assign(height, std::vector<int>(width, 0));

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            int val;
            in >> val;
            if (!in || val < 0 || val > maxGray)
                throw std::runtime_error("Invalid gray value in PGM file.");
            pixels[i][j] = val;
        }
    }
}

void PGMImage::save(const std::string& path) const {
    std::ofstream out(path.c_str());
    if (!out)
        throw std::runtime_error("Cannot write to file: " + path);

    out << "P2\n";
    out << width << " " << height << "\n";
    out << maxGray << "\n";
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            out << pixels[i][j] << " ";
        }
        out << "\n";
    }
}

void PGMImage::grayscale() {
}

void PGMImage::monochrome() {
    int threshold = maxGray / 2;
    for (auto& row : pixels) {
        for (int& val : row) {
            val = (val > threshold ? maxGray : 0);
        }
    }
}

void PGMImage::negative() {
    for (auto& row : pixels) {
        for (int& val : row) {
            val = maxGray - val;
        }
    }
}

void PGMImage::rotateLeft() {
    std::vector<std::vector<int>> rotated(width, std::vector<int>(height, 0));
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            rotated[width - j - 1][i] = pixels[i][j];
        }
    }
    pixels = rotated;
    std::swap(width, height);
}

void PGMImage::rotateRight() {
    std::vector<std::vector<int>> rotated(width, std::vector<int>(height, 0));
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            rotated[j][height - i - 1] = pixels[i][j];
        }
    }
    pixels = rotated;
    std::swap(width, height);
}

void PGMImage::flipHorizontal() {
    for (auto& row : pixels) {
        size_t left = 0;
        size_t right = (row.empty() ? 0 : row.size() - 1);
        while (left < right) {
            int tmp = row[left];
            row[left] = row[right];
            row[right] = tmp;
            ++left;
            --right;
        }
    }
}

void PGMImage::flipVertical() {
    size_t top = 0;
    size_t bottom = (pixels.empty() ? 0 : pixels.size() - 1);
    while (top < bottom) {
        std::vector<int> temp = pixels[top];
        pixels[top] = pixels[bottom];
        pixels[bottom] = temp;
        ++top;
        --bottom;
    }
}

void PGMImage::paste(const Image& srcImg, size_t posX, size_t posY) {
    const PGMImage* src = dynamic_cast<const PGMImage*>(&srcImg);
    if (!src) {
        throw std::runtime_error("PGMImage::paste: source image is not PGM format");
    }

    size_t newWidth = (posX + src->width) > width ? (posX + src->width) : width;
    size_t newHeight = (posY + src->height) > height ? (posY + src->height) : height;

    if (newWidth > width || newHeight > height) {
        std::vector<std::vector<int>> extended(newHeight, std::vector<int>(newWidth, 0));
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

std::string PGMImage::getName() const {
    return filename;
}

std::string PGMImage::getFormat() const {
    return "PGM";
}
