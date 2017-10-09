#include "Player.h"

#include <Core/GPU/Mesh.h>
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>

#define BASE_HEIGHT 150

using namespace std;

Player::Player()
{
	tx = ty = tz = 0.0f;
	mirrored = 1;
	animation = 1;
	speed = 1.0f;
	cpu_mode = false;
}

Player::Player(float x, float y)
{
	tx = x;
	ty = y;
	tz = 0.0f;
	mirrored = 1;
	animation = 1;
	speed = 1.0f;
	cpu_mode = false;
}

Player::~Player()
{
}

void Player::move(float x, float y) {
	tx += x * speed;
	ty += y;
}

void Player::resetSpeed()
{
	speed = 1.0f;
}

void Player::incrementSpeed()
{
	speed += 0.2f;
}


glm::vec3 Player::GetCoordinates() {
	return glm::vec3(tx,ty,tz);
}

/*void Player::Read(ifstream spr2) {
	int height, width, state_number, frame_number;
	string animation;
	
	spr2 >> height >> width;
	spr2 >> state_number;


	//std::cout << height << width << endl;

	for (int i = 0; i < state_number; i++)
	{
		spr2 >> animation >> frame_number;
		TextureCoords t;
		int fnr;
		vector<string> mshs;
		vector<string> mshs_reversed;
		for (int j = 0; j < frame_number; j++) {
			spr2 >> fnr >> t.rtx >> t.rty >> t.rbx >> t.rby >> t.lbx >> t.lby >> t.ltx >> t.lty;
			//cout << t.rtx << t.rty << t.rbx << t.rby << t.lbx << t.lby << t.ltx << t.lty << animation << endl;
			Mesh* mesh;
			mesh = CreateSpecificMesh(t.rtx, t.rty,
				t.rbx, t.rby,
				t.lbx, t.lby,
				t.ltx, t.lty,
				animation, animation + std::to_string(j));
			mshs.push_back(animation + std::to_string(j));
			meshes[mesh->GetMeshID()] = mesh;

			mesh = p2->CreateSpecificMesh(t.ltx, t.lty,
				t.lbx, t.lby,
				t.rbx, t.rby,
				t.rtx, t.rty,
				animation, animation + std::to_string(j) + "reversed");
			mshs_reversed.push_back(animation + std::to_string(j) + "reversed");
			meshes[mesh->GetMeshID()] = mesh;
		}
		p2->meshes_names.insert(std::make_pair(animation, mshs));
		p2->meshes_names_reversed.insert(std::make_pair(animation, mshs_reversed));
	}
}*/


Mesh* Player::CreateSpecificMesh(float rtx, float rty, float rbx, float rby, float lbx, float lby, float ltx, float lty, string animation_name, string mesh_name)
{
	vector<glm::vec3> vertices
	{
		glm::vec3(0.5f,   0.5f, 0.0f),	// Top Right
		glm::vec3(0.5f,  -0.5f, 0.0f),	// Bottom Right
		glm::vec3(-0.5f, -0.5f, 0.0f),	// Bottom Left
		glm::vec3(-0.5f,  0.5f, 0.0f),	// Top Left
	};

	vector<glm::vec3> normals
	{
		glm::vec3(0, 1, 1),
		glm::vec3(1, 0, 1),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0)
	};

	// TODO : Complete texture coords for the square
	vector<glm::vec2> textureCoords
	{
		glm::vec2(rtx, rty),
		glm::vec2(rbx, rby),
		glm::vec2(lbx, lby),
		glm::vec2(ltx, lty),

	};

	vector<unsigned short> indices =
	{
		0, 1, 3,
		1, 3, 2
	};
	
	
	Mesh* mesh = new Mesh(mesh_name);
	mesh->InitFromData(vertices, normals, textureCoords, indices);

	return mesh;
}