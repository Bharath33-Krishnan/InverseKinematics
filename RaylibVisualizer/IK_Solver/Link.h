#pragma once
#include <type_traits>
#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <string>
#include "Debug.h"

namespace IK {
	template<typename T>
	struct is_glm_vector :std::false_type {};

	template<> struct is_glm_vector<glm::ivec2> :std::true_type {};
	template<> struct is_glm_vector<glm::ivec3> :std::true_type {};
	template<> struct is_glm_vector<glm::vec2> :std::true_type {};
	template<> struct is_glm_vector<glm::vec3> :std::true_type {};

	template<typename VecT>
	class Link
	{
		static_assert(is_glm_vector<VecT>::value, "ERROR : Link Template Type T must be a GLM vector type");
	protected:
		VecT start_pos;
		VecT end_pos;

	public:
		inline Link(const VecT& start_pos, const VecT& end_pos)
			:start_pos(start_pos), end_pos(end_pos)
		{}

		inline void setStartPos(const VecT& pos) {
			start_pos = pos;
		};
		inline void setEndPos(const VecT& pos) {
			end_pos = pos;
		};

		inline const float getLength() {
			return glm::length(end_pos - start_pos);
		}

		inline const VecT getStartPos() {
			return start_pos;
		}
		inline const VecT getEndPos() {
			return end_pos;
		}
	};
};

