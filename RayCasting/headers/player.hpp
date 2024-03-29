#pragma once
#include <cmath>

constexpr float PLAYER_VELOCITY = 4.f;
constexpr float PLAYER_ROTATION_VELOCITY = 90.0;


struct cell_;
typedef struct cell_ cell_t;

typedef enum player_state_ {
    CALM,
    CLENCH,
    SCREAM,
    LAUGHT
} player_state_t;

class Game;
class Weapon;

class Player {

public:
    Player(Game* game);
    void draw();
    void update();
    void load();
    void start(int level_id);
    void rotate(float relative_mov);
    float get_angle();
    void damage(float value);
    void switch_weapon();

    float position_x;
    float position_y;
    float position_z;

    float dir_x;
    float dir_y;

    float plane_x;
    float plane_y;
    
    float health;
    float armor;
    int frag;

    player_state_t state;
    int blood_level;
    int game_over;
    int game_won;

    int weapon;
    Weapon* weapons[6];

    bool armors[3];
    bool keys[3];

private:

    Game* game;
    unsigned int state_change_cooldown;
    void pathfind();
};