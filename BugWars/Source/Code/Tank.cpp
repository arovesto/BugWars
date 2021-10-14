#include "pch.h"
#include "Tank.h"
#include "Globals.h"
#include "Game.h"
#include "Bullet.h"
#include "Bug.h"
#include "GameBase/Log.h"

void Tank::OnUpdate(float dt)
{
}

Point Tank::CalcShootDirection() const
{
	Bug* target = nullptr;
	float min_dist = std::numeric_limits<float>::max();
	for (auto object : g_Game->objects)
	{
		if (auto bug = dynamic_cast<Bug*>(object))
		{
			if (bug->disabled)
				continue;

			float dist = position.Distance(bug->position);
			if (dist < min_dist)
			{
				min_dist = dist;
				target = bug;
			}
		}
	}
	if (target == nullptr) return Point{ 1, 0 };
	Log("lock accuired");
	float ratio = BulletBase::s_Velocity / BugBase::s_Velocity;
	return (target->position * ratio - position) / (ratio - 1) - position;
}
