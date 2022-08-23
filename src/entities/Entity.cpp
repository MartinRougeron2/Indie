/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Entity
*/

#include <raylib.h>
#include <string>
#include "entities/Entity.hpp"
#include "../include/proto.hpp"
#include "iostream"

int Entity::ID = 0;

Entity::Entity(std::string path, Vector3 pos, bool isTextureLoaded, bool isModelLoaded, float scale, int id, Cardinal facing_, EntityType type)
{
    if (id == -1)
        id = ID++;
    this->pos = pos;
    this->type = type;
    this->path = path;
    if (isModelLoaded || isTextureLoaded)
    {
        this->model = LoadModel(path.c_str());// ERR => Non-existent physical address at address 0x11E82000
        path = path.substr(0, path.size() - 3);
        path += "png";
        if (isTextureLoaded)
            model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture(path.c_str());
    }
    else
    {
        this->model = {0};
    }
    collision = true;
    this->id = id;
    this->facing = facing_;
    this->scale = scale;
    if (this->model.meshCount > 0) {
        box = GetMeshBoundingBox(this->model.meshes[0]);
    }
}

Entity::Entity(Entity &other, int id)
{
    if (id == -1)
        id = ID++;
    this->pos = other.pos;
    this->model = other.model;
    this->type = other.type;
    this->id = id;
    this->path = other.path;
    this->facing = other.facing;
    this->collision = other.collision;
    this->scale = other.scale;
    if (this->model.meshCount > 0) {
        box = GetMeshBoundingBox(this->model.meshes[0]);
    }
}

Entity::Entity(Entity &other, Vector3 pos, int id, Cardinal facing_, EntityType type)
{
    if (id == -1)
        id = ID++;
    this->pos = pos;
    this->model = other.model;
    this->type = type;
    this->id = id;
    this->path = other.path;
    this->facing = facing_;
    this->scale = other.scale;
    if (this->model.meshCount > 0) {
        box = GetMeshBoundingBox(this->model.meshes[0]);
    }
}

Entity::~Entity()
{
}

Vector3 Entity::getPosition() const
{
    return this->pos;
}

void Entity::setPosition(Vector3 newPos)
{
    this->pos.x = newPos.x;
    this->pos.y = newPos.y;
    this->pos.z = newPos.z;
    this->modified.clear();
}

void Entity::move(Vector3 newPos)
{
    this->pos.x += newPos.x;
    this->pos.y += newPos.y;
    this->pos.z += newPos.z;
    this->modified.clear();
}

Vector3 Entity::moveTest(Vector3 newPos) const
{
    Vector3 _pos;

    _pos.x = pos.x + newPos.x;
    _pos.y = pos.y + newPos.y;
    _pos.z = pos.z + newPos.z;
    return _pos;
}

Vector3 Vector3Scale(Vector3 vector, float Scale, Vector3 pos)
{
    float x = vector.x * Scale + pos.x;
    float y = vector.y * Scale + pos.y;
    float z = vector.z * Scale + pos.z;

    return {x, y, z};
}

Vector3 operator+(const Vector3 a, const Vector3 b)
{
    return {
        a.x + b.x,
        a.y + b.y,
        a.y + b.y,
    };
}

BoundingBox Entity::getBox() const
{
    return box;
}

bool Entity::operator&&(const Entity &other) const
{
    BoundingBox us = this->getBox();
    BoundingBox their = other.getBox();

    us.min = Vector3Scale(us.min, this->scale, this->pos);
    us.max = Vector3Scale(us.max, this->scale, this->pos);
    their.min = Vector3Scale(their.min, other.scale, other.pos);
    their.max = Vector3Scale(their.max, other.scale, other.pos);

    return CheckCollisionBoxes(us, their);
}

bool Entity::operator&&(Vector3 pos) const
{
    if (this->pos.x == pos.x && this->pos.z == pos.z && this->pos.y == pos.y)
        return true;
    return false;
}

void Entity::displayEntity(void)
{
    static int framesCounter = 0;
    static int currentFrameWidth = 0;
    static int currentFrameHeight = 0;

    if (this->model.meshCount == 0)
        return;

        // TODO PUT FRAMECOUNTER IN MAIN GAME LOOP AND PASS IT IN ARGS
        /*framesCounter++;
        if (framesCounter >= (FPS / this->frame_info.speed)) {
            framesCounter = 0;
            currentFrameWidth++;
            if (currentFrameWidth > this->frame_info.width_nb - 1) {
                currentFrameHeight++;
                currentFrameWidth = 0;
            }
            if (currentFrameHeight * this->frame_info.width_nb + currentFrameWidth > this->frame_info.nb_frame) {
                currentFrameHeight = 0;
                currentFrameWidth = 0;
            }
            this->frame.y = (float)currentFrameHeight * (float)this->frame.height / this->frame_info.height_nb;
            this->frame.x = (float)currentFrameWidth * (float)this->frame.width / this->frame_info.width_nb;
        }*/
    float rotation_;
    switch (this->facing) {
        case SOUTH:
            rotation_ = 180.0f;
            break;
        case WEST:
            rotation_ = 270.0f;
            break;
        case EAST:
            rotation_ = 90.0f;
            break;
        default:
            rotation_ = 1.0f;
    }
    DrawModelEx(this->model, this->pos, {0, 1.0f, 0}, rotation_, {this->scale, this->scale, this->scale}, WHITE); // ! ERR on draw incomplete models
}

void Entity::setFacing(Vector3 direction)
{
    if (direction.x == 0)
        this->facing = direction.z > 0 ? NORTH : SOUTH;
    else if (direction.z == 0)
        this->facing = direction.x > 0 ? EAST : WEST;
    else
        this->facing = NORTH;
}

std::ostream& operator<< (std::ostream& out, Cardinal v) {
    switch (v) {
        case NORTH:
            std::cout << "NORTH" ;
            break;
        case SOUTH:
            std::cout << "SOUTH" ;
            break;
        case EAST:
            std::cout << "EAST ";
            break;
        case WEST:
            std::cout << "WEST ";
            break;
    }
    return out;
}
