#include <SFML/Graphics.hpp>
using namespace sf;

class Bullet : public Entity {
public:
	Bullet(IntRect rect, float newSpeed) : Entity(rect) {
		speed += -(newSpeed / 6);
	}

	// ������� ���������� ����
	void update(float time) {
		move(0, speed * time);
	}

private:
	float speed = -0.5;		// �������� ����
};

class EnemyBullet : public Entity {
public:
	EnemyBullet(IntRect rect, float newSpeed) : Entity(rect) {
		speed += -(newSpeed / 4);
	}
	// ������� ���������� ����
	void update(float time) {
		move(0, -speed * time);
	}
private:
	float speed = -0.5;		// �������� ����
};
