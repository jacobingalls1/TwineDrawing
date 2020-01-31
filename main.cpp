#include <iostream>
#include <cmath>
using namespace std;

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;


int dud = 0;
string filepath = "C:\\Users\\Ophiuchus\\Desktop\\C++\\gram\\che.png";

vector<Vector2i> onLine(Vector2f first, Vector2f second) {
    Vector2f slope(second.x - first.x, second.y - first.y);
    Vector2f uSlope(slope.x / slope.y, slope.y / slope.x);
    vector<Vector2i> out;
    int diffX = 1;
    if (first.x > second.x) {
        diffX = -1;
    }
    for (int i=int(first.x); i != int(second.x); i += diffX) {
        int y = first.y;
        y += uSlope.y*(i-int(first.x));
        out.push_back(Vector2i(i, y));
    }
    return out;
};

float brightness(Vector2f start, Vector2f end, Vector2f pos, Texture &target, bool whiteout=false) {
    int bright(0), pixCount(0);
    Image iTarg = target.copyToImage();
    Vector2f lowR(pos.x + iTarg.getSize().x, pos.y + iTarg.getSize().y);
    vector<Vector2i> onL(onLine(start, end));
    if (whiteout) {
        for (Vector2i v : onL) {
            if (pos.x < v.x and pos.y < v.y and v.x < lowR.x and v.y < lowR.y) {
                iTarg.setPixel(v.x - pos.x, v.y - pos.y, Color::White);
            }
        }
        target.update(iTarg);
        return 255;
    }
    for (Vector2i v : onL) {
        if (pos.x <= v.x and pos.y <= v.y and v.x <= lowR.x and v.y <= lowR.y) {
//            if (color == 0) {
                bright += (int) iTarg.getPixel((int) (v.x - pos.x), (int) (v.y - pos.y)).r;
//            } else if (color == 1) {
//                bright += (int) iTarg.getPixel((int) (v.x - pos.x), (int) (v.y - pos.y)).g;
//            } else {
//                bright += (int) iTarg.getPixel((int) (v.x - pos.x), (int) (v.y - pos.y)).b;
//            }
            pixCount++;
        }
    }
    if (pixCount == 0) {
        return 255;
    }
    return bright / (float)pixCount;
}

int line(Texture &target, vector<Vector2f> circle, Vector2f pos, int peg) {
    int lines[circle.size()];
    for (int i = 0; i < circle.size(); i++) {
        lines[i] = (int)brightness(circle[peg], circle[i], pos, target);
    }
    int min(lines[0]), ind(0);
    for (int i = 1; i < circle.size(); i++) {
        if (lines[i] < min) {
            min = lines[i];
            ind = i;
        }
    }
    brightness(circle[peg], circle[ind], pos, target, true);
    if (lines[ind] > 250) {
        if (dud > 10) {
            return -1;
        } else {
            dud++;
        }
    } else {
        dud = 0;
    }
    return ind;
}

int main() {

    srand(time(NULL));

    const int SCREEN_SIZE = 1000, CIRC_POINTS = 80, CIRC_INSET = 50, STRINGS_PER_FRAME=10;
    const Color prim(Color::White), sec(Color::Black);
    const float PI = 3.141592, SQUISH = .9;

    bool stop = false;

    vector<int> pegs = {rand() % CIRC_POINTS};

    Texture target;
    target.loadFromFile(filepath);


    Vector2f sprPos((SCREEN_SIZE - target.getSize().x) / 2, (SCREEN_SIZE - target.getSize().y) / 2);
    Sprite sprite(target);
    sprite.setPosition(sprPos);

    const int IMG_SIDE = max(target.getSize().x, target.getSize().y);

    Vector2f center(SCREEN_SIZE / 2, SCREEN_SIZE / 2);
    vector<Vector2f> circle;
    for (int i = 0; i < CIRC_POINTS; i++) {
        float port = (i / (float)CIRC_POINTS) * PI * 2;
        circle.push_back(Vector2f(SQUISH*cos(port) * (SCREEN_SIZE - (2 * CIRC_INSET)) / 2 ,
                                  sin(port) * (SCREEN_SIZE - (2 * CIRC_INSET)) / 2) + center);
    }

    RenderWindow window(VideoMode(SCREEN_SIZE, SCREEN_SIZE), "string");
    window.requestFocus();
    window.setFramerateLimit(1);
    RenderWindow window2(VideoMode(SCREEN_SIZE, SCREEN_SIZE), "string2");
    window2.requestFocus();
    window2.setFramerateLimit(1);
//    window2.close();
    bool readback(false);
    int readCount(0);
    Font arial;
//    arial.loadFromFile("C:\\Users\\Ophiuchus\\Desktop\\String\\data\\arial.ttf");

    while( window.isOpen() ) {
        window.clear(prim);
        window2.clear(prim);
        if (!readback) {

            for (Vector2f v : circle) {
                RectangleShape rect;
                rect.setPosition(v);
                rect.setSize(Vector2f(2, 2));
                rect.setFillColor(sec);
                window.draw(rect);
                window2.draw(rect);
            }

            for (int i=0; i < STRINGS_PER_FRAME; i++) {
                pegs.push_back(line(target, circle, sprPos, pegs[pegs.size() - 1]));
                cout << pegs.size() << endl;
            }
            if (pegs[pegs.size() - 1] == -1) {
                readback = true;
                continue;
            }

            window2.draw(sprite);

            VertexArray path(LinesStrip, pegs.size());
            for (int i = 0; i < pegs.size(); i++) {
                cout << pegs[i] << " ";
                path[i].position = circle[pegs[i]];
                path[i].color = sec;
            }
            cout << endl;
            window.draw(path);
            window2.draw(path);

            window.display();
            window2.display();
        } else {
            cout << "going" << endl;
            string str;
            if (readCount >= pegs.size()) {
                str = "You're Done!";
            } else {
                str = to_string(pegs[readCount]);
            }
            Text inst;
            inst.setString(str);
            inst.setPosition(100, 100);
            inst.setCharacterSize(70);
            inst.setFont(arial);
            inst.setFillColor(sec);
            Text index;
            index.setString(to_string(readCount));
            index.setPosition(100, 200);
            index.setCharacterSize(50);
            index.setFont(arial);
            index.setFillColor(sec);
            window.draw(inst);
            window.draw(index);
            window.display();
        }

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                window2.close();
            } else if (event.type == Event::KeyPressed) {
                switch (event.key.code) {
                    case Keyboard::Escape:
                    case Keyboard::Q:
                        window.close();
                        window2.close();
                        break;
                    case Keyboard::Space:
                        readback = true;
                        window.setFramerateLimit(1);
                        break;
                }
            } else if (event.type == Event::MouseButtonPressed and readback) {
                if (event.mouseButton.button == Mouse::Left) {
                    readCount++;
                } else {
                    readCount--;
                }
            }
        }
    }

    return EXIT_SUCCESS;
}