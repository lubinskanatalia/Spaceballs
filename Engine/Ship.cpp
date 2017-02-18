#include "MainWindow.h"
#include "Ship.h"
#include "Surface.h"

Ship::Ship(BulletManager& Manager, Surface& ShipSurface, Surface& exhaust, 
	Surface& red, AnimationFrames& shiprekt, Surface& rektsurface, AnimationFrames& holeAnim, AnimationFrames& holeRektAnim)
	:
	bManager(Manager),
	shipSurface(ShipSurface),
	exhaustSurface(exhaust),
	redSurface(red), 
	shipRekt(shiprekt, 2),
	rektSurface(rektsurface),
	blackHole(holeAnim, 1),
	blackHoleRekt(holeRektAnim, 1)
{}

void Ship::HandleCollision(int Damage)
{
	if (collidesWithHole)
	{
		if (health.GetHealthAmount() > lowHealth)
		{
			blackHole.Advance();
		}
		else
		{
			blackHoleRekt.Advance();
		}

		if (blackHole.AnimEnd() || blackHoleRekt.AnimEnd())
		{
			health.Damage(Damage);
		}
	}

	else
	{
		health.Damage(Damage);
		isHit = true;
	}
}

void Ship::Draw(Graphics& gfx)
{
	if (health.HasHealth())
	{
		if (health.GetHealthAmount() > lowHealth && !collidesWithHole)
		{
			gfx.DrawSpriteKey(int(pos.x), int(pos.y), shipSurface, shipSurface.GetPixel(0, 0));

			if (isMoving)
			{
				gfx.DrawSpriteKey(int(pos.x), int(pos.y), exhaustSurface, exhaustSurface.GetPixel(0, 0));
			}

			if (isHit)
			{
				gfx.DrawSpriteKey(int(pos.x), int(pos.y), redSurface, redSurface.GetPixel(0, 0));
			}
		}
		
		else if (!collidesWithHole)
		{
			shipRekt.Draw(int(pos.x), int(pos.y), gfx);
			if (isMoving)
			{
				gfx.DrawSpriteKey(int(pos.x), int(pos.y), rektSurface, rektSurface.GetPixel(0, 0));
			}
		}

		else
		{
			if (health.GetHealthAmount() > lowHealth)
			{
				blackHole.Draw(int(pos.x), int(pos.y), gfx);
			}
			
			else
			{
				blackHoleRekt.Draw(int(pos.x), int(pos.y), gfx);
			}
		}
		
		health.Draw(gfx);
		bManager.DrawBullets(gfx);
	}
}

void Ship::ClampScreen()
{
	pos.x = std::max(0.f, std::min(pos.x, float(Graphics::ScreenWidth) - (width + 1.f)));
	pos.y = std::max(0.f, std::min(pos.y, float(Graphics::ScreenHeight) - (height + 1.f)));
}

void Ship::PlayerInput(Keyboard& kbd, float dt)
{
	if (kbd.KeyIsPressed(VK_UP))
	{
		pos.y -= speed * dt;
		isMoving = true;
	}

	else if (kbd.KeyIsPressed(VK_DOWN))
	{
		pos.y += speed * dt;
		isMoving = false;
	}

	else
	{
		isMoving = false;
	}
	
	if (kbd.KeyIsPressed(VK_LEFT))
	{
		pos.x -= speed * dt;
	}

	else if (kbd.KeyIsPressed(VK_RIGHT))
	{
		pos.x += speed * dt;
	}

	if (kbd.KeyIsPressed(VK_SPACE))
	{
		bManager.FireBullet(Vec2(pos.x + canonX, pos.y + canonY), dt);
	}

	else
	{
		bManager.ResetShotsFired();
	}
}

void Ship::Restore(int restore)
{
	health.Restore(restore);
}

bool Ship::HasHealth() const
{
	return health.HasHealth();
}

RectF Ship::GetCollisionRect()
{
	return RectF(pos, width, height);
}

float Ship::GetX() const
{
	return pos.x;
}

float Ship::GetWidth() const
{
	return width;
}

float Ship::GetY() const
{
	return pos.y;
}

float Ship::GetHeight() const
{
	return height;
}

int Ship::GetHealth() const
{
	return health.GetHealthAmount();
}

void Ship::SethitTarget(bool hit)
{
	hitTarget = hit;
}

void Ship::AddGravity(Vec2 & gravity)
{
	pos -= gravity;
}

void Ship::SetY(float Y)
{
	pos.y = Y;
}

void Ship::SetX(float X)
{
	pos.x = X;
}

int Ship::GetDmg() const
{
	return dmg;
}

void Ship::CollidesWithHole(bool collides)
{
	collidesWithHole = collides;
}

void Ship::Reset()
{
	pos.x = 300.0f;
	pos.y = 300.0f;
	health.Reset();
	isHit = false;
	isMoving = false;
	collidesWithHole = false;
	blackHole.Reset();
	blackHoleRekt.Reset();
}

void Ship::Update(Keyboard & wnd, float dt)
{
	if (pos.y + 2 > Graphics::ScreenHeight)
	{
		health.Damage(health.GetHealthAmount());
	}
	if (HasHealth())
	{
		PlayerInput(wnd, dt);
		ClampScreen();
	}
	if (isHit)
	{
		isHitCounter++;
		if (isHitCounter >= 10)
		{
			isHitCounter = 0;
			isHit = false;
		}
	}
	if (health.GetHealthAmount() <= lowHealth)
	{
		shipRekt.Advance();
		if (shipRekt.AnimEnd())
		{
			shipRekt.Reset();
		}
	}
}