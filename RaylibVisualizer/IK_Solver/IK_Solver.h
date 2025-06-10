#pragma once
#include <vector>
#include <memory>
#include "Link.h"
#include "Debug.h"
#include "IK_Algo.h"

namespace IK{
	template<typename VecT>
	class IK_Solver
	{
	private:
		int max_links = 0;
		int end_index = -1;
		std::vector<std::unique_ptr<Link<VecT>>> ordered_system_links;
		std::unique_ptr<IK_Algo<VecT>> algorithm;
	public:
		IK_Solver(int max_links,IK_Algo<VecT>* algorithm);
		void addLink(VecT startPos, VecT stopPos);
		void addLink(Link<VecT>* new_link);
		void addLink(std::unique_ptr<Link<VecT>> new_link);
		void removeLink(int link_index);
		void popLink();
		void solve(const VecT& IK_StartPos, const VecT& IK_EndPos);
		const std::vector<std::unique_ptr<Link<VecT>>>& getLinksVector() {
			return ordered_system_links;
		}
#ifdef _DEBUG
		void printLinks() {
			for (int i = 0; i <= end_index; i++) {
				Link<VecT>* link = ordered_system_links[i].get();
				IK::Debug::Log("link %d start :  %s -> end : %s", i, Debug::to_string(link->getStartPos()).c_str(),
					Debug::to_string(link->getEndPos()).c_str());
			}
		}
#endif 
	};
};

// Templates need their definition in header itself
namespace IK {
	template<typename VecT>
	IK_Solver<VecT>::IK_Solver(int max_links, IK_Algo<VecT>* algorithm)
		: max_links(max_links), ordered_system_links(max_links) 
	{
		if(algorithm)
			this->algorithm = std::unique_ptr<IK_Algo<VecT>>(algorithm);
	}


	template<typename VecT>
	void IK_Solver<VecT>::addLink(const VecT startPos,const VecT stopPos)
	{
		if (end_index == max_links-1) {
			Debug::Log("WARN: Already Reached Max Links Supported By Solver");
			return;
		}
		ordered_system_links[++end_index] = std::unique_ptr<Link<VecT>>(
			std::make_unique<Link<VecT>>(startPos, stopPos)
		);
		return;
	}

	template<typename VecT>
	void IK_Solver<VecT>::addLink(Link<VecT>* new_link)
	{
		if (end_index == max_links-1) {
			Debug::Log("WARN: Already Reached Max Links Supported By Solver");
			return;
		}
		ordered_system_links[++end_index] = std::unique_ptr<Link<VecT>>(new_link);
		return;
	}

	template<typename VecT>
	void IK_Solver<VecT>::addLink(std::unique_ptr<Link<VecT>> new_link)
	{
		if (end_index == max_links - 1)
			return;
		ordered_system_links[++end_index] = std::move(new_link);
		return;
	}

	template<typename VecT>
	void IK_Solver<VecT>::removeLink(int link_index)
	{
		for (int index = link_index; index < end_index - 1; index++) {
			ordered_system_links[index] = std::move(ordered_system_links[index + 1]);
		}
		ordered_system_links[end_index--].reset();
		return;
	}

	template<typename VecT>
	void IK_Solver<VecT>::popLink()
	{
		removeLink(end_index - 1);
		return;
	}
	template < typename VecT>
	void IK_Solver<VecT>::solve(const VecT& IK_StartPos, const VecT& IK_EndPos) {
		if(algorithm)
			algorithm.get()->Solve(IK_StartPos, IK_EndPos, end_index, ordered_system_links);
	}
};
