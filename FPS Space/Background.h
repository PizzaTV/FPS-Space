#include <SFML/Graphics.hpp>
using namespace sf;

// Текстуры разных уровней
Texture back1, back2, back3, back4, back5, back6, back7;

class background : public Drawable, public Transformable {
public:
	background() {
		m_vertices.setPrimitiveType(Quads);				// инициализируем массив точек так, чтобы они описывали 2 прямоугольника (8 точек)
		m_vertices.resize(8);
		
		back = back1;									// задаём нач. параметры для фона
		{
			// Устанавливаем текстуру первого прямоугольника, как весь фон
			m_vertices[0].texCoords = Vector2f(0, 0);
			m_vertices[1].texCoords = Vector2f(back.getSize().x, 0);
			m_vertices[2].texCoords = Vector2f(back.getSize().x, back.getSize().y);
			m_vertices[3].texCoords = Vector2f(0, back.getSize().y);
			// Устанавливаем её на экране во весь размер
			m_vertices[0].position = Vector2f(0, 0);
			m_vertices[1].position = Vector2f(back.getSize().x, 0);
			m_vertices[2].position = Vector2f(back.getSize().x, back.getSize().y);
			m_vertices[3].position = Vector2f(0, back.getSize().y);

			// Устанавливаем текстуру второго прямоугольника, как весь фон
			m_vertices[4].texCoords = Vector2f(0, back.getSize().y);
			m_vertices[5].texCoords = Vector2f(back.getSize().x, back.getSize().y);
			m_vertices[6].texCoords = Vector2f(back.getSize().x, back.getSize().y);
			m_vertices[7].texCoords = Vector2f(0, back.getSize().y);
			// Устанавливаем её на экране впритык к верхней горизонтальной линии окна в виде линии шириной 0
			m_vertices[4].position = Vector2f(0, 0);
			m_vertices[5].position = Vector2f(back.getSize().x, 0);
			m_vertices[6].position = Vector2f(back.getSize().x, 0);
			m_vertices[7].position = Vector2f(0, 0);
		}
	}

	float getSpeed()
	{
		return speed_scrolling;
	}
	void addSpeed() {
		speed_scrolling = speed_scrolling + 0.05;
		currentBack++;									// переходим к следующей текстуре
		
		// Обновляем нач. параметры текстуры согласно новой текстуре
		switch (currentBack)							
		{
		case 1:
			back = back1;
			break;
		case 2:
			back = back2;
			break;
		case 3:
			back = back3;
			break;
		case 4:
			back = back4;
			break;
		case 5:
			back = back5;
			break;
		case 6:
			back = back6;
			break;
		default:
			back = back7;
			break;
		}
		{
			// Устанавливаем текстуру первого прямоугольника, как весь фон
			m_vertices[0].texCoords = Vector2f(0, 0);
			m_vertices[1].texCoords = Vector2f(back.getSize().x, 0);
			m_vertices[2].texCoords = Vector2f(back.getSize().x, back.getSize().y);
			m_vertices[3].texCoords = Vector2f(0, back.getSize().y);
			// Устанавливаем её на экране во весь размер
			m_vertices[0].position = Vector2f(0, 0);
			m_vertices[1].position = Vector2f(back.getSize().x, 0);
			m_vertices[2].position = Vector2f(back.getSize().x, back.getSize().y);
			m_vertices[3].position = Vector2f(0, back.getSize().y);

			// Устанавливаем текстуру второго прямоугольника, как весь фон
			m_vertices[4].texCoords = Vector2f(0, back.getSize().y);
			m_vertices[5].texCoords = Vector2f(back.getSize().x, back.getSize().y);
			m_vertices[6].texCoords = Vector2f(back.getSize().x, back.getSize().y);
			m_vertices[7].texCoords = Vector2f(0, back.getSize().y);
			// Устанавливаем её на экране впритык к верхней горизонтальной линии окна в виде линии шириной 0
			m_vertices[4].position = Vector2f(0, 0);
			m_vertices[5].position = Vector2f(back.getSize().x, 0);
			m_vertices[6].position = Vector2f(back.getSize().x, 0);
			m_vertices[7].position = Vector2f(0, 0);
		}
	}
	void reset() {
		speed_scrolling = 0.15;
		// Восстанавливаем нач. параметры первого фона(текстуры)
		currentBack = 1;
		back = back1;
		{
			// Устанавливаем текстуру первого прямоугольника, как весь фон
			m_vertices[0].texCoords = Vector2f(0, 0);
			m_vertices[1].texCoords = Vector2f(back.getSize().x, 0);
			m_vertices[2].texCoords = Vector2f(back.getSize().x, back.getSize().y);
			m_vertices[3].texCoords = Vector2f(0, back.getSize().y);
			// Устанавливаем её на экране во весь размер
			m_vertices[0].position = Vector2f(0, 0);
			m_vertices[1].position = Vector2f(back.getSize().x, 0);
			m_vertices[2].position = Vector2f(back.getSize().x, back.getSize().y);
			m_vertices[3].position = Vector2f(0, back.getSize().y);

			// Устанавливаем текстуру второго прямоугольника, как весь фон
			m_vertices[4].texCoords = Vector2f(0, back.getSize().y);
			m_vertices[5].texCoords = Vector2f(back.getSize().x, back.getSize().y);
			m_vertices[6].texCoords = Vector2f(back.getSize().x, back.getSize().y);
			m_vertices[7].texCoords = Vector2f(0, back.getSize().y);
			// Устанавливаем её на экране впритык к верхней горизонтальной линии окна в виде линии шириной 0
			m_vertices[4].position = Vector2f(0, 0);
			m_vertices[5].position = Vector2f(back.getSize().x, 0);
			m_vertices[6].position = Vector2f(back.getSize().x, 0);
			m_vertices[7].position = Vector2f(0, 0);
		}
	}
	void update(float time) {

		// Уменьшаем высоту текстуру первого прямоугольника на размер текущего кадра и одновременно уменьшаем его высоту в окне на такую же величину
		m_vertices[2].texCoords = Vector2f(back.getSize().x, back.getSize().y - int(currentFrame));
		m_vertices[3].texCoords = Vector2f(0, back.getSize().y - int(currentFrame));

		m_vertices[0].position = Vector2f(0, int(currentFrame));
		m_vertices[1].position = Vector2f(back.getSize().x, int(currentFrame));


		// устанавливаем текстуру 2ого прямоугольника, как полосу шириной currentFrame, начиная от нижнего края текстуры всего фона
		m_vertices[4].texCoords = Vector2f(0, back.getSize().y - int(currentFrame));
		m_vertices[5].texCoords = Vector2f(back.getSize().x, back.getSize().y - int(currentFrame));
		// Увеличиваем высоту этой текстуры в окне так, чтобы она стала так же currentFrame
		m_vertices[6].position = Vector2f(back.getSize().x, int(currentFrame));
		m_vertices[7].position = Vector2f(0, int(currentFrame));

		// Изменяем значение текущего кадра
		if (currentFrame <= back.getSize().y)
			currentFrame = currentFrame + speed_scrolling * time;
		else
			currentFrame = 0;
	}

private:
	virtual void draw(RenderTarget& target, RenderStates states) const
	{
		states.transform *= getTransform();
		switch (currentBack)
		{
		case 1:
			states.texture = &back1;
			break;
		case 2:
			states.texture = &back2;
			break;
		case 3:
			states.texture = &back3;
			break;
		case 4:
			states.texture = &back4;
			break;
		case 5:
			states.texture = &back5;
			break;
		case 6:
			states.texture = &back6;
			break;
		default:
			states.texture = &back7;
			break;
		}
		target.draw(m_vertices, states);
	}

	
	int currentBack = 1;			// номер текущей текстуры
	float currentFrame = 0;			// текущий кадр 
	float speed_scrolling = 0.15;	// скорость прокрутки карты
	Texture back;					// текущая текстура
	VertexArray m_vertices;			// Массив с контурами блоков фона	
};
