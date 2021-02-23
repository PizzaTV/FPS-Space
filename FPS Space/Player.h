#include <SFML/Graphics.hpp>
#include <list>
using namespace sf;

class Player : public Entity {
public:
	std::list<Bullet*> bullets;			// ����, ������� ����� ������� ����, ���������� �� ���� (�� �������� � enemies)

	Player(IntRect rect) : Entity(rect) {
	}

	void setSpeed(float speed) {
		this->speed = speed;
	}

	float getSpeed() {
		return speed;
	}

	// ����� ���������� ���������
	void control(float time) {
		// ���� �� �� ������
		if (!explodes)
		{
			// ��������� ������� ��������� ������ � � ����������� �� ������� ���������� �������
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				if (getPosition().x - speed * time >= 0)
					move(-speed * time, 0);
				else
					setPosition(0, getPosition().y);
			}
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				if (getPosition().x + speed * time <= W - getWidth())
					move(speed * time, 0);
				else
					setPosition(W - getWidth(), getPosition().y);
			}
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				if (getPosition().y - speed * time >= 0)
					move(0, -speed * time);
				else
					setPosition(getPosition().x, 0);
			}
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				if (getPosition().y + speed * time <= H - getHeight())
					move(0, speed * time);
				else
					setPosition(getPosition().x, H - getHeight());
			}
			// ��� ������� �� ������
			if (Keyboard::isKeyPressed(Keyboard::Space))
			{
				// ���� ������ ���������� ������� � ������� ���������� ��������, �� ������ ����� � ������ ��������� ����
				if (counterShots.getElapsedTime().asMilliseconds() >= shotDelay)
				{
					counterShots.restart();
					Shot();
				}
			}
		}
		else
		{
			// ���� ������ - ������������ ���������� ����� Entity ��� ������
			explosion(time);
		}
	}

	// ����� ����������� ���� �� �������� "����" �������
	void Shot() {
		bullets.push_back(new Bullet(IntRect(121, 0, 9, 42), getSpeed()));
		(*(--bullets.end()))->setPosition(getPosition().x + getWidth() / 2 - 4, getPosition().y - (*(--bullets.end()))->getHeight());
	}

	// ������ ������� � ��������� ������ (����������)
	void blowUp() {
		explodes = true;
	}
private:
	bool explodes = false;		// ������ ����������, ������������ ���������� �� ������
	float speed = 0.45;			// �������� � ������� ����� ������������ �� ������	
	float shotDelay = 300;		// ����� �������� ����� ���������� � �������������
	Clock counterShots;			// ���� ���������� �� ������� ������� ���������� � ������� ���������� ��������
};
