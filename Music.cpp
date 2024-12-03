#include "Music.h"

void Music::PlayMusic(std::string path)
{
    sf::Music music;
    if (!music.openFromFile(path)) {
    }

    music.setLoop(true);
    music.play();

    if (music.getStatus() == sf::SoundSource::Playing) {
        std::cout << "Music is playing successfully!" << std::endl;
    }
}

void Music::StopMusic()
{

}
