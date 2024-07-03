#include <SFML/Graphics.hpp>
#include <vector>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800

class Player {
private:
	float x_axis, y_axis;
	sf::Vector2f size;
	int y_dir;

public:
	sf::IntRect paddleBox;
	sf::RectangleShape paddle;
	Player() {
		size = { 20, 120 };
		x_axis = SCREEN_WIDTH - size.x * 2;
		y_axis = SCREEN_HEIGHT / 2 - size.y / 2;
		y_dir = 12;

		paddle.setSize(size);
		paddle.setPosition(x_axis, y_axis);
		paddle.setFillColor(sf::Color::White);
	}

	void Move(sf::RenderWindow& window,float dt) {
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Up) {
					if (y_axis > 0) {
						y_axis -= y_dir * dt;
					}
				}
				else if (event.key.code == sf::Keyboard::Down) {
					if (y_axis + size.y < SCREEN_HEIGHT) {
						y_axis += y_dir * dt;
					}
				}
			}
		}

		paddle.setPosition(x_axis, y_axis);
	}

	float getX_axis() const {
		return x_axis;
	}
	float getY_axis() const {
		return y_axis;
	}

	void setX_axis(float newValue) {
		x_axis = newValue;
	}

	void setY_axis(float newValue) {
		y_axis = newValue;
	}

	void draw(sf::RenderWindow& window) {
		window.draw(paddle);
	}
};

class AIPlayer {
private:
	float x_axis, y_axis;
	sf::Vector2f size;
	int y_dir;

public:
	sf::IntRect paddleBox;
	sf::RectangleShape paddle;
	AIPlayer() {
		size = { 20, 120 };
		x_axis = size.x * 2;
		y_axis = SCREEN_HEIGHT / 2 - size.y / 2;
		y_dir = 12;

		paddle.setSize(size);
		paddle.setPosition(x_axis, y_axis);
		paddle.setFillColor(sf::Color::White);
	}

	void Move(sf::CircleShape circle, float dt) {
		if (paddle.getPosition().y + 120 / 2 > circle.getPosition().y) {
			if (y_axis > 0) {
				y_axis -= y_dir * dt;
			}
		}
		if (paddle.getPosition().y + 120 / 2 <= circle.getPosition().y) {
			if (y_axis + size.y < SCREEN_HEIGHT) {
				y_axis += y_dir * dt;
			}
		}
		paddle.setPosition(x_axis, y_axis);
	}

	void draw(sf::RenderWindow& window) {
		window.draw(paddle);
	}
};


class Ball {
private:
	friend class Player;
	friend class AIPlayer;
	int radius;
	int x_axis, y_axis, dir_x, dir_y;
	int leftPointTrack;
	int rightPointTrack;

	

public:
	sf::CircleShape circle;

	Ball() {
		radius = 20;
		x_axis = SCREEN_WIDTH / 2;
		y_axis = SCREEN_HEIGHT / 2;
		dir_x = 7;
		dir_y = 7;

		leftPointTrack = 0;
		rightPointTrack = 0;

		//initialize circle dimensions
		circle.setRadius(radius);
		circle.setPosition(x_axis, y_axis);
		circle.setFillColor(sf::Color::White);
		Player paddler;
	}

	void reset() {
		x_axis = SCREEN_WIDTH/2;
		y_axis = SCREEN_HEIGHT/2;
	}

	void Update(Player& paddle, AIPlayer& paddle2) {
		if (x_axis - radius < 0 || detectCollision(paddle) || detectCollision(paddle2)) {
			dir_x *= -1;
		}

		if (y_axis < 0 || y_axis + radius > SCREEN_HEIGHT) {
			dir_y *= -1;
		}

		x_axis += dir_x;
		y_axis += dir_y;

		//updates scoring depending on where ball lands
		if (x_axis + radius > SCREEN_WIDTH) {
			reset();
			++leftPointTrack;
		}
		if (x_axis - radius < 0) {
			reset();
			++rightPointTrack;
		}
		
		circle.setPosition(x_axis, y_axis);
	}

	void draw(sf::RenderWindow& window) {
		window.draw(circle);
	}

	bool detectCollision(Player& paddle1) {
		//to be implemented
		sf::IntRect TempRectangle(paddle1.paddle.getPosition().x, paddle1.paddle.getPosition().y,
			paddle1.paddle.getGlobalBounds().width, paddle1.paddle.getGlobalBounds().height);

		sf::IntRect BallHitBox(circle.getPosition().x, circle.getPosition().y,
			circle.getGlobalBounds().width, circle.getGlobalBounds().height);

		if (TempRectangle.contains(BallHitBox.getPosition()) || TempRectangle.intersects(BallHitBox)) {
			circle.setFillColor(sf::Color::Green);
			return true;
		}
		else
			return false;
		
	}

	bool detectCollision(AIPlayer& paddle1) {
		//to be implemented
		sf::IntRect TempRectangle(paddle1.paddle.getPosition().x, paddle1.paddle.getPosition().y,
			paddle1.paddle.getGlobalBounds().width, paddle1.paddle.getGlobalBounds().height);

		sf::IntRect BallHitBox(circle.getPosition().x, circle.getPosition().y,
			circle.getGlobalBounds().width, circle.getGlobalBounds().height);

		if (TempRectangle.contains(BallHitBox.getPosition()) || TempRectangle.intersects(BallHitBox)) {
			circle.setFillColor(sf::Color::Red);
			return true;
		}
		else
			return false;

	}

};

void drawBisectingLine(sf::RenderWindow& window) {
	sf::RectangleShape line;
	line.setSize(sf::Vector2f(1, SCREEN_HEIGHT));
	line.setPosition(SCREEN_WIDTH / 2, 0);
	line.setFillColor(sf::Color::White);
	window.draw(line);
}

void backgroundColors(sf::RenderWindow& window) {
	sf::RectangleShape leftside, rightside;
	leftside.setPosition(0, 0), rightside.setPosition(SCREEN_WIDTH / 2, 0);
	sf::Color leftSideColor(100, 150, 200); // Darker pastel blue for the left side
	sf::Color rightSideColor(180, 210, 240); // Lighter pastel blue for the right side

	sf::Vector2f size(SCREEN_WIDTH / 2, SCREEN_HEIGHT);
	leftside.setSize(size), rightside.setSize(size);
	leftside.setFillColor(leftSideColor), rightside.setFillColor(rightSideColor);

	window.draw(leftside);
	window.draw(rightside);
}

void drawBisectingCircle(sf::RenderWindow& window) {
	sf::CircleShape circle;
	circle.setRadius(120);
	circle.setOutlineThickness(1);
	circle.setFillColor(sf::Color{ 255,255,255,0 });
	circle.setOutlineColor(sf::Color::White);

	sf::Vector2f dimensions{ 120, 120};
	circle.setOrigin(dimensions);
	circle.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	window.draw(circle);
}



int main() {
	Ball bouncer;
	Player paddle;
	AIPlayer paddle2;
	sf::RectangleShape line;
	line.setSize(sf::Vector2f(1, SCREEN_HEIGHT));
	line.setPosition(SCREEN_WIDTH << 1, 0);
	line.setFillColor(sf::Color::White);
    sf::Clock _clock;
    float previous_time = _clock.getElapsedTime().asSeconds();

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Pong Baby");

	window.setFramerateLimit(60);
	while (window.isOpen()) {
        float currentTime = _clock.getElapsedTime().asSeconds();
        float deltaTime = currentTime - previous_time;

		// In your game loop, after clearing the window and before displaying
		

		window.clear(sf::Color::Black);
		backgroundColors(window);
		drawBisectingLine(window);
		drawBisectingCircle(window);
		window.draw(line);
		paddle.Move(window, deltaTime);
		paddle2.Move(bouncer.circle, deltaTime);
		bouncer.Update(paddle, paddle2);
		window.draw(line);
		paddle.draw(window);
		paddle2.draw(window);
		bouncer.draw(window);
		
				
		window.display();
		
	}
}