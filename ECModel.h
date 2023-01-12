#ifndef ECMODEL_H
#define ECMODEL_H

#include<vector>
#include "Shapes.h"

using namespace std;

class ECModel
{
public:
    ECModel() {}
    ECModel(const ECModel &t) {
        _shapes = vector<Shape*>();

        for (auto val : t._shapes) {
            Shape* curr = val->clone();
            _shapes.push_back(curr);
        }
    }
    

    void AddShape(Shape *s) {
        _shapes.push_back(s);
    }

    vector<Shape*> GetShapes() {
        return _shapes;
    }

    // void AddSelected(Shape *s) {
    //     s->changeSelected();
    //     _selected.push_back(s);
    // }

    void DeleteShapes() {
        for (auto shape : _shapes) {
            if (shape->IsSelected()) {
                shape->SetDelete();
                // shape = nullptr;
            }
        }
        // _shapes.erase(std::remove(_shapes.begin(), _shapes.end(), nullptr), _shapes.end());
    }

    vector<Shape*>& GetShapesRef() {
        return _shapes;
    }

private:
    vector<Shape*> _shapes;
};

#endif