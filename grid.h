//
// Created by Thomas Miller on 11/14/24.
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>
#ifndef GRID_H
#define GRID_H

#endif //GRID_H

class Grid{
    class Square{
        bool isClicked = false;
        bool isFlagged = false;
        int numBombs;
        float x_pos;
        float y_pos;
        sf::Sprite sprite;
    public:
        bool isBomb = false;
        Square();
        bool getBomb();
        void setNumBombs(int num);
        int getNumBombs();
        void swapFlagBool();
        bool getFlag();
        void clickTile();
        bool getIsClicked();
        void set_coordinates(float x, float y);
        void set_texture(const sf::Texture& tex);
        void change_texture(const sf::Texture& tex);
        sf::Sprite& getSprite();

    };
    //16 rows 30 columns
public:
    int tiles_remaining;
    bool loss = false;
    bool win = false;
    bool isPaused = false;
    bool isShowingMines = false;
    int num_rows;
    int num_cols;
    int num_mines;
    int num_flags = 0;
    Grid();
    ~Grid();
    std::vector<std::vector<Square>> board;
    void randomize_bombs(int num_bombs);
    void generateBoard();
    void set_positions();
    void set_default_texture();
    void displayBombs();
    void debugButton();
    void checkAdjacentBombs();
    bool revealTile(int mouseX, int mouseY);
    void reveal_surrounding(int row, int col);
    void place_flag(int mouseX, int mouseY);

//fix this atrocity LMAO
    std::unordered_map<int, sf::Texture> textures;
    sf::Texture revealed_tile;
    sf::Texture default_texture;
    sf::Texture mine_texture;
    sf::Texture one;
    sf::Texture two;
    sf::Texture three;
    sf::Texture four;
    sf::Texture five;
    sf::Texture six;
    sf::Texture seven;
    sf::Texture eight;
    sf::Texture flag;
    sf::Texture digits;

};