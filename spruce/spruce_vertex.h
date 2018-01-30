#pragma once

#include "spruce_graphics.h"

namespace spruce {

	struct FSQVertex {
		fvec2 position;
	};

	struct TexturedVertex {
		fvec3 position;
		fvec3 normal;
		fvec3 color;
		fvec2 uv;
	};
}