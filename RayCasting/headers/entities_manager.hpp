#pragma once

#include <list>
#include <map>
#include <string>
#include <SDL2/SDL_mixer.h>

typedef enum npc_status_ {
    WALK,
    PAIN,
    SHOOT,
    WAIT,
    DIYING
} npc_status_t;

class Game;

class Entity {
public:
    Entity(float x, float y, float size, float health, bool transparent) : position_x(x), position_y(y), size(size), health(health), transparent(transparent) {};
    virtual ~Entity() {};

    float position_x;
    float position_y;
    
    float camera_x;
    float camera_y;

    float size;
    float health;

    bool transparent;

    void draw(Game* game, SDL_Rect& rect);
    virtual void draw(Game* game);
    virtual bool update(Game* game) = 0; // return true if the entity have to be deleted
    virtual void damage(float value) = 0;

protected:

    SDL_Surface* surface;

};


class FireBall : public Entity {
public:
    float v_x;
    float v_y;
    FireBall(float x, float y, float v_x, float v_y);
    ~FireBall() {};
    void draw(Game* game) override;
    bool update(Game* game) override;
    void damage(float value) override {(void)value;};
private:
    int cooldown;
    static std::array<SDL_Rect, 5> balls_rects;
    static Mix_Chunk* explode;
};

class PlasmaBall : public Entity {
public:
    float v_x;
    float v_y;
    PlasmaBall(float x, float y, float v_x, float v_y);
    ~PlasmaBall() {};
    void draw(Game* game) override;
    bool update(Game* game) override;
    void damage(float value) override {(void)value;};
private:
    int cooldown;
    static std::array<SDL_Rect, 4> balls_rects;
};

class Ammos : public Entity {
public:
    Ammos(float x, float y, int quantity, int weapon_id);
    ~Ammos() {};
    void draw(Game* game) override;
    bool update(Game* game) override;
    void damage(float value) override {(void) value;};
private:
    int weapon_id;
    int quantity;
    static std::array<SDL_Rect, 4> ammos_rects;
};

class Weapons : public Entity {
public:
    Weapons(float x, float y, int munitions, int weapon_id);
    ~Weapons() {};
    void draw(Game* game) override;
    bool update(Game* game) override;
    void damage(float value) override {(void) value;};
private:
    int weapon_id;
    int munitions;
    static std::array<SDL_Rect, 5> weapons_rects;
};

class HealthPack : public Entity {
public:
    HealthPack(float x, float y, int pack_id);
    ~HealthPack() {};
    void draw(Game* game) override;
    bool update(Game* game) override;
    void damage(float value) override {(void) value;};
private:
    int pack_id;
    static std::array<SDL_Rect, 3> health_pack_rects;
};


class Armor : public Entity {
public:
    Armor(float x, float y, int armor_id);
    ~Armor() {};
    void draw(Game* game) override;
    bool update(Game* game) override;
    void damage(float value) override {(void) value;};
private:
    int armor_id;
    static std::array<SDL_Rect, 6> armor_rects;
};


class Barrel : public Entity {
public:
    Barrel(float x, float y);
    ~Barrel() {};
    void draw(Game* game) override {Entity::draw(game);};
    bool update(Game* game) override {(void)game; return false;};
    void damage(float value) override {(void)value;};
};



class Enemy : public Entity {
public:
    Enemy(float x, float y, float size, float health, float weapon_damage, bool final_enemy) :
        Entity(x,y,size,health, false), animation_cooldown(0), death_cooldown(9), velocity(2), weapon_damage(weapon_damage), final_enemy(final_enemy) {};
    virtual ~Enemy() {};
    
    bool update(Game* game) override;
    void damage(float value) override;
    void death();

    unsigned int animation_cooldown;
    unsigned int death_cooldown;
    bool direct_ray;
    float velocity;
    float weapon_damage;
    bool final_enemy; 

    float dir_x;
    float dir_y;

    npc_status_t status;

};


class Soldier1 : public Enemy {
public:
    Soldier1(float x, float y);
    ~Soldier1() {};
    void draw(Game* game) override;

private:
    static std::array<SDL_Rect, 4> walk_front_rects;
    static std::array<SDL_Rect, 2> shoot_rects;
    static std::array<SDL_Rect, 1> pain_rect;
    static std::array<SDL_Rect, 9> death1;
};


class Soldier2 : public Enemy {
public:
    Soldier2(float x, float y);
    ~Soldier2() {};
    void draw(Game* game) override;

private:
    static std::array<SDL_Rect, 4> walk_front_rects;
    static std::array<SDL_Rect, 2> shoot_rects;
    static std::array<SDL_Rect, 1> pain_rect;
    static std::array<SDL_Rect, 9> death1;
};

class Soldier3 : public Enemy {
public:
    Soldier3(float x, float y);
    ~Soldier3() {};
    void draw(Game* game) override;

private:
    static std::array<SDL_Rect, 4> walk_front_rects;
    static std::array<SDL_Rect, 2> shoot_rects;
    static std::array<SDL_Rect, 1> pain_rect;
    static std::array<SDL_Rect, 9> death1;
};

class CyberDemon : public Enemy {
public:
    CyberDemon(float x, float y);
    ~CyberDemon() {};
    void draw(Game* game) override;

private:
    static std::array<SDL_Rect, 4> walk_front_rects;
    static std::array<SDL_Rect, 2> shoot_rects;
    static std::array<SDL_Rect, 1> pain_rect;
    static std::array<SDL_Rect, 9> death1;
};







class Object_manager {
public:
    Object_manager(Game* game);

    void update();
    void start(int level_id);
    void draw();

    static SDL_Surface* getSurface(std::string name);

    std::list<Entity*> entities;
    Entity* targeted_entity;
    float targeted_entity_distance;

private:

    Game* game;
    static std::map<std::string, SDL_Surface*> surfaces;
};

