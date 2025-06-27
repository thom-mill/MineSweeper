//
// Created by Thomas Miller on 11/28/24.
//
#include "leaderboard.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

Leaderboard::Leaderboard(std::string file_name) {
    textfile.open(file_name);
    if (!textfile.is_open()) {
        std::cerr << "Error opening text file" << std::endl;
        return;
    }
    std::string row;
    while(std::getline(textfile, row)) {
        std::string name;
        std::string time;
        std::istringstream rowStream(row);
        std::getline(rowStream, time, ',');
        std::getline(rowStream, name, ',');

        leaderboard.push_back(std::make_pair(name,time));
        seconds_map.push_back(std::make_pair(name, convert_to_seconds(time)));
    }
    textfile.close();
}

bool Leaderboard::add_to_leaderboard(std::string name, int seconds) {
    std::string name_replace;
    int max = seconds;
    for(int i = 0; i < leaderboard.size(); i++) {
        int it_time = convert_to_seconds(leaderboard[i].second);
        if(max < it_time) {
            name_replace = leaderboard[i].first;
            max = it_time;
        }
    }
    if(seconds < max) {
        for(auto it = leaderboard.begin(); it != leaderboard.end(); it++) {
            if(it->first == name_replace) {
                it == leaderboard.erase(it);
                leaderboard.push_back(std::make_pair(name, convert_to_time(seconds)));
            }
        }
        for(auto it = seconds_map.begin(); it != seconds_map.end(); it++) {
            if(it->first == name_replace) {
                it = seconds_map.erase(it);
                seconds_map.push_back(std::make_pair(name, seconds));
            }
        }
    }
    sortedLeaderboard();
}

int Leaderboard::convert_to_seconds(std::string& time) {
    std::string min = time.substr(0, time.find(':'));
    std::string sec = time.substr(time.find(':') + 1);
    return std::stoi(min) * 60 + std::stoi(sec);
}

std::string Leaderboard::convert_to_time(int seconds) {
    std::string time = "";
    int min = seconds / 60;
    seconds = seconds % 60;
    if(min < 10)
        time += "0";
    time += std::to_string(min);
    time += ':';
    if(seconds < 10)
        time += "0";
    time += std::to_string(seconds);
    return time;
}

void Leaderboard::writeToFile(std::string file_name) {
    outputfile.open(file_name);
    if (!outputfile.is_open()) {
        std::cerr << "Error writing to file" << std::endl;
        return;
    }
    for(auto it = leaderboard.begin(); it != leaderboard.end(); it++) {
        outputfile << it->second << ',' << it->first << std::endl;
    }

}

void Leaderboard::printLeaderboard() {
    for(auto it = leaderboard.begin(); it != leaderboard.end(); it++) {
        std::cout << it->first << " time: " << it->second << std::endl;
    }
    // std::cout << std::endl;
    // for(auto it = seconds_map.begin(); it != seconds_map.end(); it++) {
    //     std::cout << it->first << ':' << it->second << std::endl;
    // }
}

void Leaderboard::sortedLeaderboard() {
    std::vector<std::pair<std::string, int>> result(seconds_map.begin(), seconds_map.end());
    std::sort(result.begin(), result.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string,int>& b){
        return a.second < b.second;
    });
    std::vector<std::pair<std::string, std::string>> result2;
    for(auto it =result.begin(); it != result.end(); it++) {
        result2.push_back(std::make_pair(it->first, convert_to_time(it->second)));
    }
    leaderboard = result2;
    writeToFile(filePath);
    printLeaderboard();
}