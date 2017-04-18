#pragma once

#include "spruce_graphics.h"

namespace spruce {

	struct FSQ_Vertex {
		fvec2 position;
	};

	struct Textured_Vertex {
		fvec3 position;
		fvec3 normal;
		fvec3 color;
		fvec2 uv;
	};
}