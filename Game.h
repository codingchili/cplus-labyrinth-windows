/**
 * @author Robin Duda 19931019
 * @date 2016-07-22
 *
 * Game class.
 */

#ifndef ASSIGNMENT_WINDOWS_GAME_H
#define ASSIGNMENT_WINDOWS_GAME_H


typedef int DIRECTION;
const int NORTH = 0;
const int WEST = 1;
const int SOUTH = 2;
const int EAST = 3;
const int MAP_SIZE = 20;

struct Position {
    int x;
    int y;
};

struct Player {
    Position *position;
    int steps;
};


class Game {
public:
    Game();

    int GetPlayerX();

    int GetPlayerY();

    /**
     * Attempts to move the player in the given direction, no effect on invalid moves.
     */
    void MovePlayer(DIRECTION direction);

    const char *GetPlayerSteps();

    const char *GetPlayerCoords();

    /**
     * Returns the tile type at the coordinates specified.
     */
    char *GetTile(int x, int y);

    bool IsFinished();

    void Reset();

private:
    void SetSpawn();

    void SetFinish();

    Position *GetPosition(char tag);

    bool ValidMove(Position *position, DIRECTION direction);

    void UpdatePosition(Position *position, DIRECTION direction);
};

#endif //ASSIGNMENT_WINDOWS_GAME_H
