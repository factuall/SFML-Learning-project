#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <chrono>
#include <time.h>

#include "base/Object.h"
#include "base/base.h"

sf::Font fontRegular;
sf::Font fontShadow;

sf::RenderWindow window(sf::VideoMode(1024, 576), "tboifg - factuall", sf::Style::Close);
int lastTick, deltaTime; 
int lastTime = 0; double deltaTIme;
double PCFreq = 0.0;
__int64 CounterStart = 0;
Object fpsDisplay;

void StartTimer()
{
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        std::cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart) / 100000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;

}
int RelativeTime()
{

    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart) / PCFreq / 100.0;
}
int SystemTime()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart) / PCFreq / 100.0;
}
int DeltaTime()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    double now = (double(li.QuadPart) / PCFreq) / 100.0;
    if (lastTime == 0) lastTime = (int)now;
    deltaTime = (int)now - lastTime;
    lastTime = (int)now;
    return deltaTime;

}

void update() {

}


int skipTickTime = 0, secondCounter = 0, framesInSecond = 0, framesPerSecond = 0;
void render() {
    fpsDisplay.text.setString(std::to_string(framesPerSecond));

    window.clear();
    for (int displayedObj = 0; displayedObj < getObjLimit(); displayedObj++) {
        Object* currentObject = getObject(displayedObj);
        if (!(currentObject->isNull)) {
            switch (currentObject->type) {
                case GameObject: {
                    sf::RectangleShape objShape(sf::Vector2f(64, 64));
                    objShape.setPosition((currentObject->x), (currentObject->y));
                    objShape.setFillColor(sf::Color(155, 155, 155, 255));
                    window.draw(objShape);
                    break;
                }
                case TextObject: {
                    currentObject->text.setPosition(currentObject->x, currentObject->y);
                    window.draw(currentObject->text);
                }
            }

        }
    }
    window.display();
}

int main()
{
    if (!fontRegular.loadFromFile("../suture.ttf"))
    {
        // error...
        printf("error while loading font\n");
        return 0;
    }
    if (!fontShadow.loadFromFile("../future.ttf"))
    {
        // error...
        printf("error while loading font\n");
        return 0;
    }

    StartTimer();
    init();

    fpsDisplay = Object(0, 0);
    fpsDisplay.type = TextObject;
    sf::Text fpsText;
    fpsText.setFont(fontRegular);
    fpsText.setString(std::to_string(framesPerSecond));
    fpsText.setCharacterSize(32); // in pixels, not points!
    fpsText.setFillColor(sf::Color::White);
    fpsDisplay.text = fpsText;
    addObject(&fpsDisplay); 

    Object testObject = Object(32, 32);
    testObject.type = TextObject;
    sf::Text testText;
    testText.setFont(fontRegular);
    testText.setCharacterSize(32);
    testText.setFillColor(sf::Color::White);
    testText.setString("testing");
    testObject.text = testText;
    addObject(&testObject);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //limit update rate to 60
        if (DeltaTime() <= 16 && skipTickTime < 16) {
            skipTickTime += deltaTime;
        }
        secondCounter += deltaTime;
        if (secondCounter >= 1000) {
            framesPerSecond = framesInSecond;
            framesInSecond = 0;
            secondCounter = 0;
        }
        if (skipTickTime >= 16) {
            skipTickTime = 0;
            framesInSecond++;
            update();
        }

        render();
    }
    
    return 0;
}