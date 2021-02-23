#include <SFML/Graphics.hpp>
using namespace sf;

const int W = 960, H = 900;					// Ширина и высота окна
const int count_of_enemies = 25;			//Количество врагов на уровень

const float time_for_screensaver = 2500;	// В миллисекундах

Texture texture;							// Переменная, в которой хранится тайлсет на протяжении всего запуска программы