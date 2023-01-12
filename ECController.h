#ifndef ECONTROLLER_H
#define ECONTROLLER_H

// #include "ECGraphicViewImp.h"
// // #include "ECObserver.h"
#include "ECModel.h"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class ECController
{
public:
    ECController(ECModel& model): _model(model) {}

    void AddShape(Shape* s) {
        _model.AddShape(s);
    }

    void ChangeMode() {
        if (_editMode) {
            _editMode = false;
        } else {
            _editMode = true;
        }
    }

    bool GetEditMode() {return _editMode;}

    vector<Shape*> GetShapes() {
        return _model.GetShapes();
    };

    void SetInitialCursorPos(int x, int y) {
        initialX = x;
        initialY = y;
    }

    int GetInitialX() {return initialX;}
    int GetInitialY() {return initialY;}

    void DeleteShapes() {
        _model.DeleteShapes();
    }

    void UpdateUndoRedo() {
        _undo.push_back(_model);
        _redo.clear();
    }

    void Undo() {
        if (_undo.size() > 0) {
            // cout << "Undo_Size = " << _undo.size() << "\n";
            _redo.push_back(_model);
            _model = _undo.back();
            _undo.pop_back();
        }
        
    }

    void Redo() {
        if (_redo.size() > 0) {
            // cout << "Redo_Size = " << _redo.size() << "\n";
            _undo.push_back(_model);
            _model = _redo.back();
            _redo.pop_back();
        }
        
    }

    bool IsMoving() {return _isMoving;}
    void SetMoving(bool val) {
        _isMoving = val;
    }

    bool IsCreating() { return _creating;}
    void SetCreating(bool val) {_creating = val;}


    void SetCs(int x, int y) {
        c1 = x;
        c2 = y;
    }

    int GetC1() { return c1;}
    int GetC2() { return c2;}

    bool IsRectangle() { return _rectangle; }
    void ChangeRectangle() {
        _rectangle = !_rectangle;
    }

    bool IsFilled() { return _fill;}
    void ChangeFill() {
        _fill = !_fill;
    }

    bool IsHoldCtr() { return _ctrlHold; }
    void SetHoldCtrl(bool b) {
        _ctrlHold = b;
    }

    vector<Shape*>& GetShapesRef() {
        return _model.GetShapesRef();
    }



private:
    ECModel& _model;
    bool _editMode = true;
    bool _isMoving = false;
    bool _creating = false;
    bool _rectangle = true;
    bool _fill = false;
    bool _ctrlHold = false;
    int initialX, initialY;

    vector<ECModel> _undo;
    vector<ECModel> _redo;

    int c1, c2;

};


#endif