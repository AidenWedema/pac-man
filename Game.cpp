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
    window.create(sf::VideoMode(224, 288), "Pac-man", sf::Style::Close);

    while (true)
    {
        switch (gameState)
        {
        case Game::CLOSE:
            return;
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
    Music::GetInstance()->PlayMusic("assets/audio/main-theme-menu.ogg");
    bool starting = false;
    int startTimer = 0;
    int scales[] = {8, 16, 24};
    int index = 0;
    bool pressed = false;

    sf::Font font;
    sf::Text TygoTexTV1;
    sf::Text TygoTexTV2;
    sf::Text AidenTexTV1;
    sf::Text AidenTexTV2;
    sf::Text AidenTexTV3;
    sf::CircleShape AidenPointerV1 = sf::CircleShape(10, 3);
    std::vector<sf::Text*> texts = {&TygoTexTV1, &TygoTexTV2, &AidenTexTV1, &AidenTexTV2, &AidenTexTV3};
    // Load the Arial font from the assets folder
    if (!font.loadFromFile("assets/fonts/Emulogic.ttf")) {
        // Handle error if the font fails to load
        std::cerr << "Error loading Arial font from assets/fonts/Emulogic.ttf" << std::endl;
        return;
    }

    // Initialize the text object for displaying FPS
    TygoTexTV1.setString("PAC-MAN");
    TygoTexTV2.setString("Press enter\n to start");
    AidenTexTV1.setString("x1");
    AidenTexTV2.setString("x2");
    AidenTexTV3.setString("x3");
    for (int i = 0; i < texts.size(); i++)
    {
        sf::Text* text = texts[i];
		text->setFont(font);
		text->setCharacterSize(14);            // Font size
		text->setFillColor(i >= 2 ? sf::Color::White : sf::Color::Yellow); // Text color
        sf::FloatRect bounds = text->getLocalBounds();
        text->setOrigin(bounds.width / 2, bounds.height / 2);
    }
    int center = window.getSize().x / 2;
    TygoTexTV1.setPosition(center, 20);
    TygoTexTV2.setPosition(center, 80);
    AidenTexTV1.setPosition(center - 50, 200);
    AidenTexTV2.setPosition(center, 200);
    AidenTexTV3.setPosition(center + 50, 200);
    sf::FloatRect bounds = AidenPointerV1.getLocalBounds();
	AidenPointerV1.setOrigin(bounds.width / 2, bounds.height / 2);
    AidenPointerV1.setPosition(center - 50, 180);
    AidenPointerV1.setRotation(180);

    float minDelta = 1.0f / 60;
    float deltaTime = 0;
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                gameState = Game::CLOSE;
            }
            if (event.type == sf::Event::Resized)
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
            if (event.type == sf::Event::KeyReleased)
            {
				if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right)
                    pressed = false;
            }
        }
        auto start = std::chrono::high_resolution_clock::now();
        window.clear();

        for (sf::Text* text : texts)
			window.draw(*text);
        window.draw(AidenPointerV1);

        if (starting)
        {
            startTimer--;
            if (startTimer % 20 < 10)
                TygoTexTV2.setFillColor(sf::Color::Yellow);
            else
				TygoTexTV2.setFillColor(sf::Color::White);

            if (startTimer <= 0)
            {
                gameState = GameState::GAME;
                break;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !starting)
        {
            starting = true;
			startTimer = 60;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !pressed && !starting)
        {
			if (index > 0)
			    index--;
            pressed = true;
			Maze::GetInstance()->SetResolution(scales[index]);
            index++;
            window.setSize(sf::Vector2u(224 * index, 288 * index));
            int center = window.getSize().x / 2;
            TygoTexTV1.setPosition(center, 20 * index);
            TygoTexTV2.setPosition(center, 80 * index);
            AidenTexTV1.setPosition(center - (50 * index), 200 * index);
            AidenTexTV2.setPosition(center, 200 * index);
            AidenTexTV3.setPosition(center + (50 * index), 200 * index);
            AidenPointerV1.setPosition(texts[index + 1]->getPosition().x, texts[index + 1]->getPosition().y - 20);
            window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - window.getSize().x / 2,
                sf::VideoMode::getDesktopMode().height / 2 - window.getSize().y / 2));
            index--;
            window.clear();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !pressed && !starting)
        {
            if (index < 2)
				index++;
            pressed = true;
            Maze::GetInstance()->SetResolution(scales[index]);
			index++;
            window.setSize(sf::Vector2u(224 * index, 288 * index));
            int center = window.getSize().x / 2;
            TygoTexTV1.setPosition(center, 20 * index);
            TygoTexTV2.setPosition(center, 80 * index);
            AidenTexTV1.setPosition(center - (50 * index), 200 * index);
            AidenTexTV2.setPosition(center, 200 * index);
            AidenTexTV3.setPosition(center + (50 * index), 200 * index);
            AidenPointerV1.setPosition(texts[index + 1]->getPosition().x, texts[index + 1]->getPosition().y - 20);
            window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - window.getSize().x / 2,
                sf::VideoMode::getDesktopMode().height / 2 - window.getSize().y / 2));
            index--;
            window.clear();
        }

        // spin until minimum delta time has passed
        auto spinstart = std::chrono::high_resolution_clock::now();
        do
        {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            deltaTime = duration.count() / 1000000.0f; // convert to seconds
        } while (deltaTime < minDelta);

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
    Music::GetInstance()->PlayMusic("assets/audio/main-theme-game.ogg");
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
    
<<<<<<< Updated upstream
    Maze::GetInstance()->LoadMaze(0);

=======
>>>>>>> Stashed changes
    float minDelta = 1.0f / 60;
    float deltaTime = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                gameState = Game::CLOSE;
            }
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
