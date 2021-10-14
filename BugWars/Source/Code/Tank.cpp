#include "pch.h"
#include "Tank.h"
#include "Globals.h"
#include "Game.h"
#include "Bullet.h"
#include "Bug.h"

void Tank::OnUpdate(float dt)
{
}

float dot(Point a, Point b) {
	return a.x * b.x + a.y * b.y;
}

Point Tank::CalcShootDirection() const
{
	Bug* target = nullptr;
	float min_dist = std::numeric_limits<float>::max();
	for (auto object : g_Game->objects)
	{
		if (auto bug = dynamic_cast<Bug*>(object))
		{
			if (bug->disabled || !bug->visible)
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
	auto targetVector = Point(cosf(target->angle * std::numbers::pi_v<float> / 180.0), sinf(target->angle * std::numbers::pi_v<float> / 180.0));
	auto targetV = targetVector * BugBase::s_Velocity;
	auto difference = target->position - position;

	// https://stackoverflow.com/questions/2248876/2d-game-fire-at-a-moving-target-by-predicting-intersection-of-projectile-and-u
	auto a = targetV.Length2() - BulletBase::s_Velocity * BulletBase::s_Velocity;
	auto b = 2 * dot(targetV, difference);
	auto c = difference.Length2();
	auto t1 = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
	auto t2 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
	auto t = std::max(t1, t2);
	if (t1 > 0 && t2 > 0) t = std::min(t1, t2);
	return targetV * t + target->position - position;
}
