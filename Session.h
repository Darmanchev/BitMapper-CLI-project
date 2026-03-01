#pragma once

#include "Image.h"
#include <vector>
#include <string>

class Session {
private:
    int sessionId;
    std::vector<Image*> images;
    std::vector<std::string> history;
    std::vector<std::string> redoStack;
    bool isSaved;

public:
    Session(int id);
    ~Session();

    int getId() const;
    void addImage(Image* img);
    void applyTransformation(const std::string& command);
    void undo();
    void redo();
    void save() const;
    void saveAs(const std::vector<std::string>& filenames) const;
    void listInfo() const;
    std::vector<Image*>& getImages();
    void pasteImage(const std::string& srcName, const std::string& destName, size_t posX, size_t posY);

    bool hasUnsavedChanges() const;
};