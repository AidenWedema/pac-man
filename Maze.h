#pragma once
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Vector2.hpp"
/*#include "Pellet.h"*/

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

	struct Node
	{
		Node(Vector2 position) : position(position) {}
		Node(Vector2 position, bool pellet, bool super) : position(position), pellet(pellet), super(super) {}

		Vector2 position;
		std::unordered_map<Directions, Node*> connections;
		bool warp = false;
		bool pellet = false;
		bool super = false;

		void AddConnection(Directions direction, Node* node) { connections[direction] = node; }
	};

	void LoadMaze(int level);

	std::vector<Node*> GetMaze() { return maze; }
	Node* GetNode(Vector2 position) { for (Node* n : maze) if (n->position == position) return n; return nullptr; }

	void Draw(sf::RenderWindow* window);

	uint16_t GetResolution() { return resolution; }
	void SetResolution(uint16_t resolution) { this->resolution = resolution; }

	int GetPalletCounter();

	int GetPelletCounter() { return pelletCounter; }	// Get the pellet counter.

	void UpdateTile(Vector2 position, int newTileIndex);

private:
	Maze();
	~Maze() {};

	uint16_t resolution = 8;		// resolution of the tiles in pixels
	Vector2 size = Vector2(28, 36);	// Size of the maze. max 127 x 127, original maze is 28 x 36

	std::vector<Node*> maze;
	std::vector<std::tuple<sf::Sprite*, sf::Texture*>*> tiles;
	Node* spawn;
	Node* house;
	int pelletCounter;

	int palletCounter;

	void ConnectNodes();
	void CreateWarp(Node*, Node*, Directions);
	void MazeFromString(std::string);
	void LoadMaze1();
};

