#ifndef ECOBSERVERS_H
#define ECOBSERVERS_H

#include "ECObserver.h"
#include "ECGraphicViewImp.h"
#include "ECController.h"

class ECUndoRedo : public ECObserver
{
public:
    ECUndoRedo(ECGraphicViewImp& canvasInstance, ECController& controller): 
        _canvasInstance(canvasInstance), _controller(controller) {      
    }

    virtual void Update() {
        ECGVEventType currEvent = _canvasInstance.GetCurrEvent();
        if (currEvent == ECGV_EV_KEY_DOWN_Z) {
            _controller.Undo();
        }

        if (currEvent == ECGV_EV_KEY_DOWN_Y) {
            _controller.Redo();
        }
    }

private:
    ECGraphicViewImp& _canvasInstance;
    ECController& _controller; 

};

class ECDelete : public ECObserver
{
public:
   ECDelete(ECGraphicViewImp& canvasInstance, ECController& controller): 
        _canvasInstance(canvasInstance), _controller(controller) {      
    }

    virtual void Update() {
        ECGVEventType currEvent = _canvasInstance.GetCurrEvent();
        if (currEvent == ECGV_EV_KEY_DOWN_D && _controller.GetEditMode()) {
            _controller.UpdateUndoRedo();
            _controller.DeleteShapes();
        }
    }

private:
    ECGraphicViewImp& _canvasInstance;
    ECController& _controller; 
 
};

class ECMove : public ECObserver
{
public:
    ECMove(ECGraphicViewImp& canvasInstance, ECController& controller): 
        _canvasInstance(canvasInstance), _controller(controller) {
            
    }

    virtual void Update() {
        if (_controller.GetEditMode()) {
            ECGVEventType currEvent = _canvasInstance.GetCurrEvent();
            if (currEvent == ECGV_EV_MOUSE_MOVING && !_controller.IsHoldCtr()) {
                int x, y, initialX, initialY;
                _canvasInstance.GetCursorPosition(x, y);
                initialX = _controller.GetInitialX();
                initialY = _controller.GetInitialY();

                vector<Shape*> shapes = _controller.GetShapes();
                for (auto shape : shapes) {
                    if (shape->IsSelected()) {
                        shape->MovePos(x, y, initialX, initialY);
                    }
                }
                _controller.SetInitialCursorPos(x, y);
            }
        }
    }

private:
    ECGraphicViewImp& _canvasInstance;
    ECController& _controller;
};

class ECGroup : public ECObserver
{
public:
    ECGroup(ECGraphicViewImp& canvasInstance, ECController& controller): 
            _canvasInstance(canvasInstance), _controller(controller) {
                
    }

    virtual void Update() {
        if (_controller.GetEditMode()) {
            ECGVEventType currEvent = _canvasInstance.GetCurrEvent();
            if (currEvent == ECGV_EV_KEY_DOWN_G) {
                // cout << "Working\n";
                vector<Shape*> newShapes;
                vector<Shape*>& shapes = _controller.GetShapesRef();
                vector<int> nulls;
                for (int i = 0; i < shapes.size(); ++i) {
                    if (shapes[i]->IsSelected()) {
                        newShapes.push_back(shapes[i]);
                        shapes[i]->changeSelected();
                        shapes[i] = nullptr;
                        nulls.push_back(i);
                    }
                }

                if (newShapes.size() > 1 || (newShapes.size() == 1 && newShapes[0]->GetType() == "Group")) {
                    for (auto val : nulls) {
                        shapes[val] = nullptr;
                    }
                }
                shapes.erase(std::remove(shapes.begin(), shapes.end(), nullptr), shapes.end());
                

                if (newShapes.size() > 1) {
                    Group* g = new Group(newShapes);
                    _controller.AddShape(g);
                    _controller.UpdateUndoRedo();
                }

                if (newShapes.size() == 1 && newShapes[0]->GetType() == "Group") {
                    for (auto shape : newShapes[0]->GetShapes()) {
                        _controller.AddShape(shape);
                    }

                    _controller.UpdateUndoRedo();
                }

                

                
            }
        }
    }

private:
    ECGraphicViewImp& _canvasInstance;
    ECController& _controller;
};

class ECSelect : public ECObserver
{
public:
    ECSelect(ECGraphicViewImp& canvasInstance, ECController& controller): 
        _canvasInstance(canvasInstance), _controller(controller) {
            
    }

    virtual void Update() {
        if (_controller.GetEditMode()) {
            ECGVEventType currEvent = _canvasInstance.GetCurrEvent();
            if (currEvent == ECGV_EV_MOUSE_BUTTON_DOWN) {
                int x, y;
                _canvasInstance.GetCursorPosition(x, y);

                vector<Shape*> shapes = _controller.GetShapes();
                double area = 0;
                bool selected = false;
                Shape* selectedShape;
                for (auto shape : shapes) {
                    if (shape->IsInside(x, y) && shape->GetArea() >= area) {
                        selectedShape = shape;
                        selected = true;
                    }
                }

                if (selected) {
                    if (!selectedShape->IsSelected()){
                        _controller.UpdateUndoRedo(); 
                    }
                    selectedShape->changeSelected();
                    _controller.SetInitialCursorPos(x, y);
                }
            }
                

        }
    }

private:
    ECGraphicViewImp& _canvasInstance;
    ECController& _controller;

};

class ECRender : public ECObserver
{
public:
    ECRender(ECGraphicViewImp& canvasInstance, ECController& controller): 
    _canvasInstance(canvasInstance), _controller(controller) {
        
    }

    virtual void Update() {
        ECGVEventType currEvent = _canvasInstance.GetCurrEvent();

        if (currEvent == ECGV_EV_TIMER) {
            if (_controller.IsMoving()) {
                int x, y;
                _canvasInstance.GetCursorPosition(x,y);
                if (_controller.IsRectangle() && !_controller.IsFilled()) _canvasInstance.DrawRectangle(_controller.GetC1(), _controller.GetC2(),
                                                x, y);

                if (!_controller.IsRectangle() && !_controller.IsFilled()) _canvasInstance.DrawEllipse(_controller.GetC1(), _controller.GetC2(),
                                                abs(_controller.GetC1() - x), abs(_controller.GetC2() - y));

                if (_controller.IsRectangle() && _controller.IsFilled()) _canvasInstance.DrawFilledRectangle(_controller.GetC1(), _controller.GetC2(),
                                                x, y);

                if (!_controller.IsRectangle() && _controller.IsFilled()) _canvasInstance.DrawFilledEllipse(_controller.GetC1(), _controller.GetC2(),
                                                abs(_controller.GetC1() - x), abs(_controller.GetC2() - y));
            }

            vector<Shape*> shapes = _controller.GetShapes();

            for (auto shape : shapes) {
                RenderShape(shape);     
            }
            _canvasInstance.SetRedraw(true);
        }
    }

    void RenderShape(Shape* shape) {
        vector<int> data = shape->GetData();
        if (!shape->IsDeleted()) {
            if (shape->GetType() == "Group") {
                for (auto s : shape->GetShapes()) {
                        RenderShape(s);
                    }
            }
            if (shape->GetType() == "Rectangle") {
                        if (shape->IsFill()) {
                            _canvasInstance.DrawFilledRectangle(data[0], data[1], data[2], data[3], shape->GetColor());
                        } else {
                            _canvasInstance.DrawRectangle(data[0], data[1], data[2], data[3], shape->GetThickness(), shape->GetColor());
                        }
                        // cout << "Rectangle Drawn\n";
            }

            if (shape->GetType() == "Ellipse") {
                    if (shape->IsFill()) {
                        _canvasInstance.DrawFilledEllipse(data[0], data[1], data[2], data[3], shape->GetColor());
                    } else {
                        _canvasInstance.DrawEllipse(data[0], data[1], data[2], data[3], shape->GetThickness(), shape->GetColor());
                    }
                    
                }
        }
    }



private:
    ECGraphicViewImp& _canvasInstance;
    ECController& _controller;
};

class ECAddRectangle : public ECObserver
{
public:
    ECAddRectangle(ECGraphicViewImp& canvasInstance, ECController& controller): 
    _canvasInstance(canvasInstance), _controller(controller) {
        
    }

    virtual void Update() {
        ECGVEventType currEvent = _canvasInstance.GetCurrEvent();
        // if (currEvent != 0 && currEvent != 11) cout << currEvent << endl;
        if (!_controller.GetEditMode() && _controller.IsRectangle()) {
            if (currEvent == ECGV_EV_MOUSE_BUTTON_DOWN) {
                _canvasInstance.GetCursorPosition(x1, y1);
                _controller.SetCs(x1, y1);
                _controller.SetMoving(true);
            }

            if (currEvent == ECGV_EV_MOUSE_BUTTON_UP) {
                _controller.SetMoving(false);
                _controller.UpdateUndoRedo();
                _canvasInstance.GetCursorPosition(x2, y2);
                Rectangle* newShape = new Rectangle(x1, y1, x2, y2);
                if (_controller.IsFilled()) newShape->SetFill(true);
                _controller.AddShape(newShape);
            }
        }
    }

private:
    ECGraphicViewImp& _canvasInstance;
    ECController& _controller;
    int x1, y1, x2, y2;
    bool _draw = false;
};

class ECAddEllipse : public ECObserver
{
public:
    ECAddEllipse(ECGraphicViewImp& canvasInstance, ECController& controller): 
    _canvasInstance(canvasInstance), _controller(controller) {
        
    }

    virtual void Update() {
        ECGVEventType currEvent = _canvasInstance.GetCurrEvent();
        // if (currEvent != 0 && currEvent != 11) cout << currEvent << endl;
        if (!_controller.GetEditMode() && !_controller.IsRectangle()) {
            if (currEvent == ECGV_EV_MOUSE_BUTTON_DOWN) {
                _canvasInstance.GetCursorPosition(x1, y1);
                _controller.SetCs(x1, y1);
                _controller.SetMoving(true);
            }

            if (currEvent == ECGV_EV_MOUSE_BUTTON_UP) {
                _controller.SetMoving(false);
                _controller.UpdateUndoRedo();
                _canvasInstance.GetCursorPosition(x2, y2);
                
                Ellipse* newShape = new Ellipse(x1, y1, abs(x1 - x2), abs(y1 - y2));
                if (_controller.IsFilled()) newShape->SetFill(true);
                _controller.AddShape(newShape);
            }
        }
    }

private:
    ECGraphicViewImp& _canvasInstance;
    ECController& _controller;
    int x1, y1, x2, y2;
    bool _draw = false;

};

class ECChangeMode : public ECObserver
{
public:
    ECChangeMode(ECGraphicViewImp& canvasInstance, ECController& controller): 
    _canvasInstance(canvasInstance), _controller(controller) {
        
    }

    virtual void Update() {
        ECGVEventType currEvent = _canvasInstance.GetCurrEvent();

        if (currEvent == ECGV_EV_KEY_DOWN_SPACE) {
            _controller.ChangeMode();
        }

        if (currEvent == ECGV_EV_KEY_DOWN_G && !_controller.GetEditMode()) {
            _controller.ChangeRectangle();
        }

        if (currEvent == ECGV_EV_KEY_DOWN_F && !_controller.GetEditMode()) {
            _controller.ChangeFill();
        }

        if (currEvent == ECGV_EV_KEY_DOWN_CTRL && _controller.GetEditMode()) {
            _controller.SetHoldCtrl(true);
        }

        if (currEvent == ECGV_EV_KEY_UP_CTRL && _controller.GetEditMode()) {
            _controller.SetHoldCtrl(false);
        }
    }

private:
    ECGraphicViewImp& _canvasInstance;
    ECController& _controller;
};

class ECArrowMove : public ECObserver
{
public:
    ECArrowMove(ECGraphicViewImp& canvasInstance, ECController& controller): 
    _canvasInstance(canvasInstance), _controller(controller) {
        
    }

    virtual void Update() {
        ECGVEventType currEvent = _canvasInstance.GetCurrEvent();

        if (currEvent == ECGV_EV_KEY_DOWN_UP) {
            Move(0, 10);
        }
        else if (currEvent == ECGV_EV_KEY_DOWN_RIGHT) {
            Move(10,0);
        } 
        else if (currEvent == ECGV_EV_KEY_DOWN_DOWN) {
            Move(0,-10);
        } 
        else if (currEvent == ECGV_EV_KEY_DOWN_LEFT) {
            Move(-10,0);
        }
    }

    void Move(int x, int y) {
        vector<Shape*> shapes = _controller.GetShapes();
        for (auto shape : shapes) {
            if (shape->IsSelected()) {
                shape->Move(x, y);
            }
        }
        
    }

private:
    ECGraphicViewImp& _canvasInstance;
    ECController& _controller;
};


#endif