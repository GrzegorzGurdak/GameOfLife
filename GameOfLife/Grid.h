#pragma once

#include <iostream>
#include <cstring>
#include <algorithm>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

template<unsigned int gridx, unsigned int gridy>
class Grid : public sf::Drawable, public sf::Transformable
{
public:
	Grid(sf::Texture &tgrid) : grid_tex{ tgrid } //, int gridx, int gridy
	{
		tsize = grid_tex.getSize().y;
		//unsigned int vwidth = window.getSize().x / tsize + 1, vheight = window.getSize().y / tsize + 2;
		
		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(static_cast<size_t>(gridx) * gridy * 4);

		for (unsigned int i = 0; i < gridx; ++i)
			for (unsigned int j = 0; j < gridy; ++j)
			{
				sf::Vertex* quad = &m_vertices[(i + j * gridx) * 4];

				quad[0].position = sf::Vector2f(i * tsize, j * tsize);
				quad[1].position = sf::Vector2f((i + 1) * tsize, j * tsize);
				quad[2].position = sf::Vector2f((i + 1) * tsize, (j + 1) * tsize);
				quad[3].position = sf::Vector2f(i * tsize, (j + 1) * tsize);

				quad[0].texCoords = sf::Vector2f(0, 0);
				quad[1].texCoords = sf::Vector2f(tsize, 0);
				quad[2].texCoords = sf::Vector2f(tsize, tsize);
				quad[3].texCoords = sf::Vector2f(0, tsize);
			}
	}

	int click(int x, int y) {
		int click_x = (x - getPosition().x);
		int click_y = (y - getPosition().y);
		if (click_x < 0 || click_y < 0) return alive_am;
		click_x /= tsize * scale;
		click_y /= tsize * scale;

		std::cout << click_x << " -- " << click_y << std::endl;
		if (click_x >= 0 && click_x < gridx && click_y >= 0 && click_y < gridy) {
			map[click_x][click_y] = !map[click_x][click_y];
			if (map[click_x][click_y]) alive_am++;
			else alive_am--;
		}

		//updateGrid();
		return alive_am;
	}

	int step_up() {
		alive_am = 0;
		bool nmap[gridx][gridy];
		for (unsigned int j = 0; j < gridy; ++j) {
			for (unsigned int i = 0; i < gridx; ++i) {
				int neighbours = 0;
				if (i > 0 && j > 0 && map[i - 1][j - 1]) neighbours++;
				if (i < gridx - 1 && j > 0 && map[i + 1][j - 1]) neighbours++;
				if (i > 0 && j < gridy - 1 && map[i - 1][j + 1]) neighbours++;
				if (i < gridx - 1 && j < gridy - 1 && map[i + 1][j + 1]) neighbours++;
				if (i > 0 && map[i - 1][j]) neighbours++;
				if (i < gridx - 1 && map[i + 1][j]) neighbours++;
				if (j > 0 && map[i][j - 1]) neighbours++;
				if (j < gridy - 1 && map[i][j + 1]) neighbours++;

				//std::cout << neighbours << "\t";
				//if (neighbours < 2) nmap[i][j] = false;
				if (neighbours == 2 && map[i][j]) { nmap[i][j] = true; alive_am++; }
				else if (neighbours == 3) { nmap[i][j] = true; alive_am++; }
				else nmap[i][j] = false;
			}
			//std::cout << std::endl;
		}
		memcpy(map, nmap, sizeof(bool) * gridx * gridy);
		return alive_am;
	}

	int reset() {
		for (int i{}; i < gridx; i++) 
			std::fill(std::begin(map[i]), std::end(map[i]), 0);
		return 0;
	}

	void updateGrid()
	{
		for (unsigned int i = 0; i < gridx; ++i)
			for (unsigned int j = 0; j < gridy; ++j)
			{
				sf::Vertex* quad = &m_vertices[(i + (size_t)j * gridx) * 4];

				quad[0].position = sf::Vector2f(i * tsize, j * tsize);
				quad[1].position = sf::Vector2f((i + 1) * tsize, j * tsize);
				quad[2].position = sf::Vector2f((i + 1) * tsize, (j + 1) * tsize);
				quad[3].position = sf::Vector2f(i * tsize, (j + 1) * tsize);

				quad[0].texCoords = sf::Vector2f(0 + tsize * map[i][j], 0);
				quad[1].texCoords = sf::Vector2f(tsize + tsize * map[i][j], 0);
				quad[2].texCoords = sf::Vector2f(tsize + tsize * map[i][j], tsize);
				quad[3].texCoords = sf::Vector2f(0 + tsize * map[i][j], tsize);
			}
	}

	void gridMove(sf::Vector2f position)
	{
		setPosition(position);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// apply the tileset texture
		states.texture = &grid_tex;

		// draw the vertex array
		target.draw(m_vertices, states);
	}

	void rescale(const float factors) {
		Transformable::setScale(factors, factors);
		scale = factors;
		//tsize = grid_tex.getSize().y * factors;
	}

private:
	bool map[gridx][gridy] = {};
	sf::Texture &grid_tex;
	sf::VertexArray m_vertices;
	unsigned int tsize;
	float scale = 1;
	int alive_am{};
};

