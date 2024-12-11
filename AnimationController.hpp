#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <direct.h>
class AnimationController
{
public:
	AnimationController() { index = 0; timer = 0; sprite = new sf::Sprite(); reverse = false; currentAnimation = nullptr; };
	~AnimationController() { delete sprite; delete currentAnimation;  for (auto animation : animations) delete animation.second; };

	struct Animation
	{
		std::string name;								// Name of the animation
		int speed = 1;									// Amount of frames to wait before going to the next frame
		bool reverse = false;							// Should the animation be played in reverse after it reaches the end
		std::vector<sf::Texture*>* frames = nullptr;	// All frames of the animation
	};

	// Load all frames of the animation in the specified path
	void loadAnimation(std::string path, std::string name, int speed, bool reverse=false) {
		Animation* animation = new Animation();
		std::vector<sf::Texture*>* frames = new std::vector<sf::Texture*>();
		int frameCount = HowManyFrames(path);
		for (int i = 0; i < frameCount; i++) {
			sf::Texture* texture = new sf::Texture();
			texture->loadFromFile(path + "/" + std::to_string(i) + ".png");
			frames->push_back(texture);
		}
		animation->name = name;
		animation->frames = frames;
		animation->speed = speed;
		animation->reverse = reverse;
		animations[name] = animation;
	};

	// Remove the animation with the specified name from the dictionary
	void removeAnimation(std::string name) {
		auto anim = animations.find(name);
		if (anim != animations.end())
			animations.erase(anim);
		//delete anim->second;
	};

	// Set the animation with the specified name as the current animation
	void setAnimation(std::string name) {
		reverse = false;
		currentAnimation = animations[name];
		index = 0;
		timer = currentAnimation->speed;
		sf::Texture* texture = currentAnimation->frames->at(index);
		sprite->setTexture(*texture);
	};

	// Set the animation with the specified name as the current animation without resetting the index, timer and reverse
	void setAnimationNoReset(std::string name) {
		currentAnimation = animations[name];
		sf::Texture* texture = currentAnimation->frames->at(index);
		sprite->setTexture(*texture);
	}

	void nextFrame() {
		index = index + (reverse ? -1 : 1);
		if (index >= currentAnimation->frames->size() || index < 0)
		{
			if (currentAnimation->reverse)
				reverse = !reverse;
			if (reverse)
				index = (int)currentAnimation->frames->size() - 1;
			else
				index = 0;
		}
		sprite->setTexture(*currentAnimation->frames->at(index));
		timer = currentAnimation->speed;
	}

	void Update() {
		timer--;
		if (timer <= 0)
			nextFrame();
	}

	sf::Sprite* getSprite() { return sprite; };
	int getIndex() { return index; };
	int getLength() { return (int)currentAnimation->frames->size(); };
	Animation* getCurrent() { return currentAnimation; };

private:
	int index;												// Current index of frame in the animation
	int timer;												// Time left before switching to the next frame
	bool reverse;											// Is the animation playing in reverse
	sf::Sprite* sprite;										// Sprite to display
	Animation* currentAnimation;							// Currently loaded animation
	std::unordered_map<std::string, Animation*> animations;	// All animations

	// Return the amount of frames in the specified path
	int HowManyFrames(std::string path) {
		path = std::filesystem::current_path().string() + "/" + path;
		auto dirIter = std::filesystem::directory_iterator(path);
		int fileCount = 0;

		for (auto& entry : dirIter)
		{
			if (entry.is_regular_file())
				++fileCount;
		}
		return fileCount;
	}
};

