#ifndef LIGHTVARIABLES_H
#define LIGHTVARIABLES_H
#include <algorithm>
#include <string>
#include <glm/glm.hpp>

#include "../GL/Shader.h"

namespace Engine {
    enum class LightTypes {
        PointLight = 0,
        SpotLight = 1,
        DirectionalLight = 2
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

        std::string name;
    private:
        LightTypes type = LightTypes::PointLight;
        std::string shader_string = "pointlight.light";
        std::string lowercase_type = "pointlight";
        glm::vec3 lightDirection {-0.2f, -1.0f, -0.3f};
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

        glm::vec3 GetLightDirection() {
            return lightDirection;
        }
        void SetLightDirection(glm::vec3 direction, Shader& shader) {
            lightDirection = direction;
            std::string s = lowercase_type + ".direction";
            shader.SetVector3(s.c_str(), lightDirection);
        }

        LightTypes GetLightType() {
            return type;
        }
        void ChangeLightType(LightTypes& type) {
            this->type = type;
            std::string s = ToString(this->type);
            std::for_each(s.begin(), s.end(), [](char& c) {
                c = tolower(c);
            });
            lowercase_type = s;
            s += ".light";
            shader_string = s;
        }

        void SetAmbient(Shader& shader, glm::vec3 ambient) {
            std::string s = shader_string + ".ambient";
            shader.SetVector3(s.c_str(), ambient);
        }
        void SetDiffuse(Shader& shader, glm::vec3 diffuse) {
            std::string s = shader_string + ".diffuse";
            shader.SetVector3(s.c_str(), diffuse);
        }
        void SetSpecular(Shader& shader, glm::vec3 specular) {
            std::string s = shader_string + ".diffuse";
            shader.SetVector3(s.c_str(), specular);
        }
    };

}

#endif //LIGHTVARIABLES_H
