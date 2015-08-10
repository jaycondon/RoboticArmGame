#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>


using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");

    //    Setup for the Time showing to the screen
    sf::Clock clock;
    sf::Time clockTicking;
    int sec;
    sf::Font font;
    if(!font.loadFromFile("UbuntuMono-B.ttf"))
    {
        cout << "problem loading font";
    }
    sf::Text text;
    text.setFont(font);
    text.setPosition(25,25);
    text.setCharacterSize(34);
    text.setColor(sf::Color::White);

    // Set up the variables that allow the robot arm to move
    float bodyMove = 0;
    float armRotate = 0;
    float lowerArmRotate = 0;
    float upFingerRotate = 0;
    float lowFingerRotate = 0;

    // Setsup the textures for each body part
    sf::Texture bodyText;
    bodyText.loadFromFile("base.png");
    sf::Texture upArmText;
    upArmText.loadFromFile("upperArm.png");
    sf::Texture lowArmText;
    lowArmText.loadFromFile("lowerArm.png");
    sf::Texture upFingText;
    upFingText.loadFromFile("upperfinger.png");
    sf::Texture lowFingText;
    lowFingText.loadFromFile("lowerfinger.png");

    // Adds the textures to the sprites
    sf::Sprite body;
    body.setTexture(bodyText);
    sf::Sprite upArm;
    upArm.setTexture(upArmText);
    sf::Sprite lowArm;
    lowArm.setTexture(lowArmText);
    sf::Sprite upFinger;
    upFinger.setTexture(upFingText);
    sf::Sprite lowFinger;
    lowFinger.setTexture(lowFingText);

    // Sets the origins for each point of rotation
    upArm.setOrigin(63,154);
    lowArm.setOrigin(35,70);
    upFinger.setOrigin(0,upFinger.getGlobalBounds().top+upFinger.getGlobalBounds().height);
    lowFinger.setOrigin(0,0);

    // Set the total number of balloons
    const int noOfBalloons = 5;
    int balloonsBurst = 0;
    srand(time(NULL));
    sf::CircleShape ballonArray[noOfBalloons];
    for(int i = 0; i < noOfBalloons;i++)
    {
        sf::CircleShape circle(20);
        circle.setFillColor(sf::Color(100, 250, 50));
        circle.setPosition(rand() % 700 + 20,rand() %  500 + 20);
        ballonArray[i] = circle;
    }

    //  Setup the two pincer points for the collision
    sf::Vector2f lowPincer(lowFinger.getGlobalBounds().left+lowFinger.getGlobalBounds().width-10,lowFinger.getGlobalBounds().top+lowFinger.getGlobalBounds().height-25);
    sf::Vector2f upPincer(upFinger.getGlobalBounds().left-10+upFinger.getGlobalBounds().width,upFinger.getGlobalBounds().top+15);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                bodyMove -= 5;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                bodyMove += 5;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                if(armRotate >-50)
                {
                    armRotate -= 1;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                if(armRotate < 35)
                {
                    armRotate += 1;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                if(lowerArmRotate > -30)
                {
                    lowerArmRotate -= 1;
                }

            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                if(lowerArmRotate < 50)
                {
                    lowerArmRotate += 1;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                if(upFingerRotate !=22 && lowFingerRotate != -22)
                {
                    upFingerRotate += 1;
                    lowFingerRotate -= 1;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                if(upFingerRotate !=0 && lowFingerRotate != 0)
                {
                    upFingerRotate -= 1;
                    lowFingerRotate += 1;
                }
            }
        }

        // Setup translations
        sf::Vector2f moveBody(100,300);
        sf::Transform moveBodyTransform;

        sf::Vector2f moveUAtoBody((body.getGlobalBounds().left+body.getGlobalBounds().width/2)-18,body.getGlobalBounds().top+32);
        sf::Transform upArmTransform;
        upArmTransform.translate(moveUAtoBody);

        sf::Vector2f moveLAtoUA(upArm.getGlobalBounds().left+upArm.getGlobalBounds().width-20,upArm.getGlobalBounds().top);
        sf::Transform lowArmTransform;
        lowArmTransform.translate(moveLAtoUA);

        sf::Vector2f moveupFinger(lowArm.getGlobalBounds().left+lowArm.getGlobalBounds().width-30,lowArm.getGlobalBounds().top+30);
        sf::Transform upFingerTransform;
        upFingerTransform.translate(moveupFinger);

        sf::Vector2f movelowFinger(lowArm.getGlobalBounds().left+lowArm.getGlobalBounds().width-25,lowArm.getGlobalBounds().top+45);
        sf::Transform lowFingerTransform;
        lowFingerTransform.translate(movelowFinger);

        // Setup rotations
        sf::Transform rotateArm;
        rotateArm.rotate(armRotate,0,0);

        sf::Transform rotateLowerArm;
        rotateLowerArm.rotate(lowerArmRotate,0,0);

        sf::Transform rotateUpFinger;
        rotateUpFinger.rotate(upFingerRotate,0,0);

        sf::Transform rotateLowFinger;
        rotateLowFinger.rotate(lowFingerRotate,0,0);

        moveBody.x += bodyMove;

        // Add the translations and rotations
        sf::Transform MBody = moveBodyTransform.translate(moveBody);

        sf::Transform MUpArm = MBody * upArmTransform * rotateArm;

        sf::Transform MLowArm = MUpArm * lowArmTransform * rotateLowerArm;

        sf::Transform MUpFing = MLowArm * upFingerTransform * rotateUpFinger;

        sf::Transform MLowFing = MLowArm * lowFingerTransform * rotateLowFinger;

        sf::Vector2f bottomp = MLowFing.transformPoint(lowPincer);
        sf::Vector2f upp = MUpFing.transformPoint(upPincer);

        window.clear();

        // Set these two circles ip to debug the points of the pincers
        // but decided to leave them there.
        sf::CircleShape circle(5);
        circle.setFillColor(sf::Color::Red);
        circle.setPosition(upp.x,upp.y);

        sf::CircleShape lowcircle(5);
        lowcircle.setFillColor(sf::Color::Blue);
        lowcircle.setPosition(bottomp.x,bottomp.y);

        window.draw(lowcircle);
        window.draw(circle);
        window.draw(body,MBody);
        window.draw(upArm,MUpArm);
        window.draw(lowArm,MLowArm);
        window.draw(upFinger,MUpFing);
        window.draw(lowFinger,MLowFing);

        for(int i = 0; i < noOfBalloons;i++)
        {
            if(ballonArray[i].getGlobalBounds().contains(bottomp) && ballonArray[i].getGlobalBounds().contains(upp))
            {
                balloonsBurst++;
                ballonArray[i].setPosition(2000,2000);// Instead of this line the circle should be removed from the array
            }
            else
            {
                window.draw(ballonArray[i]);
            }
        }

        if(balloonsBurst == noOfBalloons)
        {
            sec = clockTicking.asSeconds();
        }
        else
        {
            clockTicking = clock.getElapsedTime();
        }


        // Setup the time to print to screen
        sec = clockTicking.asSeconds();
        stringstream ss (stringstream::in | stringstream::out);
        ss << sec;
        string t = ss.str();
        text.setString(t);

        window.draw(text);
        window.display();
    }

    return 0;
}
