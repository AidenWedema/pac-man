#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <array>
#include <algorithm>
#include <string>
#include <direct.h>
class Scoreboard
{
public:
	static Scoreboard* instance;
	static Scoreboard* GetInstance(){
		if (instance == nullptr)
			instance = new Scoreboard();
		return instance;
	}

	void ResetScore(){ score = 0; }

	// Return true if the score should be placed on the highscore list
	bool CheckHighscore() {
		// Check if the current score is higher than the highscores on the list
		for (const auto& entry : highscores)
			if (score > std::get<1>(entry))
				return true;
		return false;
	}
	// Load the highscore from file in the current working directory
	void LoadHighscores() {
		const size_t size = 1024;
		char buffer[size];
		if (!getcwd(buffer, size))
			return;
		std::string path = std::string(buffer) + "/scores.sav";
		std::ifstream file = std::ifstream(path, std::ios::binary);
		if (!file.is_open())
			return;

		for (auto& entry : highscores) {
			file.read(reinterpret_cast<char*>(&entry), sizeof(entry));
		}
		file.close();
	}
	// Save the highscore to file in the current working directory
	void SaveHighscores() {
		const size_t size = 1024;
		char buffer[size];
		if (!getcwd(buffer, size))
			return;
		std::string path = std::string(buffer) + "/scores.sav";
		std::ofstream file = std::ofstream(path, std::ios::binary);
		if (!file.is_open())
			return;

		for (auto& entry : highscores) {
			file.write(reinterpret_cast<char*>(&entry), sizeof(entry));
		}
		file.close();
	}
	// Sort the highscores
	void SortHighscores() {
		std::sort(highscores.begin(), highscores.end(), [](const auto& a, const auto& b) {
			return std::get<1>(a) > std::get<1>(b); // Compare scores
			});
	}

private:
	Scoreboard() {};
	~Scoreboard() {};

	int score = 0; // current score
	std::array<std::tuple<char[3], int>, 3> highscores; // Array of top 3 highscores
};
