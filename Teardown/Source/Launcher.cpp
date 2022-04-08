#include "Launcher.h"

#include <filesystem>
#include <fstream>

bool Launcher::GetPath() {
    // Read the text in windows temp dir + tdl.txt
    std::string sTempPath = std::filesystem::temp_directory_path().string();
    sTempPath += "\\tdl.txt";

    std::ifstream file(sTempPath);
    if (!file.is_open()) {
        return false;
    }

    std::getline(file, Launcher::sPath);
    file.close();

    return true;
}