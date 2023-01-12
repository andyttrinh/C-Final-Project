#ifndef SHAPES_H
#define SHAPES_H

#include <string>
#include <cmath>
#include "ECGraphicViewImp.h"

using namespace std;

// struct clonable {
//     virtual ~clonable() {}
//     virtual clonable* clone() const = 0;
// };

class Shape
{
public:
    virtual ~Shape() {}
    virtual Shape* clone() const = 0;

    virtual string GetType() = 0;
    virtual vector<int> GetData() = 0;
    virtual int GetThickness() = 0;
    virtual ECGVColor GetColor() = 0;
    virtual void changeSelected() = 0;
    virtual bool IsInside(int x, int y) = 0;
    virtual bool IsSelected() = 0;
    virtual int GetArea() = 0;
    virtual void MovePos(int x, int y, int initialX, int initialY) = 0;
    virtual void SetDelete() = 0;
    virtual bool IsDeleted() = 0;
    virtual bool IsFill() = 0;
    virtual void SetFill(bool b) = 0;
    virtual void Move(int x, int y) = 0;
    virtual vector<Shape*> GetShapes() {return vector<Shape*>();}
    void AddShape(Shape* s) {}

    
};

class Group : public Shape {
public:
    Group() {}
    Group(vector<Shape*> shapes): _shapes(shapes){

    }

    Group(const Group &t) {
        _shapes = vector<Shape*>();

        for (auto val : t._shapes) {
            Shape* curr = val->clone();
            _shapes.push_back(curr);
        }
    }

    virtual Group* clone() const
        { return new Group( *this );}

    string GetType() { return "Group"; }

    vector<int> GetData() {return vector<int>();}

    virtual int GetThickness() {return -1;}

    virtual ECGVColor GetColor() {return ECGV_BLACK;}

    virtual void changeSelected() {
        for (auto shape : _shapes) shape->changeSelected();
        _selected = !_selected;
    }

    virtual bool IsInside(int x, int y) {
        for (auto shape : _shapes) {
            if (shape->IsInside(x, y)) return true;
        }

        return false;
    }

    virtual bool IsSelected() {
        return _selected;
    }

    virtual int GetArea() {
        int total = 0;
        for (auto shape : _shapes) {
            total += shape->GetArea();
        }

        return total;
    }

    virtual void MovePos(int x, int y, int initialX, int initialY) {
        for (auto shape: _shapes) {
            shape->MovePos(x, y, initialX, initialY);
        }
    }

    virtual void SetDelete() {_deleted = true;}

    virtual bool IsDeleted() {return _deleted;}

    virtual bool IsFill() {return false;}

    virtual void SetFill(bool b) {}

    virtual void Move(int x, int y) {}

    vector<Shape*> GetShapes() {
        return _shapes;
    }

    void AddShape(Shape* s) {
        _shapes.push_back(s);
    }

private:
    bool _deleted = false;
    bool _selected = false;
    vector<Shape*> _shapes;

};

class Ellipse : public Shape
{
public:
    Ellipse(int x1, int y1, int xRadius, int yRadius, int thickness = 3, ECGVColor color = ECGV_BLACK):
    _x1(x1), _y1(y1), _xRadius(xRadius), _yRadius(yRadius), _thickness(thickness), _color(color) {
        
    }

    virtual Ellipse* clone() const
        { return new Ellipse( *this );}

    vector<int> GetData() {
        return vector<int>{_x1, _y1, _xRadius, _yRadius};
    }

    int GetThickness() {
        return _thickness;
    }

    ECGVColor GetColor() {
        if (_selected) return ECGV_BLUE;
        return _color;
    }

    void changeSelected() {
        _selected = !_selected;
    }

    bool IsInside(int x, int y) {
        // return true;
        return (powf(x - _x1, 2) / (_xRadius * _xRadius)) + 
        (powf(y - _y1, 2) / (_yRadius * _yRadius)) <= 1;
    }

    bool IsSelected() {return _selected;}

    int GetArea() {
        return 3.142 * _xRadius * _yRadius;
    }

    void MovePos(int x, int y, int initialX, int initialY) {
        int xDiff = x - initialX;
        int yDiff = y - initialY;

        _x1 += xDiff;
        _y1 += yDiff;

        
    }

    void SetDelete() {
        _deleted = true;
    }

    bool IsDeleted() {
        return _deleted;
    }

     bool IsFill() {
        return _filled;
    }

    void SetFill(bool b) {
        _filled = b;
    }

    void Move(int x, int y) {
        _x1 += x;
        _y1 += y;
    }


    virtual string GetType() {return "Ellipse";}


private:
    int _x1, _y1, _xRadius, _yRadius;
    int _thickness = 3;
    ECGVColor _color = ECGV_BLACK;
    bool _selected = false;
    bool _deleted = false;
    bool _filled = false;
};

class Rectangle : public Shape
{
public:

    Rectangle(int x1, int y1, int x2, int y2, int thickness = 3, ECGVColor color = ECGV_BLACK):
    _x1(x1), _y1(y1), _x2(x2), _y2(y2), _thickness(thickness), _color(color) {

    }

    virtual Rectangle* clone() const
        { return new Rectangle( *this );}

    

    vector<int> GetData() {
        return vector<int>{_x1, _y1, _x2, _y2};
    }

    int GetThickness() {
        return _thickness;
    }

    ECGVColor GetColor() {
        if (_selected) return ECGV_BLUE;
        return _color;
    }

    void changeSelected() {
        _selected = !_selected;
    }

    bool IsInside(int x, int y) {
        if (x > _x1 and x < _x2 and y > _y1 and y < _y2)return true;
        return false;
    }

    bool IsSelected() {return _selected;}

    int GetArea() {
        return abs(_x1 - _x2) * abs(_y1 - _y2);
    }

    void MovePos(int x, int y, int initialX, int initialY) {
        int xDiff = x - initialX;
        int yDiff = y - initialY;

        _x1 += xDiff;
        _x2 += xDiff;
        _y1 += yDiff;
        _y2 += yDiff;

        
    }

    void SetDelete() {
        _deleted = true;
    }

    bool IsDeleted() {
        return _deleted;
    }

    bool IsFill() {
        return _filled;
    }

    void SetFill(bool b) {
        _filled = b;
    }

    void Move(int x, int y) {
        _x1 += x;
        _x2 += x;
        _y1 += y;
        _y2 += y;
    }



    virtual string GetType() {return "Rectangle";}

private:
    int _x1, _y1, _x2, _y2;
    int _thickness = 3;
    ECGVColor _color = ECGV_BLACK;
    bool _selected = false;
    bool _deleted = false;
    bool _filled = false;
};

#endif