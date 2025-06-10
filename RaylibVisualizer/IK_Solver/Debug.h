#pragma once
#include <string>
#include <cstdio>
#include <cstdarg>
#include <glm/glm.hpp>

namespace IK {
	static class Debug
	{
	private:
		static bool print_new_line_after_log;
	public:
		static inline void SetNewLineAfterLog(const bool print_new_line) {
			print_new_line_after_log = print_new_line;
		}
		static inline void Log(const char* fmt, ...) {
#ifdef _DEBUG
			va_list args;
			va_start(args,fmt);
			vprintf(fmt, args);
			print_new_line_after_log && std::printf("\n");
			va_end(args);
#endif 
		}


		static inline std::string to_string(const glm::ivec3& v) {
#ifdef _DEBUG
			return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
#else
			return "";
#endif 		
		}

		static inline std::string to_string(const glm::vec3& v) {
#ifdef _DEBUG
			return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
#else
			return "";
#endif 		
		}

		static inline std::string to_string(const glm::ivec2& v) {
#ifdef _DEBUG
			return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
#else
			return "";
#endif 		
		}

		static inline std::string to_string(const glm::vec2& v) {
#ifdef _DEBUG
			return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
#else
			return "";
#endif 		
		}
	};
};

