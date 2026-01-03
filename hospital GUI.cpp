#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
using namespace std;

struct Patient {
    string id, name, age, disease;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(900, 600), "Hospital Management System");
    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("arial.ttf");

    // Theme colors
    sf::Color bgColor(220, 245, 230);
    sf::Color boxColor(0, 120, 80);
    sf::Color activeColor(0, 180, 120);

    // Input boxes
    sf::RectangleShape boxes[4];
    sf::Text labels[4], inputs[4];

    string inputData[4] = { "", "", "", "" };
    int activeBox = -1;

    string labelText[4] = { "Patient ID", "Name", "Age", "Disease" };

    for (int i = 0; i < 4; i++) {
        boxes[i].setSize(sf::Vector2f(300, 45));
        boxes[i].setPosition(300, 100 + i * 70);
        boxes[i].setFillColor(boxColor);

        labels[i].setFont(font);
        labels[i].setString(labelText[i]);
        labels[i].setCharacterSize(18);
        labels[i].setFillColor(sf::Color::Black);
        labels[i].setPosition(180, 110 + i * 70);

        inputs[i].setFont(font);
        inputs[i].setCharacterSize(18);
        inputs[i].setFillColor(sf::Color::White);
        inputs[i].setPosition(310, 110 + i * 70);
    }

    // Buttons
    sf::RectangleShape addBtn(sf::Vector2f(150, 50));
    addBtn.setPosition(300, 420);
    addBtn.setFillColor(sf::Color(0, 150, 90));

    sf::Text addText("Add Patient", font, 20);
    addText.setPosition(325, 432);

    sf::RectangleShape viewBtn(sf::Vector2f(150, 50));
    viewBtn.setPosition(480, 420);
    viewBtn.setFillColor(sf::Color(60, 60, 60));

    sf::Text viewText("View File", font, 20);
    viewText.setPosition(510, 432);

    sf::Text message("", font, 18);
    message.setPosition(300, 500);
    message.setFillColor(sf::Color::Black);

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();

            // Mouse click
            if (e.type == sf::Event::MouseButtonPressed) {
                for (int i = 0; i < 4; i++) {
                    if (boxes[i].getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y))
                        activeBox = i;
                }

                // Add button
                if (addBtn.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                    ofstream file("patients.txt", ios::app);
                    file << inputData[0] << " "
                         << inputData[1] << " "
                         << inputData[2] << " "
                         << inputData[3] << endl;
                    file.close();

                    message.setString("Patient added successfully!");
                    for (int i = 0; i < 4; i++) inputData[i] = "";
                }

                // View button
                if (viewBtn.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                    system("notepad patients.txt");
                }
            }

            // Keyboard typing
            if (e.type == sf::Event::TextEntered && activeBox != -1) {
                if (e.text.unicode == 8 && !inputData[activeBox].empty()) {
                    inputData[activeBox].pop_back();
                }
                else if (e.text.unicode < 128 && e.text.unicode != 8) {
                    inputData[activeBox] += static_cast<char>(e.text.unicode);
                }
            }

            // Enter key to add
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter) {
                ofstream file("patients.txt", ios::app);
                file << inputData[0] << " "
                     << inputData[1] << " "
                     << inputData[2] << " "
                     << inputData[3] << endl;
                file.close();

                message.setString("Patient added using keyboard!");
                for (int i = 0; i < 4; i++) inputData[i] = "";
            }
        }

        // Update visuals
        for (int i = 0; i < 4; i++) {
            boxes[i].setFillColor(i == activeBox ? activeColor : boxColor);
            inputs[i].setString(inputData[i] + (i == activeBox ? "|" : ""));
        }

        window.clear(bgColor);
        for (int i = 0; i < 4; i++) {
            window.draw(labels[i]);
            window.draw(boxes[i]);
            window.draw(inputs[i]);
        }

        window.draw(addBtn);
        window.draw(addText);
        window.draw(viewBtn);
        window.draw(viewText);
        window.draw(message);

        window.display();
    }

    return 0;
}
