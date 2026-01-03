#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode(900, 600), "Hospital Management System");
    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("arial.ttf");

    // Colors
    sf::Color bg(220, 245, 230);
    sf::Color green(0, 130, 90);
    sf::Color dark(40, 40, 40);
    sf::Color active(0, 180, 120);

    int page = 0; // 0=menu, 1=add, 2=history, 3=slip

    // ---------------- MENU ----------------
    sf::RectangleShape opt1({300, 60});
    opt1.setPosition(300, 200);
    opt1.setFillColor(dark);

    sf::RectangleShape opt2({300, 60});
    opt2.setPosition(300, 300);
    opt2.setFillColor(dark);

    sf::Text t1("1. Add Patient", font, 22);
    t1.setPosition(380, 215);

    sf::Text t2("2. View History", font, 22);
    t2.setPosition(380, 315);

    // ---------------- ADD PATIENT ----------------
    string labelsTxt[4] = {"Patient ID", "Name", "Age", "Disease"};
    string data[4] = {"", "", "", ""};
    int activeBox = -1;

    sf::RectangleShape boxes[4];
    sf::Text labels[4], inputs[4];

    for (int i = 0; i < 4; i++) {
        boxes[i].setSize({300, 45});
        boxes[i].setPosition(320, 120 + i * 70);
        boxes[i].setFillColor(green);

        labels[i].setFont(font);
        labels[i].setString(labelsTxt[i]);
        labels[i].setCharacterSize(18);
        labels[i].setFillColor(sf::Color::Black);
        labels[i].setPosition(200, 130 + i * 70);

        inputs[i].setFont(font);
        inputs[i].setCharacterSize(18);
        inputs[i].setFillColor(sf::Color::White);
        inputs[i].setPosition(330, 130 + i * 70);
    }

    sf::RectangleShape saveBtn({200, 50});
    saveBtn.setPosition(350, 430);
    saveBtn.setFillColor(dark);

    sf::Text saveText("Save (Enter)", font, 20);
    saveText.setPosition(395, 443);

    // ---------------- SLIP ----------------
    sf::Text slip("", font, 22);
    slip.setPosition(250, 200);
    slip.setFillColor(sf::Color::Black);
    sf::Clock slipClock;

    // ---------------- HISTORY ----------------
    sf::Text history("", font, 18);
    history.setPosition(200, 100);
    history.setFillColor(sf::Color::Black);

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();

            // ---------- MENU ----------
            if (page == 0) {
                if (e.type == sf::Event::KeyPressed) {
                    if (e.key.code == sf::Keyboard::Num1) page = 1;
                    if (e.key.code == sf::Keyboard::Num2) {
                        page = 2;
                        ifstream file("patients.txt");
                        string line, all;
                        while (getline(file, line)) all += line + "\n";
                        history.setString(all);
                    }
                }
                if (e.type == sf::Event::MouseButtonPressed) {
                    if (opt1.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y))
                        page = 1;
                    if (opt2.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                        page = 2;
                        ifstream file("patients.txt");
                        string line, all;
                        while (getline(file, line)) all += line + "\n";
                        history.setString(all);
                    }
                }
            }

            // ---------- ADD PATIENT ----------
            if (page == 1) {
                if (e.type == sf::Event::MouseButtonPressed) {
                    for (int i = 0; i < 4; i++)
                        if (boxes[i].getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y))
                            activeBox = i;

                    if (saveBtn.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                        ofstream f("patients.txt", ios::app);
                        f << data[0] << " " << data[1] << " "
                          << data[2] << " " << data[3] << endl;
                        f.close();

                        slip.setString("PATIENT SLIP\n\nID: " + data[0] +
                                       "\nName: " + data[1] +
                                       "\nAge: " + data[2] +
                                       "\nDisease: " + data[3]);
                        slipClock.restart();
                        page = 3;
                    }
                }

                if (e.type == sf::Event::TextEntered && activeBox != -1) {
                    if (e.text.unicode == 8 && !data[activeBox].empty())
                        data[activeBox].pop_back();
                    else if (e.text.unicode < 128)
                        data[activeBox] += static_cast<char>(e.text.unicode);
                }

                if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter) {
                    ofstream f("patients.txt", ios::app);
                    f << data[0] << " " << data[1] << " "
                      << data[2] << " " << data[3] << endl;
                    f.close();

                    slip.setString("PATIENT SLIP\n\nID: " + data[0] +
                                   "\nName: " + data[1] +
                                   "\nAge: " + data[2] +
                                   "\nDisease: " + data[3]);
                    slipClock.restart();
                    page = 3;
                }
            }

            // ---------- BACK ----------
            if (page == 2 && e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
                page = 0;
        }

        // Slip timeout
        if (page == 3 && slipClock.getElapsedTime().asSeconds() > 2) {
            for (int i = 0; i < 4; i++) data[i] = "";
            activeBox = -1;
            page = 0;
        }

        // Update cursor
        for (int i = 0; i < 4; i++) {
            boxes[i].setFillColor(i == activeBox ? active : green);
            inputs[i].setString(data[i] + (i == activeBox ? "|" : ""));
        }

        window.clear(bg);

        if (page == 0) {
            window.draw(opt1); window.draw(opt2);
            window.draw(t1); window.draw(t2);
        }
        else if (page == 1) {
            for (int i = 0; i < 4; i++) {
                window.draw(labels[i]);
                window.draw(boxes[i]);
                window.draw(inputs[i]);
            }
            window.draw(saveBtn);
            window.draw(saveText);
        }
        else if (page == 2) {
            window.draw(history);
        }
        else if (page == 3) {
            window.draw(slip);
        }

        window.display();
    }

    return 0;
}
