#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "CameraRecording.h"

class CameraRecording {
public:
    std::vector<std::pair<float, glm::vec3>> positions;  // Timestamped camera positions
    std::vector<std::pair<float, glm::vec3>> directions; // Timestamped camera front directions

    // Function to record camera state
    void recordState(float timestamp, const glm::vec3& position, const glm::vec3& direction) {
        positions.emplace_back(timestamp, position);
        directions.emplace_back(timestamp, direction);
    }

    // Function to save recorded data to a file
    void saveToFile(const std::string& filename);

    // Function to load recorded data from a file
    void loadFromFile(const std::string& filename);
};

void CameraRecording::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (size_t i = 0; i < positions.size(); ++i) {
            file << positions[i].first << " "
                << positions[i].second.x << " " << positions[i].second.y << " " << positions[i].second.z << " "
                << directions[i].second.x << " " << directions[i].second.y << " " << directions[i].second.z << "\n";
        }
        file.close();
        std::cout << "Recorded data saved to " << filename << std::endl;
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

void CameraRecording::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        positions.clear();
        directions.clear();

        float timestamp;
        glm::vec3 position, direction;
        while (file >> timestamp >> position.x >> position.y >> position.z >> direction.x >> direction.y >> direction.z) {
            positions.emplace_back(timestamp, position);
            directions.emplace_back(timestamp, direction);
        }

        file.close();
        std::cout << "Recorded data loaded from " << filename << std::endl;
    }
    else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}