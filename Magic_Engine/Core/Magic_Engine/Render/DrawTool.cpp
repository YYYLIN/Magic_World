#include "DrawTool.h"

namespace Magic {
	bool operator < (const PTEID& left, const PTEID& right) {
		if (left.Layers != right.Layers) {
			return left.Layers < right.Layers;
		}
		else {
			return left.pTE < right.pTE;
		}

		return false;
	}
}