
#pragma once

#include <vector>
#include <random>
#include <glm/glm.hpp>


class SSAOBuilder {

    std::vector<glm::vec3> kernel;
    std::vector<glm::vec3> noise;
    
    std::mt19937 mt;

    void BuildKernel(unsigned size) {
        kernel.resize(size);

        std::uniform_real_distribution<float> dist1(-1.0f, 1.0f);
        std::uniform_real_distribution<float> dist2( 0.0f, 1.0f);
        
        for (unsigned i = 0; i < size; ++i) {
            kernel[i] = glm::vec3(dist1(mt), dist1(mt), dist2(mt));
            glm::normalize(kernel[i]);

            // fallof
            float scale = static_cast<float>(i) / size;
            scale = glm::mix(0.1f, 1.0f, scale * scale);
            kernel[i] *= scale;
        }
    }

    void BuildNoise(unsigned size) {
        noise.resize(size * size);

        std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

        for (unsigned i = 0; i < size * size; ++i) {
            noise[i] = glm::vec3(dist(mt), dist(mt), 0.0f);
            glm::normalize(noise[i]);
        }
    }

public:
    const int KERNEL_SIZE = 4;

    SSAOBuilder() {
        std::random_device rd;
        mt.seed(rd());

        BuildKernel(32);
        BuildNoise(4);       
    }

    const std::vector<glm::vec3>& GetKernel() {
        return kernel;
    }

    const std::vector<glm::vec3>& GetNoise() {
        return noise;
    }
};