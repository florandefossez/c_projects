#pragma once
#include <vector>
#include <map>
#include <string>

class Game;

class Weapon {
    public:

    float damage;
    unsigned int cooldown;
    unsigned int munitions;
    SDL_Texture* texture;

    Weapon(float damage, unsigned int munitions) : damage(damage), munitions(munitions), cooldown(0) {};
    virtual ~Weapon() {};

    virtual void draw(SDL_Renderer* renderer) {};
    virtual void update(bool tick) {};
    virtual void shoot() {};

    bool can_shoot() {return cooldown == 0;};


};


class ShotGun : public Weapon {
    public:

    ShotGun(SDL_Renderer* renderer);
    ~ShotGun() override {};

    static std::array<SDL_Rect, 6> shoot_rects;

    void draw(SDL_Renderer* renderer) override;
    void update(bool tick) override;
    void shoot() override;
};