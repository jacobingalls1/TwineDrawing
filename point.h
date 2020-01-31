#ifndef SFML_TEMPLATE_POINT_H
#define SFML_TEMPLATE_POINT_H


class point {
public:
    float x, y, cX, cY, rad, xMod, yMod;
    int xBound, yBound;
    point(int rad, int centX, int centY, int xBound, int yBound, bool random=true);
    bool within(int max);
    void reset(int min=0, int max=0);
    void move(int centX, int centY);
    void rectify();
};


#endif //SFML_TEMPLATE_POINT_H
