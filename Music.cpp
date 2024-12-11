#include "Music.h"

Music::Music()
{
    music = new sf::Music();
    sound = new sf::Sound();
}

Music* Music::instance = nullptr;
Music* Music::GetInstance()
{
    if (instance == nullptr)
        instance = new Music();
    return instance;
}

void Music::PlayMusic(std::string path)
{
    if (!music->openFromFile(path)) {
    }

    music->setLoop(true);
    music->play();
}

void Music::StopMusic()
{

}
