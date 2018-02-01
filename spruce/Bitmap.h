#pragma once

#include "Graphics.h"

namespace spruce {

	class Bitmap {
	private:
		uint16_t mWidth;
		uint16_t mHeight;
		uint8_t* mPixelData;

		void releasePixelData();

	public:
		// Creates a new bitmap with no data.
		Bitmap();

		// Creates a new black bitmap of the given size.
		Bitmap(uint16_t width, uint16_t height);

		// Creates a new bitmap using data from an input file.
		Bitmap(std::string const& filename);

		// Fills an existing bitmap with data from an input file.
		bool loadFromFile(std::string const& filename);

		inline uint16_t getWidth() const { return mWidth; }
		inline uint16_t getHeight() const { return mHeight; }
		inline uint8_t const* getPixelData() const { return mPixelData; }
		inline uint8_t* getPixelData() { return mPixelData; }

		~Bitmap() {
			releasePixelData();
		}
	};
}