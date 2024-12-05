#include "Game.h"
#include "RNG.h"

Game* Game::instance = nullptr;
Game* Game::GetInstance()
{
	if (instance == nullptr)
		instance = new Game();
	return instance;
}

void Game::Start()
{
    window.create(sf::VideoMode(224, 288), "Pac-man");

    Maze::GetInstance()->LoadMaze(0);

    while (true)
    {
        switch (gameState)
        {
        case Game::MENU:
            Menu();
            break;
        case Game::GAME:
            Run();
            break;
        default:
            gameState = Game::MENU;
			break;
        }
    }
}

void Game::Menu()
{
    sf::Font font;
    sf::Text TygoTexTV1;
    sf::Text TygoTexTV2;
    sf::Text TygoTexTV3;
    // Load the Arial font from the assets folder
    if (!font.loadFromFile("assets/fonts/Emulogic.ttf")) {
        // Handle error if the font fails to load
        std::cerr << "Error loading Arial font from assets/fonts/Emulogic.ttf" << std::endl;
        return;
    }

    // Initialize the text object for displaying FPS
    TygoTexTV1.setFont(font);
    TygoTexTV1.setCharacterSize(14);           // Font size
    TygoTexTV1.setFillColor(sf::Color::Yellow); // Text color
    TygoTexTV1.setString("PAC-MAN");
    
    TygoTexTV2.setFont(font);
    TygoTexTV2.setCharacterSize(14);           
    TygoTexTV2.setFillColor(sf::Color::Yellow); 
    TygoTexTV2.setString("Press enter\n to start");

   //TygoTexTV3.setFont(font);
   // TygoTexTV3.setCharacterSize(14);           
   // TygoTexTV3.setFillColor(sf::Color::Yellow); 
   // TygoTexTV3.setString("Press 4 to quit");

    int center = window.getSize().x / 2;
    sf::FloatRect textBounds = TygoTexTV1.getLocalBounds();
    TygoTexTV1.setOrigin(textBounds.width / 2,2); 
    TygoTexTV1.setPosition(center,20);

    textBounds = TygoTexTV2.getLocalBounds();
    TygoTexTV2.setOrigin(textBounds.width / 2, 2);
    TygoTexTV2.setPosition(center, 80);

    //textBounds = TygoTexTV3.getLocalBounds();
    //TygoTexTV3.setOrigin(textBounds.width / 2, 2);
    //TygoTexTV3.setPosition(center, 250);

    sf::Event event;
    while (window.isOpen()){
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
        }
        window.clear();
        window.draw(TygoTexTV1);
        window.draw(TygoTexTV2);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            gameState = GameState::GAME;
            break;
        }

        //window.draw(TygoTexTV3);
        window.display();
        
    }
}

void Game::Run()
{
#ifdef _DEBUG
    sf::Font font;
    sf::Text fpsText;
    // Load the Arial font from the assets folder
    if (!font.loadFromFile("assets/fonts/Arial.ttf")) {
        // Handle error if the font fails to load
        std::cerr << "Error loading Arial font from assets/fonts/Arial.ttf" << std::endl;
        return;
    }

    // Initialize the text object for displaying FPS
    fpsText.setFont(font);
    fpsText.setCharacterSize(14);           // Font size
    fpsText.setFillColor(sf::Color::White); // Text color
    fpsText.setPosition(10, 10);            // Position on screen (top-left corner)
#endif
    // TEST
    debugGhostTarget = Vector2(17, 20);
    debugGhostDirection = Directions::RIGHT;
    Blinky* blinky = new Blinky();
    Pinky* pinky = new Pinky();
	Inky* inky = new Inky(blinky);
    Clyde* clyde = new Clyde();

    blinky->SetPosition(Vector2(13, 14));
    blinky->SetPosition(7 * Maze::GetInstance()->GetResolution(), 14 * Maze::GetInstance()->GetResolution());
	pinky->SetPosition(Vector2(13, 14));
	pinky->SetPosition(7 * Maze::GetInstance()->GetResolution(), 14 * Maze::GetInstance()->GetResolution());
	inky->SetPosition(Vector2(13, 14));
	inky->SetPosition(7 * Maze::GetInstance()->GetResolution(), 14 * Maze::GetInstance()->GetResolution());
	clyde->SetPosition(Vector2(13, 14));
	clyde->SetPosition(7 * Maze::GetInstance()->GetResolution(), 14 * Maze::GetInstance()->GetResolution());
    

    float minDelta = 1.0f / 60;
    float deltaTime = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
        }
        auto start = std::chrono::high_resolution_clock::now();
        window.clear();

        blinky->Update();
		pinky->Update();
		inky->Update();
		clyde->Update();

        sf::CircleShape pacmanShape;
		pacmanShape.setRadius(Maze::GetInstance()->GetResolution() / 2);
		pacmanShape.setFillColor(sf::Color::Yellow);
		pacmanShape.setPosition(debugGhostTarget.x * Maze::GetInstance()->GetResolution(), debugGhostTarget.y * Maze::GetInstance()->GetResolution());
		window.draw(pacmanShape);

        // move debugghosttarget with arrow keys and change debugghostdirection accordingly
        Maze::Node* node = Maze::GetInstance()->GetNode(debugGhostTarget);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
            if (node->connections.find(debugGhostDirection) != node->connections.end())
                debugGhostTarget = node->connections[debugGhostDirection]->position;
            debugGhostDirection = Directions::LEFT; 
        }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
            if (node->connections.find(debugGhostDirection) != node->connections.end())
				debugGhostTarget = node->connections[debugGhostDirection]->position;
			debugGhostDirection = Directions::RIGHT;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
            if (node->connections.find(debugGhostDirection) != node->connections.end())
				debugGhostTarget = node->connections[debugGhostDirection]->position;
			debugGhostDirection = Directions::UP;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
            if (node->connections.find(debugGhostDirection) != node->connections.end())
				debugGhostTarget = node->connections[debugGhostDirection]->position;
			debugGhostDirection = Directions::DOWN;
		}

        // make ghosts switch states based on keybord input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            blinky->SetState(Ghost::States::CHASE);
			pinky->SetState(Ghost::States::CHASE);
			inky->SetState(Ghost::States::CHASE);
			clyde->SetState(Ghost::States::CHASE);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
		{
			blinky->SetState(Ghost::States::SCATTER);
			pinky->SetState(Ghost::States::SCATTER);
			inky->SetState(Ghost::States::SCATTER);
			clyde->SetState(Ghost::States::SCATTER);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
		{
			blinky->SetState(Ghost::States::FRIGHTEND);
			pinky->SetState(Ghost::States::FRIGHTEND);
			inky->SetState(Ghost::States::FRIGHTEND);
			clyde->SetState(Ghost::States::FRIGHTEND);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
		{
			blinky->SetState(Ghost::States::EATEN);
			pinky->SetState(Ghost::States::EATEN);
			inky->SetState(Ghost::States::EATEN);
			clyde->SetState(Ghost::States::EATEN);
		}

		blinky->Draw(window);
		pinky->Draw(window);
		inky->Draw(window);
		clyde->Draw(window);

        Maze::GetInstance()->Draw(&window);

#ifndef _DEBUG
        window.display();
#endif
        // spin until minimum delta time has passed
        auto spinstart = std::chrono::high_resolution_clock::now();
        do
        {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            deltaTime = duration.count() / 1000000.0f; // convert to seconds
        } while (deltaTime < minDelta);

#ifdef _DEBUG
        /*auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - spinstart);
        float spintime = duration.count() / 1000000.0f;
        fpsText.setString("FPS: " + std::to_string(1.0f / deltaTime) + "\nDelta Time: " + std::to_string(deltaTime)
            + "\nSpin Time: " + std::to_string(spintime) + "\nTime spent spinning: " + std::to_string(spintime / deltaTime * 100.0f) + "%" + "\nRNG: " + std::to_string(RNG::seed));
        window.draw(fpsText);*/
        window.display();
#endif
    }
}

void Game::GameOver()
{
}
