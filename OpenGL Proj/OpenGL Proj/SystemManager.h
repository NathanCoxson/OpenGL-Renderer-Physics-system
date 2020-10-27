#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <chrono>
#include "Windows.h"

#include "SystemRenderer.h"
#include "SystemPhysics.h"
#include "SystemBuffer.h"
#include "SystemColliders.h"
#include "Entity.h"
#include "RenderMesh.h"
#include "ColliderMesh.h"
#include "OBBCollider.h"
#include "AABBCollider.h"
#include "SphereCollider.h"
#include "ForceCommand.h"
#include "SetVelocityCommand.h"
#include "SetMainCameraCommand.h"
#include "ToggleCullOrientationCommand.h"
#include "ToggleMSAACommand.h"
#include "MultiCommand.h"
#include "ToggleBoolCommand.h"
#include "SetInputEntityCommand.h"
#include "CheckInputEntityCommand.h"
#include "ToggleFloatCommand.h"
#include "SetEntityVelocityCommand.h"
#include "Script.h"
#include "BallSpawnScript.h"


enum EEntityGroups
{
	eAllEntities,
	eRenderEntities,
	eMotionEntities,
	eCollisionEntities,
	eCameraEntities,
	eLightEntities,
	eScriptEntities
};


class SystemManager
{
public:

	static SystemManager* myself;
	
	std::map<EEntityGroups, std::vector<Entity*>*> entity_groups;

	std::map<Entity*, std::vector<EEntityGroups>> entity_add_queue;
	std::map<Entity*, std::vector<EEntityGroups>> entity_deletion_queue;

	/*std::vector<Entity*> entities;
	std::vector<Entity*> render_entities;
	std::vector<Entity*> motion_entities;
	std::vector<Entity*> collision_entities;*/

	int time_since_start = 0;
	int time_elapsed = 0;
	float delta_time = 0.0f;

	std::chrono::high_resolution_clock::time_point previousTime;

	std::map<std::string, RenderMesh*> render_meshes;
	std::map<Entity*, ColliderMesh*> collider_meshes;

	SystemRenderer* renderer;
	SystemPhysics* physics;
	SystemColliders* colliders;
	SystemBuffer* buffer;

	Entity* main_camera;


	Entity* input_focus_entity = nullptr;
	std::map<char, Command*> key_mapping;
	std::map<char, Command*> key_toggle_mapping;
	std::set<char> keys_pressed;
	std::set<char> keys_toggled;
	std::set<char> key_already_toggled;

	SystemManager(int* argc, char** argv);

	~SystemManager();

	void Start();

	void Tick();

	Entity* CreateEntity();

	bool DeleteEntity(Entity* entity);

	bool GetEntityGroup(Entity* entity, EEntityGroups index);

	bool DeleteEntityGroup(Entity* entity, EEntityGroups index);

	bool ProcessEntityGroup();

	Command* AddKeyInput(char key, Command* command);

	Command* AddToggleKeyInput(char key, Command* command);

	Command* CheckKeyInput(char key);

	Command* CheckToggleKeyInput(char key);

	void ClearKeyInput(char key);

	void CheckInput();

	std::thread CheckInputThread();

	void ProcessInput();

	void KeyInput(unsigned char c, int a, int b);

	void static StaticKeyInput(unsigned char c, int a, int b);
};

