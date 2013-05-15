#ifndef BATTLE_GRID

#define BATTLE_GRID

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <windows.h>


class BattleGrid {
private:
    /**
    * Arrays used for shuffeling the ships on the grid
    */
    int _shipNuber[4] = {1, 2, 3, 4};
    int _shipSize[4] = {4, 3, 2, 1};

    int _xPos;
    int _yPos;
    int _sampling;
    bool _enemy;

    std::vector<int> _attackRow;
    std::vector<int> _attackColl;

    /**
    * Shooting flags
    */
    bool _startAttack;

    static bool checkBounds(int row, int coll);
    bool checkAvailability(int row, int coll);
    void replaceValues(int value_replaced, int value);

    bool checkNeighbours();
    void dotWounded();
    void dotDead();
    void resetAttack();

    int attackPosition(int position);
    int attack(int row, int coll);
public:

    /// Constants used as mesh values on the grid
    static const int MESH_NONE = 0;
    static const int MESH_MISS = 1;
    static const int MESH_SHIP = 2;
    static const int MESH_DEAD = 3;
    /// This one is special... it is used only while shuffling the ships
    static const int MESH_TEMP = 4;

    /// This are the constants for attacking
    static const int ATTACK_RIGHT = 0;
    static const int ATTACK_LEFT  = 1;
    static const int ATTACK_UP    = 2;
    static const int ATTACK_DOWN  = 3;

    /// Attack result
    static const int ATTACK_NONE = 0;
    static const int ATTACK_MISS = 1;
    static const int ATTACK_SUCCESS = 2;

    /// Constants tnat marks the orientation of the ship
    static const int ORIENTATION_HORIZONTAL = 0;
    static const int ORIENTATION_VERTICAL = 1;

    int _grid[10][10];
    int _attackResult;

    BattleGrid();
    BattleGrid(int xPos, int yPos, int sampling, bool enemy);

    void shuffleShip();
    void autoAttack();
    void showGrid();

    void drawGrid(HDC hdc);
    void drawFriendShip(HDC hdc);
};

#endif
