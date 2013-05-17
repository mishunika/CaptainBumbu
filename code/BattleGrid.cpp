#include "BattleGrid.h"

BattleGrid::BattleGrid() {
    _startAttack = false;
}

BattleGrid::BattleGrid(int xPos, int yPos, int sampling, bool enemy) {
    _xPos = xPos;
    _yPos = yPos;
    _sampling = sampling;
    _startAttack = false;
    _enemy = enemy;
    _shipNuber[0] = 1;
    _shipNuber[1] = 2;
    _shipNuber[2] = 3;
    _shipNuber[3] = 4;
    _shipSize[0] = 4;
    _shipSize[1] = 3;
    _shipSize[2] = 2;
    _shipSize[3] = 1;
}

/**
* Shuffles the the ships on the grid
*/
void BattleGrid::shuffleShip() {
    srand (time(NULL));
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            _grid[i][j] = 0;
        }
    }

    /*
     * easterShipType selects one of the ship types from 1, 2, 3, 4 cells.
     * easterShipIndex selects the ship of the specified type
     * easterShipCell the cell index that will contain a bumb :)
     */
    int easterShipType = rand() % 4;
    int easterShipIndex = rand() % _shipNuber[easterShipType];
    int easterShipCell = rand() % _shipSize[easterShipIndex];

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
                    if(easterShipType == i && easterShipIndex == j)
                    {
                        char * message = new char[100];
                        if(orientation == ORIENTATION_HORIZONTAL)
                        {
                            _grid[row][coll + easterShipCell] = MESH_BUMB;
                            sprintf(message, "%d %d", row, coll+easterShipCell);
                        }
                        else
                        {
                            _grid[row + easterShipCell][coll] = MESH_BUMB;
                            sprintf(message, "%d %d", row + easterShipCell, coll);
                        }

                        MessageBox(NULL, message, "AE", MB_OK);
                    }
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
                if (checkBounds(i + 1, j) && (_grid[i + 1][j] == MESH_SHIP || _grid[i + 1][j] == MESH_BUMB))
                    return true;
                if (checkBounds(i - 1, j) && (_grid[i - 1][j] == MESH_SHIP || _grid[i - 1][j] == MESH_BUMB))
                    return true;
                if (checkBounds(i, j + 1) && (_grid[i][j + 1] == MESH_SHIP || _grid[i][j + 1] == MESH_BUMB))
                    return true;
                if (checkBounds(i, j - 1) && (_grid[i][j - 1] == MESH_SHIP || _grid[i][j - 1] == MESH_BUMB))
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
            if (_grid[i][j] == MESH_DEAD || _grid[i][j] == MESH_BUMB_DEAD) {
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
                        if (checkBounds(k, l) && _grid[k][l] != MESH_DEAD && _grid[k][l] != MESH_BUMB_DEAD)
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
                } else {
                    dotDead();
                    resetAttack();
                }
                break;
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

    for (int k = 0; k < _attackRow.size(); k++) {
        int i = _attackRow[k];
        int j = _attackColl[k];
        switch (position) {
        case ATTACK_RIGHT:
            if (_grid[i][j] == MESH_DEAD && checkBounds(i, j + 1) && (_grid[i][j + 1] == MESH_NONE || _grid[i][j + 1] == MESH_SHIP || _grid[i][j + 1] == MESH_BUMB)) {
                int result = attack(i, j + 1);
                /// Check if it is different from none
                if (result != ATTACK_NONE)
                    return result;
            }
            break;
        case ATTACK_LEFT:
            if (_grid[i][j] == MESH_DEAD && checkBounds(i, j - 1) && (_grid[i][j - 1] == MESH_NONE || _grid[i][j - 1] == MESH_SHIP || _grid[i][j - 1] == MESH_BUMB)) {
                int result = attack(i, j - 1);
                /// Check if it is different from none
                if (result != ATTACK_NONE)
                    return result;
            }
            break;
        case ATTACK_UP:
            if (_grid[i][j] == MESH_DEAD && checkBounds(i - 1, j) && (_grid[i - 1][j] == MESH_NONE || _grid[i - 1][j] == MESH_SHIP || _grid[i - 1][j] == MESH_BUMB)) {
                int result = attack(i - 1, j);
                /// Check if it is different from none
                if (result != ATTACK_NONE)
                    return result;
            }
            break;
        case ATTACK_DOWN:
            if (_grid[i][j] == MESH_DEAD && checkBounds(i + 1, j) && (_grid[i + 1][j] == MESH_NONE || _grid[i + 1][j] == MESH_SHIP || _grid[i + 1][j] == MESH_BUMB)) {
                int result = attack(i + 1, j);
                /// Check if it is different from none
                if (result != ATTACK_NONE)
                    return result;
            }
            break;
        }
    }

    return ATTACK_NONE;
}

/**
* Attacks the grid. If somenthig was shoot then the modifications are maid into the main grid
* @param int, represents the row coordinate
* @param int, represents the column coordinate
*/
int BattleGrid::attack(int row, int coll) {
    if (_grid[row][coll] == MESH_MISS || _grid[row][coll] == MESH_DEAD)
        return ATTACK_NONE;
    if (_grid[row][coll] == MESH_NONE) {
        _grid[row][coll] = MESH_MISS;

        return ATTACK_MISS;
    }
    if (_grid[row][coll] == MESH_SHIP) {
        _attackRow.push_back(row);
        _attackColl.push_back(coll);
        _grid[row][coll] = MESH_DEAD;
        return ATTACK_SUCCESS;
    }

    if (_grid[row][coll] == MESH_BUMB)
    {
        // Oh, you've just killed the captain.
        _grid[row][coll] = MESH_BUMB_DEAD;
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                if(_grid[i][j] == MESH_SHIP)
                    _grid[i][j] = MESH_DEAD;
            }
        }
        return ATTACK_SUCCESS;
    }
    return ATTACK_NONE;
}

/**
* Resets all the shooting flags
*/
void BattleGrid::resetAttack() {
    _startAttack = false;
    _attackRow.clear();
    _attackColl.clear();
}

/**
* Draw the grid of ships
* @param HDC handle for the displayed DC, for painting
*/
void BattleGrid::drawGrid(HDC hdc) {
    HPEN hpen = CreatePen(PS_DOT, 1, RGB(166, 166, 166));
    SelectObject(hdc, hpen);
    for (int i = _xPos; i <= _xPos + _sampling * 10; i += _sampling) {
        MoveToEx(hdc, i, _yPos, NULL);
        LineTo(hdc, i, _yPos + _sampling * 10);
    }

    for (int i = _yPos; i <= _yPos + _sampling * 10; i += _sampling) {
        MoveToEx(hdc, _xPos, i, NULL);
        LineTo(hdc, _xPos + _sampling * 10, i);
    }
    DeleteObject(hpen);
    if (!_enemy)
        drawFriendShip(hdc);
}

/**
* Draw ships for the friend grid
* @param HDC handle for the displayed DC, for painting
*/
void BattleGrid::drawFriendShip(HDC hdc) {
    HPEN hpen = CreatePen(PS_SOLID, 2, RGB(120, 120, 120));
    SelectObject(hdc, hpen);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (_grid[i][j] == MESH_SHIP || _grid[i][j] == MESH_DEAD || _grid[i][j] == MESH_BUMB || _grid[i][j] == MESH_BUMB_DEAD) {
                if (!checkBounds(i, j - 1) || (_grid[i][j - 1] == MESH_NONE || _grid[i][j - 1] == MESH_MISS)) {
                    MoveToEx(hdc, _xPos + j * _sampling, _yPos + i * _sampling, NULL);
                    LineTo(hdc, _xPos + j * _sampling, _yPos + i * _sampling + _sampling);
                }
                if (!checkBounds(i, j + 1) || (_grid[i][j + 1] == MESH_NONE || _grid[i][j + 1] == MESH_MISS)) {
                    MoveToEx(hdc, _xPos + j * _sampling + _sampling, _yPos + i * _sampling, NULL);
                    LineTo(hdc, _xPos + j * _sampling + _sampling, _yPos + i * _sampling + _sampling);
                }
                if (!checkBounds(i - 1, j) || (_grid[i - 1][j] == MESH_NONE || _grid[i - 1][j] == MESH_MISS)) {
                    MoveToEx(hdc, _xPos + j * _sampling, _yPos + i * _sampling, NULL);
                    LineTo(hdc, _xPos + j * _sampling + _sampling, _yPos + i * _sampling);
                }
                if (!checkBounds(i + 1, j) || (_grid[i + 1][j] == MESH_NONE || _grid[i + 1][j] == MESH_MISS)) {
                    MoveToEx(hdc, _xPos + j * _sampling, _yPos + i * _sampling + _sampling, NULL);
                    LineTo(hdc, _xPos + j * _sampling + _sampling, _yPos + i * _sampling + _sampling);
                }
            }
        }
    }
    DeleteObject(hpen);
}

/**
*
* @param HDC handle for the displayed DC, for painting
* @param HBITMAP hatch bitmap to the hatched element
*/
void BattleGrid::drawLivingShips(HDC hdc, HBITMAP hatch, HBITMAP bumbu) {
    HDC hdcMem = CreateCompatibleDC(hdc);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (_grid[i][j] == MESH_SHIP) {
                SelectObject (hdcMem, hatch);
                BitBlt(hdc, _xPos + j * _sampling + 3, _yPos + i * _sampling + 1, 35, 39, hdcMem, 0, 0, SRCCOPY);
            }

            if (_grid[i][j] == MESH_BUMB) {
                SelectObject (hdcMem, bumbu);
                BitBlt(hdc, _xPos + j * _sampling + 3, _yPos + i * _sampling + 1, 35, 39, hdcMem, 0, 0, SRCCOPY);
            }
        }
    }
    DeleteDC(hdcMem);
}

/**
* Draw all the dead & missed elements
* @param HDC handle for the displayed DC, for painting
* @param HBITMAP dead bitmap to dead element
* @param HBITMAP miss bitmap to a missed element
*/
void BattleGrid::drawDamage(HDC hdc, HBITMAP dead, HBITMAP miss, HBITMAP bumbu_x) {
    HDC hdcMem = CreateCompatibleDC(hdc);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (_grid[i][j] == MESH_DEAD) {
                SelectObject (hdcMem, dead);
                BitBlt(hdc, _xPos + j * _sampling + 2, _yPos + i * _sampling + 1, 35, 39, hdcMem, 0, 0, SRCCOPY);
            }
            if (_grid[i][j] == MESH_MISS) {
                SelectObject (hdcMem, miss);
                BitBlt(hdc, _xPos + j * _sampling + 3, _yPos + i * _sampling + 1, 35, 39, hdcMem, 0, 0, SRCCOPY);
            }
            if (_grid[i][j] == MESH_BUMB_DEAD) {
                SelectObject (hdcMem, bumbu_x);
                BitBlt(hdc, _xPos + j * _sampling + 3, _yPos + i * _sampling + 1, 35, 39, hdcMem, 0, 0, SRCCOPY);
            }
        }
    }
    DeleteDC(hdcMem);
}

/**
* It draws the grid with white color and forces to redraw the rectangle
* @param HWND the main window handler
* @param HDC handle for the dispalied DC, for painting
*/
void BattleGrid::invalidateGrid(HWND hwnd, HDC hdc) {
    RECT rect;
    rect.left = _xPos - 1;
    rect.right = _xPos + _sampling * 10 + 1;
    rect.top = _yPos - 1;
    rect.bottom = _yPos + _sampling * 10 + 1;
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
    InvalidateRect(hwnd, &rect, false);
}
/**
* The methods attack the ship having the the coordinate where the mouse was pressed
* @param int x coordinate from GDI
* @param int y coordinate from GDI
* @return int value of the attack result
*/
int BattleGrid::attackByCoords(int x, int y) {
    int deltaX = x - _xPos;
    int deltaY = y - _yPos;
    if (deltaX < 0 || deltaY < 0) {
        _attackResult = ATTACK_NONE;
        return ATTACK_NONE;
    }
    _attackResult = attack(deltaY / _sampling ,deltaX / _sampling);
    if (_attackResult == ATTACK_SUCCESS) {
        if (checkNeighbours())
            dotWounded();
        else
            dotDead();
    }
    return _attackResult;
}

/**
* This method checks if there is at least one alive section on the grid.
* @return bool true if is alive.
*/
bool BattleGrid::isAlive()
{
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            if(_grid[i][j] == MESH_SHIP || _grid[i][j] == MESH_BUMB)
                return true;

            if(_grid[i][j] == MESH_BUMB_DEAD)
                return false;
        }
    }
    return false;
}
