/**
 * @author Robin Duda 19931019
 * @date 2016-07-22
 *
 * Game class.
 */

#include <string>
#include <iostream>
#include <fstream>
#include "Game.h"

using namespace std;


const char WALL = '1';
const char OPEN = ' ';
const char FINISH = 'e';
const char START = 's';

char map[MAP_SIZE][MAP_SIZE];
Player *player = new Player();
Position *finish = new Position();

/**
 * Loads the level into a 2d map.
 */
void Load() {
    ifstream file("level.txt", fstream::in);
    char pos;
    int count = 0;

    if (file.good()) {
        while (file.get(pos)) {
            if (pos == WALL || pos == OPEN || pos == FINISH || pos == START) {
                map[count / MAP_SIZE][count % MAP_SIZE] = pos;
                count += 1;
            }
        }
        file.close();
    } else {
        exit(404);
    }
}

/**
 * Get the position of a marker.
 */
Position *Game::GetPosition(char tag) {
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            if (map[y][x] == tag) {
                Position *position = new Position();
                position->y = y;
                position->x = x;
                return position;
            }
        }
    }
    return nullptr;
}

void Game::SetSpawn() {
    player->position = GetPosition(START);
}

void Game::SetFinish() {
    finish = GetPosition(FINISH);
}

void Game::Reset() {
    delete finish;
    delete player->position;

    Load();
    SetSpawn();
    SetFinish();
    player->steps = 0;
}

Game::Game() {
    Reset();
}

char *Game::GetTile(int x, int y) {
    return (char *) std::string(1, map[x][y]).c_str();
}

int Game::GetPlayerX() {
    return player->position->x;
}

int Game::GetPlayerY() {
    return player->position->y;
}

const char *Game::GetPlayerSteps() {
    return std::to_string(player->steps).append(" steps").c_str();
}

const char *Game::GetPlayerCoords() {
    return std::string().append("x = ").append(std::to_string(player->position->y)).append(", y = ").append(
            std::to_string(player->position->x)).c_str();
}

/**
 * Check if a direction is blocked by a wall.
 */
bool Game::ValidMove(Position *player, DIRECTION direction) {
    Position *position = new Position();
    position->x = player->x;
    position->y = player->y;

    UpdatePosition(position, direction);

    bool valid = (map[position->x][position->y]) != WALL;
    delete position;
    return valid;
}

/**
 * Update a position with a direction.
 */
void Game::UpdatePosition(Position *position, DIRECTION direction) {
    switch (direction) {
        case NORTH:
            position->y -= 1;
            break;
        case WEST:
            position->x -= 1;
            break;
        case SOUTH:
            position->y += 1;
            break;
        case EAST:
            position->x += 1;
            break;
    }
}

/**
 * Attempt to move the player.
 */
void Game::MovePlayer(DIRECTION direction) {
    if (ValidMove(player->position, direction)) {
        UpdatePosition(player->position, direction);
        player->steps += 1;
    }
}

bool Game::IsFinished() {
    return (player->position->x == finish->x && player->position->y == finish->y);
}




