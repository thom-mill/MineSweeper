//
// Created by Thomas Miller on 11/14/24.
//

#include "grid.h"
#include <random>
#include <iostream>
#include <fstream>

Grid::Square::Square() {

    sprite.setScale(20/32.0, 20/32.0);
}

bool Grid::Square::getBomb() {
    return isBomb;
}

void Grid::Square::setNumBombs(int num) {
    numBombs = num;
}

void Grid::Square::clickTile() {
    isClicked = true;
}

bool Grid::Square::getIsClicked() {
    return isClicked;
}
int Grid::Square::getNumBombs() {
    return numBombs;
}
bool Grid::Square::getFlag() {
    return isFlagged;
}
void Grid::Square::swapFlagBool() {
    if(isFlagged) {
        isFlagged = false;
    } else {
        isFlagged = true;
    }

}
void Grid::Square::set_coordinates(float x, float y) {
    x_pos = x;
    y_pos = y;
    sprite.setPosition(x_pos, y_pos);
}

void Grid::Square::set_texture(const sf::Texture& tex) {
    sprite.setTexture(tex);
}

void Grid::Square::change_texture(const sf::Texture& tex) {
    sprite.setTexture(tex);
    sprite.setScale(20/32.0, 20/32.0);
}

sf::Sprite& Grid::Square::getSprite() {
    return sprite;
}


Grid::Grid() {
    std::ifstream config("files/config.cfg");
    if(!config.is_open()) {
        std::cerr << "Error opening config file" << std::endl;
    }
    std::string colsStr;
    std::string rowsStr;
    std::string minesStr;
    getline(config, colsStr);
    getline(config, rowsStr);
    getline(config, minesStr);
    num_cols = std::stoi(colsStr);
    num_rows = std::stoi(rowsStr);
    num_mines = std::stoi(minesStr);
    tiles_remaining = num_rows * num_cols - num_mines;


    if(!default_texture.loadFromFile("files/images/tile_hidden.png") ||
        !mine_texture.loadFromFile("files/images/mine.png") ||
        !one.loadFromFile("files/images/number_1.png") ||
        !two.loadFromFile("files/images/number_2.png") ||
        !three.loadFromFile("files/images/number_3.png") ||
        !four.loadFromFile("files/images/number_4.png") ||
        !five.loadFromFile("files/images/number_5.png") ||
        !six.loadFromFile("files/images/number_6.png") ||
        !seven.loadFromFile("files/images/number_7.png") ||
        !eight.loadFromFile("files/images/number_8.png") ||
        !revealed_tile.loadFromFile("files/images/tile_revealed.png") ||
        !flag.loadFromFile("files/images/flag.png") ||
        !digits.loadFromFile("files/images/digits.png")){
        std::cerr << "failed to load texture" << std::endl;
        return;
    }
    textures = {
        {0, revealed_tile} , {1, one}, {2, two},
        {3, three}, {4, four}, {5, five}, {6, six},
        {7, seven}, {8, eight}
    };

}

void Grid::generateBoard() {
    board.resize(16);
    for(int i = 0; i < num_rows; i++) {
        std::vector<Square> row(num_cols);
        for(int j = 0; j < num_cols; j++) {
            row.emplace_back();
        }
        board[i] = std::move(row);
    }
    //automatically fill in the positions of all the tiles on the screen
    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_cols; j++) {
            if(board[i][j].getBomb())
                num_mines++;
        }
    }
    set_positions();
    randomize_bombs(num_mines);
    checkAdjacentBombs();
}

void Grid::randomize_bombs(int num_bombs) {
    int i = 0;
    while(i < num_bombs) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> _row(0,num_rows - 1);
        std::uniform_int_distribution<std::mt19937::result_type> _col(0,num_cols - 1);
        int row = _row(rng);
        int col = _col(rng);
        if(!board[row][col].getBomb()) {
            board[row][col].isBomb = true;
            i++;
        }
    }
    num_mines = num_bombs;


}

void Grid::set_positions() {
    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_cols; j++) {
            board[i][j].set_coordinates(j * 20.0f, i * 20.0f);

        }
    }
}

void Grid::set_default_texture() {
    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_cols; j++) {
            board[i][j].set_texture(default_texture);
        }
    }
}

//debugging purposes
void Grid::displayBombs() {
    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_cols; j++) {
            // if(board[i][j].getBomb()) {
            //     std::cout << "Bomb found" << std::endl;
            // }
            // else {
            //     std::cout << "No bomd" << std::endl;
            // }
            std::cout << board[i][j].getNumBombs() << std::endl;
        }
    }
}

//DEBUG BUTTON SHOWS ALL BOMBS ON SCREEN
void Grid::debugButton() {
    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_cols; j++) {
            if(board[i][j].getBomb()) {
                if(!isShowingMines)
                    board[i][j].change_texture(mine_texture);
                else if(board[i][j].getFlag())
                    board[i][j].change_texture(flag);
                else
                    board[i][j].change_texture(default_texture);

            }
        }
    }
    if(!isShowingMines)
        isShowingMines = true;
    else
        isShowingMines = false;
}

void Grid::checkAdjacentBombs() {
    std::vector<std::pair<int,int>> directions = {
        {0, -1}, {0, 1}, {1, 0}, {-1, 0},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_cols; j++) {
            int counter = 0;
            if(!board[i][j].getBomb()) { // if tile is a bomb we don't care about numBombs
                for(const auto& pair : directions) {
                    int dx = pair.first;
                    int dy = pair.second;
                    if(i + dx >= 0 && i + dx < num_rows && j + dy >= 0 && j + dy < num_cols) {
                        if(board[i+dx][j+dy].getBomb()) {
                            counter++;
                        }
                    }
                }
                board[i][j].setNumBombs(counter);
            }
        }
    }
}

bool Grid::revealTile(int mouseX, int mouseY) {
    int tile_row = mouseY / 20;
    int tile_col = mouseX / 20;
    int adjacent_bombs = board[tile_row][tile_col].getNumBombs();
    std::string filename;
    //do something better
    if(board[tile_row][tile_col].getBomb() && !board[tile_row][tile_col].getFlag()) { //loss scenario build in future
        loss = true;
        board[tile_row][tile_col].change_texture(mine_texture);
        return false;
    }
    if(board[tile_row][tile_col].getFlag()) {
        return true;
    }
    if(adjacent_bombs == 0) {
        //do something to reveal all adjacent tiles
        reveal_surrounding(tile_row, tile_col);
    }
    if(adjacent_bombs > 0) {
        board[tile_row][tile_col].change_texture(textures[adjacent_bombs]);
        board[tile_row][tile_col].clickTile();
        tiles_remaining--;
    }
    return true;
}

void Grid::reveal_surrounding(int row, int col) {
    if(row < 0 || col < 0 || row >= num_rows || col >= num_cols || board[row][col].getIsClicked()) {
        return;
    }
    tiles_remaining--;
    board[row][col].change_texture(textures[board[row][col].getNumBombs()]);
    board[row][col].clickTile();
    if(board[row][col].getNumBombs() > 0) {
        return;
    }

    for(int dx = -1; dx <= 1; dx++) {
        for(int dy = -1; dy <= 1; dy++) {
            if(dx != 0 || dy != 0) {
                reveal_surrounding(row + dx, col + dy);
            }
        }
    }
}

void Grid::place_flag(int mouseX, int mouseY) {
    int row = mouseY / 20;
    int col = mouseX / 20;
    if(!board[row][col].getIsClicked()) {
        if(!board[row][col].getFlag()) {
            board[row][col].change_texture(flag);
            board[row][col].swapFlagBool();
            num_flags++;
        }
        else {
            board[row][col].change_texture(default_texture);
            board[row][col].swapFlagBool();
            num_flags--;
        }
    }
}

Grid::~Grid() {
    board.clear();
}