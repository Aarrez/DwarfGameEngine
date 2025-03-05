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
    inline const char* ToCString(const LightTypes& msg) {
        switch (msg) {
            case LightTypes::PointLight: return "pointlight";
            case LightTypes::SpotLight: return "spotlight";
            case LightTypes::DirectionalLight: return "directionallight";
            default: return "Unknown";
        }
    }
    inline std::string ToString(const LightTypes& msg) {
        switch (msg) {
            case LightTypes::PointLight: return "pointlight";
            case LightTypes::SpotLight: return "spotlight";
            case LightTypes::DirectionalLight: return "directionallight";
            default: return "Unknown";
        }
    }

    struct LightEntity {
        unsigned int id;

        std::string name;
    private:
        LightTypes type = LightTypes::PointLight;
        std::string shader_string = ToString(type) + "[" + std::to_string(id) + "}.light";

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 posModel = glm::mat4(1.0);
        glm::mat4 scaleModel = glm::mat4(1.0);

        //DirectionalLight Variables
        glm::vec3 dir_lightDirection {-0.2f, -1.0f, -0.3f};
        //PointLight Variables
        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;
        //Spotlight Variables
        glm::vec3 spot_lightDirection {-0.2f, -1.0f, -0.3f};
        float cutOff = glm::cos(glm::radians(12.5f));
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
        void SetUniformLightPosition(Shader& shader) {
            std::string s = ToString(type) + "[" + std::to_string(id) + "].position";
            shader.SetVector3(s.c_str(), GetPosition());
        }
        void SetScale(glm::vec3 scale) {
            scaleModel = glm::scale(glm::mat4(1.0f), scale);
        }
        void CombineModels() {
            model = posModel * scaleModel;
        }

        [[nodiscard("Directional light Direction")]] const glm::vec3& GetDirectionalLightDirection() const {
            return dir_lightDirection;
        }
        void SetDirectionalLightDirection(Shader& shader, glm::vec3 direction) {
            dir_lightDirection = direction;
            std::string s = ToString(type) + ".direction";
            shader.SetVector3(s.c_str(), dir_lightDirection);
        }

        [[nodiscard("Spot Light Direction")]] const glm::vec3& GetPointLightDirection() const {
            return spot_lightDirection;
        }
        void SetSpotLightDirection(Shader& shader, glm::vec3 direction) {
            spot_lightDirection = direction;
            std::string s = ToString(type) + ".direction";
            shader.SetVector3(s.c_str(), spot_lightDirection);
        }


        LightTypes GetLightType() {
            return type;
        }
        void ChangeLightType(LightTypes& type) {
            this->type = type;
            std::string s = ToString(this->type) + ".light";
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

        [[nodiscard("Constant")]] const float& GetConstant() const {
            return constant;
        }
        [[nodiscard("Linear")]] const float& GetLinear() const {
            return linear;
        }
        [[nodiscard("Quadratic")]] const float& GetQuadratic() const {
            return quadratic;
        }

        void SetConstant(Shader& shader, float _constant) {
            constant = _constant;
            std::string s = ToString(type) +"[" + std::to_string(id) + "].constant";
            shader.SetFloat(s.c_str(), constant);
        }
        void SetLinear(Shader& shader, float _linear) {
            linear = _linear;
            std::string s = ToString(type)+ "[" + std::to_string(id) + "].linear";
            shader.SetFloat(s.c_str(), _linear);
        }
        void SetQuadratic(Shader& shader, float _quadratic) {
            quadratic = _quadratic;
            std::string s = ToString(type) + "[" + std::to_string(id) +"].quadratic";
            shader.SetFloat(s.c_str(), _quadratic);
        }

        [[nodiscard("CutOff")]] const float& GetCutOff() const {
            return cutOff;
        }

        void SetCutOff(Shader& shader, float& cutOff) {
            this->cutOff = cutOff;
            std::string s = shader_string + ".cutOff";
            shader.SetFloat(s.c_str(), cutOff);
        }
    };
}

#endif //LIGHTVARIABLES_H
