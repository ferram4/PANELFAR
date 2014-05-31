/* Pair of vertices to be contracted */

#pragma once
#include "PANELFARMeshPrimitives-body.hpp"
#include <vector>

namespace panelfar_cpp
{
	namespace mesh_simplification
	{
		public class PairContraction
		{
		public:
			inline PairContraction() { }
			inline PairContraction(const PairContraction& c) { *this = c; }

			inline PairContraction& operator=(const PairContraction& c)
			{
				v1 = c.v1;
				v2 = c.v2;
				dv1 = Vector3(c.dv1);
				dv2 = Vector3(c.dv2);

				delta_faces.clear();
				delta_faces.assign(c.delta_faces.begin(), c.delta_faces.end());
				dead_faces.clear();
				dead_faces.assign(c.dead_faces.begin(), c.dead_faces.end());

				delta_pivot = c.delta_pivot;

				return *this;
			}

			MeshVertID v1, v2;
			Vector3 dv1, dv2;  // dv2 is not really necessary

			unsigned int delta_pivot;
			std::vector<unsigned int> delta_faces;
			std::vector<unsigned int> dead_faces;
		};
	}
}
