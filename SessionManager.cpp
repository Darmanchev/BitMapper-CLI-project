#include "SessionManager.h"
#include <iostream>
#include <stdexcept>
#include <cstring> 

SessionManager::SessionManager()
    : currentSession(nullptr), nextSessionId(1) {
}

SessionManager::~SessionManager() {
    for (Session* sess : sessions) {
        delete sess;
    }
}

void SessionManager::startNewSession(const std::vector<std::string>& imagePaths) {
    Session* session = new Session(nextSessionId++);
    for (const std::string& path : imagePaths) {
        Image* img = nullptr;
        size_t dot = path.find_last_of('.');
        if (dot == std::string::npos) {
            delete session;
            throw std::runtime_error("Invalid file name: " + path);
        }
        std::string ext = path.substr(dot + 1);
        for (char& c : ext) c = char(std::tolower(c));

        if (ext == "pbm") {
            img = new PBMImage(path);
        }
        else if (ext == "pgm") {
            img = new PGMImage(path);
        }
        else if (ext == "ppm") {
            img = new PPMImage(path);
        }
        else {
            delete session;
            throw std::runtime_error("Unsupported file extension: " + ext);
        }
        session->addImage(img);
    }
    sessions.push_back(session);
    currentSession = session;
    std::cout << "Session with ID: " << currentSession->getId() << " started\n";
}

Session* SessionManager::getCurrentSession() {
    if (!currentSession) {
        throw std::runtime_error("No active session.");
    }
    return currentSession;
}

void SessionManager::addImageToCurrentSession(const std::string& path) {
    if (!currentSession) {
        throw std::runtime_error("No active session.");
    }

    Image* img = nullptr;
    size_t dot = path.find_last_of('.');
    if (dot == std::string::npos) {
        throw std::runtime_error("Invalid file name: " + path);
    }
    std::string ext = path.substr(dot + 1);
    for (char& c : ext) c = char(std::tolower(c));

    if (ext == "pbm") {
        img = new PBMImage(path);
    }
    else if (ext == "pgm") {
        img = new PGMImage(path);
    }
    else if (ext == "ppm") {
        img = new PPMImage(path);
    }
    else {
        throw std::runtime_error("Unsupported file extension: " + ext);
    }

    currentSession->addImage(img);
    std::cout << "Image added to session.\n";
}

void SessionManager::listCurrentSessionInfo() const {
    if (!currentSession) {
        std::cout << "No active session.\n";
        return;
    }
    currentSession->listInfo();
}
