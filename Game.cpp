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
    Time* time = Time::GetInstance();
    time->fps = 60;
    time->minDelta = 1.0f / time->fps;
    time = nullptr;
    delete time;
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

    Time* time = Time::GetInstance();
    time->deltaTime = 0;
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
            time->deltaTime = duration.count() / 1000000.0f; // convert to seconds
        } while (time->deltaTime < time->minDelta);

        window.display();
        time->frameCount++;
    }
    time = nullptr;
    delete time;
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
    running = true;
    Music::GetInstance()->PlayMusic("assets/audio/main-theme-game.ogg");
    Maze::GetInstance()->LoadMaze(0);
    // TEST
    debugGhostTarget = Vector2(17, 20);
    debugGhostDirection = Directions::RIGHT;
    Vector2 pacSpawn = Maze::GetInstance()->GetSpawn()->position;
    Vector2 ghostHouse = Maze::GetInstance()->GetHouse()->position;
    pacman = new Player();
    Blinky* blinky = new Blinky(ghostHouse);
    Pinky* pinky = new Pinky(ghostHouse + Vector2(0, 3));
	Inky* inky = new Inky(ghostHouse + Vector2(-2, 3), blinky);
    Clyde* clyde = new Clyde(ghostHouse + Vector2(2, 3));

    int resolution = Maze::GetInstance()->GetResolution();
    pacman->SetPosition(pacSpawn);
	pacman->SetPosition(pacSpawn.x * resolution, pacSpawn.y * resolution);
    
    Time* time = Time::GetInstance();
    time->frameCount = 0;
    while (window.isOpen() && running)
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

        pacman->Update();

        debugGhostTarget = pacman->GetPosition();
        debugGhostDirection = pacman->GetDirection();

        blinky->Update();
		pinky->Update();
		inky->Update();
		clyde->Update();

        Maze::GetInstance()->Draw(&window);

		blinky->Draw(window);
		pinky->Draw(window);
		inky->Draw(window);
		clyde->Draw(window);

        pacman->Draw(window);

        // spin until minimum delta time has passed
        auto spinstart = std::chrono::high_resolution_clock::now();
        do
        {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            time->deltaTime = duration.count() / 1000000.0f; // convert to seconds
        } while (time->deltaTime < time->minDelta);

#ifdef _DEBUG
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - spinstart);
        float spintime = duration.count() / 1000000.0f;
        fpsText.setString("FPS: " + std::to_string(1.0f / time->deltaTime) + "\nDelta Time: " + std::to_string(time->deltaTime)
            + "\nSpin Time: " + std::to_string(spintime) + "\nTime spent spinning: " + 
            std::to_string(spintime / time->deltaTime * 100.0f) + "%" + "\nRNG: " + std::to_string(RNG::seed) + 
            "\nFrame Count: " + std::to_string(time->frameCount));
        window.draw(fpsText);
#endif
        window.display();
        time->frameCount++;
    }
    // delete all pointers
	blinky = nullptr;
	pinky = nullptr;
	inky = nullptr;
	clyde = nullptr;
    time = nullptr;
	delete blinky;
	delete pinky;
	delete inky;
	delete clyde;
	delete time;
}

void Game::GameOver()
{
    pacman->animations.setAnimation("die");
    int frameCount = pacman->animations.getLength();
    Time* time = Time::GetInstance();
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

        pacman->animations.Update();

        if (pacman->animations.getIndex() == frameCount - 1)
		{
			gameState = MENU;
		}
		if (pacman->animations.getIndex() == 0 && gameState == MENU)
		{
			running = false;
			break;
		}

        Maze::GetInstance()->Draw(&window);

        pacman->Draw(window);

        // spin until minimum delta time has passed
        auto spinstart = std::chrono::high_resolution_clock::now();
        do
        {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            time->deltaTime = duration.count() / 1000000.0f; // convert to seconds
        } while (time->deltaTime < time->minDelta);

        window.display();
    }
}
