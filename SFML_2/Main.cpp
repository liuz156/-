#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "Base.hpp"
#include <iostream>
#include <random>
#include <memory>

int main() {

	sf::RenderWindow window(sf::VideoMode(800, 1080), "Plane Wars (Beta)");
	window.setFramerateLimit(60);
	//�÷�
	int score = 0;

	//��������
	sf::Music musicbgm;
	if (!musicbgm.openFromFile("bgm.mp3")) {
		return -1;
	}
	sf::Music enemyMusic;
	if (!enemyMusic.openFromFile("enemyAttack.mp3")) {
		return - 1;
	}
	sf::Music crashMusic;
	if (!crashMusic.openFromFile("explode3.mp3")) {
		return -1;
	}

	//��������
	sf::Font font;
	if (!font.loadFromFile("simsun.ttc")) {
		return -1;
	}

	//����Ѫ��
	sf::RectangleShape rectangle(sf::Vector2f(200, 20));
	rectangle.setFillColor(sf::Color::Green);
	rectangle.setPosition(200, 10);
	sf::Text Hptext;
	Hptext.setFont(font);
	Hptext.setCharacterSize(20);
	Hptext.setString("HP : ");
	Hptext.setPosition(sf::Vector2f(rectangle.getPosition().x - Hptext.getGlobalBounds().width, rectangle.getPosition().y));
	Hptext.setFillColor(sf::Color::Black);
	
	//���õ÷�
	std::string ScoreString;
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(sf::Color::Black);
	scoreText.setPosition(sf::Vector2f(500.f, rectangle.getPosition().y));

	//����ʱ��
	std::string TimeString;
	sf::Text TimeText;
	TimeText.setFont(font);
	TimeText.setFillColor(sf::Color::Black);
	TimeText.setCharacterSize(20);

	//���ý�ɫ
	sf::Texture texture;
	if (!texture.loadFromFile("Jumko.png")) {
		return -1;
	}
	Player role(texture, 400.f, 500.f, 600.f,100);
	role.setHealth(100);

	//�����ӵ�
	sf::Texture textBullet;
	if (!textBullet.loadFromFile("Bullet_Enemy.png")) {
		return -1;
	}
	sf::Texture textPlayBullet;
	if (!textPlayBullet.loadFromFile("Bullet_Player.png")) {
		return -1;
	}

	//���õ���
	sf::Texture textenemy;
	if (!textenemy.loadFromFile("Enemy.png")) {
		return -1;
	}

	//�����洢���˺��ӵ���Vector
	std::vector<std::unique_ptr<Bullet>> enemyBullets;
	std::vector<std::unique_ptr<Enemy>> enemys;
	std::vector<std::unique_ptr<Bullet>> playBullets;

	float isEnemy = 0.f;
	float isBullet = 0.f;

	std::random_device rd; // ���ڻ�ȡ�����������
	std::mt19937 gen(rd()); // Mersenne Twister 19937 ���������

	// ����Ҫ���ɵ��������Χ
	std::uniform_int_distribution<> dis(0, 3); // ���� 0 �� 3 ֮����������
	std::uniform_int_distribution<> dix(0, 800);

	sf::Clock clock;
	sf::Clock runclock;
	sf::Time runTime;
	sf::Time dataTime;
	//��������
	musicbgm.play();


	while (window.isOpen()) {
		window.clear(sf::Color::Cyan);

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				window.close();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
				enemys.clear();
				enemyBullets.clear();
				playBullets.clear();
				role.setHealth(100);
				score = 0;
			}
			//��������ӵ�
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !role.isDestroyed()) {
				playBullets.emplace_back(std::make_unique<Bullet>(textPlayBullet, role.getPosition().x + role.getSprite().getGlobalBounds().width / 2.f, role.getPosition().y + role.getSprite().getGlobalBounds().height / 2.f, 3.f, role.getPosition().x + role.getSprite().getGlobalBounds().width / 2.f, role.getPosition().y - 1.f + role.getSprite().getGlobalBounds().height / 2.f, 5));
			}
		}
		//��¼��ǰʱ��
		runTime = runclock.getElapsedTime();
			
		//��¼ÿ���ƶ�ʱ��
		dataTime = clock.restart();
			
		//��������ʱ��
		isEnemy -= dataTime.asSeconds();
			
		//�ӵ�����ʱ��
		isBullet -= dataTime.asSeconds();

		//��ʾ����
		ScoreString = "Score : " + std::to_string(score);
		scoreText.setString(ScoreString);
		window.draw(scoreText);

		//��ʾʱ��
		TimeString = "Time : ";
		int Timex = static_cast<int>(runTime.asSeconds()) / 60;
		int Timey = static_cast<int>(runTime.asSeconds()) % 60;
		if (Timex < 10) {
			TimeString += "0";
		}
		TimeString += std::to_string(Timex) + ":";
		if (Timey < 10)
		{
			TimeString += "0";
		}
		TimeString += std::to_string(Timey);
		TimeText.setString(TimeString);
		TimeText.setPosition(sf::Vector2f(550 + scoreText.getGlobalBounds().width, rectangle.getPosition().y));
		window.draw(TimeText);

		//��ʾѪ��
		rectangle.setSize(sf::Vector2f(role.getHealth() * 2.f ? role.getHealth() * 2.f : 200, 20));
		window.draw(Hptext);
		window.draw(rectangle);

		if (role.isDestroyed() == false) {

			//�жϽ�ɫ�Ƿ����ӵ��͵�����ײ
			for (auto& tmp : enemyBullets) {
				if (role.getSprite().getGlobalBounds().intersects(tmp->getSprite().getGlobalBounds())) {
					role.takeDamage(tmp->getHealth());
					tmp->setHealth(0);
					crashMusic.play();
					score += 10;
				}
			}
			for (auto& tmp : playBullets) {
				for (auto& tp : enemyBullets) {
					if (tmp->getSprite().getGlobalBounds().intersects(tp->getSprite().getGlobalBounds())) {
						tp->takeDamage(tmp->getHealth());
						tmp->setHealth(0);
						crashMusic.play();
					}
				}
			}
			for (auto& tmp : playBullets) {
				for (auto& tp : enemys) {
					if ( !tmp->isDestroyed() && tmp->getSprite().getGlobalBounds().intersects(tp->getSprite().getGlobalBounds())) {
						tp->takeDamage(tmp->getHealth());
						tmp->setHealth(0);
						crashMusic.play();
						if (tp->isDestroyed()) score += 20;
					}
				}
			}

			/*
			for (auto& tmp : enemys) {
				if (role.getSprite().getGlobalBounds().intersects(tmp->getSprite().getGlobalBounds())) {
					role.takeDamage(tmp->getHealth());
					tmp->setHealth(0);
				}
			}*/
			
		
			
			//���ɵ���
			if (isEnemy < 0) {
				int numEnemy = dis(gen);
				for (int i = 0; i < numEnemy; i++) {
					enemys.emplace_back(std::make_unique<Enemy>(textenemy, dix(gen), 0.f, 100.f, 20));
				}
				isEnemy = 2.f;
			}
			//ÿ�����������ӵ�
			if (enemys.size() > 0) {
				if (isBullet < 0) {
					for (auto& tmp : enemys) {
						enemyBullets.emplace_back(std::make_unique<Bullet>(textBullet, tmp->getPosition().x, tmp->getPosition().y, 3.f, role.getPosition().x, role.getPosition().y, 5));
						enemyMusic.play();
					}
					isBullet = 1.f;
					role.takeDamage(-5);
				}
			}

			//����Խ�����
			enemys.erase(std::remove_if(enemys.begin(), enemys.end(), [](const std::unique_ptr<Enemy>& enemy) {
				return enemy->getPosition().y > 1080 || enemy->isDestroyed();
				}), enemys.end());

			//�����ӵ�
			enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(), [](const std::unique_ptr<Bullet>& bullet) {
				return bullet->getPosition().x < 0 || bullet->getPosition().y < 0 || bullet->getPosition().x > 800 || bullet->getPosition().y > 1080 || bullet->isDestroyed();
				}), enemyBullets.end());

			//��������ӵ�
			playBullets.erase(std::remove_if(playBullets.begin(), playBullets.end(), [](const std::unique_ptr<Bullet>& bullet) {
				return bullet->getPosition().y < 0 || bullet->isDestroyed();
				}), playBullets.end());

			//���µ���λ��
			for (auto& tmp : enemys) {
				tmp->update(window, dataTime.asSeconds());
			}
			
			//��ӡ����
			for (auto& tmp : enemys) {
				tmp->draw(window);
			}

			//�����ӵ�
			for (auto& tmp : enemyBullets) {
				tmp->update(window, dataTime.asSeconds());
			}
			for (auto& tmp : playBullets) {
				tmp->update(window, dataTime.asSeconds());
			}

			//��ӡ�ӵ�
			for (auto& tmp : enemyBullets) {
				tmp->draw(window);
			}	
			for (auto& tmp : playBullets) {
				tmp->draw(window);
			}
		}
		else {
			//��ʾ��Ϸ����������
			sf::Text text;
			text.setFont(font);
			text.setString("Game Over! ! !");
			text.setPosition(sf::Vector2f(400 - text.getGlobalBounds().width / 2.f, 540 - text.getGlobalBounds().height / 2.f));
			text.setFillColor(sf::Color::Black);
			window.draw(text);

			text.setString("Press R to continue and esc to exit!!");
			text.setPosition(sf::Vector2f(text.getPosition().x - 150, text.getPosition().y + text.getGlobalBounds().height));
			window.draw(text);
			runclock.restart();
			//role.setHealth(100);
		}
		
		if (musicbgm.getStatus() == sf::Music::Stopped) {
			musicbgm.play();
		}

		//���½�ɫ
		role.update(window, dataTime.asSeconds());
		role.draw(window);
		window.display();
	}

	return 0;
}