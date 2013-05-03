#include "BattleGrid.h"

BattleGrid::BattleGrid() {
    for (int i = 0; i < 4; i++) {
        _startAttack = false;

    }
}

/**
* Shuffles the the ships on the grid
*/
void BattleGrid::shuffleShip() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            _grid[i][j] = 0;
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < _shipNuber[i]; j++) {
            int shipSize = _shipSize[i];

            while (1) {
                bool placed_success = true;

                int row = rand() % 10;
                int coll = rand() % 10;
                int orientation = rand() % 2;

                if (orientation == ORIENTATION_HORIZONTAL) {
                    for (int k = 0; k < shipSize; k++) {
                        int temp_coll = coll + k;
                        if (!checkAvailability(row, temp_coll))
                            placed_success = false;
                    }
                }

                if (orientation == ORIENTATION_VERTICAL) {
                    for (int k = 0; k < shipSize; k++) {
                        int temp_row = row + k;
                        if (!checkAvailability(temp_row, coll))
                            placed_success = false;
                    }
                }
                if (placed_success) {
                    replaceValues(MESH_TEMP, MESH_SHIP);
                    break;
                } else
                    replaceValues(MESH_TEMP, MESH_NONE);
            }
        }
    }
}
/**
* Check if the selected values are not out of bounds
* @param int the row coordinate of the grid
* @param int the collumn coordinate of the grid
* @return bool whether it is on the bounds or not
*/
bool BattleGrid::checkBounds(int row, int coll) {
    if (row > 9 || row < 0)
        return false;
    if (coll > 9 || coll < 0)
        return false;

    return true;
}

/**
* Check if the selected coordinates can be placed on the grid
* @param int the row coordinate of the grid
* @param int the collumn coordinate of the grid
* @return bool whether the spot is available on the grid or not
*/
bool BattleGrid::checkAvailability(int row, int coll) {

    if (!checkBounds(row, coll))
        return false;

    /// Check if the coordinate has already a value
    if (_grid[row][coll] != MESH_NONE)
        return false;

    /// Gives the coordinate a temporary value
    _grid[row][coll] = MESH_TEMP;

    /// Check if the values around the cooridnate are not occupied
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = coll - 1; j <= coll + 1; j++) {
            if (checkBounds(i, j)) {
                if (_grid[i][j] == MESH_TEMP)
                    continue;
                if (_grid[i][j] != MESH_NONE)
                    return false;
            }
        }
    }
    return true;
}

/**
* Replace all the selceted value of the grid with specified value
* @param int value to be replaced
* @param int the replacing value
*/
void BattleGrid::replaceValues(int value_replaced, int value) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (_grid[i][j] == value_replaced)
                _grid[i][j] = value;
        }
    }
}

/**
* Shows the grid
*/
void BattleGrid::showGrid() {
    for (int i = 0; i < 10 ; i++) {
        for (int j = 0; j < 10; j++){
            std::cout << _grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl << std::endl;
    if (_attackResult == ATTACK_MISS)
        std::cout << "My term" << std::endl;
    else
        std::cout << "Bot's Term" << std::endl;
}


/**
* Check neighbours. It's needed to check if the ship is fully destroyed or not
* @return bool if there are dead meshes with alive meshes in neighbourhood
*/
bool BattleGrid::checkNeighbours() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (_grid[i][j] == MESH_DEAD) {
                if (checkBounds(i + 1, j) && _grid[i + 1][j] == MESH_SHIP)
                    return true;
                if (checkBounds(i - 1, j) && _grid[i - 1][j] == MESH_SHIP)
                    return true;
                if (checkBounds(i, j + 1) && _grid[i][j + 1] == MESH_SHIP)
                    return true;
                if (checkBounds(i, j - 1) && _grid[i][j - 1] == MESH_SHIP)
                    return true;
            }
        }
    }
    return false;
}

/**
* In case of woundieg a ship, fills the close area with MISS valuess
*/
void BattleGrid::dotWounded() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (_grid[i][j] == MESH_DEAD) {
                if (checkBounds(i + 1, j + 1))
                    _grid[i + 1][j + 1] = MESH_MISS;
                if (checkBounds(i - 1, j + 1))
                    _grid[i - 1][j + 1] = MESH_MISS;
                if (checkBounds(i - 1, j - 1))
                    _grid[i - 1][j - 1] = MESH_MISS;
                if (checkBounds(i + 1, j - 1))
                    _grid[i + 1][j - 1] = MESH_MISS;
            }
        }
    }
}

/**
* In case of destroying a ship, fills the close area with MISS valuess
*/
void BattleGrid::dotDead() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (_grid[i][j] == MESH_DEAD) {
                for (int k = i - 1; k <= i + 1; k++) {
                    for (int l = j - 1; l <= j + 1; l++) {
                        if (checkBounds(k, l) && _grid[k][l] != MESH_DEAD)
                            _grid[k][l] = MESH_MISS;
                    }
                }
            }
        }
    }
}

/**
* Atacks the Grid randomly. (It prays to the god [kami] of probability)
* Still it implements a little bit of AI. In case a ship is wounded, it-s shooting around the wound
*/
void BattleGrid::autoAttack() {
    if (_startAttack) {
        if (_horizAttack) {
            int bidlaCod = 0;
            while (1) {
                int position = rand() % 2;
                int result = attackPosition(position);

                if (result == ATTACK_MISS) {
                    _attackResult = result;
                    break;
                }
                if (result == ATTACK_SUCCESS) {
                    _attackResult = result;
                    if (checkNeighbours())
                        dotWounded();
                    else {
                        dotDead();
                        resetAttack();
                    }

                    break;
                }
                bidlaCod++;
                if (bidlaCod < 1000) {
                    _horizAttack;
                    break;
                }
            }
        } else if (_vertAttack) {
            while (1) {
                int position = rand() % 2 + 2;
                int result = attackPosition(position);

                if (result == ATTACK_MISS) {
                    _attackResult = result;
                    break;
                }
                if (result == ATTACK_SUCCESS) {
                    _attackResult = result;
                    if (checkNeighbours())
                        dotWounded();
                    else {
                        dotDead();
                        resetAttack();
                    }
                    break;
                }
            }
        } else {
            while (1) {
                int position = rand() % 4;
                int result = attackPosition(position);

                if (result == ATTACK_MISS) {
                    _attackResult = result;
                    break;
                }
                if (result == ATTACK_SUCCESS) {
                    _attackResult = result;
                    if (checkNeighbours()) {
                        dotWounded();
                        if (position == ATTACK_RIGHT || position == ATTACK_LEFT) {
                            _horizAttack = true;
                            _vertAttack = false;
                        } else {
                            _vertAttack = true;
                            _horizAttack = false;
                        }
                    } else {
                        dotDead();
                        resetAttack();
                    }
                    break;
                }
            }
        }
    } else {
        while (1) {
            int row = rand() % 10;
            int coll = rand() % 10;

            int result = attack(row, coll);

            if (result == ATTACK_MISS) {
                _attackResult = result;
                resetAttack();
                break;
            }

            if (result == ATTACK_SUCCESS) {
                    _attackResult = result;
                if (checkNeighbours()) {
                    dotWounded();
                    _startAttack = true;
                } else {
                    dotDead();
                    resetAttack();
                }
                break;
            }
        }
    }
}

/**
* The function checks all the dead elements and checks if it can shoot to the coresponded position. If valid then shoots.
* @param int the shooting position (usally a constatn)
* @return retuns and int wich represents the result of shooting the position. (Miss, Wound, Nothing)
*/
int BattleGrid::attackPosition(int position) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            switch (position) {
            case ATTACK_RIGHT:
                // AICI
                if (_grid[i][j] == MESH_DEAD && checkBounds(i, j + 1) && (_grid[i][j + 1] == MESH_NONE || _grid[i][j + 1] == MESH_SHIP)) {
                    int result = attack(i, j + 1);
                    /// Check if it is different from none
                    if (result != ATTACK_NONE)
                        return result;
                }
                break;
            case ATTACK_LEFT:
                if (_grid[i][j] == MESH_DEAD && checkBounds(i, j - 1) && (_grid[i][j - 1] == MESH_NONE || _grid[i][j - 1] == MESH_SHIP)) {
                    int result = attack(i, j - 1);
                    /// Check if it is different from none
                    if (result != ATTACK_NONE)
                        return result;
                }
                break;
            case ATTACK_UP:
                if (_grid[i][j] == MESH_DEAD && checkBounds(i - 1, j) && (_grid[i - 1][j] == MESH_NONE || _grid[i - 1][j] == MESH_SHIP)) {
                    int result = attack(i - 1, j);
                    /// Check if it is different from none
                    if (result != ATTACK_NONE)
                        return result;
                }
                break;
            case ATTACK_DOWN:
                if (_grid[i][j] == MESH_DEAD && checkBounds(i + 1, j) && (_grid[i + 1][j] == MESH_NONE || _grid[i + 1][j] == MESH_SHIP)) {
                    int result = attack(i + 1, j);
                    /// Check if it is different from none
                    if (result != ATTACK_NONE)
                        return result;
                }
                break;
            }
        }
    }
    return ATTACK_NONE;
}

/**
* Attacks the grid. If somenthig was shoot then the modifications are maid into the main grid
* @param of type int. Represents the row coordinate.
* @param of type
*/
int BattleGrid::attack(int row, int coll) {
    if (_grid[row][coll] == MESH_MISS || _grid[row][coll] == MESH_DEAD)
        return ATTACK_NONE;
    if (_grid[row][coll] == MESH_NONE) {
        _grid[row][coll] = MESH_MISS;

        return ATTACK_MISS;
    }
    if (_grid[row][coll] == MESH_SHIP) {
        _grid[row][coll] = MESH_DEAD;
        return ATTACK_SUCCESS;
    }
}

/**
* Resets all the shooting flags
*/
void BattleGrid::resetAttack() {
    _startAttack = false;
    _horizAttack = false;
    _vertAttack = false;
}



