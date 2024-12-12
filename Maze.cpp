#include "Maze.h"

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

void Maze::LoadMaze(int level)
{
	// clear the maze
	for (std::tuple<sf::Sprite*, sf::Texture*>* tile : tiles)
	{
		delete std::get<0>(*tile);
		delete std::get<1>(*tile);
		delete tile;
	}
	tiles.clear();
	maze.clear();

	switch (level)
	{
		case 1:
			LoadMaze1();
			break;

		default:
			LoadMaze1();
			break;
	}
    ConnectNodes();
}

void Maze::Draw(sf::RenderWindow* window)
{
	for (std::tuple<sf::Sprite*, sf::Texture*>* tile : tiles)
		window->draw(*std::get<0>(*tile));

#ifdef _DEBUG // draw the connection from each node
    const sf::Color color = sf::Color(255, 255, 255, 128);
    for (Node* node : maze)
    {
        // draw the node
        sf::RectangleShape rect;
        rect.setPosition(node->position.x * resolution + resolution / 4, node->position.y * resolution + resolution / 4);
        rect.setSize(sf::Vector2f(resolution * 0.5f, resolution * 0.5f));
        rect.setFillColor(color);
		window->draw(rect);

        if (node->warp) continue;

        // draw the path to the connections
        for (auto& connection : node->connections)
        {
            Directions direction = connection.first;
            Node* neighbor = connection.second;
            Vector2 pos = node->position;
            Vector2 neighborPos = neighbor->position;

			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(pos.x * resolution + resolution / 2, pos.y * resolution + resolution / 2), color),
				sf::Vertex(sf::Vector2f(neighborPos.x * resolution + resolution / 2, neighborPos.y * resolution + resolution / 2), color)
			};
			window->draw(line, 2, sf::Lines);
            neighbor = nullptr;
            delete neighbor;
        }
    }
#endif // _DEBUG
}

void Maze::ConnectNodes()
{
    const int rows = size.y;
    const int cols = size.x;

    // Create a 2D array (vector of vectors) to represent the maze grid
    std::vector<std::vector<Node*>> grid(rows, std::vector<Node*>(cols));

    // Populate the grid with nodes
    for (Node* node : maze)
    {
        Vector2 pos = node->position;
        grid[pos.y][pos.x] = node;  // Place node in its corresponding position
    }

    // Iterate over each node in the grid to check its neighbors
    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            Node* currentNode = grid[y][x];

            if (currentNode == nullptr) continue;

            // Connect to the node to the right (if it exists)
            if (x + 1 < cols && grid[y][x + 1] != nullptr)
                currentNode->AddConnection(Directions::RIGHT, grid[y][x + 1]);
            else if (x + 1 == cols) // Wrap around to the left edge
                CreateWarp(currentNode, grid[y][0], Directions::RIGHT);

            // Connect to the node below (if it exists)
            if (y + 1 < rows && grid[y + 1][x] != nullptr)
                currentNode->AddConnection(Directions::DOWN, grid[y + 1][x]);
            else if (y + 1 == rows) // Wrap around to the top edge
                CreateWarp(currentNode, grid[0][x], Directions::DOWN);

            // Connect to the node to the left (if it exists)
            if (x - 1 >= 0 && grid[y][x - 1] != nullptr)
                currentNode->AddConnection(Directions::LEFT, grid[y][x - 1]);
            else if (x - 1 < 0) // Wrap around to the right edge
                CreateWarp(currentNode, grid[y][cols - 1], Directions::LEFT);

            // Connect to the node above (if it exists)
            if (y - 1 >= 0 && grid[y - 1][x] != nullptr)
                currentNode->AddConnection(Directions::UP, grid[y - 1][x]);
            else if (y - 1 < 0) // Wrap around to the bottom edge
                CreateWarp(currentNode, grid[rows - 1][x], Directions::UP);
            currentNode = nullptr;
            delete currentNode;
        }
    }
}

void Maze::CreateWarp(Node* from, Node* to, Directions direction)
{
    Directions opposite = (Directions)((direction + 2) % 4);
    Node* warp1 = new Node(from->position + direction);
    Node* warp2 = new Node(to->position + opposite);

    maze.push_back(warp1);
	maze.push_back(warp2);

    warp1->warp = true;
    warp2->warp = true;

	from->AddConnection(direction, warp1);
	to->AddConnection(opposite, warp2);
	warp1->AddConnection(opposite, from);
	warp2->AddConnection(direction, to);
    warp1->AddConnection(direction, warp2);
	warp2->AddConnection(opposite, warp1);
}

void Maze::MazeFromString(std::string level)
{
    pelletCounter = 0;

    char t[] = { ' ', '=', '#', 'I', 'H', '<', '{', '>', '}', 'q', 'p', 'd', 'b', 'y','t',
        'k', 'x', '!', 'i', '-', '_', 'f', '(', 'z', ')', '$', '%', '€', '^', ']', '[', '@'};

    char s[] = { '.', 'o', 'G', 'S'};

    for (int i = 0; i < level.size(); i++)
    {
        char c = level[i]; // get the current character
        int x = i % size.x; // get the x position
        int y = i / size.x; // get the y position
        // get the index of c in tiles
        int index = 0;
        if (std::find(t, t + 32, c) != std::end(t))
            index = std::find(t, t + 32, c) - t;
        else
            index = std::find(s, s + 4, c) - s + 100; // if c is not in tiles, get the index of c in symbols

        bool isPellet = false;
        bool isSuper = false;
        bool isSpawn = false;
        bool isHouse = false;
        if (index >= 100)
        {
            index -= 100;
            switch (index)
            {
            case 1:
                isSuper = true;
            case 0:
                isPellet = true;
                break;
            case 2:
                isHouse = true;
                break;
            case 3:
                isSpawn = true;
                break;
            default:
                break;
            }
            index = 0;
        }
        // create a node if the tile is empty otherwise load the tile
        if (index == 0)
        {
            Node* newnode = new Node(Vector2(x, y), isPellet, isSuper);
            if (isSpawn)
                spawn = newnode;
            if (isHouse)
				house = newnode;
            maze.push_back(newnode);
            // continue if the node is empty, since nothing has to be drawn at it's position
            if (!isPellet && !isSuper)
			    continue;
            index = 99 + isPellet + isSuper; // set the index to load the correct sprite. (100 for pellet, 101 for powerpellet)
            pelletCounter += isPellet;
        }
        // load the tile from "assets/sprites/maze tiles/"
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
        "I............i!............H"
        "I.f__z.f___z.i!.f___z.f__z.H"
        "Ioi  !.i   !.i!.i   !.i  !oH"
        "I.(--).(---).().(---).(--).H"
        "I..........................H"
        "I.f__z.fz.f______z.fz.f__z.H"
        "I.(--).i!.(--zf--).i!.(--).H"
        "I......i!....i!....i!......H"
        "{####z.i(__z i! f__)!.f####}"
        "     I.if--) () (--z!.H     "
        "     I.i!    G     i!.H     "
        "     I.i! $#]@@[#€ i!.H     "
        "=====).() H      I ().(====="
        "      .   H      I   .      "
        "#####z.fz H      I fz.f#####"
        "     I.i! %======^ i!.H     "
        "     I.i!          i!.H     "
        "     I.i! f______z i!.H     "
        "<====).() (--zf--) ().(====>"
        "I............i!............H"
        "I.f__z.f___z.i!.f___z.f__z.H"
        "I.(-z!.(---).().(---).if-).H"
        "Io..i!.......S .......i!..oH"
        "t_z.i!.fz.f______z.fz.i!.f_y"
        "x-).().i!.(--zf--).i!.().(-k"
        "I......i!....i!....i!......H"
        "I.f____)(__z.i!.f__)(____z.H"
        "I.(--------).().(--------).H"
        "I..........................H"
        "{##########################}"
        "                            "
        "                            ";

    try
    {
        MazeFromString(level);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return;
    }
}