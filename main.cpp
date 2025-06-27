#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "grid.h"
#include "leaderboard.h"

bool isLetter(sf::Event& event) {
    sf::Keyboard::Key key = event.key.code;
    if(key >= sf::Keyboard::A && key <= sf::Keyboard::Z) {
        return true;
    }
    return false;
}

void print_title_screen(sf::RenderWindow &window, std::vector<char>& name) {
    sf::Font font;
    if(!font.loadFromFile("files/font.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        return;
    }
    //"Welcome to Minesweeper!"
    const sf::Color title_screen_color = sf::Color::Blue;
    sf::Text welcome_message;
    welcome_message.setFont(font);
    welcome_message.setCharacterSize(35);
    welcome_message.setFillColor(sf::Color::White);
    welcome_message.setString("Welcome to Minesweeper!");
    welcome_message.setPosition(sf::Vector2f(55, 50));
    welcome_message.setStyle(sf::Text::Bold | sf::Text::Underlined);

    //"Enter your name:
    sf::Text enter_name;
    enter_name.setFont(font);
    enter_name.setCharacterSize(20);
    enter_name.setFillColor(sf::Color::White);
    enter_name.setString("Enter your name:");
    enter_name.setPosition(sf::Vector2f(200, 150));
    window.clear(title_screen_color);

    //Setting the name
    if(!name.empty()) {
        std::string playerName;
        for(int i = 0; i < name.size(); i++) {
            if(i == 0) {
                playerName += name[i];
            }
            else {
                playerName += tolower(name[i]);
            }
        }
        sf::Text displayed_name;
        displayed_name.setFillColor(sf::Color::White);
        displayed_name.setFont(font);
        displayed_name.setCharacterSize(20);
        displayed_name.setPosition(sf::Vector2f(300 - name.size() * 7, 180));
        displayed_name.setString(playerName);
        displayed_name.setStyle(sf::Text::Bold);
        window.draw(displayed_name);
    }

    // adding the cursor
    sf::RectangleShape cursor(sf::Vector2f(6, 22));
    cursor.setFillColor(sf::Color::White);
    cursor.setPosition(sf::Vector2f(300 + name.size() * 6, 183));

    //print to screen
    window.draw(welcome_message);
    window.draw(enter_name);
    window.draw(cursor);
}

void print_timer(sf::Clock clock, sf::Time pausedTime, sf::RenderWindow& window, Grid& grid) {
    int offsetX = 20;
    const int digit_width = 21;
    const int digit_height = 32;
    int total_time;
    if(grid.isPaused || grid.loss || grid.win)
        total_time = static_cast<int>(pausedTime.asSeconds());
    else
        total_time = clock.getElapsedTime().asSeconds() + pausedTime.asSeconds();
    int seconds = total_time % 60;
    int minutes = total_time / 60;
    std::string timeString = (minutes < 10 ? "0" : "") + std::to_string(minutes) + (seconds < 10 ? "0" : "") + std::to_string(seconds);
    sf::Texture digits;
    if(!digits.loadFromFile("files/images/digits.png")) {
        std::cerr<<"Failed to load texture" << std::endl;
        return;
    }
    for(char c : timeString) {
        int digitIndex = c - '0';
        sf::Sprite digit(digits, sf::IntRect(digitIndex * digit_width, 0, digit_width, digit_height));
        digit.setPosition(offsetX, 340);
        offsetX += digit_width;
        if(offsetX == 20 + 2*digit_width)
            offsetX += 5;
        window.draw(digit);
    }
    int counterOffset = 150;
    int number = grid.num_mines - grid.num_flags;
    std::string flagString = (number < 10 ? "0" : "") + std::to_string(number);
    for(char c : flagString) {
        int digitIndex = c - '0';
        sf::Sprite counter(digits, sf::IntRect(digitIndex * digit_width, 0, digit_width, digit_height));
        counter.setPosition(counterOffset, 340);
        counterOffset += digit_width;
        window.draw(counter);
    }
}


void print_game_screen(sf::RenderWindow& window, Grid& board, sf::Clock clock, sf::Time pausedTime) {
    window.clear(sf::Color::White);
    //draw all the tiles
    for(int i = 0; i < board.board.size(); i++) {
        for(int j = 0; j < board.board[0].size(); j++) {
            window.draw(board.board[i][j].getSprite());
        }
    }
    //sprites at the bottom
    sf::Texture digits;
    sf::Texture smily_tex;
    sf::Texture victory;
    sf::Texture loss;
    sf::Texture leaderboard_tex;
    sf::Texture debug_tex;
    sf::Texture pause_tex;
    sf::Texture play_tex;
    if(!smily_tex.loadFromFile("files/images/face_happy.png") ||
        !leaderboard_tex.loadFromFile("files/images/leaderboard.png") ||
        !debug_tex.loadFromFile("files/images/debug.png") ||
        !pause_tex.loadFromFile("files/images/pause.png") ||
        !play_tex.loadFromFile("files/images/play.png") ||
        !victory.loadFromFile("files/images/face_win.png") ||
        !loss.loadFromFile("files/images/face_lose.png"))
    {
        std::cerr << "Failed to load texture files" << std::endl;
        return;
    }
    sf::Sprite smily_sprite;
    sf::Sprite leaderboard_sprite;
    sf::Sprite debug_sprite;
    sf::Sprite pause_sprite;
    if(board.tiles_remaining < 1)
        smily_sprite.setTexture(victory);
    else if(board.loss)
        smily_sprite.setTexture(loss);
    else
        smily_sprite.setTexture(smily_tex);
    smily_sprite.setPosition(268, 328);
    leaderboard_sprite.setTexture(leaderboard_tex);
    leaderboard_sprite.setPosition(528, 328);
    debug_sprite.setTexture(debug_tex);
    debug_sprite.setPosition(400, 328);
    if(board.isPaused)
        pause_sprite.setTexture(play_tex);
    else
        pause_sprite.setTexture(pause_tex);
    pause_sprite.setPosition(464, 328);
    //draw the sprites
    print_timer(clock, pausedTime, window, board);
    window.draw(smily_sprite);
    window.draw(leaderboard_sprite);
    window.draw(debug_sprite);
    window.draw(pause_sprite);
}

int pauseGame(sf::Clock& clock, Grid& board) {
    if(board.isPaused) {
        for(int i = 0; i < board.num_rows; i++) {
            for(int j = 0; j < board.num_cols; j++) {
                board.board[i][j].change_texture(board.revealed_tile);
            }
        }
    } else {
        for(int i = 0; i < board.num_rows; i++) {
            for(int j = 0; j < board.num_cols; j++) {
                if(board.board[i][j].getIsClicked())
                    board.board[i][j].change_texture(board.textures[board.board[i][j].getNumBombs()]);
                else if(board.board[i][j].getFlag())
                    board.board[i][j].change_texture(board.flag);
                else
                    board.board[i][j].change_texture(board.default_texture);

            }
        }
    }
}

void print_leaderboard(sf::RenderWindow& leader_window, Leaderboard& leaderboard) {
    sf::Font font;
    if(!font.loadFromFile("files/font.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return;
    }
    sf::Text title_text;
    title_text.setFont(font);
    title_text.setCharacterSize(20);
    title_text.setFillColor(sf::Color::White);
    title_text.setString("Leaderboard");
    title_text.setPosition(130, 20);

    std::vector<sf::Text> vec;
    int i = 1;
    for(int j = 0; j < leaderboard.leaderboard.size(); j++) {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::White);
        std::string row = "";
        row += std::to_string(i);
        row += ".  " + leaderboard.leaderboard[j].second + "   " + leaderboard.leaderboard[j].first;
        text.setString(row);
        text.setPosition(80, i * 30 + 40);
        i++;
        vec.push_back(text);
    }
    leader_window.clear(sf::Color::Blue);
    for(int i = 0; i < vec.size(); i++) {
        leader_window.draw(vec[i]);
    }
    leader_window.draw(title_text);
    leader_window.display();
}

int main() {
    const int WINDOW_WIDTH = 600;
    const int WINDOW_HEIGHT = 400;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Works!");

    const int WIDTH = 400;
    const int HEIGHT = 250;
    sf::RenderWindow leader_window;
    bool leaderOpen = false;

    std::vector<char> name;
    std::string nameStr = "";
    bool keyDown = false;
    bool mouseLeftDown = false;
    int screen_type = 0;
    bool gameLost = false;
    bool gamePaused = false;
    int gameTime;

    Grid game_board;
    game_board.generateBoard();
    game_board.set_default_texture();

    Leaderboard top5("files/leaderboard.txt");

    sf::Clock clock;
    sf::Time pausedTime = sf::Time::Zero;

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
                if(leaderOpen) {
                    leader_window.close();
                    leaderOpen = false;

                }
            }
            //Title screen keyboard input for the name
            if(event.type == sf::Event::KeyPressed && screen_type == 0) {
                if(!keyDown) { //make sure key isn't already being held
                    if(isLetter(event) && name.size() < 10) {
                        char letter = 'A' + (event.key.code - sf::Keyboard::A);
                        name.push_back(letter);
                        nameStr += letter;
                        keyDown = true;
                    }
                    else if(event.key.code == sf::Keyboard::BackSpace) {
                        name.pop_back();
                        nameStr.pop_back();
                        keyDown = true;
                    }
                    else if(event.key.code == sf::Keyboard::Enter && !name.empty()) {
                        screen_type = 1;
                        clock.restart();
                    }

                }
            }
            if(event.type == sf::Event::KeyReleased)
                keyDown = false;
            //Game screen mouse input goes here
            if(event.type == sf::Event::MouseButtonPressed && screen_type > 0 && !leaderOpen) {
                if(event.mouseButton.button == sf::Mouse::Left) {
                    //check for position here
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;
                    //tile hits
                    if (mouseY < 320 && !gamePaused) {
                        if(!game_board.revealTile(mouseX, mouseY)) {
                            game_board.debugButton();
                        }

                    }
                    //Debug button
                    if(mouseX > 400 && mouseX < 400 + 64 && mouseY > 328 && mouseY < 328+64 && !mouseLeftDown) {
                        std::cout << "Debug button hit" << std::endl;
                        game_board.debugButton();
                        mouseLeftDown = true;
                    }
                    //leaderboard button
                    if(mouseX > 528 && mouseX < 528+64 && mouseY > 328 && mouseY < 328+64 && !mouseLeftDown) {
                        mouseLeftDown = true;
                        if(!leaderOpen) {
                            leader_window.create(sf::VideoMode(WIDTH, HEIGHT), "Leaderboard");
                            leaderOpen = true;
                        }
                    }
                    //pause button
                    if(mouseX > 464 && mouseX < 464+64 && mouseY > 328 && mouseY < 328+64 && !mouseLeftDown) {
                        if(!gamePaused) {
                            gamePaused = true;
                            game_board.isPaused = true;
                            pausedTime += clock.getElapsedTime();
                        }
                        else {
                            clock.restart();
                            gamePaused = false;
                            game_board.isPaused = false;
                        }
                        pauseGame(clock, game_board);
                    }
                    //smiley button
                    if(mouseX > 268 && mouseX < 268+64 && mouseY > 328 && mouseY < 328+64 && !mouseLeftDown) {
                        game_board = Grid();
                        game_board.generateBoard();
                        game_board.set_default_texture();
                        clock.restart();
                        pausedTime = sf::Time::Zero;
                    }

                }
                else if(event.mouseButton.button == sf::Mouse::Right) {
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;
                    game_board.place_flag(mouseX, mouseY);
                }
            }
            //change bool to false on mouse release
            else if(event.type == sf::Event::MouseButtonReleased && screen_type > 0) {
                mouseLeftDown = false;
            }
        }
        if(game_board.tiles_remaining < 1 && !game_board.win) {
            gameTime = clock.getElapsedTime().asSeconds() + pausedTime.asSeconds();
            game_board.win = true;
            top5.add_to_leaderboard(nameStr,gameTime);
            leader_window.create(sf::VideoMode(WIDTH, HEIGHT), "Leaderboard");
            leaderOpen = true;

        }
        if(leaderOpen) {
            sf::Event leaderEvent;
            while (leader_window.pollEvent(leaderEvent)) {
                if (leaderEvent.type == sf::Event::Closed) {
                    leader_window.close();
                    leaderOpen = false;
                }
            }

        }

        if (screen_type == 0)
            print_title_screen(window, name);
        else if (screen_type == 1)
            print_game_screen(window,game_board, clock, pausedTime);
        if(leaderOpen) {
            print_leaderboard(leader_window, top5);
        }

        window.display();
    }
    return 0;
}
