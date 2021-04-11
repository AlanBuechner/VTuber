#pragma once

namespace Engine
{
	class Mesh
	{
		struct Vertex {
			struct
			{
				float x, y, z;
			} Position;
			
			struct{
				float r, g, b, a;
			} Color;
		};
	}
}