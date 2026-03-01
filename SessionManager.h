#pragma once

#include "Session.h"
#include "PBMImage.h"
#include "PGMImage.h"
#include "PPMImage.h"
#include <vector>
#include <string>

class SessionManager {
private:
    std::vector<Session*> sessions;
    Session* currentSession;
    int nextSessionId;

public:
    SessionManager();
    ~SessionManager();

    void startNewSession(const std::vector<std::string>& imagePaths);

    Session* getCurrentSession();

    void addImageToCurrentSession(const std::string& path);

    void listCurrentSessionInfo() const;
};
