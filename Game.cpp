#include "Game.h"

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

    Run();
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
    debugGhostTarget = Vector2(8, 20);
    debugGhostDirection = Directions::RIGHT;
    Ghost* ghost = new Ghost();
    Blinky* blinky = new Blinky();
    Pinky* pinky = new Pinky();
	Inky* inky = new Inky(blinky);
    Clyde* clyde = new Clyde();


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

        ghost->Update();
        blinky->Update();
		pinky->Update();
		inky->Update();
		clyde->Update();

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
            + "\nSpin Time: " + std::to_string(spintime) + "\nTime spent spinning: " + std::to_string(spintime / deltaTime * 100.0f) + "%");
        window.draw(fpsText);*/
        window.display();
#endif
    }
}