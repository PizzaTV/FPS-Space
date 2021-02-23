#include <SFML/Graphics.hpp>
#include <list>
using namespace sf;

class Player : public Entity {
public:
	std::list<Bullet*> bullets;			// лист, который будет хранить пули, выпущенные на поле (по аналогии с enemies)

	Player(IntRect rect) : Entity(rect) {
	}

	void setSpeed(float speed) {
		this->speed = speed;
	}

	float getSpeed() {
		return speed;
	}

	// метод обновления персонажа
	void control(float time) {
		// Если он не подбит
		if (!explodes)
		{
			// Проверяем нажатие различных клавиш и в зависимости от клавиши перемещаем корабль
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
			// При нажатии на пробел
			if (Keyboard::isKeyPressed(Keyboard::Space))
			{
				// Если прошло достаточно времени с момента последнего выстрела, то делаем новый и заного запускаем часы
				if (counterShots.getElapsedTime().asMilliseconds() >= shotDelay)
				{
					counterShots.restart();
					Shot();
				}
			}
		}
		else
		{
			// Если подбит - использовать защещённый метод Entity для взрыва
			explosion(time);
		}
	}

	// метод выпускающий пулю от позициии "носа" корабля
	void Shot() {
		bullets.push_back(new Bullet(IntRect(121, 0, 9, 42), getSpeed()));
		(*(--bullets.end()))->setPosition(getPosition().x + getWidth() / 2 - 4, getPosition().y - (*(--bullets.end()))->getHeight());
	}

	// Первод корабля в состояние подбит (взрывается)
	void blowUp() {
		explodes = true;
	}
private:
	bool explodes = false;		// Булева переменная, определяющая взрывается ли объект
	float speed = 0.45;			// Скорость с которой игрок перемещается по экрану	
	float shotDelay = 300;		// Время задержки между выстрелами в миллисекундах
	Clock counterShots;			// Часы отвечающие за подсчёт времени прошедшего с момента последнего выстрела
};
