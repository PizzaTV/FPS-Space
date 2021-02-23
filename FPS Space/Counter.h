#include <SFML/Graphics.hpp>
#include <sstream>
using namespace sf;

Font font;

class Counter {
public:
	Counter() {
		currentCount = 0;
		maxCount = 0;
		
		// Настройки текста
		{
			text.setFont(font);
			text.setPosition(10, 5);
			text.setCharacterSize(30);
			text.setFillColor(Color::Red);
			text.setStyle(Text::Bold);
		}
	}

	void addCount(int c) {
		currentCount += c;
	}
	void update() {
		// Если текущий больше максимального
		if (currentCount > maxCount)
			maxCount = currentCount;		// максимальный увеличивается
	}

	void currentCountClear() {
		currentCount = 0;
	}

	Text getCurrentCount() {
		std::stringstream ss;
		ss << currentCount;
		text.setString(ss.str());
		return text;
	}
	Text getMaxCount() {
		std::stringstream ss;
		ss << maxCount;
		// L указывает на то, что это Unicode строка
		String str = L"Прошлый результат: " + ss.str();
		text.setString(str);
		return text;
	}

private:
	int maxCount;			// Лучший счёт
	int currentCount;		// счёт прямо в игре

	Text text;
};
