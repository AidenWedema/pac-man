#include "Maze.h"
#include<stdio.h>

Maze::Maze()
{
	resolution = 8;
}

Maze* Maze::instance = nullptr;
Maze* Maze::GetInstance()
{
	if (instance == nullptr)
		instance = new Maze();
	return instance;
}

int Maze::GetTileAtPosition(Vector2 position)
{
	// Ensure the position is within bounds
	if (maze.find(position) != maze.end())
		return maze[position];

	// Return a wall or impassable tile type if out-of-bounds
	std::cout << position.toString() << " is out of bounds" << std::endl;
	return -1;
}

void Maze::LoadMaze(int level)
{
	// clear the maze
	tiles.clear();
	maze.clear();
	for (int y = 0; y < 28; y++)
		for (int x = 0; x < 36; x++)
			maze[Vector2(x, y)] = Tiles::EMPTY;

	switch (level)
	{
		case 1:
			LoadMaze1();
			break;

		default:
			LoadMaze1();
			break;
	}
}

void Maze::Draw(sf::RenderWindow* window)
{
	for (std::tuple<sf::Sprite*, sf::Texture*>* tile : tiles)
		window->draw(*std::get<0>(*tile));
}

void Maze::MazeFromString(std::string level)
{
	char t[] = {' ', '=', '#', 'I', 'H', '<', '{', '>', '}', 'q', 'p', 'd', 'b', 'y','t',
		'k', 'x', '!', 'i', '-', '_', 'f', '(', 'z', ')', '$', '%', '€', '^', ']', '[', '@' };

	for (int i = 0; i < level.size(); i++)
	{
		char c = level[i]; // get the current character
		int x = i % 28; // get the x position
		int y = i / 28; // get the y position
		// get the index of c in tiles
		int index = std::find(t, t + 32, c) - t;
		maze[Vector2(x, y)] = index;

		//load the tile from "assets/sprites/maze tiles/"
		sf::Sprite* sprite = new sf::Sprite();
		sf::Texture* texture = new sf::Texture();
		if (!texture->loadFromFile("assets/sprites/maze tiles/tile_" + std::to_string(index) + ".png"))
		{
			std::cout << "Failed to load tile_" << index << ".png" << std::endl;
			return;
		}

		sprite->setTexture(*texture);
		sprite->setPosition(x * resolution, y * resolution);
		sprite->setScale(resolution / texture->getSize().x, resolution / texture->getSize().y);
		std::tuple<sf::Sprite*, sf::Texture*>* tile = new std::tuple<sf::Sprite*, sf::Texture*>(sprite, texture);
		tiles.push_back(tile);
	}
}

void Maze::LoadMaze1()
{
	std::string level =
		"                            "
		"                            "
		"                            "
		"<============qp============>"
		"I            !i            H"
		"I f--z f---z !i f---z f--z H"
		"I !  i !   i !i !   i !  i H"
		"I (__) (___) () (___) (__) H"
		"I                          H"
		"I f--z fz f------z fz f--z H"
		"I (__) !i (__zf__) !i (__) H"
		"I      !i    !i    !i      H"
		"{####z !(--z !i f--)i f####}"
		"     I !f__) () (__zi H     "
		"     I !i          !i H     "
		"     I !i $#]@@[#€ !i H     "
		"=====) () H      I () (====="
		"          H      I          "
		"#####z fz H      I fz f#####"
		"     I !i %======^ !i H     "
		"     I !i          !i H     "
		"     I !i f------z !i H     "
		"<====) () (__zf__) () (====>"
		"I            !i            H"
		"I f--z f---z !i f---z f--z H"
		"I (_zi (___) () (___) !f_) H"
		"I   !i                !i   H"
		"t-z !i fz f------z fz !i f-y"
		"x_) () !i (__zf__) !i () (_k"
		"I      !i    !i    !i      H"
		"I f----)(--z !i f--)(----z H"
		"I (________) () (________) H"
		"I                          H"
		"{##########################}"
		"                            "
		"                            ";

	MazeFromString(level);
}

/*void Maze::LoadMaze1()
{
	// first 3 rows of the maze are empty
	// 4th row
	for (int x = 0; x < 28; x++)
	{
		switch (x)
		{
			case 0:
				maze[Vector2(x, 3)] = Tiles::THICK_CORNER_DR; break;
			case 14:
				maze[Vector2(x, 3)] = Tiles::SPLIT_H_DL; break;
			case 15:
				maze[Vector2(x, 3)] = Tiles::SPLIT_H_DR; break;
			case 27:
				maze[Vector2(x, 3)] = Tiles::THICK_CORNER_DL; break;
			default:
				maze[Vector2(x, 3)] = Tiles::THICK_WALL_H_UPPER; break;
		}
	}

	// outer walls from row 5 to 12
	for (int y = 0; y < 8; y++)
	{
		maze[Vector2(0, y)] = Tiles::THICK_WALL_V_LEFT;
		maze[Vector2(27, y)] = Tiles::THICK_WALL_V_RIGHT;
	}
}
*/