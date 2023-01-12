//
//  Created by Yufeng Wu on 3/2/22.
//
// #include "ECGraphicViewImp.h"
// #include "ECObserver.h"
// #include "ECController.h"
// #include "ECObserver.h"
#include "Observers.h"
#include <fstream> 
#include <sstream>

fstream newfile;
ECModel model;
ECController controller(model);
string s;
stringstream ss;

Shape* CreateShape(int type);

// LOAD SHAPES ==================

void GetLine() {
  ss.str("");
  ss.clear();
  getline(newfile, s);
  ss << s;
}

int GetVal() {
  string c;
  getline(ss, c, ' ');
  int n = stoi(c);


  return n;
}

Rectangle* CreateRectangle(int type) {
  vector<int> data;
  for (int i = 0; i < 10; ++i) {
    data.push_back(GetVal());
  }

  Rectangle* newObj = new Rectangle(data[1], data[2], data[5], data[6]);
  if (type == 1) newObj->SetFill(true);
  return newObj;
}

Ellipse* CreateEllipse(int type) {
  vector<int> data;
  for (int i = 0; i < 5; ++i) {
    data.push_back(GetVal());
  }
  Ellipse* newObj = new Ellipse(data[0], data[1], data[2], data[3]);
  if (type == 3) newObj->SetFill(true);
  return newObj;
}

Group* CreateGroup() {
  Group* newObj = new Group;
  int n = GetVal();

  for (int i = 0; i < n; ++i) {
    GetLine();
    int t = GetVal();
    newObj->AddShape(CreateShape(t));
  }

  return newObj;
}

Shape* CreateShape(int type) {
  if (type == 0 || type == 2) return CreateRectangle(type);
  if (type == 1 || type == 3) return CreateEllipse(type);
  if (type == 4) return CreateGroup();

  return nullptr;
}

void Load(int n) {
  for (int i = 0; i < n; ++i) {
    GetLine();
    int n = GetVal();
    // cout << n << endl;
    controller.AddShape(CreateShape(n));
  }
}

// SAVE SHAPES =====================
void Write(vector<int> &info) {
  for (int i = 0; i < info.size() - 1; ++i) {
    newfile << info[i] << " ";
  }
  newfile << info[info.size() - 1];
}


void SaveRectangle(Shape* s) {
  vector<int> data = s->GetData();
  vector<int> info;
  if (s->IsFill()) {
    info.push_back(2);
  } else {
    info.push_back(0);
  }
  info.push_back(4);
  info.push_back(data[0]);
  info.push_back(data[1]);
  info.push_back(data[0]);
  info.push_back(data[3]);
  info.push_back(data[2]);
  info.push_back(data[3]);
  info.push_back(data[2]);
  info.push_back(data[1]);
  info.push_back(0); // color

  Write(info);
}

void SaveEllipse(Shape* s) {
  vector<int> data = s->GetData();
  vector<int> info;
  if (s->IsFill()) {
    info.push_back(3);
  } else {
    info.push_back(1);
  }


  info.push_back(data[0]);
  info.push_back(data[1]);
  info.push_back(data[2]);
  info.push_back(data[3]);
  info.push_back(0); //color

  Write(info);
}

void SaveGroup(Shape* s) {
  vector<Shape*> shapes = s->GetShapes();
  newfile << 4 << " " << shapes.size();

  for (auto shape : shapes) {
    newfile << '\n';
    if (shape->GetType() == "Rectangle") SaveRectangle(shape);
    if (shape->GetType() == "Ellipse") SaveEllipse(shape);
    if (shape->GetType() == "Group") SaveGroup(shape);
  }

}

void Save() {
  vector<Shape*> shapes = controller.GetShapes();
  int n = shapes.size();
  newfile << n;

  for (auto shape : shapes) {
    newfile << '\n';
    if (shape->GetType() == "Rectangle") SaveRectangle(shape);
    if (shape->GetType() == "Ellipse") SaveEllipse(shape);
    if (shape->GetType() == "Group") SaveGroup(shape);
  }
}


// Test graphical view code
int real_main(int argc, char **argv)
{
  const int widthWin = 500, heightWin = 500;
  ECGraphicViewImp view(widthWin, heightWin);
  // ECModel model;
  // ECController controller(model);
  ECAddRectangle* recObs = new ECAddRectangle(view, controller);
  ECChangeMode* changeObs = new ECChangeMode(view, controller);
  ECRender* renderObs = new ECRender(view, controller);
  ECSelect* selectObs = new ECSelect(view, controller);
  ECMove* moveObs = new ECMove(view, controller);
  ECDelete* delObs = new ECDelete(view, controller);
  ECUndoRedo* undoredoObs = new ECUndoRedo(view, controller);
  ECAddEllipse* ellObs = new ECAddEllipse(view, controller);
  ECArrowMove* arrObs = new ECArrowMove(view, controller);
  ECGroup* groupObs = new ECGroup(view, controller);
  view.Attach(recObs);
  view.Attach(changeObs);
  view.Attach(renderObs);
  view.Attach(selectObs);
  view.Attach(moveObs);
  view.Attach(delObs);
  view.Attach(undoredoObs);
  view.Attach(ellObs);
  view.Attach(arrObs);
  view.Attach(groupObs);


  // FILE READING
  if (argc == 2) {
      // cout << "Running File\n";
      newfile.open(argv[1]);  // open a file to perform write operation using file object
      if (newfile.is_open()) { //checking whether the file is open {
        if (newfile.peek() != std::ifstream::traits_type::eof()) {
          string tp;
          int n;
          GetLine();
          n = GetVal();
          // cout << n << endl;
          
          Load(n);
        }
      }
        newfile.close();
    }
  
  view.Show();

  // FILE SAVING
  if (argc == 2) {
    newfile.open(argv[1], ios::out | ios::trunc );

    Save();

    newfile.close();
  }
  


  return 0;
}













int main(int argc, char **argv)
{
    return real_main(argc, argv);
    //return al_run_main(argc, argv, real_main);
}

