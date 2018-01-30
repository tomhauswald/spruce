#include "spruce_bitmap.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

namespace spruce {

	Bitmap::Bitmap() : 
		mWidth(0),
		mHeight(0),
		mPixelData(nullptr) {
	}

	Bitmap::Bitmap(uint16_t width, uint16_t height) : 
		mWidth(width),
		mHeight(height) {

		auto sizeInBytes = width * height * 4;
		mPixelData = static_cast<uint8_t*> (malloc(sizeInBytes));
		memset(mPixelData, 0, sizeInBytes);
	}

	Bitmap::Bitmap(std::string const& filename) : 
		mWidth(0),
		mHeight(0),
		mPixelData(nullptr) {
		panic_if(
			!loadFromFile(filename), 
			"Failed to load bitmap from file: '" + filename + "'."
		);
	}

	void Bitmap::releasePixelData() {
		if (mPixelData != nullptr) {
			free(mPixelData);
			mPixelData = nullptr;
		}
	}

	bool Bitmap::loadFromFile(std::string const& filename) {
		releasePixelData();

		int w, h, channels;
		mPixelData = stbi_load(filename.c_str(), &w, &h, &channels, 4);

		if (mPixelData != nullptr) {
			if (w > 0 && h > 0 && channels > 0) {
				mWidth = static_cast<uint16_t>(w);
				mHeight = static_cast<uint16_t>(h);
				return true;
			}
			else {
				releasePixelData();
			}
		}

		return false;
	}
}