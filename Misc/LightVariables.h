#ifndef LIGHTVARIABLES_H
#define LIGHTVARIABLES_H
#include <string>
#include <glm/glm.hpp>

#include "../GL/Shader.h"

namespace Engine {
    enum class LightTypes {
        PointLight,
        SpotLight,
        DirectionalLight
    };
    inline const char* ToString(const LightTypes& msg) {
        switch (msg) {
            case LightTypes::PointLight: return "PointLight";
            case LightTypes::SpotLight: return "SpotLight";
            case LightTypes::DirectionalLight: return "DirectionalLight";
            default: return "Unknown";
        }
    }

    struct LightEntity {
        unsigned int id;
        LightTypes type;
        std::string name;
    private:
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
    public:
        void SetModel(const glm::mat4& _model) {
            model = _model;
        }
        [[nodiscard]] const glm::mat4& GetModel() const {
            return model;
        }
        void SetView(const glm::mat4& _view) {
            view = _view;
        }
        void SetProjection(const glm::mat4& _projection) {
            projection = _projection;
        }

        void SetModelMatrix(Shader lightShader) {
            lightShader.SetMatrix4("model", model);
        }
        void SetViewMatrix(Shader lightShader) {
            lightShader.SetMatrix4("view", view);
        }
        void SetProjectionMatrix(Shader lightShader) {
            lightShader.SetMatrix4("projection", projection);
        }
        glm::vec3 GetPosition() {
            return glm::vec3{model[3]};
        }
        glm::vec3 GetScale() {
            glm::vec3 scale;
            scale.x = glm::length(glm::vec3(model[0]));
            scale.y = glm::length(glm::vec3(model[1]));
            scale.z = glm::length(glm::vec3(model[2]));
            return scale;
        }
        void Translate(glm::vec3 translation) {
            model = glm::translate(model, translation);
        }
        void SetPosition(glm::vec3 position) {
            model = glm::translate(glm::mat4(1.0f), position);
        }
        void SetScale(glm::vec3 scale) {
            model = glm::scale(glm::mat4(1.0f), scale);
        }


    };

}

#endif //LIGHTVARIABLES_H
