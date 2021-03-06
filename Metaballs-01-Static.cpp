#include <iostream>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#define SCREEN_WIDTH  600
#define SCREEN_HEIGHT 600

#define METABALL_RADIUS 10

#define METABALL_SPEED 100.f

#define METABALL_THRESHOLD 20

#define DISTANCE(x, y, x0, y0) std::sqrtf(std::powf(y0 - y, 2) + std::powf(x0 - x, 2))

void update(sf::RenderWindow& renderWindow);
void draw(sf::RenderWindow& renderWindow);

struct metaball {
	metaball(sf::Vector2f position, float radius) : position{ position }, radius{ radius }
	{}

	sf::Vector2f position;
	float radius;
};

std::vector<metaball*> metaballs;

sf::Texture texture;
sf::Sprite sprite;

sf::Uint8 pixels[SCREEN_WIDTH * SCREEN_HEIGHT * 4];

bool prevMouseRBState = false;
bool updateImage = false;

int main()
{
	sf::RenderWindow renderWindow;	
	
	renderWindow.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Metaballs - Static Example", sf::Style::Titlebar | sf::Style::Close);
	renderWindow.setFramerateLimit(60);

	texture.create(SCREEN_WIDTH, SCREEN_HEIGHT);

	while (renderWindow.isOpen())
	{
		sf::Event e;

		while (renderWindow.pollEvent(e))
		{
			if (e.type == sf::Event::Closed || e.key.code == sf::Keyboard::Escape)
				renderWindow.close();
		}

		update(renderWindow);

		draw(renderWindow);

		renderWindow.display();
	}

	for (int i = 0; i < metaballs.size(); i++)
		delete metaballs[i];
	
	metaballs.clear();

	return 1;
}

void update(sf::RenderWindow& renderWindow)
{
	/// Inputs

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !prevMouseRBState)
	{
		sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(renderWindow));
		mousePosition.x -= METABALL_RADIUS;
		mousePosition.y -= METABALL_RADIUS;

		if (!(mousePosition.x > SCREEN_WIDTH || mousePosition.x < 0 || mousePosition.y > SCREEN_HEIGHT || mousePosition.y < 0))
		{
			metaballs.push_back(new metaball(mousePosition, METABALL_RADIUS));
			updateImage = true;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
	{
		for (int i = 0; i < metaballs.size(); i++)
			delete metaballs[i];
		metaballs.clear();
		updateImage = true;
	}

	prevMouseRBState = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	/// Image to draw
	if (updateImage)
	{
		for (int i = 0; i < SCREEN_WIDTH; i++)
			for(int j = 0; j < SCREEN_HEIGHT; j++)
			{
				int index = (j * SCREEN_WIDTH + i) * 4;

				float d=0, sum=0;

				for (int m = 0; m < metaballs.size(); m++)
				{
					// In this version, we are using an inverse squared distance function
					// Artifacts of this method include growth in size of metaballs as more are added to the scene
					// Refer to the GLSL fragment shader code for another approach ( which is certainly better )

					float radiusSquared = metaballs.at(m)->radius * metaballs.at(m)->radius;

					d = DISTANCE(metaballs.at(m)->position.x, metaballs.at(m)->position.y, i, j);

					if (d > 2.f * radiusSquared)
						continue;

					sum += 100.f * (radiusSquared / ((d*d) + 0.001f));
				}

				if (sum >= 50.f)
					sum = 1.f;
				if (sum >= 30.f)
					sum = 0.5f;
				else
					sum = 0.f;

				pixels[index + 0] = sum * 255;
				pixels[index + 1] = sum * 255;
				pixels[index + 2] = sum * 255;
				pixels[index + 3] = 255;

			}

		texture.update(pixels);
		sprite.setTexture(texture);
		updateImage = false;
	}
}

void draw(sf::RenderWindow& renderWindow)
{
	renderWindow.draw(sprite);
}