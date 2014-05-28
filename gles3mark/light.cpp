/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#include "light.h"

Light::Light() : type(Type::Directional) {

}    

Light::Light(const glm::vec3& pos, const glm::vec3& diffuseColor, float size) : position(pos), diffuseColor(diffuseColor), size(size), type(Type::Point) {
    UpdateMatrix();        
}

void Light::Move(const glm::vec3& vec) {
    position += vec;
    UpdateMatrix();    
}

void Light::UpdateMatrix() {
    matrix = glm::scale(glm::translate(glm::mat4(), position), glm::vec3(size));
} 