#pragma once
#include <memory>
#include <vector>
#include "Link.h"

namespace IK {
	template<typename VecT>
	class IK_Algo {
	private:
	public:
		virtual void Solve(const VecT& IK_StartPos, const VecT& IK_StopPos, int last_vec_index,
			std::vector<std::unique_ptr<Link<VecT>>>& ordered_link_vector) = 0;
		virtual ~IK_Algo() = default;
	};
};
