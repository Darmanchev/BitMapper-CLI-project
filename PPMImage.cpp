#include "PPMImage.h"
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <cstdlib> 

PPMImage::PPMImage(const std::string& path) {
    load(path);
}

void PPMImage::load(const std::string& path) {
    filename = path;
    std::ifstream in(path.c_str());
    if (!in.is_open())
        throw std::runtime_error("Cannot open PPM file: " + path);

    in >> format;
    if (format != "P3")
        throw std::runtime_error("Unsupported PPM format (only P3 supported): " + format);

    in >> std::ws;
    while (in.peek() == '#') {
        std::string dummy;
        std::getline(in, dummy);
        in >> std::ws;
    }

    in >> width >> height >> maxColor;
    if (!in || width == 0 || height == 0 || maxColor <= 0)
        throw std::runtime_error("Invalid PPM header.");

    pixels.assign(height,
        std::vector<std::vector<int>>(width, std::vector<int>(3, 0)));

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            for (int c = 0; c < 3; ++c) {
                int val;
                in >> val;
                if (!in || val < 0 || val > maxColor)
                    throw std::runtime_error("Invalid color value in PPM file.");
                pixels[i][j][c] = val;
            }
        }
    }
}

void PPMImage::save(const std::string& path) const {
    std::ofstream out(path.c_str());
    if (!out)
        throw std::runtime_error("Cannot write to file: " + path);

    out << "P3\n";
    out << width << " " << height << "\n";
    out << maxColor << "\n";
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            out << pixels[i][j][0] << " "
                << pixels[i][j][1] << " "
                << pixels[i][j][2] << " ";
        }
        out << "\n";
    }
}

void PPMImage::grayscale() {
    for (auto& row : pixels) {
        for (auto& px : row) {
            int avg = (px[0] + px[1] + px[2]) / 3;
            px[0] = px[1] = px[2] = avg;
        }
    }
}

void PPMImage::monochrome() {
    for (auto& row : pixels) {
        for (auto& px : row) {
            int avg = (px[0] + px[1] + px[2]) / 3;
            int bw = (avg > maxColor / 2 ? maxColor : 0);
            px[0] = px[1] = px[2] = bw;
        }
    }
}

void PPMImage::negative() {
    for (auto& row : pixels) {
        for (auto& px : row) {
            for (int c = 0; c < 3; ++c) {
                px[c] = maxColor - px[c];
            }
        }
    }
}

void PPMImage::rotateLeft() {
    std::vector<std::vector<std::vector<int>>> rotated(
        width,
        std::vector<std::vector<int>>(height, std::vector<int>(3, 0))
    );
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            rotated[width - j - 1][i] = pixels[i][j];
        }
    }
    pixels = rotated;
    std::swap(width, height);
}

void PPMImage::rotateRight() {
    std::vector<std::vector<std::vector<int>>> rotated(
        width,
        std::vector<std::vector<int>>(height, std::vector<int>(3, 0))
    );
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            rotated[j][height - i - 1] = pixels[i][j];
        }
    }
    pixels = rotated;
    std::swap(width, height);
}

void PPMImage::flipHorizontal() {
    for (auto& row : pixels) {
        size_t left = 0;
        size_t right = (row.empty() ? 0 : row.size() - 1);
        while (left < right) {
            std::vector<int> tmp = row[left];
            row[left] = row[right];
            row[right] = tmp;
            ++left;
            --right;
        }
    }
}

void PPMImage::flipVertical() {
    size_t top = 0;
    size_t bottom = (pixels.empty() ? 0 : pixels.size() - 1);
    while (top < bottom) {
        std::vector<std::vector<int>> temp = pixels[top];
        pixels[top] = pixels[bottom];
        pixels[bottom] = temp;
        ++top;
        --bottom;
    }
}

void PPMImage::paste(const Image& srcImg, size_t posX, size_t posY) {
    const PPMImage* src = dynamic_cast<const PPMImage*>(&srcImg);
    if (!src) {
        throw std::runtime_error("PPMImage::paste: source image is not PPM format");
    }

    size_t newWidth = (posX + src->width) > width ? (posX + src->width) : width;
    size_t newHeight = (posY + src->height) > height ? (posY + src->height) : height;

    if (newWidth > width || newHeight > height) {
        std::vector<std::vector<std::vector<int>>> extended(
            newHeight,
            std::vector<std::vector<int>>(newWidth, std::vector<int>(3, 0))
        );
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

std::string PPMImage::getName() const {
    return filename;
}

std::string PPMImage::getFormat() const {
    return "PPM";
}
