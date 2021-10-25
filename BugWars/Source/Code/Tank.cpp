#include "pch.h"
#include "Tank.h"
#include "Globals.h"
#include "Game.h"
#include "Bullet.h"
#include "Bug.h"

IMPLEMENT_RTTI(Tank);

void Tank::OnUpdate(float dt)
{
}

float dot(Point a, Point b) {
	return a.x * b.x + a.y * b.y;
}

Point Tank::CalcShootDirection(Point target_pos, Point target_dir, float target_vel, float bullet_vel) const
{
	auto targetV = target_dir * target_vel;
	auto difference = target_pos - position;

	// https://stackoverflow.com/questions/2248876/2d-game-fire-at-a-moving-target-by-predicting-intersection-of-projectile-and-u
	auto a = targetV.Length2() - bullet_vel * bullet_vel;
	auto b = 2 * dot(targetV, difference);
	auto c = difference.Length2();
	auto t1 = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
	auto t2 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
	auto t = std::max(t1, t2);
	if (t1 > 0 && t2 > 0) t = std::min(t1, t2);
	return targetV * t + target_pos - position;
}

BugBase* Tank::GetBugToShoot() const
{
	for (auto obj : g_Game->objects)
		if (auto bug = dynamic_cast<Bug*>(obj))
			return bug;

	return nullptr;
}