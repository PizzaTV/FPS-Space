#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
using namespace sf;

class Enemy : public Entity {
public:
	std::list<EnemyBullet*> bullets;			// лист, который будет хранить пули, выпущенные на поле (по аналогии с enemies)

	Enemy(IntRect rect, float newSpeed, int enemyClass = 0) : Entity(rect) {
		this->enemyClass = enemyClass;

		verticalSpeed = newSpeed - newSpeed / 5;
		horisontalSpeed = 0;
		speed = newSpeed - newSpeed / 5;

		// Конструкция для болеенепредсказуемого начала полёта врага
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dist(0, 4);
		int x = dist(gen);
		switch (x)
		{
		case 1:
			accelerationH = -accelerationH;
		case 2:
			accelerationV = -accelerationV;
		case 3:
			accelerationH = -accelerationH;
			accelerationV = -accelerationV;
		default:
			break;
		}
	}
	
	// метод выпускающий пулю от позициии "носа" корабля
	void Shot() {
		bullets.push_back(new EnemyBullet(IntRect(121, 41, 9, -42), getSpeed()));
		(*(--bullets.end()))->setPosition(getPosition().x + getWidth() / 2 - 2, getPosition().y + getHeight()/* - (*(--bullets.end()))->getHeight()*/);
	}

	void setSpeed(float speed) {
		this->verticalSpeed = speed;
	}
	float getSpeed() {
		return speed;
	}
	int ClassOfEnemy()
	{
		return enemyClass;
	}

	// метод обновления противника 
	void update(float time, int level) {
		if (!explodes)
		{
			// в зависимости от того, какой сейчас уровень решаем стрелять или не стрелять
			if (level > 3)
			{
				timeAfterShot += time;
				if (timeAfterShot > 800)
				{
					Shot();
					timeAfterShot = 0;
				}
			}
			// В зависимости от класса врага задаём ему разное движение
			switch (enemyClass)
			{
			case 0:
				move(0, verticalSpeed*time);
				break;
			case 1:
				if (abs(horisontalSpeed) > 0.3)
				{
					accelerationH = -accelerationH;
				}
				horisontalSpeed -= accelerationH;
				move(horisontalSpeed*time, verticalSpeed*time);
				break;
			case 2:
				if (abs(horisontalSpeed) > 0.3)
				{
					accelerationH = -accelerationH;
				}
				if (verticalSpeed > speed+0.2 || verticalSpeed < speed - 0.2)
				{
					accelerationV = -accelerationV;
				}
				horisontalSpeed -= accelerationH;
				verticalSpeed += accelerationV;
				move(horisontalSpeed*time, verticalSpeed*time);
				break;
			default:
				break;
			}
			
		}
		else
			explosion(time);
	}
	
	// Первод противника в состояние подбит (взрывается)
	void blowUp() {
		explodes = true;
	}
	
	// Проверить - взрывается ли в данный момент?
	const bool isExplodes() const {
		return explodes;
	}

private:
	int enemyClass;						// Определяет какой это враг (0 = статический, 1 = двигающийся горизонтально, 2 = двигающийся полукругом)
	bool explodes = false;				// Булева переменная, определяющая взрывается ли объект	
	float speed;						// желаемая стредняя скорость движения врага
	float verticalSpeed;				// Скорость движения противника
	float horisontalSpeed;
	float accelerationH = 0.00015;		// скорость с которой изменяетеся ГОРИЗОНТАЛЬНОЕ направление полёта противника
	float accelerationV = 0.00015;		// скорость с которой изменяетеся ВЕРТИКАЛЬНОЕ направление полёта противника
	float timeAfterShot = 0;			// время прошедшее с последнего выстрела
};