#include "Spritegame.h"
#include "pugixml.hpp"
#include "Player.h"


#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "utils.h"



#include <Core/Engine.h>

#define BASE_HEIGHT 150

using namespace std;
using namespace util;

Player* p1 = new Player(300, BASE_HEIGHT);
Player* p2 = new Player(900, BASE_HEIGHT);

int drop_animation = 0;
int kick_animation = 0;

int stay_no = 0;
int run_no = 0;
int block_no = 0;
int drop_no = 0;
int kick_no = 0;

float stay_change_time = 0.0f;
float run_change_time = 0.0f;
float drop_change_time = 0.0f;
float kick_change_time = 0.0f;

int stay_no2 = 0;
int run_no2 = 0;
int block_no2 = 0;
int drop_no2 = 0;
int kick_no2 = 0;

float stay_change_time2 = 0.0f;
float run_change_time2 = 0.0f;
float drop_change_time2 = 0.0f;
float kick_change_time2 = 0.0f;
float block_change_time2 = 0.0f;

Spritegame::Spritegame()
{

}

Spritegame::~Spritegame()
{

}

void Spritegame::Init()
{
	const string textureLoc = "Source/Laboratoare/Laborator9/Textures/";
	int height, width, state_number, frame_number;
	string animation;

	p2->cpu_mode = true;

	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	float squareSide = 100;

	// compute coordinates of square center
	float cx = corner.x + squareSide / 2;
	float cy = corner.y + squareSide / 2;

	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;

	// initialize angularStep
	angularStep = 0;




	//Mesh* square1 = Object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
	//AddMeshToList(square1);

	// Load textures
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "grass_bilboard.png").c_str(), GL_REPEAT);
		mapTextures["grass"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "crate.jpg").c_str(), GL_REPEAT);
		mapTextures["create"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "earth.png").c_str(), GL_REPEAT);
		mapTextures["earth"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/Models/Vegetation/Bamboo/bamboo.png", GL_REPEAT);
		mapTextures["bamboo"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "back4.jpg").c_str(), GL_CLAMP_TO_BORDER);
		mapTextures["backgr"] = texture;

	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "sprite1.png").c_str(), GL_CLAMP_TO_BORDER);
		mapTextures["goku"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "sprite2.png").c_str(), GL_CLAMP_TO_BORDER);
		mapTextures["frieza"] = texture;
	}

	{
		mapTextures["random"] = CreateRandomTexture(25, 25);
	}

	// Load meshes
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("bamboo");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Vegetation/Bamboo/", "bamboo.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a simple quad
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
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f),

		};

		vector<unsigned short> indices =
		{
			0, 1, 3,
			1, 2, 3
		};

		Mesh* mesh = new Mesh("square");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}

	// create a rectangle for background
	{
		vector<glm::vec3> vertices
		{
			
			glm::vec3(resolution.x,   resolution.y, 0.0f),	// Top Right
			glm::vec3(resolution.x, 0.0f, 0.0f),	// Bottom Right
			glm::vec3(0.0f, 0.0f, 0.0f),	// Bottom Left
			glm::vec3(0.0f,  resolution.y, 0.0f),	// Top Left
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
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f),

		};

		vector<unsigned short> indices =
		{
			0, 1, 3,
			1, 3, 2
		};

		Mesh* mesh = new Mesh("rectangle");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
			
	}

	ifstream spr1;
	spr1.open(textureLoc + "spritesheet.txt");
	spr1 >> height >> width;
	spr1 >> state_number;

	cout << state_number << endl;

	for(int i = 0 ; i < state_number; i++) 
	{
		spr1 >> animation >> frame_number;
		TextureCoords t;
		int fnr;
		vector<string> mshs;
		vector<string> mshs_reversed;
		for (int j = 0; j < frame_number; j++)  {
			spr1 >> fnr >> t.rtx >> t.rty >> t.rbx >> t.rby >> t.lbx >> t.lby >> t.ltx >> t.lty;
			cout << t.rtx << t.rty << t.rbx << t.rby << t.lbx << t.lby << t.ltx << t.lty << animation << endl;
			Mesh* mesh;
			mesh = p1->CreateSpecificMesh(t.rtx, t.rty,
				t.rbx, t.rby,
				t.lbx, t.lby,
				t.ltx, t.lty,
				animation, animation + std::to_string(j));
			mshs.push_back(animation + std::to_string(j));
			meshes[mesh->GetMeshID()] = mesh;

			mesh = p1->CreateSpecificMesh(t.ltx, t.lty, 
											t.lbx, t.lby,
											t.rbx, t.rby,
											t.rtx, t.rty,
				animation, animation + std::to_string(j) + "reversed");
			mshs_reversed.push_back(animation + std::to_string(j) + "reversed");
			meshes[mesh->GetMeshID()] = mesh;
		}
		p1->meshes_names.insert(std::make_pair(animation, mshs));
		p1->meshes_names_reversed.insert(std::make_pair(animation, mshs_reversed));
	}
	spr1.close();
	
	ifstream spr2;
	spr2.open(textureLoc + "spritesheet2.txt");
	spr2 >> height >> width;
	spr2 >> state_number;

	cout << height << width << endl;

	for (int i = 0; i < state_number; i++)
	{
		spr2 >> animation >> frame_number;
		TextureCoords t;
		int fnr;
		vector<string> mshs2;
		vector<string> mshs_reversed2;
		for (int j = 0; j < frame_number; j++) {
			spr2 >> fnr >> t.rtx >> t.rty >> t.rbx >> t.rby >> t.lbx >> t.lby >> t.ltx >> t.lty;
			cout << t.rtx << t.rty << t.rbx << t.rby << t.lbx << t.lby << t.ltx << t.lty << animation << endl;
			Mesh* mesh2;
			mesh2 = p2->CreateSpecificMesh(t.rtx, t.rty,
				t.rbx, t.rby,
				t.lbx, t.lby,
				t.ltx, t.lty,
				animation, animation + std::to_string(j));
			mshs_reversed2.push_back(animation + std::to_string(j));
			meshes[mesh2->GetMeshID()] = mesh2;

			mesh2 = p2->CreateSpecificMesh(t.ltx, t.lty,
				t.lbx, t.lby,
				t.rbx, t.rby,
				t.rtx, t.rty,
				animation, animation + std::to_string(j) + "reversed");
			mshs2.push_back(animation + std::to_string(j) + "reversed");
			meshes[mesh2->GetMeshID()] = mesh2;
		}
		p2->meshes_names.insert(std::make_pair(animation, mshs2));
		p2->meshes_names_reversed.insert(std::make_pair(animation, mshs_reversed2));
	}

	spr2.close(); 
	

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderLab9");
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

void Spritegame::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Spritegame::Update(float deltaTimeSeconds)
{
	//cout << deltaTimeSeconds << endl;
	
	if (stay_change_time + 0.2 < Engine::GetElapsedTime()) {
		stay_no = (stay_no + 1) % (p1->meshes_names["stay"].size());
		stay_change_time = Engine::GetElapsedTime();
	}

	if (run_change_time + 0.1 < Engine::GetElapsedTime()) {
		run_no = (run_no + 1) % (p1->meshes_names["run"].size());
		run_change_time = Engine::GetElapsedTime();
		if(p1->animation == 2)
			p1->incrementSpeed();
	}

	if (drop_change_time + 0.15 < Engine::GetElapsedTime()) {
		drop_no = (drop_no + 1) % (p1->meshes_names["drop"].size());
		drop_change_time = Engine::GetElapsedTime();
	}

	if (kick_change_time + 0.4 < Engine::GetElapsedTime()) {
		kick_no = (kick_no + 1) % (p1->meshes_names["kick"].size());
		kick_change_time = Engine::GetElapsedTime();
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, p1->GetCoordinates());
		modelMatrix = glm::scale(modelMatrix, glm::vec3(100.0f));
		if (p1->animation == 1) {
			if (p1->mirrored == 1)
				RenderSimpleMesh(meshes[p1->meshes_names["stay"].at(stay_no)], shaders["ShaderLab9"], modelMatrix, mapTextures["goku"]);
			else
				RenderSimpleMesh(meshes[p1->meshes_names_reversed["stay"].at(stay_no)], shaders["ShaderLab9"], modelMatrix, mapTextures["goku"]);
		}
		else if (p1->animation == 2) {
			if (p1->mirrored == 1)
				RenderSimpleMesh(meshes[p1->meshes_names["run"].at(run_no)], shaders["ShaderLab9"], modelMatrix, mapTextures["goku"]);
			else
				RenderSimpleMesh(meshes[p1->meshes_names_reversed["run"].at(run_no)], shaders["ShaderLab9"], modelMatrix, mapTextures["goku"]);
		}
		else if (p1->animation == 3) {
			if (p1->mirrored == 1)
				RenderSimpleMesh(meshes[p1->meshes_names["block"].at(block_no)], shaders["ShaderLab9"], modelMatrix, mapTextures["goku"]);
			else
				RenderSimpleMesh(meshes[p1->meshes_names_reversed["block"].at(block_no)], shaders["ShaderLab9"], modelMatrix, mapTextures["goku"]);
		}
		else if (p1->animation == 4) {
			if (p1->mirrored == 1)
				RenderSimpleMesh(meshes[p1->meshes_names["drop"].at(drop_no)], shaders["ShaderLab9"], modelMatrix, mapTextures["goku"]);
			else
				RenderSimpleMesh(meshes[p1->meshes_names_reversed["drop"].at(drop_no)], shaders["ShaderLab9"], modelMatrix, mapTextures["goku"]);
			if (drop_no == p1->meshes_names["drop"].size() - 1) {
				p1->animation = 1;
				drop_animation = 0;
			}
		}
		else if (p1->animation == 5) {
			if (p1->mirrored == 1)
				RenderSimpleMesh(meshes[p1->meshes_names["kick"].at(kick_no)], shaders["ShaderLab9"], modelMatrix, mapTextures["goku"]);
			else
				RenderSimpleMesh(meshes[p1->meshes_names_reversed["kick"].at(kick_no)], shaders["ShaderLab9"], modelMatrix, mapTextures["goku"]);
			if (kick_no == p1->meshes_names["kick"].size() - 1)
				p1->animation = 1;
		}
	}

	if (stay_change_time2 + 0.4 < Engine::GetElapsedTime()) {
		stay_no2 = (stay_no2 + 1) % (p2->meshes_names["stay2"].size());
		stay_change_time2 = Engine::GetElapsedTime();
	}
	if (run_change_time2 + 0.2 < Engine::GetElapsedTime()) {
		run_no2 = (run_no2 + 1) % (p2->meshes_names["run2"].size());
		run_change_time2 = Engine::GetElapsedTime();
		if (p2->animation == 2)
			p2->incrementSpeed();
	}
	if (block_change_time2 + 0.3 < Engine::GetElapsedTime()) {
		block_no2 = (block_no2 + 1) % (p2->meshes_names["block2"].size());
		block_change_time2 = Engine::GetElapsedTime();
	}

	if (drop_change_time2 + 0.15 < Engine::GetElapsedTime()) {
		drop_no2 = (drop_no2 + 1) % (p2->meshes_names["drop2"].size());
		drop_change_time2 = Engine::GetElapsedTime();
	}

	if (kick_change_time2 + 0.4 < Engine::GetElapsedTime()) {
		kick_no2 = (kick_no2 + 1) % (p2->meshes_names["kick2"].size());
		kick_change_time2 = Engine::GetElapsedTime();
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, p2->GetCoordinates());
		if (p2->cpu_mode) {
			//modelMatrix = glm::translate(modelMatrix, glm::vec3((p1->GetCoordinates().x - p2->GetCoordinates().x)/10,0,0));
			p2->move((p1->GetCoordinates().x - p2->GetCoordinates().x) / 1000, 0);
			if ((p1->GetCoordinates().x - p2->GetCoordinates().x) > 0) {
				p2->mirrored = -1;
			}
			else {
				p2->mirrored = 1;
			}
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(100.0f));

		if (p2->animation == 1) {
			if (p2->mirrored == 1)
				RenderSimpleMesh(meshes[p2->meshes_names["stay2"].at(stay_no2)], shaders["ShaderLab9"], modelMatrix, mapTextures["frieza"]);
			else
				RenderSimpleMesh(meshes[p2->meshes_names_reversed["stay2"].at(stay_no2)], shaders["ShaderLab9"], modelMatrix, mapTextures["frieza"]);
		}
		else if (p2->animation == 2) {
			if (p2->mirrored == 1)
				RenderSimpleMesh(meshes[p2->meshes_names["run2"].at(run_no2)], shaders["ShaderLab9"], modelMatrix, mapTextures["frieza"]);
			else
				RenderSimpleMesh(meshes[p2->meshes_names_reversed["run2"].at(run_no2)], shaders["ShaderLab9"], modelMatrix, mapTextures["frieza"]);
		}
		else if (p2->animation == 3) {
			if (p2->mirrored == 1)
				RenderSimpleMesh(meshes[p2->meshes_names["block2"].at(block_no2)], shaders["ShaderLab9"], modelMatrix, mapTextures["frieza"]);
			else
				RenderSimpleMesh(meshes[p2->meshes_names_reversed["block2"].at(block_no2)], shaders["ShaderLab9"], modelMatrix, mapTextures["frieza"]);
		}
		else if (p2->animation == 4) {
			if (p2->mirrored == 1)
				RenderSimpleMesh(meshes[p2->meshes_names["drop2"].at(drop_no2)], shaders["ShaderLab9"], modelMatrix, mapTextures["frieza"]);
			else
				RenderSimpleMesh(meshes[p2->meshes_names_reversed["drop2"].at(drop_no2)], shaders["ShaderLab9"], modelMatrix, mapTextures["frieza"]);
			if (drop_no2 == p2->meshes_names["drop2"].size() - 1) {
				p2->animation = 1;
				drop_animation = 0;
			}
		}
		else if (p2->animation == 5) {
			if (p2->mirrored == 1)
				RenderSimpleMesh(meshes[p2->meshes_names["kick2"].at(kick_no2)], shaders["ShaderLab9"], modelMatrix, mapTextures["frieza"]);
			else
				RenderSimpleMesh(meshes[p2->meshes_names_reversed["kick2"].at(kick_no2)], shaders["ShaderLab9"], modelMatrix, mapTextures["frieza"]);
			if (kick_no2 == p2->meshes_names["kick2"].size() - 1) {
				p2->animation = 1;
				kick_animation = 0;
			}
		}
	}

	if (p2->cpu_mode && abs(p1->GetCoordinates().x - p2->GetCoordinates().x) < 100 && !kick_animation) {
		p2->animation = 5;
		kick_animation = 1;
	}

	if (!drop_animation) {
		if (abs(p1->GetCoordinates().x - p2->GetCoordinates().x) < 100 && p2->animation != 3) {
			if (p1->animation == 5) {
				p2->animation = 4;
				drop_change_time2 = 0;
				drop_animation = 1;
			}
		}

		if (abs(p1->GetCoordinates().x - p2->GetCoordinates().x) < 100 && p1->animation != 3) {
			if (p2->animation == 5) {
				p1->animation = 4;
				drop_change_time = 0;
				drop_animation = 1;
			}
		}
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 1, -3));
		//modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		RenderSimpleMesh(meshes["rectangle"], shaders["ShaderLab9"], modelMatrix, mapTextures["backgr"]);
	}


}

void Spritegame::FrameEnd()
{
	DrawCoordinatSystem();
}

void Spritegame::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	
	float time = Engine::GetElapsedTime();
	int location;
	location = glGetUniformLocation(shader->program, "time");
	glUniform1f(location, time);
	int check;
	if (mesh->GetMeshID() == meshes["sphere"]->GetMeshID()) {
		check = 1;
	}
	else {
		check = 0;
	}
	location = glGetUniformLocation(shader->program, "check");
	glUniform1i(location, check);


	if (texture1)
	{
		//TODO : activate texture location 0
		//TODO : Bind the texture1 ID
		//TODO : Send texture uniform value

		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());

		glUniform1i(glGetUniformLocation(shader->GetProgramID(), "texture_1"), 0);
	}

	if (texture2)
	{
		//TODO : activate texture location 1
		//TODO : Bind the texture2 ID
		//TODO : Send texture uniform value

		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());

		glUniform1i(glGetUniformLocation(shader->GetProgramID(), "texture_2"), 1);

	}



	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

Texture2D* Spritegame::CreateRandomTexture(unsigned int width, unsigned int height)
{
	GLuint textureID = 0;
	unsigned int channels = 3;
	unsigned int size = width * height * channels;
	unsigned char* data = new unsigned char[size];
	for (int i = 0; i < size; i++) {
		data[i] = rand() % 255;
	}

	// TODO: generate random texture data
	unsigned int gl_texture_object;

	glGenTextures(1, &gl_texture_object);

	// Generate and bind the new texture ID

	// TODO: Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S , GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	CheckOpenGLError();

	// TODO: Use glTextImage2D to set the texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	// TODO: Generate texture mip-maps
	glGenerateMipmap(GL_TEXTURE_2D);

	CheckOpenGLError();

	// Save the texture into a wrapper Texture2D class for using easier later during rendering phase
	Texture2D* texture = new Texture2D();
	texture->Init(textureID, width, height, channels);

	SAFE_FREE_ARRAY(data);
	return texture;
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Spritegame::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 2;

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		p1->move(0.5f,0);
		p1->animation = 2;
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		p1->move(-0.5f, 0);
		p1->animation = 2;
	}

	if (window->KeyHold(GLFW_KEY_E)) {
		//p1->move(-1, 0);
		p1->animation = 3;
	}

	if (p2->cpu_mode == false) {
		if (window->KeyHold(GLFW_KEY_LEFT)) {
			p2->move(-0.5f, 0);
			p2->animation = 2;
		}

		if (window->KeyHold(GLFW_KEY_RIGHT)) {
			p2->move(0.5f, 0);
			p2->animation = 2;
		}

		if (window->KeyHold(GLFW_KEY_RIGHT_SHIFT)) {
			//p1->move(-1, 0);
			p2->animation = 3;
		}
	}
}

void Spritegame::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_A) {
		p1->mirrored = -1;
		p1->animation = 2;
	}

	if (key == GLFW_KEY_D) {
		p1->mirrored = 1;
		p1->animation = 2;
	}

	if (key == GLFW_KEY_SPACE && (p1->animation == 1 || p1->animation == 2)) {
		p1->animation = 5;
		kick_change_time = 0;
		kick_no = -1;
	}

	if (p2->cpu_mode == false) {
		if (key == GLFW_KEY_LEFT) {
			p2->mirrored = 1;
			p2->animation = 2;
		}

		if (key == GLFW_KEY_RIGHT) {
			p2->mirrored = -1;
			p2->animation = 2;
		}

		if (key == GLFW_KEY_RIGHT_CONTROL && (p2->animation == 1 || p2->animation == 2)) {
			p2->animation = 5;
			kick_change_time2 = 0;
			kick_no2 = -1;
		}
	}
}

void Spritegame::OnKeyRelease(int key, int mods)
{
	// add key release event
	if (key == GLFW_KEY_W || key == GLFW_KEY_A || key == GLFW_KEY_S || key == GLFW_KEY_D || key == GLFW_KEY_E) {
		if (p1->animation != 5) {
			p1->animation = 1;
			p1->resetSpeed();
		}
	}

	if (p2->cpu_mode == false) {
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT || key == GLFW_KEY_DOWN || key == GLFW_KEY_RIGHT_SHIFT || key == GLFW_KEY_UP) {
			if (p2->animation != 5) {
				p2->animation = 1;
				p2->resetSpeed();
			}
		}
	}
}

void Spritegame::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Spritegame::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Spritegame::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Spritegame::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Spritegame::OnWindowResize(int width, int height)
{
}
