#include "global_variable.h"
#include "Entity.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"
#include "Background.h"
#include "Counter.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <random>
using namespace sf;

int menu(RenderWindow &Game, background &bg, Counter &counter)
{
	Clock clock;			// ���� ������������ �� ������� ������� ������ ������ �����
	bg.reset();				// ���������� ���� �� ���������� ���������

	// ������ ������ � � �������������
	Sprite play;{
		
		play.setTexture(texture);
		play.setTextureRect(IntRect(2, 936, 392, 90));
		play.setPosition(W / 2 - play.getGlobalBounds().width / 2, H / 2 - play.getGlobalBounds().height);
	}
	// ������ ����� � � �������������
	Sprite exit;{
		exit.setTexture(texture);
		exit.setTextureRect(IntRect(397, 936, 392, 90));
		exit.setPosition(W / 2 - exit.getGlobalBounds().width / 2, H / 2 + exit.getGlobalBounds().height);
	}

	while (Game.isOpen())
	{
		float time = clock.restart().asMicroseconds();
		time /= 800;

		Event event;
		while (Game.pollEvent(event))
		{
			if (event.type == Event::MouseButtonPressed)
				if (event.key.code == Mouse::Left)
				{
					// �������� ���������� ����� ������������ ���� ����
					Vector2i mousePosition = Mouse::getPosition(Game);
					// ���� ������ �� ������ ������
					if (play.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
					{
						// ��������� �������� screen ��������������� ������ ����
						return 1;
					}
					// ���� ������ �� ������ �����
					if (exit.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
					{
						// ��������� �������� screen ��������������� ������ �� ����
						return -2;
					}
				}
			if (event.type == Event::Closed) // ��������� �������� screen ��������������� ������ �� ����
				return -2;

		}
		
		bg.update(time);

		Game.clear();
		Game.draw(bg);
		Game.draw(play);
		Game.draw(exit);
		Game.draw(counter.getMaxCount());
		Game.display();
	}
	return -2;
}
int levels(RenderWindow &Game, background &bg, Counter &counter)
{
	for (int level = 1; level < 8; level++)
	{
		Text screensaver;						// �������� ��� ������ ������ � ��� ���������
		bool isLevelPassed = false;				// ���� ���������� �� ������������ � ���, ��� ������� �������
		float screensaverDuration = 0;			// ����� � ������ �������
		// ��������� ��������
		{
			screensaver.setFont(font);
			screensaver.setPosition(325, 270);
			screensaver.setCharacterSize(70);
			screensaver.setFillColor(Color::White);
			screensaver.setStyle(Text::Bold);

			// ������ ������� � ����������� �� ������
			std::stringstream ss;
			if (level < 7)
			{
				ss << level;
				screensaver.setString("Level " + ss.str());
			}
			else
			{
				screensaver.setPosition(280, 270);
				screensaver.setString("Final level");
			}
		}

		// ������������� ������ ������ (� �������� ��������� ��������� ��������� ��� �������� �� ��������)
		// � setOutline() ���������� ���������� �����, ����������� ������� ������� ������� (��� �������� �������� ����������� ��������)
		// ������ ��������� ������� �������� � ������ ������ (�� ��������� ����)
		Player player(IntRect(1, 130, 128, 117)); {
			player.setOutline({ Vector2f(117, 49), Vector2f(102, 46), Vector2f(84, 28), Vector2f(77, 15), Vector2f(66, 0), Vector2f(61, 0), Vector2f(50, 15), Vector2f(43, 28), Vector2f(25, 46), Vector2f(10, 49), Vector2f(0, 60), Vector2f(0, 102), Vector2f(32, 102), Vector2f(40, 116), Vector2f(64, 88), Vector2f(87, 116), Vector2f(95, 102), Vector2f(127, 102), Vector2f(127, 60), Vector2f(117, 49) });
			player.setPosition(W / 2 - 64, H - 200);
		}
		Clock clock;							// ���� ������������ �� ������� ������� ������ ������ �����
		float timeFromLevelStart = 0;			// ����� ��������� � ������ ������

		Clock en;								// ���� ������������ �� ������� ������� � ������� ��������� ���������� �����
		int countEnemies = count_of_enemies;	// ���������� ������, ������� ������ ��������� �� ������� (count_of_enemies ��������� � ���������� ����������)
		float createEnemy = 500;				// ����� � ������������� �� ��������� ���������� ����������
		std::list<Enemy*> enemies;				// ����, � ������� �� ������ ������, ������� ��������� � ������ ������ �� �����


		// ���� ��� ������ ����������� �������� ������ ("Level n")
		while (Game.isOpen())
		{
			float time = clock.restart().asMicroseconds();
			time /= 800;

			Event event;
			while (Game.pollEvent(event))
			{
				if (event.type == Event::Closed) // ��������� �������� screen ��������������� ������ �� ����
					return -2;
			}

			bg.update(time);

			Game.clear();
			Game.draw(bg);
			Game.draw(player);
			Game.draw(screensaver);
			Game.display();

			timeFromLevelStart += time;
			if (timeFromLevelStart > time_for_screensaver)
				break;
		}

		while (Game.isOpen())
		{
			float time = clock.restart().asMicroseconds();
			time /= 800;

			Event event;
			while (Game.pollEvent(event))
			{
				if (event.type == Event::Closed) // ��������� �������� screen ��������������� ������ �� ����
					return -2;
			}

			// ���� ����� ���� - ������� � ����
			if (!player.isLife())
			{
				counter.currentCountClear();
				return -1;
			}

			// ���� ����� �� ������ ��������� - ��������� � ����������
			if (enemies.size() == 0 && countEnemies == 0)
			{
				if (level < 7)
				{
					isLevelPassed = true;
					screensaver.setPosition(220, 270);
					screensaver.setString("Level passed");
				}

				if (screensaverDuration < time_for_screensaver)
					screensaverDuration += time;						// ����������� ������������� ��������
				else
				{
					bg.addSpeed();
					if (level < 7)
						break;
					else
					{
						countEnemies = count_of_enemies;
					}
				}
			}

			// ��������� ���������� � ����(������ ���), ���� �� ��� �� ������� ���� ��������� ����������� 
			// � ������ ���������� ������� � �������� ����������� ����������
			if (countEnemies > 0 && en.getElapsedTime().asMilliseconds() >= createEnemy)
			{
				en.restart();			// ������������� ����
				countEnemies--;			// ��������� ���-�� ������������� ����������� �� ���� ������

				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> dist(570, 3000);
				createEnemy = dist(gen);


				if (level < 7)
					switch (level%3)
					{
					case 1:
					{
						// �������������� ��������� ��������� ����� � ������ ��������� ��� ���� (����� �� ��������)
						std::random_device rd;
						std::mt19937 gen(rd());
						std::uniform_int_distribution<> dist(100, W - 200);
						int x = dist(gen);

						enemies.push_back(new Enemy(IntRect(0, 0, 112, 116), bg.getSpeed(), 0)); {
							(*enemies.back()).setOutline({ Vector2f(90, 48), Vector2f(111, 0), Vector2f(60, 11),
															Vector2f(51, 11), Vector2f(0, 0), Vector2f(21, 49),
															Vector2f(50, 114), Vector2f(61, 114), Vector2f(90, 48) });
							(*enemies.back()).setPosition(x, -(*enemies.back()).getHeight());
						}
						break;
					}
					case 2:
					{
						std::random_device rd;
						std::mt19937 gen(rd());
						std::uniform_int_distribution<> dist(200, W - 400);
						int x = dist(gen);

						enemies.push_back(new Enemy(IntRect(0, 378, 125, 120), bg.getSpeed(), 1));{
							(*enemies.back()).setOutline({ Vector2f(122, 6), Vector2f(106, 0), Vector2f(63, 1),
								Vector2f(19, 0), Vector2f(3, 6), Vector2f(1, 37),
								Vector2f(39, 42), Vector2f(63, 120), Vector2f(86, 42), Vector2f(124, 37), Vector2f(122, 6) });
							(*enemies.back()).setPosition(x, -(*enemies.back()).getHeight());
						}
						break;
					}
					case 0:
					{
						std::random_device rd;
						std::mt19937 gen(rd());
						std::uniform_int_distribution<> dist(350, W - 400);
						int x = dist(gen);

						enemies.push_back(new Enemy(IntRect(1, 253, 128, 121), bg.getSpeed(), 2)); {
							(*enemies.back()).setOutline({ Vector2f(128, 13), Vector2f(71, 0), Vector2f(58, 0),
								Vector2f(1, 13), Vector2f(35, 73), Vector2f(47, 121),
								Vector2f(64, 121), Vector2f(82, 121), Vector2f(94, 73), Vector2f(128, 13) });
							(*enemies.back()).setPosition(x, -(*enemies.back()).getHeight());
						}
						break;
					}
					default:
						break;
					}
				else
				{
					std::random_device rd;
					std::mt19937 gen(rd());
					std::uniform_int_distribution<> dist(0, 2);
					int c = dist(gen);									// ��������� ����� �� 0 �� 2 (����� ����������, �������� �� ��������� � ������ ������)
					
					switch (c)
					{
					case 0:
					{
						std::random_device rd;
						std::mt19937 gen(rd());
						std::uniform_int_distribution<> dist(100, W - 200);
						int x = dist(gen);

						enemies.push_back(new Enemy(IntRect(0, 0, 112, 116), bg.getSpeed(), 0)); {
							(*enemies.back()).setOutline({ Vector2f(90, 48), Vector2f(111, 0), Vector2f(60, 11),
								Vector2f(51, 11), Vector2f(0, 0), Vector2f(21, 49),
								Vector2f(50, 114), Vector2f(61, 114), Vector2f(90, 48) });
							(*enemies.back()).setPosition(x, -(*enemies.back()).getHeight());
						}
						break;
					}
					case 1:
					{
						std::random_device rd;
						std::mt19937 gen(rd());
						std::uniform_int_distribution<> dist(200, W - 400);
						int x = dist(gen);

						enemies.push_back(new Enemy(IntRect(0, 378, 125, 120), bg.getSpeed(), 1)); {
							(*enemies.back()).setOutline({ Vector2f(122, 6), Vector2f(106, 0), Vector2f(63, 1),
								Vector2f(19, 0), Vector2f(3, 6), Vector2f(1, 37),
								Vector2f(39, 42), Vector2f(63, 120), Vector2f(86, 42), Vector2f(124, 37), Vector2f(122, 6) });
							(*enemies.back()).setPosition(x, -(*enemies.back()).getHeight());
						}
					break;
					}
					case 2:
					{
						std::random_device rd;
						std::mt19937 gen(rd());
						std::uniform_int_distribution<> dist(350, W - 400);
						int x = dist(gen);

						enemies.push_back(new Enemy(IntRect(1, 253, 128, 121), bg.getSpeed(), 2)); {
							(*enemies.back()).setOutline({ Vector2f(128, 13), Vector2f(71, 0), Vector2f(58, 0),
								Vector2f(1, 13), Vector2f(35, 73), Vector2f(47, 121),
								Vector2f(64, 121), Vector2f(82, 121), Vector2f(94, 73), Vector2f(128, 13) });
							(*enemies.back()).setPosition(x, -(*enemies.back()).getHeight());
						}
						break;
					}
					default:
						break;
					}
				}
			}

			// ���������� �� ������ ��������� �����������
			for (std::list<Enemy*>::iterator e = enemies.begin(); e != enemies.end(); e++)
			{
				// ��� ������� ���������� ���������� �� ������ ���� ���������� ������� ����
				for (std::list<Bullet*>::iterator bu = player.bullets.begin(); bu != player.bullets.end(); bu++)
				{
					// ���� ������� ������� ���� � ������� ���������� �����������
					if (!(*e)->isExplodes() && (*e)->intersects((*bu)->getOutline()))
					{
						(*e)->blowUp();		// ��������� ��������� � ��������� ������
						(*bu)->die();		// ���� ������������
						counter.addCount((*e)->ClassOfEnemy()+1);	// ���� �������������
					}
				}
				// ��� ������� ���������� �������� �� ������� ���������� �� ���� � ��������� �� ����������� � �������
				for (std::list<EnemyBullet*>::iterator bu = (*e)->bullets.begin(); bu != (*e)->bullets.end(); bu++)
				{
					if (player.intersects((*bu)->getOutline()))
					{
						player.blowUp();
						(*bu)->die();		// ���� ������������
					}
				}

				// ��� ������� ���������� ��������� ������������ �� �� � �������
				if (!(*e)->isExplodes() && (*e)->intersects(player.getOutline()))
				{
					(*e)->blowUp();		// ��������� ��������� � ��������� ������
					player.blowUp();	// ����� ��������� � ��������� ������
				}
			}

			player.control(time);
			bg.update(time);
			counter.update();

			Game.clear();
			Game.draw(bg);
			Game.draw(player);
			// �������� �� ����� ���� ������, ��������� �� � �������������
			for (std::list<Bullet*>::iterator it = player.bullets.begin(); it != player.bullets.end();)
			{
				(*it)->update(time);
				Game.draw(**it);

				// ���� ���� ��� �� ���� ��� ��� ���� �� ������� ����
				if (!(*it)->isLife() || (*it)->getPosition().y + (*it)->getHeight() < 0)
				{
					delete *it;						// ������� ������ ������ ����
					it = player.bullets.erase(it);	// ������� ��������� �� ������ ������ ���� � ������� �������� ��������� �� ������� ����
				}
				else								// ����� ��������� � ��������� ����
					it++;

				if (player.bullets.empty()) break;	// ���� �� ������� ����, � ��� ���� ��������� - ������� �� �����
			}
			// �������� ��������, ����������� ��������� � �����
			for (std::list<Enemy*>::iterator it = enemies.begin(); it != enemies.end();)
			{
				(*it)->update(time, level);
				Game.draw(**it);

				// �������� �� ����� ���� ������� �����, ��������� �� � �������������
				for (std::list<EnemyBullet*>::iterator bu = (*it)->bullets.begin(); bu != (*it)->bullets.end();)
				{
					(*bu)->update(time);
					Game.draw(**bu);

					// ���� ���� ��� �� ���� ��� ��� ���� �� ������� ����
					if (!(*bu)->isLife() || (*bu)->getPosition().y + (*bu)->getHeight() < 0)
					{
						delete *bu;						// ������� ������ ������ ����
						bu = (*it)->bullets.erase(bu);	// ������� ��������� �� ������ ������ ���� � ������� �������� ��������� �� ������� ����
					}
					else								// ����� ��������� � ��������� ����
						bu++;

					if ((*it)->bullets.empty()) break;	// ���� �� ������� ����, � ��� ���� ��������� - ������� �� �����
				}

				if (!(*it)->isLife() || (*it)->getPosition().y > H)
				{
					delete *it;
					it = enemies.erase(it);
				}
				else
					it++;

				if (enemies.empty()) { break; }
			}

			if (isLevelPassed)
			{
				Game.draw(screensaver);
			}

			Game.draw(counter.getCurrentCount());	// �������� ������� ����
			Game.display();							// ������� ��, ��� ���������� �� �����
		}
	}
	return -2;
}


int main()
{	
	RenderWindow Game(VideoMode(W, H), "SFML works!");		// �������������� ���� ����

	// ���������� ����������� � ���������� (�������) 
	// � �� �������� ��� ��������� �������� - ������� ��� ��� ����, ����� ��������� ������� �� ��������� ������� 
	Image image;
	image.loadFromFile("images\\TileSet.png");
	font.loadFromFile("fonts\\9303.ttf");

	texture.loadFromImage(image);							// ������ �������� �� ������ ������� ����������� (��������� ��� � ������������ ����� "global_variable.h")
	{
		back1.loadFromFile("images\\background_1.jpg");
		back2.loadFromFile("images\\background_2.jpg");
		back3.loadFromFile("images\\background_3.jpg");
		back4.loadFromFile("images\\background_4.jpg");
		back5.loadFromFile("images\\background_5.jpg");
		back6.loadFromFile("images\\background_6.jpg");
		back7.loadFromFile("images\\background_7.jpg");
	}


	Music music;//������� ������ ������
	music.openFromFile("music.ogg");//��������� ����
	music.play();//������������� ������
	music.setLoop(true);
	music.setVolume(50);

	
	Counter counter;
	background bg;											// ������������� ������� ������ ��� ����������� ����
	
	/*�����: -2; ����: -1; ������� n: n; */
	int screen = -1;										

	// � ������� ������� ����� ������������ ������������ ����� ���� � �����
	// ������� menu() � level_n() ���������� ��������, ������� ������������ ����, ���� ����� ������� ������������
	// (��. ����������� � screen)
	while (screen != -2)
		switch (screen)
		{
		case -1:
			screen = menu(Game, bg, counter);
			break;
		case 1:
			screen = levels(Game, bg, counter);
			break;
		default:
			break;
		}
	
	if (Game.isOpen())
		Game.close();

	return 0;
}