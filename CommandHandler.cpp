#include "CommandHandler.h"
#include <iostream>
#include <cstdlib>   
#include <stdexcept>
#include <cstring>

CommandHandler::CommandHandler()
    : running(true) {
}

std::vector<std::string> CommandHandler::tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    size_t start = 0;

    while (start < line.size()) {

        while (start < line.size() && (line[start] == ' ' || line[start] == '\t')) {
            ++start;
        }
        if (start >= line.size()) break;

        size_t end = start;
        while (end < line.size() && line[end] != ' ' && line[end] != '\t') {
            ++end;
        }

        tokens.push_back(line.substr(start, end - start));
        start = end;
    }

    return tokens;
}

void CommandHandler::run() {
    std::string input;

    while (running) {
        std::cout << "> ";
        std::getline(std::cin, input);
        auto tokens = tokenize(input);
        if (tokens.empty()) continue;

        try {
            const std::string& cmd = tokens[0];

            if (cmd == "exit") {
                try {
                    Session* sess = sessionManager.getCurrentSession();
                    if (sess->hasUnsavedChanges()) {
                        std::cout << "Session contains unsaved work. Save it now (y/n)? ";
                        std::string answer;
                        std::getline(std::cin, answer);
                        if (answer == "y" || answer == "Y") {
                            sess->save();
                        }
                    }
                }
                catch (...) {
                }
                running = false;

            }
            else if (cmd == "load") {
                if (tokens.size() < 2) {
                    std::cout << "Usage: load <file1> <file2> ...\n";
                    continue;
                }
                std::vector<std::string> paths(tokens.begin() + 1, tokens.end());
                sessionManager.startNewSession(paths);

            }
            else if (cmd == "grayscale" || cmd == "monochrome" || cmd == "negative") {
                Session* sess = sessionManager.getCurrentSession();
                sess->applyTransformation(cmd);

            }
            else if (cmd == "rotate" || cmd == "flip") {
                if (tokens.size() != 2) {
                    std::cout << "Usage: rotate <left|right> or flip <top|left>\n";
                    continue;
                }
                std::string fullCmd = cmd + " " + tokens[1];
                Session* sess = sessionManager.getCurrentSession();
                sess->applyTransformation(fullCmd);

            }
            else if (cmd == "undo") {
                Session* sess = sessionManager.getCurrentSession();
                sess->undo();

            }
            else if (cmd == "redo") {
                Session* sess = sessionManager.getCurrentSession();
                sess->redo();

            }
            else if (cmd == "add") {
                if (tokens.size() != 2) {
                    std::cout << "Usage: add <file>\n";
                    continue;
                }
                sessionManager.addImageToCurrentSession(tokens[1]);

            }
            else if (cmd == "save") {
                Session* sess = sessionManager.getCurrentSession();
                sess->save();

            }
            else if (cmd == "saveas") {
                if (tokens.size() < 2) {
                    std::cout << "Usage: saveas <name1> <name2> ...\n";
                    continue;
                }
                std::vector<std::string> names(tokens.begin() + 1, tokens.end());
                Session* sess = sessionManager.getCurrentSession();
                sess->saveAs(names);

            }
            else if ((cmd == "list" && tokens.size() == 2 && tokens[1] == "session") ||
                (cmd == "session" && tokens.size() == 2 && tokens[1] == "info")) {
                sessionManager.listCurrentSessionInfo();

            }
            else if (cmd == "paste") {
                if (tokens.size() != 5) {
                    std::cout << "Usage: paste <srcName> <destName> <posX> <posY>\n";
                    continue;
                }
                const std::string& srcName = tokens[1];
                const std::string& destName = tokens[2];
                size_t posX = std::stoul(tokens[3]);
                size_t posY = std::stoul(tokens[4]);
                Session* sess = sessionManager.getCurrentSession();
                sess->pasteImage(srcName, destName, posX, posY);

            }
            else {
                std::cout << "Unknown command: " << cmd << "\n";
            }

        }
        catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << "\n";
        }
    }
}