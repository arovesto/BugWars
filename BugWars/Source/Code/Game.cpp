#include "pch.h"
#include "Game.h"
#include "GameBase/Log.h"
#include "Tank.h"
#include "Bug.h"
#include "Bullet.h"
#include "GameBase/Framework.h"
#include "GameBase/Log.h"

Game* g_Game;

Game::Game()
	: GameBase({ [] {auto tank = new Tank; tank->disabled = false; return tank; },
				 [] {auto bug = new Bug; bug->disabled = false; return bug; },
				 [] {auto bullet = new Bullet; bullet->disabled = false; return bullet; } })
{
	g_Game = this;
}

Game::~Game()
{
	g_Game = nullptr;
	for (auto obj : objects) delete obj;
}

bool onScreen(GameObject* obj, Point from, Point to)
{
	Point size = obj->GetSize();
	return obj->position.x >= -size.x + from.x && obj->position.x <= size.x + to.x &&
		obj->position.y >= -size.y + from.y && obj->position.y <= size.y + to.y;
}

void Game::OnUpdate(float dt)
{
	PIXScopedEvent(PIX_COLOR_INDEX(5), __FUNCTION__);
	objects.insert(objects.end(), to_add.begin(), to_add.end());
	to_add.clear();
	if (to_remove.size() > 0) {
		for (auto gone : to_remove) {
			for (int i = 0; i < objects.size(); i++) {
				if (objects[i] == gone) {
					objects[i] = objects[objects.size() - 1];
					objects.pop_back();
					break;
				}
			}
			delete gone;
		}
		to_remove.clear();
	}
	for (auto obj : objects) 
	{
		if (!obj->disabled) obj->Update(dt);
		if (!onScreen(obj, tank->position - framework->screenSize / 2, tank->position + framework->screenSize / 2)) to_remove.push_back(obj);
	}
}

void Game::OnRender() const
{
	for (auto obj : objects)
		if (obj->visible)
			DoRender(obj);
}

void Game::AddObject(GameObject* object)
{
	to_add.push_back(object);
	if (object->GetRTTI() == Bug::s_RTTI) Log("I'm a bug");
}

void Game::OnBugsSpawned()
{
}
