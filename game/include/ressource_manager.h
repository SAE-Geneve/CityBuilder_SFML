//
// Created by sebas on 26/06/2025.
//

#ifndef RESSOURCE_MANAGER_H
#define RESSOURCE_MANAGER_H
#include <vector>

#include "ressource.h"
#include "graphics/tilemap.h"

class RessourceManager {

    std::vector<Ressource> ressources_;

public:
    void LoadRessources(Ressource::Type type, std::vector<int> indexes);

};

inline void RessourceManager::LoadRessources(Ressource::Type type, std::vector<int> indexes){

    for(auto& index : indexes) {
        ressources_.emplace_back();
        ressources_.back().SetType(type);
        ressources_.back().SetIndex(index);
        ressources_.back().SetQuantity(1);
    }

}




#endif //RESSOURCE_MANAGER_H
