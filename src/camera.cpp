#include "camera.h"
//#include <glm/glm.hpp>
//#include <glm/gtc/transform.hpp> // после <glm/glm.hpp>

void Camera::Init(){
    // Проекционная матрица : 45&deg; поле обзора, 4:3 соотношение сторон, диапазон : 0.1 юнит <-> 100 юнитов
    this->Projection = glm::perspective(glm::radians(90.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    // Или, для ортокамеры
    this-> View = glm::lookAt(
        glm::vec3(0,1,-1), // Камера находится в мировых координатах (4,3,3)
        glm::vec3(0,0,0), // И направлена в начало координат
        glm::vec3(0,1,0)  // "Голова" находится сверху
    );

    this->PV = this->Projection * this->View;
}