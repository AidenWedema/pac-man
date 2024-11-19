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
        }
    }
#endif // _DEBUG // draw the connection from each node
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

            // Connect to the node below (if it exists)
            if (y + 1 < rows && grid[y + 1][x] != nullptr)
                currentNode->AddConnection(Directions::DOWN, grid[y + 1][x]);

            // Connect to the node to the left (if it exists)
			if (x - 1 >= 0 && grid[y][x - 1] != nullptr)
				currentNode->AddConnection(Directions::LEFT, grid[y][x - 1]);

			// Connect to the node above (if it exists)
            if (y - 1 >= 0 && grid[y - 1][x] != nullptr)
				currentNode->AddConnection(Directions::UP, grid[y - 1][x]);
        }
    }
}


void Maze::MazeFromString(std::string level)
{
    char t[] = { ' ', '=', '#', 'I', 'H', '<', '{', '>', '}', 'q', 'p', 'd', 'b', 'y','t',
        'k', 'x', '!', 'i', '-', '_', 'f', '(', 'z', ')', '$', '%', '€', '^', ']', '[', '@' };

    for (int i = 0; i < level.size(); i++)
    {
        char c = level[i]; // get the current character
        int x = i % size.x; // get the x position
        int y = i / size.x; // get the y position
        // get the index of c in tiles
        int index = std::find(t, t + 32, c) - t;

        // create a node if the tile is empty otherwise load the tile
        if (index == 0)
        {
            maze.push_back(new Node(Vector2(x, y)));
			continue;
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

/*
void Maze::LoadMaze1()
{
    Node n1 = Node(Vector2(1, 4));
    Node n2 = Node(Vector2(6, 4));
    Node n3 = Node(Vector2(12, 4));
    Node n4 = Node(Vector2(15, 4));
    Node n5 = Node(Vector2(21, 4));
    Node n6 = Node(Vector2(26, 4));

    Node n7 = Node(Vector2(1, 8));
    Node n8 = Node(Vector2(6, 8));
    Node n9 = Node(Vector2(9, 8));
    Node n10 = Node(Vector2(12, 8));
    Node n11 = Node(Vector2(15, 8));
    Node n12 = Node(Vector2(18, 8));
    Node n13 = Node(Vector2(21, 8));
    Node n14 = Node(Vector2(26, 8));

    Node n15 = Node(Vector2(1, 11));
    Node n16 = Node(Vector2(6, 11));
    Node n17 = Node(Vector2(9, 11));
    Node n18 = Node(Vector2(19, 11));
    Node n19 = Node(Vector2(21, 11));
    Node n20 = Node(Vector2(26, 11));

    Node n21 = Node(Vector2(9, 14));
    Node n22 = Node(Vector2(12, 14));
    Node n23 = Node(Vector2(15, 14));
    Node n24 = Node(Vector2(18, 14));

    Node n25 = Node(Vector2(6, 17));
    Node n26 = Node(Vector2(9, 17));
    Node n27 = Node(Vector2(18, 17));
    Node n28 = Node(Vector2(21, 17));
    Node warpL = Node(Vector2(0, 17));
    Node warpR = Node(Vector2(27, 17));

    Node n29 = Node(Vector2(9, 20));
    Node n30 = Node(Vector2(18, 20));

    Node n31 = Node(Vector2(1, 23));
    Node n32 = Node(Vector2(6, 23));
    Node n33 = Node(Vector2(9, 23));
    Node n34 = Node(Vector2(12, 23));
    Node n35 = Node(Vector2(15, 23));
    Node n36 = Node(Vector2(18, 23));
    Node n37 = Node(Vector2(21, 23));
    Node n38 = Node(Vector2(26, 23));

    Node n39 = Node(Vector2(1, 26));
    Node n40 = Node(Vector2(3, 26));
    Node n41 = Node(Vector2(6, 26));
    Node n42 = Node(Vector2(9, 26));
    Node n43 = Node(Vector2(12, 26));
    Node n44 = Node(Vector2(15, 26));
    Node n45 = Node(Vector2(18, 26));
    Node n46 = Node(Vector2(21, 26));
    Node n47 = Node(Vector2(24, 26));
    Node n48 = Node(Vector2(26, 26));

    Node n49 = Node(Vector2(1, 29));
    Node n50 = Node(Vector2(3, 29));
    Node n51 = Node(Vector2(6, 29));
    Node n52 = Node(Vector2(9, 29));
    Node n53 = Node(Vector2(12, 29));
    Node n54 = Node(Vector2(15, 29));
    Node n55 = Node(Vector2(18, 29));
    Node n56 = Node(Vector2(21, 29));
    Node n57 = Node(Vector2(24, 29));
    Node n58 = Node(Vector2(26, 29));

    Node n59 = Node(Vector2(1, 32));
    Node n60 = Node(Vector2(12, 32));
    Node n61 = Node(Vector2(15, 32));
    Node n62 = Node(Vector2(26, 32));

    n1.AddConnection(Directions::RIGHT, &n2);
    n1.AddConnection(Directions::DOWN, &n7);

    n2.AddConnection(Directions::RIGHT, &n3);
	n2.AddConnection(Directions::DOWN, &n8);

	n3.AddConnection(Directions::DOWN, &n10);

    n4.AddConnection(Directions::RIGHT, &n5);
    n4.AddConnection(Directions::DOWN, &n11);

    n5.AddConnection(Directions::RIGHT, &n6);
	n5.AddConnection(Directions::DOWN, &n13);

    n6.AddConnection(Directions::DOWN, &n14);

    n7.AddConnection(Directions::RIGHT, &n8);
    n7.AddConnection(Directions::DOWN, &n15);

	n8.AddConnection(Directions::RIGHT, &n9);
	n8.AddConnection(Directions::DOWN, &n16);

    n9.AddConnection(Directions::RIGHT, &n10);
	n9.AddConnection(Directions::DOWN, &n17);

    n10.AddConnection(Directions::RIGHT, &n11);
	
    n11.AddConnection(Directions::RIGHT, &n12);
    
    n12.AddConnection(Directions::RIGHT, &n13);
    n12.AddConnection(Directions::DOWN, &n18);

	n13.AddConnection(Directions::RIGHT, &n14);
	n13.AddConnection(Directions::DOWN, &n19);

	n14.AddConnection(Directions::DOWN, &n20);

	n15.AddConnection(Directions::RIGHT, &n16);

    n16.AddConnection(Directions::DOWN, &n25);

    n17.AddConnection(Directions::RIGHT, &n18);

    n18.AddConnection(Directions::DOWN, &n);
}*/