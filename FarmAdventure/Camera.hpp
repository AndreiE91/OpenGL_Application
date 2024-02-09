#ifndef Camera_hpp
#define Camera_hpp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

namespace gps {

    enum MOVE_DIRECTION { MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT, MOVE_UP, MOVE_DOWN };

    struct BezierCurve {
        glm::vec3 start;
        glm::vec3 control;
        glm::vec3 end;
        float time;
        float duration;
    };

    struct BezierRotationCurve {
        glm::vec3 start;
        glm::vec3 control;
        glm::vec3 end;
        float time;
        float duration;
    };

    class Camera {

    public:
        //Camera constructor
        Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp);
        //return the view matrix, using the glm::lookAt() function
        glm::mat4 getViewMatrix();
        //update the camera internal parameters following a camera move event
        void move(MOVE_DIRECTION direction, float speed);
        //update the camera internal parameters following a camera rotate event
        //yaw - camera rotation around the y axis
        //pitch - camera rotation around the x axis
        void rotate(float pitch, float yaw);
        void startBezier(glm::vec3 controlPoint, glm::vec3 endPoint, float duration);
        void bezierTick(float speed);
        void addBezierCurve(glm::vec3 controlPoint, glm::vec3 endPoint, float duration);
        void gradualTurn(float degrees, float duration);
        void startBezierRotation(glm::vec3 startRotation, glm::vec3 controlRotation, glm::vec3 endRotation, float duration);
        void addBezierRotationCurve(glm::vec3 controlRotation, glm::vec3 endRotation, float duration);
        void bezierRotationTick(float speed);

    private:
        glm::vec3 cameraPosition;
        glm::vec3 cameraTarget;
        glm::vec3 cameraFrontDirection;
        glm::vec3 cameraRightDirection;
        glm::vec3 cameraUpDirection;

        std::vector<BezierCurve> bezierCurves;

        glm::vec3 bezierStart;
        glm::vec3 bezierEnd;
        glm::vec3 bezierControl;
        float bezierTime;
        float bezierDuration;


        glm::vec3 bezierRotationStart;
        glm::vec3 bezierRotationControl;
        glm::vec3 bezierRotationEnd;
        float bezierRotationTime;
        float bezierRotationDuration;
        std::vector<BezierRotationCurve> bezierRotationCurves;
    };
}

#endif /* Camera_hpp */
