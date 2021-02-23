#include <SFML/Graphics.hpp>
using namespace sf;

class Entity : public Drawable, public Transformable {
public:
	Entity(IntRect rect) {
		sprite.setTexture(texture);
		sprite.setTextureRect(rect);

		// задаём контур по умолчанию (прямоугольник спрайта)
		outline.setPrimitiveType(LineStrip);
		outline.resize(4);
		this->outline[0].position = Vector2f(0, 0);
		this->outline[1].position = Vector2f(0, rect.height);
		this->outline[2].position = Vector2f(rect.width, rect.height);
		this->outline[3].position = Vector2f(rect.width, 0);

		// Запоминаем значения ширины и высоты на будущее
		width = rect.width;
		height = rect.height;
	}
	
	// Переопределяем getPosition(), чтобы пользоваться им напрямую, а не через вызов спрайта
	const Vector2f &getPosition() const {
		return sprite.getPosition();
	}
	
	// Переопределяем setPosition(), чтобы положения контура объекта перемещались вместе с самим объектом (спрайтом)
	void setPosition(float x, float y) {
		float offsetX = x - getPosition().x;
		float offsetY = y - getPosition().y;
		sprite.setPosition(x, y);
		for (int i = 0; i < outline.getVertexCount(); i++) {
			outline[i].position.x += offsetX;
			outline[i].position.y += offsetY;
		}
	}
	void setPosition(Vector2f pos) {
		float offsetX = pos.x - getPosition().x;
		float offsetY = pos.y - getPosition().y;
		sprite.setPosition(pos);
		for (int i = 0; i < outline.getVertexCount(); i++) {
			outline[i].position.x += offsetX;
			outline[i].position.y += offsetY;
		}
	}

	// Переопределяем move()
	void move(float offsetX, float offsetY) {
		setPosition(getPosition().x + offsetX, getPosition().y + offsetY);
	}
	void move(const Vector2f &offset) {
		setPosition(getPosition() + offset);
	}

	// Получаем текущие ширину и высоту объекта(спрайта)
	const float getWidth() const {
		return width;
	}
	const float getHeight() const {
		return height;
	}

	// Получаем текущие контуры объекта
	const VertexArray getOutline()  const {
		return outline;
	}

	//Используестся для изменения контуров объекта, используется только перед setPosition(), чтобы контуры имели такую же позицию, как и объект(спрайт)
	void setOutline(std::vector<Vector2f> outline) {
		this->outline.clear();
		this->outline.resize(outline.size());
		for (unsigned int i = 0; i < outline.size(); i++)
			this->outline[i].position = outline[i];
	}
	
	// метод проверки пересечения контуров текущего объекта и контуров, переданных в качестве параметра
	bool intersects(const VertexArray outline) {
		// Проходимся по всему массиву точек данного объекта и из каждой пары ищем функцию(прямую) y(x) = kx + c, которая проходит через эти две точки
		for (unsigned int i = 1; i < this->outline.getVertexCount(); i++)
		{
			// коэффициент наклона прямой
			long double k1;
			// логическая переменная, принимающая true, если получившаяся прямая будет не горизантальной (не y = c) и false в противном случае
			bool y1 = true;

			// Если вид прямой не y=c, то считаем k
			if (this->outline[i].position.x - this->outline[i - 1].position.x != 0)
				k1 = (this->outline[i].position.y - this->outline[i - 1].position.y) / (this->outline[i].position.x - this->outline[i - 1].position.x);
			else
			{
				k1 = 1;
				y1 = false;
			}
			// некоторая константа данной прямой
			long double c1;

			// Если ф-я не вида y=c
			if (y1)
				// находим по формуле c = y - kx (следует из y = kx + c)
				c1 = this->outline[i].position.y - this->outline[i].position.x * k1;
			else
				// В протисном случае просто c = y (из y = c) (с минусом из-за ориентации координатной плоскости в sfml)
				c1 = -this->outline[i].position.x;

			// строим все аналогичные прямые из контура другого объекта - подобные части кода ограничим (*)
			for (unsigned int j = 1; j < outline.getVertexCount(); j++)
			{
				// (*)
				long double k2;
				bool y2 = true;
				if (outline[j].position.x - outline[j - 1].position.x != 0)
					k2 = (outline[j].position.y - outline[j - 1].position.y) / (outline[j].position.x - outline[j - 1].position.x);
				else
				{
					k2 = 1;
					y2 = false;
				}

				long double c2;
				if (y2)
					c2 = outline[j].position.y - outline[j].position.x * k2;
				else
					c2 = -outline[j].position.x;
				// (*)


				// Если k1*x + c1 == k2*x + c2 и обе прямые не вид y=c
				if (c2 - c1 == 0 && k2 - k1 == 0 && y1 == y2)
					// Прямые пересекаются
					return true;
				// Если они параллельны => переходим к следующей паре прямых
				else if (k2 - k1 == 0 && y1 == y2)
					continue;
				else
				{
					long double x;
					long double y;

					// В зависимости от вида каждой прямой ищем точку пересечения прямых [её координаты (x, y)]
					if (!y1 && y2)
					{
						x = -c1;
						y = k2 * x + c2;
					}
					else if (y1 && !y2)
					{
						x = -c2;
						y = k1 * x + c1;
					}
					else
					{
						x = -(c2 - c1) / (k2 - k1);
						y = k1 * x + c1;
					}
					// Если найденные координаты  входят в промежутки дынных отрезков контуров
					if (fminf(this->outline[i].position.y, this->outline[i - 1].position.y) <= y && y <= fmaxf(this->outline[i].position.y, this->outline[i - 1].position.y) &&
						fminf(this->outline[i].position.x, this->outline[i - 1].position.x) <= x && x <= fmaxf(this->outline[i].position.x, this->outline[i - 1].position.x) &&
						fminf(outline[j].position.y, outline[j - 1].position.y) <= y && y <= fmaxf(outline[j].position.y, outline[j - 1].position.y) &&
						fminf(outline[j].position.x, outline[j - 1].position.x) <= x && x <= fmaxf(outline[j].position.x, outline[j - 1].position.x))
					{
						// контуры пересекаются
						return true;
					}
				}
			}
		}
		// Во всех остальных случаях контуры не пересекаются
		return false;
	}

	// метод "убицства" объекта 
	void die() {
		life = false;
	}
	
	// функция инкасулирующая чтение переменной life
	const bool isLife() const {
		return life;
	}
	
protected:
	// защищённый метод для прорисовки взрыва, вызывается в функциях update дочерних классов
	void explosion(float time) {
		// Если мы не прошли все 74 кадра анимации взрыва
		if (currentFrameExplosion <= 74)
			// изменить анимацию на следующий кадр
			sprite.setTextureRect(IntRect(130 + (int(currentFrameExplosion) % 9) * 100, (int(currentFrameExplosion) / 9) * 100, 100, 100));
		else
			// иначе уничтожить объект 
			die();
		// изменяем текущий номер текстуры в анимации
		currentFrameExplosion += time * speedExplosion;
	}

private:
	// виртуальный метод draw, чтобы рисовать сами объекты, а не их спрайты
	virtual void draw(RenderTarget& target, RenderStates states) const
	{
		target.draw(sprite);
		// можно закоментировать строку выше и раскомментировать строку ниже, чтобы увидеть контуры объектов наглядно
		//target.draw(outline, states);
	}

	bool life = true;						// Логическая переменная определяющая жив объект или нет
	float width;							// Ширина и высота текущего объекта
	float height;
	float speedExplosion = 0.1;				// скорость анимации взрыва
	float currentFrameExplosion = 0;		// Текущий кадр взырва
	Sprite sprite;							// спрайт данного объекта
	VertexArray outline;					// Массив точек для сложных спрайтов (описывается с левой верхней точки) (он же контур объекта)
};
