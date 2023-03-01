#pragma once

#include "homeworks/tema_2/utils.h"

namespace tema2
{
    class Camera
    {
     public:
        Camera()
        {
            position    = glm::vec3(0, 2, 5);
            forward     = glm::vec3(0, 0, -1);
            up          = glm::vec3(0, 1, 0);
            right       = glm::vec3(1, 0, 0);
            distanceToTarget = 2;
        }

        Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            Set(position, center, up);
        }

        ~Camera()
        { }

        // Update camera
        void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            this->position = position;
            forward     = glm::normalize(center - position);
            right       = glm::cross(forward, up);
            this->up = up;
        }

        void MoveForward(float distance)
        {
            // Translates the camera using the `dir` vector computed from
            // `forward`. Movement will always keep the camera at the same
            // height. For example, if you rotate your head up/down, and then
            // walk forward, then you will still keep the same relative
            // distance (height) to the ground!
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            // What's the difference between `TranslateForward()` and
            // `MoveForward()`?
            position += glm::normalize(forward) * distance;
        }

        void TranslateUpward(float distance)
        {
            position += glm::normalize(this->up) * distance;
        }

        void TranslateRight(float distance)
        {
            // You need to translate the camera using the `right` vector.
            // Usually, however, translation using camera's `right` vector
            // is not very useful, because if the camera is rotated around the
            // `forward` vector, then the translation on the `right` direction
            // will have an undesired effect, more precisely, the camera will
            // get closer or farther from the ground. The solution is to
            // actually use the projected `right` vector (projected onto the
            // ground plane), which makes more sense because we will keep the
            // same distance from the ground plane.

            position += glm::normalize(right) * distance;
        }

        void RotateFirstPerson_OX(float angle)
        {
            // Don't forget to normalize the vectors! Use `glm::rotate()`.
            glm::vec4 new_vec = glm::rotate(glm::mat4(1.f), angle, right) * glm::vec4(forward, 0);
            forward = glm::normalize(glm::vec3(new_vec));
            up = glm::cross(right, forward);
        }

        void RotateFirstPerson_OY(float angle)
        {
            // vectors. Use `glm::rotate()`. Don't forget to normalize the
            // vectors!
            glm::vec4 new_vec = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0);
            forward = glm::normalize(glm::vec3(new_vec));

            new_vec = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0);
            right = glm::normalize(glm::vec3(new_vec));

            up = glm::cross(right, forward);
        }

        void RotateFirstPerson_OZ(float angle)
        {
            // `forward` stays the same. Use `glm::rotate()`. Don't forget
            // to normalize the vectors!

            glm::vec4 new_vec = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
            right = glm::normalize(glm::vec3(new_vec));

            new_vec = glm::rotate(glm::mat4(1.f), angle, forward) * glm::vec4(up, 0);
            forward = glm::normalize(glm::vec3(new_vec));

            up = glm::cross(right, forward);

        }

        void RotateThirdPerson_OX(float angle)
        {
            // the OX axis. Use `distanceToTarget` as translation distance.
            this->TranslateForward(this->distanceToTarget);
            this->RotateFirstPerson_OX(angle);
            this->TranslateForward(-this->distanceToTarget);
        }

        void RotateThirdPerson_OY(float angle)
        {
            // the OY axis.
            this->TranslateForward(this->distanceToTarget);
            this->RotateFirstPerson_OY(angle);
            this->TranslateForward(-this->distanceToTarget);
        }

        void RotateThirdPerson_OZ(float angle)
        {
            // the OZ axis.
            this->TranslateForward(this->distanceToTarget);
            this->RotateFirstPerson_OZ(angle);
            this->TranslateForward(-this->distanceToTarget);
        }

        glm::mat4 GetViewMatrix()
        {
            // Returns the view matrix
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

     public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
        glm::mat4 projectionMatrix;
    };
}   // namespace implemented
