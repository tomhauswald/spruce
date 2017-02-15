#include "spruce_bitmap.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

namespace spruce {

	void Bitmap::release_current_data() {
		if (data_ != nullptr) {
			free(data_);
			data_ = nullptr;
		}
	}

	Bitmap::Bitmap()
		: width_(0),
		height_(0),
		data_(nullptr) {}

	Bitmap::Bitmap(uint16_t width, uint16_t height)
		: width_(width),
		height_(height) {

		auto sizeInBytes = width * height * 4;
		data_ = static_cast<uint8_t*> (malloc(sizeInBytes));
		memset(data_, 0, sizeInBytes);
	}

	Bitmap::Bitmap(std::string const& filename)
		: width_(0),
		height_(0),
		data_(nullptr) {
		panic_if(
			!load_from_file(filename), 
			"Failed to load bitmap from file: '" + filename + "'."
		);
	}

	bool Bitmap::load_from_file(std::string const& filename) {
		release_current_data();

		int w, h, channels;
		data_ = stbi_load(filename.c_str(), &w, &h, &channels, 4);

		if (data_ != nullptr) {
			if (w > 0 && h > 0 && channels > 0) {
				width_ = static_cast<uint16_t>(w);
				height_ = static_cast<uint16_t>(h);
				return true;
			}
			else {
				release_current_data();
				return false;
			}
		}
	}
}