//
// Created by nechamar on 22.5.18.
//

#ifndef PGR_PARAMETERS_H
#define PGR_PARAMETERS_H

#include "model.h"
#include "modelLoader.h"
#include "starship.h"
#include "globe.h"
#include "entityManager.h"

namespace nechamar{
    void addEntities(entityManager * sceneManager) {
        glm::vec3 *points = new glm::vec3[10]{
                glm::vec3(-100, 0, 20),
                glm::vec3(100, 0, 20),
                glm::vec3(200, 0, 10),
                glm::vec3(200, 0, -10),
                glm::vec3(100, 0, -20),
                glm::vec3(-100, 0, -20),
                glm::vec3(-200, 0, -10),
                glm::vec3(-200, 0, 10)
        };
        spline *circle = new spline(points, 8);

        char *starDestroyerPath = const_cast<char *>("./res/Star Destroyer3");
        char *viperPath = const_cast<char *>("./res/viper");
        char *moonPath = const_cast<char *>("./res/moon");
        char *luminarisPath = const_cast<char *>("./res/Luminaris");
        char *basicShipPath = const_cast<char *>("./res/shipA_OBJ");
        char *myShipPath = const_cast<char *>("./res/myShip");

        modelGeometry *starDestroyerModelGeometry = new modelGeometry;
        loadFromOBJ(starDestroyerPath, &starDestroyerModelGeometry);
        model *sdm = new model(&starDestroyerModelGeometry);
        starship *sd = new starship(sdm, 0, 0, 0, 0, 0, 0, 30);
        sceneManager->addEntity(sd);


        model *sdm5 = new model(&starDestroyerModelGeometry);
        starship *sd1 = new starship(sdm5, 0, 0, -10, 0, 0, 0, 30);
        sceneManager->addEntity(sd1);

        modelGeometry *mg1 = new modelGeometry;
        loadFromOBJ(viperPath, &mg1);
        model *sdm1 = new model(&mg1);

        starship *viper2 = new starship(sdm1, 50, 0, -90, -90, 0, 0, 4,circle);
        sceneManager->addEntity(viper2);

        modelGeometry *mg3 = new modelGeometry;
        loadFromOBJ(moonPath, &mg3);
        model *moonModel = new model(&mg3);
        globe *moon = new globe(moonModel, 30, 0, 420, 0, 0, 0, 1.2f, glm::vec3(0, 0, 90));

        sceneManager->addEntity(moon);

        modelGeometry *mgMy = new modelGeometry;
        loadFromOBJ(myShipPath, &mgMy);
        model *mSModel = new model(&mgMy);
        starship *myShip = new starship(mSModel, 0, 0, 200, 0, 0, 0, 4, nullptr, glm::vec3(6,0,37));

        sceneManager->addEntity(myShip);

        modelGeometry *mg2 = new modelGeometry;
        loadFromOBJ(luminarisPath, &mg2);
        model *as = new model(&mg2);
        starship *luminaris = new starship(as, -10, 0, 200, 0, 0, 90, 1);

        sceneManager->addEntity(luminaris);

        modelGeometry *basicShipModelGeometry;
        basicShipModelGeometry = new modelGeometry;
        loadFromOBJ(basicShipPath, &basicShipModelGeometry);
        model *basicShipModel = new model(&basicShipModelGeometry);
        starship *basicShip = new starship(basicShipModel, 90, 0, 200, 0, 0, 0, 1);

        sceneManager->addEntity(basicShip);

    }
}

#endif //PGR_PARAMETERS_H
