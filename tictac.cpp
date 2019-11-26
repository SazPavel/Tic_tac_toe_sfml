#include <SFML/Window.hpp> 
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "windows.h"

#define TILESIZE 64
using namespace std;
class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tileSize, 
			  const int* tiles, unsigned int width, unsigned int height)
    {
        // texture loading
        if (!m_tileset.loadFromFile(tileset))
            return false;

        // change the size of the array of vertices so that it corresponds to the size of the level
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // fill an array of vertices, with one square per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = tiles[i + j * width];

                // find the right position in the tile texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the square of the current tile
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // determine four corners.
                quad[0].position = sf::Vector2f(i * tileSize.x,
												j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x,
												j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x,
												(j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x,
												(j + 1) * tileSize.y);

                // determine four texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x,
												 tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x,
												 tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x,
												 (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x,
												 (tv + 1) * tileSize.y);
            }

        return true;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        states.texture = &m_tileset;
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};



void location(sf::Sprite &item, int y)
{	
	if(y >= 0)
		item.setPosition((y%3) * TILESIZE, (y/3) *TILESIZE);
}

//Victory check
int winning(int arr[9]){
	if(((arr[0] == arr[1]) && (arr[1] == arr[2]) && (arr[0] != 0)) ||
		((arr[3] == arr[4])	&& (arr[4] == arr[5]) && (arr[3] != 0)) ||
		((arr[6] == arr[7]) && (arr[7] == arr[8]) && (arr[6] != 0)) ||
		((arr[0] == arr[3]) && (arr[3] == arr[6]) && (arr[0] != 0)) ||
		((arr[1] == arr[4]) && (arr[4] == arr[7]) && (arr[1] != 0)) ||
		((arr[2] == arr[5]) && (arr[5] == arr[8]) && (arr[2] != 0)) ||
		((arr[0] == arr[4]) && (arr[4] == arr[8]) && (arr[0] != 0)) ||
		((arr[2] == arr[4]) && (arr[4] == arr[6]) && (arr[2] != 0)))
			return 1;
	else
		return 0;				
}

int minimax(int arr[9], int who, int whoam)
{	
	if(winning(arr))
	{
		if(who%2 == 0)
			return 10;
		else
			return -10;
	}
	
	int arr3[9], arr2[9] = {0};
	for(int i = 0; i < 9; i++)
	{
		arr3[i] = arr[i];
	}
	
	for(int i = 0; i < 9; i++)
	{		
		if(arr3[i] == 0)
		{
			if(who%2 == 0)
				arr3[i] = whoam%2 + 1;
			else
				arr3[i] = whoam;
			arr2[i] += minimax(arr3, who+1, whoam);
			arr3[i] = 0;
		}
	}
	int k=-1;
	for(int i = 0; i < 9; i++)
	{
		if(arr[i] == 0)
		{
			k = i;
			break;
		}
	}
	if(k != -1)
	{
		if(who%2 == 1)
		{
			for(int i = 0; i < 9; i++)
			{
				if(arr2[k] < arr2[i] && arr[i] == 0)
					k = i;
			}
		}else{
			for(int i = 0; i < 9; i++)
			{
				if(arr2[k] > arr2[i] && arr[i] == 0)
					k = i;
			}
		}
	}
	
	return arr2[k];
}


int AI(int arr[9], int whoam, int diff)
{
	int arr2[9] = {0}, arr3[9];
	for(int i = 0; i < 9; i++)
		arr3[i] = arr[i];
	for(int i = 0; i < 9; i++)
	{
		if(arr3[i] == 0)
		{
			arr3[i] = whoam;
			if(winning(arr3))
				return i;
			arr2[i] = minimax(arr3, 0, whoam);
			arr3[i] = 0;
		}
	}
	int k = 0;
	for(int i = 0; i < 9; i++){
		if(arr[i] == 0)
		{
			k = i;
			break;
		}
	}
	for(int i = 0; i < 9; i++)
	{
		if(arr2[k] < arr2[i] && arr[i] == 0)
			k = i;
		int r = rand()%2;
		if(arr2[k] == arr2[i]  && arr[i] == 0 && r == 1)
			k = i;
	}
	int complication = rand()%100;	
	int rn = rand()%9;
	if(arr[rn] == 0 && complication < diff)
		k = rn;	
	return k;
}


int main()
{
	int X[5];
	sf::Texture x_texture;
	sf::Sprite x_sprite[5];
	if (!x_texture.loadFromFile("resources/circles.png",
							 sf::IntRect(0, 0, TILESIZE, TILESIZE)))
    	return -1;
	for(int i = 0; i< 5; i++)	
		x_sprite[i].setTexture(x_texture);
	
	int O[5];
	sf::Texture o_texture;
	sf::Sprite o_sprite[5];
	if (!o_texture.loadFromFile("resources/circles.png",
							   sf::IntRect(TILESIZE, 0,
							   			   TILESIZE, 2 * TILESIZE)))
    	return -1;	
	for(int i = 0; i< 5; i++)	
		o_sprite[i].setTexture(o_texture);
	
	int game = 1;
    // create a window
    sf::RenderWindow window(sf::VideoMode(576, 576), "Tic-tac-toe");
   
	sf::Font font;
	if (!font.loadFromFile("resources/sansation.ttf"))
        return -1;	

    // create a tilemap
    TileMap map;
    int level[9] = {0, 1, 0, 1, 0, 1, 0, 1, 0};
    if (!map.load("resources/tileset1.png", sf::Vector2u(TILESIZE, TILESIZE),
				  level, 3, 3))
        return -1;
	int diff;
	cout << "Chose difficulty (0 - 50)\n";
	cin >> diff;
	if(diff > 50 || diff < 0)
		diff = 0;
	sf::Text text;
	text.setFont(font);
	text.setString("New Game");
	text.setCharacterSize(32);
	text.setFillColor(sf::Color::White);	
	text.setPosition(408, 384);
	
	sf::Text text2;
	text2.setFont(font);
	text2.setString("Draw");
	text2.setCharacterSize(32);
	text2.setFillColor(sf::Color::White);	
	text2.setPosition(308, 304);
	    
    srand(time(NULL));
    int arr[9] = {0};
	window.setKeyRepeatEnabled(false);
	int i = 0, j = 0, k = 0, fin = 0;
	
			window.clear();  
			window.draw(map); 
			window.draw(text);
	int wh = rand()%2;	
    // run the main loop
    while (window.isOpen())
	{
        // event handling
        sf::Event event;
        while (window.pollEvent(event))
		{
        	if(event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
			{
           	 	if(event.mouseButton.x >= 408 &&
				   event.mouseButton.y <= 428 && 
				   event.mouseButton.y >= 384)
				{  
	           	 	game = 1;
	           	 	j = 0;
	           	 	i = 0;
	           	 	k = 0;
	           	 	wh = (wh + 1) % 2;
					fin = 0;
	           	 	for(int i = 0; i < 9; i++)
	           	 		arr[i] = 0;
           		}
           	}
			if(game == 1 && wh == 0)
			{
           		if (event.type == sf::Event::MouseButtonPressed)
				   {           	 			
           	 		if(event.mouseButton.x <= 190 &&
					   event.mouseButton.y <= 190)
					{
           	 			if(i == 0)
						{
							X[j] = event.mouseButton.y / TILESIZE * 3 +
									   event.mouseButton.x / TILESIZE;
							if(arr[X[j]] == 0)
							{							
								location(x_sprite[j], X[j]);
								arr[X[j]] = 1;
								i = 1;
           	 					j += 1;
							}
						}
						if(winning(arr))
						{
							game = 0;
							fin = 1;
							text2.setString("Player WIN");
							break;	
						} 
						if(j > 4)
						{
							game = 0;
							fin = 1;
							text2.setString("Draw");
						}
						if(k < 4 && i == 1)
						{
							O[k] = AI(arr, 2, diff);
							location(o_sprite[k], O[k]);
							arr[O[k]] = 2;
							k += 1;
							i = 0;
						}		
						if(winning(arr))
						{
							game = 0;
							fin = 1;
							text2.setString("Computer WIN");
						}
					}
					
       				//	std::cout << "mouse x: " << event.mouseButton.x << std::endl;
        			//	std::cout << "mouse y: " << event.mouseButton.y << std::endl;
				}
			}
			
			if(game == 1 && wh == 1)
			{
					if(i == 0)
					{		
						O[k] = AI(arr, 2, diff);
						location(o_sprite[k], O[k]);
						arr[O[k]] = 2;
						k += 1;
						i = 1;
					}		
				           	 				
					if(winning(arr))
					{
						game = 0;
						fin = 1;
						text2.setString("Computer WIN");
						break;	
					} 
					
					if(k > 4)
					{
						game = 0;
						fin = 1;
						text2.setString("Draw");
					}
					
           			if (event.type == sf::Event::MouseButtonPressed)
					   {           	 			
           				if(event.mouseButton.x <= 190 &&
						   event.mouseButton.y <= 190)
						   {
           					if(k < 5 && i == 1)
							   {
								X[j] = event.mouseButton.y / TILESIZE * 3 +
									   event.mouseButton.x / TILESIZE;
								if(arr[X[j]] == 0){							
									location(x_sprite[j], X[j]);
									arr[X[j]] = 1;
									i = 0;
									j ++;
									if(winning(arr))
									{
										game = 0;
										fin = 1;
										text2.setString("Player WIN");
									}
								}
							}		
						}
       				//	std::cout << "mouse x: " << event.mouseButton.x << std::endl;
        			//	std::cout << "mouse y: " << event.mouseButton.y << std::endl;
					}
					
				
			}
			
			window.clear();  
			window.draw(map); 
			for(int i = 0; i < j; i++)
				window.draw(x_sprite[i]);
			
			for(int i = 0; i < k; i++)
				window.draw(o_sprite[i]);
			window.draw(text);
			if(fin == 1)
				window.draw(text2);				
        	window.display();
    	} 
    	Sleep(1);
    	
	}
    return 0;
}
