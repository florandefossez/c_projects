#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "headers/raycaster.hpp"
#include "headers/player.hpp"
#include "headers/map.hpp"
#include "headers/entities_manager.hpp"
#include "headers/game.hpp"



Raycaster::Raycaster(Game* game) : game(game) {}


void Raycaster::load() {
    SDL_Surface* tmp = IMG_Load("ressources/redbrick.png");
    brick_surface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(tmp);

    tmp = IMG_Load("ressources/mossy.png");
    mosse_surface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(tmp);

    tmp = IMG_Load("ressources/greystone.png");
    stone_surface = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(tmp);

    rays_lenght = new float[game->width];

    opening_state = 0;
    opening_door_x = 0;
    opening_door_y = 0;
}

void Raycaster::update_width() {
    free(rays_lenght);
    rays_lenght = new float[game->width];
}


void Raycaster::update() {
    if (opening_state > 0 && game->animation) {
        opening_state -= 0.1;
    }
    if (opening_state <= 0) {
        game->map.map[opening_door_x][opening_door_y].is_wall = false;
        game->map.map[opening_door_x][opening_door_y].is_door = false;
        opening_state = 0;
    }
}

void Raycaster::draw() {
    draw_floor();
    draw_wall();
}


void Raycaster::draw_floor() {

    Uint32* mosse_pixels = (Uint32*) mosse_surface->pixels;
    Uint32* stone_pixels = (Uint32*) stone_surface->pixels;

    // again we use rays to find the coordinate of the floor
    float left_ray_x = game->player.dir_x - game->player.plane_x;
    float left_ray_y = game->player.dir_y - game->player.plane_y;
    float right_ray_x = game->player.dir_x + game->player.plane_x;
    float right_ray_y = game->player.dir_y + game->player.plane_y;

    int height = 2 * game->width / 3;

    // p index of the scanline from the center of the screen
    for (int p=1; p<height/2; p++) {

        // horizontal distance from the player to the floor line (x distance of the ray)
        float floor_distance = 4.f / 4.2f * height / p * game->player.position_z;

        // floor coordinate of the left ray
        float floor_x = game->player.position_x + floor_distance * left_ray_x;
        float floor_y = game->player.position_y + floor_distance * left_ray_y;

        // floor step to go to the next right pixel on the screen
        float floor_step_x = floor_distance * (right_ray_x - left_ray_x) / game->width;
        float floor_step_y = floor_distance * (right_ray_y - left_ray_y) / game->width;

        for (int x=0; x < game->width; x++) {

            // the cell coord is simply got from the integer parts of floorX and floorY
            int current_cell_x = (int)(floor_x);
            int current_cell_y = (int)(floor_y);

            // get the texture coordinate from the fractional part
            int tx = (int) std::abs(64.f * (floor_x - current_cell_x));
            int ty = (int) std::abs(64.f * (floor_y - current_cell_y));

            floor_x += floor_step_x;
            floor_y += floor_step_y;

            // choose texture and draw the pixel on the image
            if ((current_cell_x + current_cell_y)%2 == 1) {
                game->scene_pixels[x + (p-1) * game->width + height/2 * game->width ] = mosse_pixels[tx + 64*ty];
            } else {
                game->scene_pixels[x + (p-1) * game->width + height/2 * game->width ] = stone_pixels[tx + 64*ty];
            }

        }
    }
}


void Raycaster::draw_wall() {

    Uint32* brick_pixels = (Uint32*) brick_surface->pixels;

    int height = 2 * game->width / 3;
    char collision_side;
    float perp_rays_lenght;
    float texture_offset;

    for (int r=0; r<game->width; r++) {

        // direction step
        int step_x;
        int step_y;

        // euclidian ray length between the position and the wall
        float ray_length = 0;

        // x and y ray sides
        float x_ray_length = 0;
        float y_ray_length = 0;

        // x coord of the ray in the camera plane in [-1,1]
        float camera = 2.f * (float) r / (float) game->width - 1.f;

        // ray directions
        float ray_dir_x = game->player.dir_x + game->player.plane_x * camera;
        float ray_dir_y = game->player.dir_y + game->player.plane_y * camera;

        // ray lengths per x,y step
        float x_ray_unit_length = std::abs(1 / ray_dir_x);
        float y_ray_unit_length = std::abs(1 / ray_dir_y);

        // player cell location
        int current_cell_x = (int) game->player.position_x;
        int current_cell_y = (int) game->player.position_y;

        if (ray_dir_x > 0) { // we look at west
            x_ray_length += x_ray_unit_length * (1 - (game->player.position_x - (float) current_cell_x));
            step_x = 1;

        } else if (ray_dir_x < 0) { //we look at east
            x_ray_length += x_ray_unit_length * (game->player.position_x - (float) current_cell_x);
            step_x = -1;
        
        } else {
            step_x = 0;
        }

        if (ray_dir_y > 0) { // we look at south
            y_ray_length += y_ray_unit_length * (1 - (game->player.position_y - (float) current_cell_y));
            step_y = 1;
            
        } else if (ray_dir_y < 0) { //we look at north
            y_ray_length += y_ray_unit_length * (game->player.position_y - (float) current_cell_y);
            step_y = -1;

        } else {
            step_y = 0;
        }


        // we MUST break this loop
raycast_label:
        for (int i=0; i<50; i++) {
            if (x_ray_length < y_ray_length) {
                ray_length = x_ray_length;
                x_ray_length += x_ray_unit_length;

                current_cell_x += step_x;
                collision_side = 'x';
            }
            else {
                ray_length = y_ray_length;
                y_ray_length += y_ray_unit_length;

                current_cell_y += step_y;
                collision_side = 'y';
            }
            if (game->map.collide(current_cell_x,current_cell_y)){
                break;
            }
        }
        
        // targeted wall
        if (r == game->width/2) {
            targeted_wall_x = current_cell_x;
            targeted_wall_y = current_cell_y;
        }

        

        // we don't use the real ray length to avoid fisheye effect
        if (collision_side == 'x') {
            perp_rays_lenght = x_ray_length - x_ray_unit_length;
            texture_offset = game->player.position_y + ray_length * ray_dir_y - (float) current_cell_y;

            if (game->map.map[current_cell_x][current_cell_y].is_door) {
                float cell_y = game->player.position_y + ray_length * ray_dir_y + 0.4f/ray_dir_x * ray_dir_y;
                if (int(cell_y) == current_cell_y) {
                    texture_offset = cell_y - floor(cell_y);
                    if (opening_state > 0 && current_cell_x == opening_door_x && current_cell_y == opening_door_y && texture_offset > opening_state) {
                        goto raycast_label;
                    } else {
                        perp_rays_lenght += 0.4f/ray_dir_x;
                    }
                } else {
                    ray_length = y_ray_length;
                    y_ray_length += y_ray_unit_length;

                    current_cell_y += step_y;
                    collision_side = 'y';
                    perp_rays_lenght = y_ray_length - y_ray_unit_length;
                    texture_offset = game->player.position_x + ray_length * ray_dir_x - (float) current_cell_x;
                }
            }
            
        } else {
            perp_rays_lenght = y_ray_length - y_ray_unit_length;
            texture_offset = game->player.position_x + ray_length * ray_dir_x - (float) current_cell_x;

            if (game->map.map[current_cell_x][current_cell_y].is_door) {
                float cell_x = game->player.position_x + ray_length * ray_dir_x + 0.4f/ray_dir_y * ray_dir_x;
                if (int(cell_x) == current_cell_x) {
                    texture_offset = cell_x - floor(cell_x);
                    if (opening_state > 0 && current_cell_x == opening_door_x && current_cell_y == opening_door_y && texture_offset > opening_state) {
                        goto raycast_label;
                    } else {
                        perp_rays_lenght += 0.4f/ray_dir_y;
                    }
                } else {
                    ray_length = x_ray_length;
                    x_ray_length += x_ray_unit_length;

                    current_cell_x += step_x;
                    collision_side = 'x';
                    perp_rays_lenght = x_ray_length - x_ray_unit_length;
                    texture_offset = game->player.position_y + ray_length * ray_dir_y - (float) current_cell_y;
                }
            }

        }

        rays_lenght[r] = ray_length;


        // where to start and end the dawing of the strip
        int start,end;
        if (perp_rays_lenght<1) {
            start = 0;
            end = height;
        } else {
            start = (height - height/perp_rays_lenght)/2;
            end = start + height/perp_rays_lenght;
        }

        // How much to increase the texture coordinate per screen pixel
        float step = 64.f / height * perp_rays_lenght;
        float texPos = (start - height / 2 + height/perp_rays_lenght / 2) * step;

        // draw
        for (int y=start; y<end; y++) {
            int texY = (int)texPos & (64 - 1);
            texPos += step;
            Uint32 color = brick_pixels[static_cast<unsigned int>(64.f * texY + texture_offset*64.f)];
            game->scene_pixels[r + y * game->width] = color;
        }
    }
}

void Raycaster::trigger() {
    if (!game->map.map[targeted_wall_x][targeted_wall_y].is_door) return;
    opening_door_x = targeted_wall_x;
    opening_door_y = targeted_wall_y;
    opening_state = 1.f;
}