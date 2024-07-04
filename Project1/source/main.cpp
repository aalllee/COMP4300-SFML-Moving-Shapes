#include "SFML/Graphics.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <memory>



class BaseShape {

protected:
    sf::Color color;
    sf::Vector2f speed;
    sf::Vector2f pos;
    
    sf::Text text;

public:
    BaseShape(sf::Color color, sf::Vector2f speed, sf::Vector2f pos, sf::Text text) : color(color), speed(speed),pos(pos),text(text) {}
    ~BaseShape() {};

    virtual void drawShape(sf::RenderWindow& window) = 0;
};


class Circle : public BaseShape {
   
    
public:
    sf::CircleShape shape;
    Circle(float radius, sf::Color color, sf::Vector2f speed, sf::Vector2f pos, sf::Text text) : BaseShape(color, speed,pos, text), shape(sf::CircleShape(radius)) {
        shape.setFillColor(color);

    };
    ~Circle() {};

    void drawShape(sf::RenderWindow& window) override {
        pos += speed;
        shape.setPosition(pos);
        window.draw(shape);

       

        text.setPosition(pos.x + shape.getRadius() - text.getLocalBounds().getSize().x / 2.0, pos.y + shape.getRadius() - text.getLocalBounds().getSize().y / 2.0);
        window.draw(text);

        if (pos.x < 0.0 || pos.x > (window.getSize().x - 2* shape.getRadius()))
            speed.x *= -1;

        if (pos.y < 0.0 || pos.y >(window.getSize().y - 2 * shape.getRadius()))
            speed.y *= -1;

        
    }

};

class RectangleShp : public BaseShape {
    
   
public:
    sf::RectangleShape shape;
    RectangleShp(sf::Vector2f size, sf::Color color, sf::Vector2f speed, sf::Vector2f pos, sf::Text text) : BaseShape(color, speed,pos,text), shape(sf::RectangleShape(size)){ shape.setFillColor(color); };
    ~RectangleShp() {};

    void drawShape(sf::RenderWindow& window) override {
        pos += speed;
        shape.setPosition(pos);
        window.draw(shape);

        text.setPosition(pos.x + shape.getSize().x / 2.0 - text.getLocalBounds().getSize().x / 2.0, pos.y + shape.getSize().y / 2.0 - text.getLocalBounds().getSize().y / 2.0);
        window.draw(text);

        if (pos.x < 0.0 || pos.x >(window.getSize().x -  shape.getSize().x))
            speed.x *= -1;

        if (pos.y < 0.0 || pos.y >(window.getSize().y - shape.getSize().y))
            speed.y *= -1;
    }
};



int main()
{
    std::string filePath = "data/config.txt";
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file" << filePath << std::endl;
        return 1;
    }

    std::string token;
    int windowX;
    int windowY;


    sf::Font font; std::string fontPath; int fontSize; sf::Vector3i fontColor;
    std::vector<std::shared_ptr<BaseShape>> shapes = {};
   
    while (file >> token) {

        if (token == "Window")
        {
            file >> windowX;
            file >> windowY;
        }

        if (token == "Font")
        {
            file >> fontPath >> fontSize >> fontColor.x >> fontColor.y >> fontColor.z;
            std::cout << fontPath << fontSize << fontColor.x << fontColor.y << fontColor.z << std::endl;

            
            if (!font.loadFromFile(fontPath)) {
                std::cerr << "Error loading font\n";
                return -1;
            }
        }

        if (token == "Circle")
        {
            std::string name;
            sf::Vector2f position, speed;
            sf::Color color;
            int R,G,B;
            int radius;
          
            file >> name >> position.x >> position.y >> speed.x >> speed.y >> R >> G >> B >> radius;
            
            sf::Color fColor = sf::Color(fontColor.x, fontColor.y, fontColor.z);
            sf::Text text;
            text.setFont(font);
            text.setString(name);
            text.setCharacterSize(fontSize);      
            text.setFillColor(fColor);
            text.setPosition(position.x, position.y); 

            std::shared_ptr<BaseShape> c1 = std::make_shared<Circle>(radius, sf::Color(R,G,B), speed, position,text);
            shapes.push_back(c1);
        }

        if (token == "Rectangle")
        {
           
            std::string name;
            sf::Vector2f position, speed;
            sf::Color color;
            int R, G, B;
            int w,h;

            file >> name >> position.x >> position.y >> speed.x >> speed.y >> R >> G >> B >> w >> h;
            
            sf::Color fColor = sf::Color(fontColor.x, fontColor.y, fontColor.z);
            sf::Text text;
            text.setFont(font);
            text.setString(name);
            text.setCharacterSize(fontSize);     
            text.setFillColor(fColor); 
            text.setPosition(position.x, position.y); 

            std::shared_ptr<BaseShape> c4 = std::make_shared<RectangleShp>(sf::Vector2f(w,h), sf::Color(R, G, B), speed, position,text);
            shapes.push_back(c4);
        }

    }



    sf::RenderWindow window(sf::VideoMode(windowX, windowY), "Window Title");
    window.setFramerateLimit(60);
   
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(18, 33, 43)); 

        for (auto& shape : shapes)
        {
            shape->drawShape(window);
        }
           
        window.display();
    }

    return 0;
}
