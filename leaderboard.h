//
// Created by Thomas Miller on 11/28/24.
//

#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#endif //LEADERBOARD_H

#include <fstream>
#include <vector>
#include <string>

class Leaderboard {
public:
    std::string filePath = "files/leaderboard.txt";
    std::ifstream textfile;
    std::ofstream outputfile;
    std::vector<std::pair<std::string, std::string>> leaderboard; //key: name of player, value: time min:sec
    std::vector<std::pair<std::string, int>> seconds_map;
    Leaderboard(std::string file_name);
    void sortedLeaderboard();
    bool add_to_leaderboard(std::string name, int seconds);
    void writeToFile(std::string file_name);
    void printLeaderboard();
    int convert_to_seconds(std::string& time);
    std::string convert_to_time(int seconds);
};