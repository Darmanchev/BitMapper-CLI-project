#include "Session.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib> 
#include <string>    

Session::Session(int id)
    : sessionId(id), isSaved(false) {
}

Session::~Session() {
    for (auto* img : images) {
        delete img;
    }
}

int Session::getId() const {
    return sessionId;
}

void Session::addImage(Image* img) {
    images.push_back(img);
    isSaved = true;
}

void Session::applyTransformation(const std::string& command) {
    if (command == "grayscale") {
        for (auto* img : images) img->grayscale();
    }
    else if (command == "monochrome") {
        for (auto* img : images) img->monochrome();
    }
    else if (command == "negative") {
        for (auto* img : images) img->negative();
    }
    else if (command == "rotate left") {
        for (auto* img : images) img->rotateLeft();
    }
    else if (command == "rotate right") {
        for (auto* img : images) img->rotateRight();
    }
    else if (command == "flip top") {
        for (auto* img : images) img->flipVertical();
    }
    else if (command == "flip left") {
        for (auto* img : images) img->flipHorizontal();
    }
    else {
        throw std::runtime_error("Unknown transformation command: " + command);
    }
    history.push_back(command);
    redoStack.clear();
    isSaved = true;
}

void Session::undo() {
    if (history.empty()) {
        std::cout << "Nothing to undo\n";
        return;
    }
    std::string last = history.back();
    history.pop_back();
    redoStack.push_back(last);
    std::cout << "Undone last operation: " << last << "\n";
    isSaved = true;
}

void Session::redo() {
     if (history.empty()) { std::cout<<"Nothing to undo\n"; return; }
    std::string last = history.back();
    history.pop_back();

    if      (last == "rotate left")  for (auto* img: images) img->rotateRight();
    else if (last == "rotate right") for (auto* img: images) img->rotateLeft();
    else if (last == "flip top")     for (auto* img: images) img->flipVertical();
    else if (last == "flip left")    for (auto* img: images) img->flipHorizontal();
    else if (last == "negative")     for (auto* img: images) img->negative();
    else if (last == "grayscale" || last == "monochrome") {
        std::cout<<"Cannot undo irreversible operation: "<<last<<"\n";
        return;
    }

    redoStack.push_back(last);
    isSaved = true;
    std::cout << "Undone last operation: " << last << "\n";
}

void Session::save() const {
    long now = time(0);  
    std::string timestamp = std::to_string(now);

    for (const auto* img : images) {
        std::string original = img->getName();
        size_t dot = original.find_last_of('.');
        std::string base = (dot != std::string::npos) ? original.substr(0, dot) : original;
        std::string ext = (dot != std::string::npos) ? original.substr(dot + 1) : "img";

        std::string outName = base + "_" + timestamp + "." + ext;
        img->save(outName);
    }
    std::cout << "Files saved with timestamp " << timestamp << "\n";

    const_cast<Session*>(this)->isSaved = false;
}

void Session::saveAs(const std::vector<std::string>& filenames) const {
    long now = time(0);
    std::string timestamp = std::to_string(now);

    for (size_t i = 0; i < images.size(); ++i) {
        if (i < filenames.size()) {
            images[i]->save(filenames[i]);
        }
        else {
            std::string original = images[i]->getName();
            size_t dot = original.find_last_of('.');
            std::string base = (dot != std::string::npos) ? original.substr(0, dot) : original;
            std::string ext = (dot != std::string::npos) ? original.substr(dot + 1) : "img";

            std::string outName = base + "_" + timestamp + "." + ext;
            images[i]->save(outName);
        }
    }
    std::cout << "Files saved with custom names.\n";
    const_cast<Session*>(this)->isSaved = false;
}

void Session::listInfo() const {
    std::cout << "Files in session with ID " << sessionId << ":\n";
    for (const auto* img : images) {
        std::string format = img->getFormat();
        std::string description;
        if (format == "PPM")      description = "portable pixmap format";
        else if (format == "PGM") description = "portable graymap format";
        else if (format == "PBM") description = "portable bitmap format";
        else description = format;

        std::cout << img->getName() << " - " << description << "\n";
    }
}

std::vector<Image*>& Session::getImages() {
    return images;
}

void Session::pasteImage(const std::string& srcName,
    const std::string& destName,
    size_t posX,
    size_t posY) {
    Image* src = nullptr;
    Image* dest = nullptr;
    for (auto* img : images) {
        if (img->getName() == srcName)  src = img;
        if (img->getName() == destName) dest = img;
    }
    if (!src || !dest) {
        throw std::runtime_error("One or both images not found in current session.");
    }
    dest->paste(*src, posX, posY);
    std::cout << "Pasted " << srcName << " into " << destName
        << " at position (" << posX << ", " << posY << ")\n";
    const_cast<Session*>(this)->isSaved = true;
}

bool Session::hasUnsavedChanges() const {
    return isSaved;
}