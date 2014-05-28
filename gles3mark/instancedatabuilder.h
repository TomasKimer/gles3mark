/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class InstanceDataBuilder {
public:
    static std::vector<glm::mat4>* BuildChairsInstanceData();
    static std::vector<glm::mat4>* BuildDeskMidInstanceData();
    static std::vector<glm::mat4>* BuildDeskSideInstanceData();
};