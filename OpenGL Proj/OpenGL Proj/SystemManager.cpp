#include "SystemManager.h"

SystemManager* SystemManager::myself;

SystemManager::SystemManager(int* argc, char** argv)
{
	argc = new int (1);
	//argv = new char*();

	glutInit(argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);

	glutSetOption(GLUT_MULTISAMPLE, 8);

	glutCreateWindow("212CR & 217CR ECS");

	glutDisplayFunc(renderer->StaticRender);
	glutReshapeFunc(renderer->StaticResizeWindow);
	glutKeyboardFunc(StaticKeyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE_ARB);
	glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

	glLineWidth(4); 
	glPointSize(8);

	GLint multisample = -1;
	GLint numsample = -1;
	glGetIntegerv(GL_SAMPLE_BUFFERS, &multisample);
	glGetIntegerv(GL_SAMPLES, &numsample);

	std::cout << multisample << " " << numsample << std::endl;
	
	renderer = new SystemRenderer(this);
	physics = new SystemPhysics(this);
	buffer = new SystemBuffer(this);
	colliders = new SystemColliders(this);

	entity_groups[eAllEntities] = new std::vector<Entity*>;
	entity_groups[eRenderEntities] = new std::vector<Entity*>;
	entity_groups[eMotionEntities] = new std::vector<Entity*>;
	entity_groups[eCollisionEntities] = new std::vector<Entity*>;
	entity_groups[eCameraEntities] = new std::vector<Entity*>;
	entity_groups[eLightEntities] = new std::vector<Entity*>;
	entity_groups[eScriptEntities] = new std::vector<Entity*>;

	previousTime = std::chrono::steady_clock::now();
}


SystemManager::~SystemManager()
{
	delete renderer;
	delete colliders;
	delete physics;
	delete buffer;

	for (auto entry : (*entity_groups[eAllEntities]))
	{
		delete entry;
	}
}

void SystemManager::Start()
{
	buffer->ProcessBufferQueue();
	buffer->ProcessTransformQueue();
	buffer->ProcessTransformPRS();
	ProcessEntityGroup();


	while (true)
	{
		this->Tick();
	}
}

void SystemManager::Tick()
{

	auto currentTime = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime);
	previousTime = currentTime;

	delta_time = elapsed.count() / 1000000.0f;

	std::cout << "SYSTEM TICK - FPS: " << (1/delta_time) << " INTEGRATION: " << ((physics->verlet_integration) ? "VERLET" : "SEMI IMPLICIT EULER") << std::endl;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	colliders->all_oct_trees = colliders->main_oct_tree->GetAllOctTrees();

	for (auto oct_tree : colliders->all_oct_trees)
	{
		oct_tree->PreProcess();
	}

	glutPostRedisplay(); //Effectively says call glutDisplayFunc on the next main loop iteration
	
	std::thread physics_tick = physics->TickThread(); //Calls the physics member method to create a thread for the tick function
	std::thread colliders_tick = colliders->TickThread();
	std::thread input_tick = CheckInputThread();

	glutMainLoopEvent(); //Glut main loop iteration - effectively the render thread is the main thread

	physics_tick.join(); //Wait for physics thread to finish if it's still going
	colliders_tick.join(); //Wait for colliders thread to finish if it's still going
	input_tick.join();

	ProcessInput();

	for (auto entity : *entity_groups[eScriptEntities])
	{
		entity->components[eScript]->Update();
	}

	colliders->GenerateCollisionResponses();

	for (auto oct_tree : colliders->all_oct_trees)
	{
		oct_tree->PostProcess();
	}

	buffer->Tick();	//Perform buffer operations after system tick

	ProcessEntityGroup();

	glutSwapBuffers(); //Swap glut buffers
	
}

Entity* SystemManager::CreateEntity()
{
	Entity* new_entity = new Entity(this);
	entity_groups[eAllEntities]->emplace_back(new_entity);
	return new_entity;
}

bool SystemManager::DeleteEntity(Entity* entity)
{
	return true;
}

bool SystemManager::GetEntityGroup(Entity* entity, EEntityGroups index)
{
	entity_add_queue[entity].push_back(index);
	return true;
}

bool SystemManager::DeleteEntityGroup(Entity* entity, EEntityGroups index)
{
	entity_deletion_queue[entity].push_back(index);
	return true;
}

bool SystemManager::ProcessEntityGroup()
{
	for (auto entity : entity_add_queue)
	{
		for (auto entry : entity.second)
		{
			entity_groups[entry]->push_back(entity.first);
		}
	}
	entity_add_queue.clear();
	return true;
}

Command* SystemManager::AddKeyInput(char key, Command* command)
{
	key_mapping[key] = command;
	return key_mapping[key];
}

Command* SystemManager::AddToggleKeyInput(char key, Command* command)
{
	key_toggle_mapping[key] = command;
	return key_toggle_mapping[key];
}

Command* SystemManager::CheckKeyInput(char key)
{
	return key_mapping[key];
}

Command* SystemManager::CheckToggleKeyInput(char key)
{
	return key_toggle_mapping[key];
}

void SystemManager::ClearKeyInput(char key)
{
	delete key_mapping[key];
	key_mapping.erase(key);
}

void SystemManager::CheckInput()
{
	for (auto key_map : key_mapping)
	{
		if (GetAsyncKeyState(key_map.first) & 0x8000) keys_pressed.insert(key_map.first);
	}

	for (auto key_map : key_toggle_mapping)
	{
		if (GetAsyncKeyState(key_map.first) & 0x8000) keys_toggled.insert(key_map.first);
	}
}

std::thread SystemManager::CheckInputThread()
{
	return std::thread([=] {CheckInput(); });
}

void SystemManager::ProcessInput()
{
	if (keys_pressed.size() != 0)
	{
		for (auto i = keys_pressed.begin(); i != keys_pressed.end(); ++i)
		{
			key_mapping[*i]->Execute(input_focus_entity);
		}
		keys_pressed.clear();
	}

	if (keys_toggled.size() != 0)
	{

		for (auto i = keys_toggled.begin(); i != keys_toggled.end(); ++i)
		{
			if (key_already_toggled.count(*i) == 0)
			{
				key_toggle_mapping[*i]->Execute(input_focus_entity);
				key_already_toggled.insert(*i);
			}
		}
	}

	std::vector<char> keys_to_delete = {};

	for (auto key : key_already_toggled)
	{
		if (keys_toggled.count(key) == 0)
		{
			keys_to_delete.push_back(key);
		}
	}

	for (auto key : keys_to_delete)
	{
		key_already_toggled.erase(key);
	}

	keys_toggled.clear();

}

void SystemManager::KeyInput(unsigned char c, int a, int b)
{
	if (key_mapping[c] != NULL) keys_pressed.insert(c);
}

void SystemManager::StaticKeyInput(unsigned char c, int a, int b)
{
	//myself->KeyInput(c, a, b);
}
