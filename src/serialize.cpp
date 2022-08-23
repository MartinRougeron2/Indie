/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** serialize
*/

#include "entities/Entity.hpp"
#include <sstream>
#include "Bomberman.hpp"

// x,y,z,id,type,facing,path\n

std::vector<std::string> SerializeEntity(std::vector<Entity *> entities, int index)
{
    std::vector<std::string> data;

    for (auto &e : entities) {
        if (std::find(e->modified.begin(), e->modified.end(), index) == e->modified.end()) {
            Vector3 pos = e->getPosition();
            std::stringstream res;
            res <<
            pos.x << "," <<
            pos.y << "," <<
            pos.z << "," <<
            e->getID() << "," <<
            e->getType() << "," <<
            e->getFacing() << "," <<
            e->getPath() << "," <<
            e->getToRemove() << "," <<
            e->scale << "\n";

            data.push_back(res.str());
            e->modified.push_back(index);
        }
    }
    return data;
}

/*void packDataEntity(std::vector<std::string> data, Bomberman &bomberman)
{
    //std::sort(entities.begin(), entities.end(), [](Entity const & a, Entity const &b){return a.getID() < b.getID();});
    for (size_t i = 0; i != data.size(); i++) {
        std::istringstream iss(data.at(i));
        std::string str;
        EntityData result;
        int index = 0;
        if (std::count(data[i].begin(), data[i].end(), ',') != 7)
            continue;
        while (std::getline(iss, str, ',')) {
            if (index == 0) {
                try {
                    result.pos.x = std::stof(str);
                } catch (std::invalid_argument) {
                    result.pos.x = 0;
                    printf("%s\n", data[i].c_str());
                }
            }
            if (index == 1) {
                try {
                    result.pos.y = std::stof(str);
                } catch (std::invalid_argument) {
                    result.pos.x = 0;
                    printf("%s\n", data[i].c_str());
                }
            }
            if (index == 2) {
                try {
                    result.pos.z = std::stof(str);
                } catch (std::invalid_argument) {
                    result.pos.x = 0;
                    printf("%s\n", data[i].c_str());
                }
            }
            if (index == 3)
                result.id = std::stoi(str);
            if (index == 4)
                result.type = static_cast<EntityType>(std::stoi(str));
            if (index == 5)
                result.facing = static_cast<Cardinal>(std::stoi(str));
            if (index == 6)
                result.path = str;
            if (index == 7) {
                str.pop_back();
                result.remove = std::stoi(str);
            }
            index++;
        }
        int found = 0;
        int index_e = 0;
        for (auto &e : bomberman.getEntities()) {
            if (result.id == e->getID()) {
                e->setPosition(result.pos);
                e->setFacing(result.facing);
                if (result.remove) {
                    bomberman.removeEntity(index_e);
                    index_e--;
                }
                found = 1;
                break;
            }
            index_e++;
        }
        int alreadyloaded = 0;
        if (not found) {
            for (auto &e : bomberman.getEntities()) {
                if (result.path == e->getPath()) {
                    bomberman.addEntity(new Entity(*e, result.pos, result.id, result.facing));
                    alreadyloaded = 1;
                    break;
                }
            }
            if (not alreadyloaded) {
                bomberman.addEntity(new Entity(result.path, result.pos, true, true, result.id, result.facing));
            }
        }
    }
    return;
}*/
