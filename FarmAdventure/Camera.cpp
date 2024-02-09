#include "Camera.hpp"
#include <iostream>
#include <glm/gtc/quaternion.hpp>

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) : bezierTime(0.0f), bezierDuration(0.0f) {
        
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;
        this->cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        this->cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection));
        this->cameraUpDirection = glm::cross(cameraRightDirection, cameraFrontDirection);

        bezierStart = cameraPosition;
        bezierEnd = cameraPosition;
        bezierControl = cameraPosition;

    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {

        return glm::lookAt(cameraPosition, cameraPosition + cameraFrontDirection, this->cameraUpDirection);
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        switch (direction) {
        case MOVE_FORWARD:
            cameraPosition += cameraFrontDirection * speed;
            cameraTarget += cameraFrontDirection * speed;
            break;
        case MOVE_BACKWARD:
            cameraPosition -= cameraFrontDirection * speed;
            cameraTarget -= cameraFrontDirection * speed;
            break;
        case MOVE_RIGHT:
            cameraPosition += cameraRightDirection * speed;
            cameraTarget += cameraRightDirection * speed;
            break;
        case MOVE_LEFT:
            cameraPosition -= cameraRightDirection * speed;
            cameraTarget -= cameraRightDirection * speed;
            break;
        case MOVE_UP:
            cameraPosition += cameraUpDirection * speed;
            cameraTarget += cameraUpDirection * speed;
            break;
        case MOVE_DOWN:
            cameraPosition -= cameraUpDirection * speed;
            cameraTarget -= cameraUpDirection * speed;
            break;
        }
        std::cout << cameraPosition.x << " " << cameraPosition.y << " " << cameraPosition.z << std::endl;

    }


    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        //printf("pitch=%f\nyaw=%f\n", pitch, yaw);
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cameraFrontDirection = glm::normalize(direction);
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection));
    }

    void Camera::startBezier(glm::vec3 controlPoint, glm::vec3 endPoint, float duration) {
        bezierStart = cameraPosition;
        bezierControl = controlPoint;
        bezierEnd = endPoint;
        bezierTime = 0.0f;
        bezierDuration = duration;
    }

    void Camera::addBezierCurve(glm::vec3 controlPoint, glm::vec3 endPoint, float duration) {
        bezierCurves.push_back({ cameraPosition, controlPoint, endPoint, 0.0f, duration });
    }

    void Camera::gradualTurn(float degrees, float duration) {
        //calculate the target yaw based on the current yaw and the specified degrees
        float targetYaw = glm::degrees(glm::atan(cameraFrontDirection.z, cameraFrontDirection.x)) + degrees;

        //start bezier curve for rotation
        startBezierRotation(glm::vec3(0.0f, glm::radians(targetYaw), 0.0f), glm::vec3(0.0f), glm::vec3(0.0f), duration);
    }

    void Camera::startBezierRotation(glm::vec3 startRotation, glm::vec3 controlRotation, glm::vec3 endRotation, float duration) {
        bezierRotationStart = startRotation;
        bezierRotationControl = controlRotation;
        bezierRotationEnd = endRotation;
        bezierRotationTime = 0.0f;
        bezierRotationDuration = duration;

        bezierRotationCurves.push_back({ bezierRotationStart, bezierRotationControl, bezierRotationEnd, 0.0f, duration });
    }

    void Camera::bezierRotationTick(float speed) {
        if (!bezierRotationCurves.empty()) {
            BezierRotationCurve& currentRotationCurve = bezierRotationCurves.front();

            if (currentRotationCurve.time < currentRotationCurve.duration) {
                float t = currentRotationCurve.time / currentRotationCurve.duration;
                glm::vec3 bezierRotation = glm::mix(glm::mix(currentRotationCurve.start, currentRotationCurve.control, t), glm::mix(currentRotationCurve.control, currentRotationCurve.end, t), t);

                glm::quat rotationQuaternion = glm::quat(bezierRotation);
                cameraFrontDirection = glm::normalize(rotationQuaternion * glm::vec3(0.0f, 0.0f, -1.0f));
                cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection));

                currentRotationCurve.time += speed;
            }
            else {
                // Remove the completed rotation bezier curve
                bezierRotationCurves.erase(bezierRotationCurves.begin());

                // If there are more rotation curves in the list, start the next one
                if (!bezierRotationCurves.empty()) {
                    BezierRotationCurve& nextRotationCurve = bezierRotationCurves.front();
                    bezierRotationTime = nextRotationCurve.time;
                    bezierRotationDuration = nextRotationCurve.duration;
                }
            }
        }
    }

    void Camera::bezierTick(float speed) {
        if (!bezierCurves.empty()) {
            BezierCurve& currentCurve = bezierCurves.front();

            if (currentCurve.time < currentCurve.duration) {
                float t = currentCurve.time / currentCurve.duration;
                glm::vec3 bezierPoint = glm::mix(glm::mix(currentCurve.start, currentCurve.control, t), glm::mix(currentCurve.control, currentCurve.end, t), t);

                cameraPosition = bezierPoint;
                cameraTarget = bezierPoint + cameraFrontDirection;

                currentCurve.time += speed;
            }
            else {
                // Remove the completed bezier curve
                bezierCurves.erase(bezierCurves.begin());

                // If there are more curves in the list, start the next one
                if (!bezierCurves.empty()) {
                    BezierCurve& nextCurve = bezierCurves.front();
                    bezierTime = nextCurve.time;
                    bezierDuration = nextCurve.duration;
                }
            }
        }
    }
}
