#pragma once

#include "spruce_graphics.h"

namespace spruce {
	class Bitmap {
	private:
		uint16_t width_;
		uint16_t height_;
		uint8_t  channelCount_;
		uint8_t* data_;

		void release_current_data();

	public:
		// Creates a new bitmap with no data.
		Bitmap();

		// Creates a new black bitmap of the given size.
		Bitmap(uint16_t width, uint16_t height, uint8_t channels);

		// Creates a new bitmap using data from an input file.
		Bitmap(std::string const& filename, uint8_t desiredChannels = 4);

		// Fills an existing bitmap with data from an input file.
		bool load_from_file(std::string const& filename, uint8_t desiredChannels = 4);

		inline uint16_t width() const { return width_; }
		inline uint16_t height() const { return height_; }
		inline uint8_t channelCount() const { return channelCount_; }
		inline uint8_t const* data() const { return data_; }
		inline uint8_t* data() { return data_; }

		~Bitmap() {
			release_current_data();
		}
	};
}