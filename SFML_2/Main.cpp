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
	//得分
	int score = 0;

	//音乐设置
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

	//字体设置
	sf::Font font;
	if (!font.loadFromFile("simsun.ttc")) {
		return -1;
	}

	//设置血量
	sf::RectangleShape rectangle(sf::Vector2f(200, 20));
	rectangle.setFillColor(sf::Color::Green);
	rectangle.setPosition(200, 10);
	sf::Text Hptext;
	Hptext.setFont(font);
	Hptext.setCharacterSize(20);
	Hptext.setString("HP : ");
	Hptext.setPosition(sf::Vector2f(rectangle.getPosition().x - Hptext.getGlobalBounds().width, rectangle.getPosition().y));
	Hptext.setFillColor(sf::Color::Black);
	
	//设置得分
	std::string ScoreString;
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(20);
	scoreText.setFillColor(sf::Color::Black);
	scoreText.setPosition(sf::Vector2f(500.f, rectangle.getPosition().y));

	//设置时间
	std::string TimeString;
	sf::Text TimeText;
	TimeText.setFont(font);
	TimeText.setFillColor(sf::Color::Black);
	TimeText.setCharacterSize(20);

	//设置角色
	sf::Texture texture;
	if (!texture.loadFromFile("Jumko.png")) {
		return -1;
	}
	Player role(texture, 400.f, 500.f, 600.f,100);
	role.setHealth(100);

	//设置子弹
	sf::Texture textBullet;
	if (!textBullet.loadFromFile("Bullet_Enemy.png")) {
		return -1;
	}
	sf::Texture textPlayBullet;
	if (!textPlayBullet.loadFromFile("Bullet_Player.png")) {
		return -1;
	}

	//设置敌人
	sf::Texture textenemy;
	if (!textenemy.loadFromFile("Enemy.png")) {
		return -1;
	}

	//建立存储敌人和子弹的Vector
	std::vector<std::unique_ptr<Bullet>> enemyBullets;
	std::vector<std::unique_ptr<Enemy>> enemys;
	std::vector<std::unique_ptr<Bullet>> playBullets;

	float isEnemy = 0.f;
	float isBullet = 0.f;

	std::random_device rd; // 用于获取真随机数种子
	std::mt19937 gen(rd()); // Mersenne Twister 19937 随机数引擎

	// 定义要生成的随机数范围
	std::uniform_int_distribution<> dis(0, 3); // 生成 0 到 3 之间的随机整数
	std::uniform_int_distribution<> dix(0, 800);

	sf::Clock clock;
	sf::Clock runclock;
	sf::Time runTime;
	sf::Time dataTime;
	//播放音乐
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
			//生成玩家子弹
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !role.isDestroyed()) {
				playBullets.emplace_back(std::make_unique<Bullet>(textPlayBullet, role.getPosition().x + role.getSprite().getGlobalBounds().width / 2.f, role.getPosition().y + role.getSprite().getGlobalBounds().height / 2.f, 3.f, role.getPosition().x + role.getSprite().getGlobalBounds().width / 2.f, role.getPosition().y - 1.f + role.getSprite().getGlobalBounds().height / 2.f, 5));
			}
		}
		//记录当前时间
		runTime = runclock.getElapsedTime();
			
		//记录每次移动时间
		dataTime = clock.restart();
			
		//敌人生成时间
		isEnemy -= dataTime.asSeconds();
			
		//子弹生成时间
		isBullet -= dataTime.asSeconds();

		//显示分数
		ScoreString = "Score : " + std::to_string(score);
		scoreText.setString(ScoreString);
		window.draw(scoreText);

		//显示时间
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

		//显示血量
		rectangle.setSize(sf::Vector2f(role.getHealth() * 2.f ? role.getHealth() * 2.f : 200, 20));
		window.draw(Hptext);
		window.draw(rectangle);

		if (role.isDestroyed() == false) {

			//判断角色是否与子弹和敌人相撞
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
			
		
			
			//生成敌人
			if (isEnemy < 0) {
				int numEnemy = dis(gen);
				for (int i = 0; i < numEnemy; i++) {
					enemys.emplace_back(std::make_unique<Enemy>(textenemy, dix(gen), 0.f, 100.f, 20));
				}
				isEnemy = 2.f;
			}
			//每个敌人生成子弹
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

			//清理越界敌人
			enemys.erase(std::remove_if(enemys.begin(), enemys.end(), [](const std::unique_ptr<Enemy>& enemy) {
				return enemy->getPosition().y > 1080 || enemy->isDestroyed();
				}), enemys.end());

			//清理子弹
			enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(), [](const std::unique_ptr<Bullet>& bullet) {
				return bullet->getPosition().x < 0 || bullet->getPosition().y < 0 || bullet->getPosition().x > 800 || bullet->getPosition().y > 1080 || bullet->isDestroyed();
				}), enemyBullets.end());

			//清理玩家子弹
			playBullets.erase(std::remove_if(playBullets.begin(), playBullets.end(), [](const std::unique_ptr<Bullet>& bullet) {
				return bullet->getPosition().y < 0 || bullet->isDestroyed();
				}), playBullets.end());

			//更新敌人位置
			for (auto& tmp : enemys) {
				tmp->update(window, dataTime.asSeconds());
			}
			
			//打印敌人
			for (auto& tmp : enemys) {
				tmp->draw(window);
			}

			//更新子弹
			for (auto& tmp : enemyBullets) {
				tmp->update(window, dataTime.asSeconds());
			}
			for (auto& tmp : playBullets) {
				tmp->update(window, dataTime.asSeconds());
			}

			//打印子弹
			for (auto& tmp : enemyBullets) {
				tmp->draw(window);
			}	
			for (auto& tmp : playBullets) {
				tmp->draw(window);
			}
		}
		else {
			//显示游戏结束！！！
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

		//更新角色
		role.update(window, dataTime.asSeconds());
		role.draw(window);
		window.display();
	}

	return 0;
}