#pragma once
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Vector2.hpp"

enum Tiles
{
	EMPTY = 0,					// ' '
	THICK_WALL_H_UPPER = 1,		// '='
	THICK_WALL_H_LOWER = 2,		// '#'
	THICK_WALL_V_LEFT = 3,		// 'I'
	THICK_WALL_V_RIGHT = 4,		// 'H'
	THICK_CORNER_DR = 5,		// '<'
	THICK_CORNER_DL = 7,		// '>'
	THICK_CORNER_UR = 6,		// '{'
	THICK_CORNER_UL = 8,		// '}'
	THIN_WALL_H_UPPER = 19,		// '-'
	THIN_WALL_H_LOWER = 20,		// '_'
	THIN_WALL_V_LEFT = 17,		// '!'
	THIN_WALL_V_RIGHT = 18,		// 'i'
	THIN_CORNER_DR = 21,		// 'f'
	THIN_CORNER_DL = 23,		// '('
	THIN_CORNER_UR = 22,		// 'z'
	THIN_CORNER_UL = 24,		// ')'
	SPLIT_H_DL = 9,				// 'q'
	SPLIT_H_DR = 10,			// 'p'
	SPLIT_H_UL = 11,			// 'd'
	SPLIT_H_UR = 12,			// 'b'
	SPLIT_V_DL = 15,			// 'k'
	SPLIT_V_DR = 16,			// 'x'
	SPLIT_V_UL = 13,			// 'y'
	SPLIT_V_UR = 14,			// 't'
	THICK_WALL_DR = 25,			// '$'
	THICK_WALL_DL = 27,			// '€'
	THICK_WALL_UR = 26,			// '%'
	THICK_WALL_UL = 28,			// '^'
	THICK_WALL_END_L = 30,		// '['
	THICK_WALL_END_R = 29,		// ']'
	HOUSE_DOOR = 31				// '@'
};

class Maze
{
public:
	static Maze* instance;
	static Maze* GetInstance();

	int GetTileAtPosition(Vector2 position);
	Vector2 ScreenToMazePosition(Vector2 position);
	Vector2 MazeToScreenPosition(Vector2 position);
	void LoadMaze(int level);

	void Draw(sf::RenderWindow* window);

private:
	Maze();
	~Maze() {};

	std::unordered_map<Vector2, int> maze; // <position, tile>
	std::vector<std::tuple<sf::Sprite*, sf::Texture*>*> tiles;

	void MazeFromString(std::string level);
	void LoadMaze1();
};

