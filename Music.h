#include <SFML/Audio.hpp>
#include <iostream>
class Music
{
public:
	static Music* GetInstance();


	void PlayMusic(std::string path);
	void StopMusic();
	void PlaySound(std::string path);
private:
	Music();
	~Music() {};
	static Music* instance;
	sf::Music* music;
	sf::Sound* sound;
	sf::SoundBuffer* buffer;

};
