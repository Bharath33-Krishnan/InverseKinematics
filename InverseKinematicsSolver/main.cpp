#include "Link.h"
#include "IK_Solver.h"
#include "IK_Algo_FABRIK.h"
#include <glm/vec3.hpp>

int main() {
	IK::Link<glm::vec3>* link1 = new IK::Link<glm::vec3>({ 2,2,2 }, {1,1,1});
	//IK::Link<glm::vec3>* link2 = new IK::Link<glm::vec3>({ 1,1,1 }, {2,2,2});
	//IK::Link<glm::vec3>* link3 = new IK::Link<glm::vec3>({ 1,1,1 }, {2,2,2});

	IK::IK_Algo<glm::vec3>* fabrik = new IK::IK_Algo_FABRIK<glm::vec3>();
	IK::IK_Solver<glm::vec3> solver(10,fabrik);
	solver.addLink({ 1,2,0 }, { 2,2,2 });
	solver.addLink({ 2,2,2 }, { 3,3,3 });
	solver.addLink({ 3,3,3 }, { 4,4,4 });
	solver.printLinks();
	solver.solve({ 1,1,1 }, { 0,0,5 });
	solver.printLinks();

	return 0;
}