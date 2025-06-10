#include "IK_Algo.h"
#include <glm/glm.hpp>
#include "Debug.h"

namespace IK {
	template<typename VecT>
	class IK_Algo_FABRIK:public IK_Algo<VecT> {
	public:
		void Solve(const VecT& IK_StartPos, const VecT& IK_StopPos, int last_vec_index
			, std::vector<std::unique_ptr<Link<VecT>>>& ordered_link_vector) {
			//loop from last 
			VecT next_end_pos = IK_StopPos;
			Debug::Log("Solving \n");
			for (int link = last_vec_index; link >= 0; link--) {
				Link<VecT>* curr_link = ordered_link_vector[link].get();
				VecT curr_link_start = curr_link->getStartPos();
				VecT curr_link_end = curr_link->getEndPos();
				float length = curr_link->getLength();
				curr_link->setEndPos(next_end_pos);
				VecT dir_vec = glm::normalize(curr_link_start - next_end_pos);
				curr_link->setStartPos(next_end_pos + dir_vec * length);
				Debug::Log("\n new pos of link %d length %f :\n start -> %s\n end ->%s",
					link,
					length,
					Debug::to_string(dir_vec).c_str(),
					Debug::to_string(next_end_pos).c_str()
				);
				next_end_pos = curr_link->getStartPos();
			}
			next_end_pos = IK_StartPos;
			for (int link = 0; link <= last_vec_index; link++) {
				Link<VecT>* curr_link = ordered_link_vector[link].get();
				VecT curr_link_start = curr_link->getStartPos();
				VecT curr_link_end = curr_link->getEndPos();
				float length = curr_link->getLength();
				curr_link->setStartPos(next_end_pos);
				VecT dir_vec = glm::normalize(curr_link_end - next_end_pos);
				VecT new_end_pos = next_end_pos + dir_vec * length;
				curr_link->setEndPos(new_end_pos);
				next_end_pos = new_end_pos;
			}
		}
	};
};
