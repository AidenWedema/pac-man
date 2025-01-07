#pragma once
#include <iostream>
#include <fstream>
#include <cstdint>
class RNG
{
public:
	static int seed;
	static int GetRandom()
	{
		seed = (seed * 5 + 1) % 8192;
		return seed;
        // Open the program binary file
        std::ifstream binaryFile("C:\\Users\\aiden\\Downloads\\pac-man\\pac-man.exe", std::ios::binary);
        if (!binaryFile.is_open()) {
            throw std::runtime_error("Failed to open program binary");
        }

        // Seek to the offset specified by the seed
        binaryFile.seekg(seed, std::ios::beg);

        // Read a single byte from the binary
        char byte = 0;
        binaryFile.read(&byte, 1);

        if (binaryFile.eof()) {
            throw std::runtime_error("Reached EOF while reading program binary");
        }

        // Close the file
        binaryFile.close();

        // Return the byte as an unsigned value
        return static_cast<uint8_t>(byte);
	}
};