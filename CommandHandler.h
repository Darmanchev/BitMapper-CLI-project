#pragma once

#include "SessionManager.h"
#include <string>
#include <vector>

class CommandHandler {
private:
    SessionManager sessionManager;
    bool running;

    std::vector<std::string> tokenize(const std::string& line);

public:
    CommandHandler();
    void run();
};
