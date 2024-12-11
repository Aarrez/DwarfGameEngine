#pragma once
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;


namespace Dwarf{
    static const fs::path init_path = fs::current_path();

    class DwarfPathChange{
    public:

        static fs::path ChangeCurrentPathToProjectRoot(){
            fs::path p = fs::current_path();
            const char* removeFrom {"cmake"};
            std::string path = p.string();
            size_t i = path.find(removeFrom);
            if(std::string::npos == i){
                return "";
            }
            path = path.erase(i);
            std::replace(path.begin(), path.end(), '\\', '/');
            fs::current_path(path);
            return path;
        }
        static fs::path GetCurrentPath(){
            return fs::current_path();
        }
        static void SetCurrentPath(const fs::path &p){
            fs::current_path(p);
        }
        static void AppendCurrentPath(const std::string &append_string){
            fs::path s = fs::current_path().append(append_string);
            fs::current_path(s);
        }
        static void ResetToDefaultPath(){
            fs::current_path(init_path);
        }

        static std::vector<std::string> GetNameFilesInDirectory(const std::string &directory) {
            std::vector<std::string> pathList;
            for (const auto & entry : fs::directory_iterator(directory)) {
                std::string file = entry.path().string();
                file = file.substr(file.find_last_of('/') + 1);
                pathList.push_back(file);
            }
            if (pathList.empty()) {
                std::cerr << directory << "Directory does not exist or does not contain any files" << std::endl;
            }
            return pathList;
        }

    };
}