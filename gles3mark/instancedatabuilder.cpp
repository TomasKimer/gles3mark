
#include "instancedatabuilder.h"

std::vector<glm::mat4>* InstanceDataBuilder::BuildChairsInstanceData() {
    glm::mat4 scale(glm::scale(glm::mat4(), glm::vec3(0.1f)));

    glm::mat4 rows[] = {
        glm::translate(scale, glm::vec3(-740, 19, -70)),
        glm::translate(scale, glm::vec3(-740, 39, -170)),
        glm::translate(scale, glm::vec3(-740, 59, -270)),
        glm::translate(scale, glm::vec3(-740, 79, -370)),
        glm::translate(scale, glm::vec3(-740, 99, -470))
    };

    std::vector<glm::mat4> *chairsInstanceData = new std::vector<glm::mat4>();
    
    for (unsigned int rowI = 0; rowI < 5; rowI++)
    {
        int offsetX = 0;
        for (unsigned int i = 0; i < 13; i++)
        {				
            if (i == 3 || i == 10)
                offsetX += 100;
            glm::mat4 col = glm::translate(rows[rowI], glm::vec3(offsetX, 0, 0));
            chairsInstanceData->push_back(col);
            offsetX += 105;
        }		
    }

    return chairsInstanceData;
}

std::vector<glm::mat4>* InstanceDataBuilder::BuildDeskMidInstanceData() {
    glm::mat4 scale(glm::scale(glm::mat4(), glm::vec3(0.1f)));
    
    return new std::vector<glm::mat4> {
        glm::translate(scale, glm::vec3(-365, 13, -43)),
        glm::translate(scale, glm::vec3(-365, 33, -143)),
        glm::translate(scale, glm::vec3(-365, 53, -243)),
        glm::translate(scale, glm::vec3(-365, 73, -343)),
        glm::translate(scale, glm::vec3(-365, 93, -443)),       
    };
}

std::vector<glm::mat4>* InstanceDataBuilder::BuildDeskSideInstanceData() {
    glm::mat4 scale(glm::scale(glm::mat4(), glm::vec3(0.1f)));

    //glm::vec3 otherside(1250, 0, 0);
    
    return new std::vector<glm::mat4> {
        glm::translate(scale, glm::vec3(-785,  20, -15)),
        glm::translate(scale, glm::vec3(-785,  40, -115)),
        glm::translate(scale, glm::vec3(-785,  60, -215)),
        glm::translate(scale, glm::vec3(-785,  80, -315)),
        glm::translate(scale, glm::vec3(-785, 100, -415)),

        glm::translate(scale, glm::vec3(465,  20, -15)),
        glm::translate(scale, glm::vec3(465,  40, -115)),
        glm::translate(scale, glm::vec3(465,  60, -215)),
        glm::translate(scale, glm::vec3(465,  80, -315)),
        glm::translate(scale, glm::vec3(465, 100, -415)),
    };
}