/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Entity
*/

#ifndef ENTITIE_HPP_
#define ENTITIE_HPP_

#include <string>
#include <raylib.h>
#include <vector>

enum EntityType {
    ENTITY, BLOCK, BOMBE, PLAYER, ENEMIE, BONUS, POWDER
};

enum Cardinal {
    NORTH, SOUTH, EAST, WEST
};

typedef struct data {
    Vector3 pos;
    int id;
    EntityType type;
    Cardinal facing;
    bool modified;
    std::string path;
    int remove;
    float scale;
} EntityData;

class Entity {
    public:
        Entity(std::string path, Vector3 pos = {0, 0, 0}, bool loadModel = true, bool LoadTexture = true, float scale = 0.01f, int id = -1, Cardinal facing = EAST, EntityType type = ENTITY);
        Entity(Entity &ent, Vector3 pos, int id = -1, Cardinal facing = EAST, EntityType type = ENTITY);
        Entity(Entity &other, int id = -1);
        ~Entity();

        void displayEntity(void);

        bool operator&&(const Entity &other) const;
        bool operator&&(Vector3 pos) const;

        Vector3 getPosition() const;
        void setPosition(Vector3 newPos);

        void move(Vector3 newPos);
        Vector3 moveTest(Vector3 newPos) const;


        EntityType getType() const { return this->type;}


        int getToRemove() const { return this->toRemove;}


        Model getModel() const { return this->model;}
        void setModel(Model newModel) { this->model = newModel;}


        int getID() const { return this->id;}


        Cardinal getFacing() const { return this->facing;}

        void setFacing(Vector3 moveDir);
        void setFacing(Cardinal card) { this->facing = card;}


        std::string getPath() { return this->path;}


        BoundingBox getBox() const;


        void setToRemove(int inf) { this->toRemove = inf;}
        int getToRemove() {return this->toRemove;}


        // void runAnimation() {
        //     animFrameCounter++;
        //     UpdateModelAnimation(model, animation[0], animFrameCounter);
        //     if (animFrameCounter >= animation[0].frameCount)
        //         animFrameCounter = 0;
        // }

        bool collision = true;
        std::vector<int> modified;
        float scale;
        // ModelAnimation *animation;
        // int animFrameCounter = 0;
        // int animTotalCount = 0;
    protected:
        Vector3 pos;
        Vector3 hitbox = {1, 1, 1};
        Model model;
        int toRemove = 0;
        EntityType type = ENTITY;
        int id;
        Cardinal facing = NORTH;
        std::string path;
        BoundingBox box;
    private:
        static int ID;
};

std::vector<std::string> SerializeEntity(std::vector<Entity *> entities, int index);

#endif /* !ENTITIE_HPP_ */
