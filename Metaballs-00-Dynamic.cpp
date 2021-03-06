#include <iostream>
#include <vector>
#include <random>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#define METABALL_RADIUS 25

#define METABALL_SPEED 200.f
#define DT			   0.01f
#define METABALL_POS_INC METABALL_SPEED * DT

// If you change this, be sure to modify the fragment shader too
#define MAX_METABALLS 100

#define SCREEN_WIDTH  600
#define SCREEN_HEIGHT 600

#define SCREEN_WIDTH_BOUNDARY SCREEN_WIDTH
#define SCREEN_HEIGHT_BOUNDARY SCREEN_HEIGHT

#define PI 3.14159265358

void update(sf::RenderWindow& renderWindow);
void draw(sf::RenderWindow& renderWindow, sf::RenderTexture& renderTexture);
void clearMetaballVector();
sf::Vector2f randomDirection();

struct metaball {
	metaball(sf::Vector2f position, sf::Vector2f direction, float radius) : position{ position }, radius{ radius }, direction{ direction }
	{}

	sf::Vector2f position;
	sf::Vector2f direction;
	float radius;
};

std::vector<metaball*> metaballs;

float metaballPositions[2 * MAX_METABALLS] = { 0 };

sf::Shader metaballShader;

sf::RenderStates states;

std::mt19937 rng(std::random_device{}());
std::uniform_real_distribution<> dist(0, 2.f * PI);

bool prevMouseRBState = false;
bool updateImage = false;

int main()
{
	sf::RenderWindow renderWindow;
	sf::RenderTexture renderTexture;

	renderWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Metaballs - Dynamic Example", sf::Style::Titlebar | sf::Style::Close);
	renderWindow.setFramerateLimit(60);

	renderTexture.create(SCREEN_WIDTH, SCREEN_HEIGHT);

	metaballShader.loadFromFile("metaballs.vert","metaballs.frag");
	metaballShader.setUniform("screenHeight", (float)SCREEN_HEIGHT);
	metaballShader.setUniform("metaballRadiusSquared", (float)(METABALL_RADIUS*METABALL_RADIUS));

	while (renderWindow.isOpen())
	{
		sf::Event e;

		while (renderWindow.pollEvent(e))
		{
			if (e.type == sf::Event::Closed || e.key.code == sf::Keyboard::Escape)
				renderWindow.close();
		}

		update(renderWindow);

		draw(renderWindow, renderTexture);

		renderWindow.display();
	}

	clearMetaballVector();

	return 0;
}

void update(sf::RenderWindow& renderWindow)
{
	/// Inputs
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !prevMouseRBState)
	{
		sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(renderWindow));

		if (!(mousePosition.x > SCREEN_WIDTH || mousePosition.x < 0 || mousePosition.y > SCREEN_HEIGHT || mousePosition.y < 0) && metaballs.size() < MAX_METABALLS)
		{
			metaballs.push_back(new metaball(mousePosition, randomDirection(), METABALL_RADIUS));
			metaballShader.setUniform("numMetaballs", (int)metaballs.size());
			updateImage = true;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
	{
		clearMetaballVector();

		metaballShader.setUniform("numMetaballs", (int)metaballs.size());

		updateImage = true;
	}

	prevMouseRBState = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	// Metaball position updates

	for (int i = 0; i < metaballs.size(); i++)
	{
		metaballs[i]->position += metaballs[i]->direction * METABALL_POS_INC;
		
		// This is redundant, optimize if time comes up to come back
		metaballPositions[2 * i] = metaballs[i]->position.x;
		metaballPositions[2 * i + 1] = metaballs[i]->position.y;

		if (metaballs[i]->position.x >= SCREEN_WIDTH_BOUNDARY || metaballs[i]->position.x <= 0)
			metaballs[i]->direction.x *= -1.f;
		if (metaballs[i]->position.y >= SCREEN_HEIGHT_BOUNDARY || metaballs[i]->position.y <= 0)
			metaballs[i]->direction.y *= -1.f;
	}
}

void draw(sf::RenderWindow& renderWindow, sf::RenderTexture& renderTexture)
{
	renderTexture.clear();

	renderTexture.display();

	metaballShader.setUniformArray("metaballArray", metaballPositions, MAX_METABALLS);

	states.shader = &metaballShader;

	sf::Sprite bufferFinal(renderTexture.getTexture());

	renderWindow.draw(bufferFinal, states);
}

sf::Vector2f randomDirection()
{
	float rAngle = dist(rng);
	return sf::Vector2f(std::cosf(rAngle), std::sinf(rAngle));
}

void clearMetaballVector()
{
	for (int i = 0; i < metaballs.size(); i++)
		delete metaballs[i];
	metaballs.clear();
}