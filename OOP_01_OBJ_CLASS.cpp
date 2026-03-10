#include <iostream>
using namespace std;

class Color{
    
public:

    // Constructor
    Color() : r(0), g(0), b(0), id(idCounter++) {}
    Color(float r, float g, float b) : r(r), g(g), b(b), id(idCounter++) {}
    

    // Getter Function
    float GetId() {return id;}
    float GetR() {return r;}
    float GetG() {return g;}
    float GetB() {return b;}

    static Color MixedColors(Color a, Color b) {return Color((a.r+b.r)/2, (a.g+b.g)/2, (a.b+b.b)/2);}


private:
    static int idCounter;
    int id;
    float r,g,b;
};

int Color::idCounter = 1;

int main(){

    // Object
    Color red = Color(1,0,0);
    Color blue = Color(0,1,0);
    Color purple = Color::MixedColors(red, blue);
    
    // print
    cout << red.GetId() << endl;
    cout << blue.GetId() << endl;
    cout << purple.GetId() << endl;
    

    return 0;
}