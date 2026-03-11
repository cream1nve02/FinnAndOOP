/******** 연산자 오버로딩********/
// 개념 이해 정도면 충분!
#include <iostream>
using namespace std;

class Vector2 {

public:
// constructor
    Vector2();
    Vector2(float x, float y);
// getter  
    float GetX() const;
    float GetY() const;

// operator overloading
    Vector2 operator+(const Vector2 other) const;
    Vector2 operator-(const Vector2 other) const;
    Vector2 operator*(const float k)const;
    float operator*(const Vector2 other) const;

private:
    float x, y;
};

Vector2 operator*(const float k, const Vector2 v);

int main() {
    Vector2 a(2,3), b(5,6);
    Vector2 c1 = a + b;
    Vector2 c2 = a - b;
    float c3 = a * b;

    cout << "c1 = (" << c1.GetX() << ", " << c1.GetY() << ")" << endl;
    cout << "c2 = (" << c2.GetX() << ", " << c2.GetY() << ")" << endl;
    cout << "c3 = " << c3 << endl;
    return 0;
}

Vector2::Vector2() : x(0), y(0) {}
Vector2::Vector2(float x, float y) : x(x), y(y) {}
float Vector2::GetX() const {return x; }
float Vector2::GetY() const {return y; }

Vector2 Vector2::operator+(const Vector2 other) const{
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2 other) const{
    return Vector2(x - other.x, y-other.y);
}

Vector2 Vector2::operator*(const float k) const {
    return Vector2(x * k, y * k);
}

Vector2 operator*(const float k, const Vector2 v) {
    return Vector2(k * v.GetX(), k *  v.GetY());
}

float Vector2::operator*(const Vector2 other) const {
    return x * other.x + y * other.y;
}