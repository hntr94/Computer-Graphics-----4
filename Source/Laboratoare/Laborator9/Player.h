#include <Component/Transform/Transform.h>
#include <vector>
#include <string>
#include "utils.h"

#include <Core/Engine.h>
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>

using namespace std;
using namespace util;

class Player {
public:
	Player();
	Player(float x, float y);
	~Player();

	void move(float x, float y);
	void resetSpeed();
	void incrementSpeed();
	glm::vec3 GetCoordinates();
	
	Mesh* CreateSpecificMesh(float rtx, float rty, float rbx, float rby, float lbx, float lby, float ltx, float lty, string animation_name, string mesh_name);
	

	std::unordered_map<string, vector<TextureCoords>> map;
	std::unordered_map<string, vector<string>> meshes_names;
	std::unordered_map<string, vector<string>> meshes_names_reversed;
	int mirrored, animation;
	bool cpu_mode;
	//vector<TextureCoords> stay_vector;
private:
	float tx, ty, tz;
	float speed;
};