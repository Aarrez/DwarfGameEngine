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
        glm::mat4 posModel = glm::mat4(1.0);
        glm::mat4 scaleModel = glm::mat4(1.0);
    public:
        void SetModel(const glm::mat4& _model) {
            model = _model;
        }
        [[nodiscard]] const glm::mat4& GetModel() const {
            return model;
        }
        void SetModelMatrix(Shader& lightShader) {
            lightShader.SetMatrix4("model", model);
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
            posModel = glm::translate(posModel, translation);
        }
        void SetPosition(glm::vec3 position) {
            posModel = glm::translate(glm::mat4(1.0f), position);
        }
        void SetScale(glm::vec3 scale) {
            scaleModel = glm::scale(glm::mat4(1.0f), scale);
        }
        void CombineModels() {
            model = posModel * scaleModel;
        }


    };

}

#endif //LIGHTVARIABLES_H
