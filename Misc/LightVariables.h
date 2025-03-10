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

        glm::vec3 ambient = glm::vec3(0.2f);
        glm::vec3 diffuse = glm::vec3(.5f);
        glm::vec3 specular = glm::vec3(1.0f);

        float ambient_intensity = 1.0f;
        float diffuse_intensity = 1.0f;
        float specular_intensity = 1.0f;

        //DirectionalLight Variables
        glm::vec3 dir_lightDirection {-0.2f, -1.0f, -0.3f};
        //PointLight Variables
        float constant = 1.0f;
        float linear = 0.09f;
        float quadratic = 0.032f;
        //Spotlight Variables
        glm::vec3 spot_lightDirection {-0.2f, -1.0f, -0.3f};
        float cutOff = 12.5f;
        float outer_cutOff = 17.5;
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

        [[nodiscard("Spot Light Direction")]] const glm::vec3& GetSpotLightDirection() const {
            return spot_lightDirection;
        }
        void SetSpotLightDirection(Shader& shader, glm::vec3 direction) {
            spot_lightDirection = direction;
            std::string s = ToString(type) + "[" + std::to_string(id) + "].direction";
            shader.SetVector3(s.c_str(), spot_lightDirection);
        }


        LightTypes GetLightType() {
            return type;
        }
        void ChangeLightType(LightTypes& type) {
            this->type = type;
            std::string s;
            switch (this->type) {
                case LightTypes::DirectionalLight:
                    s = ToString(this->type) + ".light";
                    break;
                default:
                    s = ToString(this->type) + "[" + std::to_string(id) + "].light";
                    break;
            }
            shader_string = s;
        }

        glm::vec3 GetAmbient() const {
            return ambient;
        }
        glm::vec3 GetDiffuse() const {
            return diffuse;
        }
        glm::vec3 GetSpecular() const {
            return specular;
        }

        void SetAmbient(glm::vec3 ambient) {
            this->ambient = ambient;
        }
        void SetAmbientUniform(Shader& shader) {
            std::string s = shader_string + ".ambient";
            glm::vec3 result = ambient * ambient_intensity;
            shader.SetVector3(s.c_str(), result);
        }
        void SetDiffuse(glm::vec3 diffuse) {
            this->diffuse = diffuse;
        }
        void SetDiffuseUniform(Shader& shader) {
            std::string s = shader_string + ".diffuse";
            glm::vec3 result = diffuse * diffuse_intensity;
            shader.SetVector3(s.c_str(), result);
        }
        void SetSpecular(glm::vec3 specular) {
            this->specular = specular;
        }
        void SetSpecularUniform(Shader& shader) {
            std::string s = shader_string + ".specular";
            glm::vec3 result = specular * specular_intensity;
            shader.SetVector3(s.c_str(), result);
        }

        void SetAmbientIntensity(float intensity) {
            ambient_intensity = intensity;
        }
        void SetDiffuseIntensity(float intensity) {
            diffuse_intensity = intensity;
        }
        void SetSpecularIntensity(float intensity) {
            specular_intensity = intensity;
        }

        float GetAmbinetIntensity() const {
            return ambient_intensity;
        }
        float GetDiffuseIntensity() const {
            return diffuse_intensity;
        }
        float GetSpecularIntensity() const {
            return specular_intensity;
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
            std::string s = ToString(type) + "[" + std::to_string(id) + "].cutOff";
            auto result = glm::cos(glm::radians(this->cutOff));
            shader.SetFloat(s.c_str(), result);
        }

        [[nodiscard("OuterCutOff")]] const float& GetOuterCutOff() const {
            return outer_cutOff;
        }

        void SetOuterCutOff(Shader& shader, float& outer_cutOff) {
            this->outer_cutOff = outer_cutOff;
            std::string s = ToString(type) + "[" + std::to_string(id) + "].outercutoff";
            auto result = glm::cos(glm::radians(this->outer_cutOff));
            shader.SetFloat(s.c_str(), result);
        }

        void SetSetUnifromValues(Shader& shader) {
            SetAmbientUniform(shader);
            SetDiffuseUniform(shader);
            SetSpecularUniform(shader);
            if (type == LightTypes::DirectionalLight) {
                SetDirectionalLightDirection(shader, dir_lightDirection);
                return;
            }
            if (type == LightTypes::PointLight) {
                SetUniformLightPosition(shader);
                SetConstant(shader, constant);
                SetLinear(shader, linear);
                SetQuadratic(shader, quadratic);
                return;
            }
            if (type == LightTypes::SpotLight) {
                SetUniformLightPosition(shader);
                SetSpotLightDirection(shader, spot_lightDirection);
                SetCutOff(shader, cutOff);
                SetOuterCutOff(shader, outer_cutOff);
                SetConstant(shader, constant);
                SetLinear(shader, linear);
                SetQuadratic(shader, quadratic);
                return;
            }
        }
    };
}

#endif //LIGHTVARIABLES_H
