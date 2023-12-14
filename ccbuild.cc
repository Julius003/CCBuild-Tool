#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <regex>
#include <algorithm>

std::vector<std::string> checkFile(std::string fileName){
    std::ifstream file(fileName);
    std::vector<std::string> dependencies;
    std::string match = "#include \"";

    if(file.is_open()){
        std::string line;

        // Read the file
        while(std::getline(file, line)){
            // Go through the line and find all occurences of #include 
            std::regex pattern("#includ.*\"(.*)\"");
            std::smatch match;
            if (std::regex_match(line, match, pattern)) {
                if (match.size() > 1) {
                    if(std::find(dependencies.begin(), dependencies.end(), match[1]) == dependencies.end()){
                        dependencies.push_back(match[1]);
                    }
                }
            } 
        }

        file.close();

    }

    // Recursion
    int size = dependencies.size();
    for(int i = 0; i < size; ++i){
        for(auto newDepenency: checkFile(dependencies[i])){
            if(std::find(dependencies.begin(), dependencies.end(), newDepenency) == dependencies.end()){
                dependencies.push_back(newDepenency);
            }
        }
    }
    return dependencies;
}

bool fileExists(std::string fileName){
    std::ifstream file(fileName);
    bool exists = false;
    if(file.is_open()){
        exists = true;
    }
    file.close();
    return exists;
}

std::string findSourceFile(std::string headerFileName){
    std::string header_extensions[] = {".h", ".hh", ".hpp"};
    std::string source_extensions[] = {".cpp", ".cc", ".C", ".cxx", ".cppm"};

    std::string raw_name;

    // Get the raw name of the header file without its extension
    for(int i = 0; i < 3; ++i){
        size_t found = headerFileName.find(header_extensions[i]);
        if(found != std::string::npos){
            raw_name = headerFileName.erase(found, header_extensions[i].length());
            break;
        }
    }

    // See if the source file exists
    for(int i = 0; i < 5; ++i){
        std::string source_file_name = raw_name + source_extensions[i];
        if(fileExists(source_file_name)){
            return source_file_name;
        }
    }
    return "";
}

int main(int argc, char** argv){
    std::string fileName;
    std::string outputName = "a.out";
    std::vector<std::string> additionalArguments;
    std::vector<std::string> existingDependencyFiles; // The dependencies that will have to be in the compile command

    if(argc < 2){
        std::cout << "Useage: " << argv[0] << " ./MainFile [-o OutputName]" << std::endl;
        return 0;
    }

    fileName = argv[1];

    for(int i = 0; i < argc; ++i){
        std::string current_argument = argv[i];
        if(current_argument == "-o"){
            outputName = argv[i+1];
        }
        if(current_argument.find("-std=") != std::string::npos){
            additionalArguments.push_back(current_argument);
        }
    }

    std::vector<std::string> dependencies = checkFile(fileName);

    for(auto dependency: dependencies){
        std::string source_file = findSourceFile(dependency);
        if(source_file.length() > 0){
            existingDependencyFiles.push_back(source_file);
        } 
    }

    std::string command = "g++ " + fileName;
    for(auto dependency: existingDependencyFiles){
        command += " " + dependency;
    }
    command += " -o " + outputName;
    for(auto arg: additionalArguments){
        command += " " + arg;
    }

    system(command.c_str());
}