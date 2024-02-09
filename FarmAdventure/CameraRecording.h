#ifndef CAMERA_RECORDING_H
#define CAMERA_RECORDING_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class CameraRecording {
public:
    std::vector<std::pair<float, glm::vec3>> positions;
    std::vector<std::pair<float, glm::vec3>> directions;

    void recordState(float timestamp, const glm::vec3& position, const glm::vec3& direction);

    void saveToFile(const std::string& filename);

    void loadFromFile(const std::string& filename);
};

#endif
