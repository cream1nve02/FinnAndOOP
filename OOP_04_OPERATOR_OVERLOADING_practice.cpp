#include <iostream>
using namespace std;

class Vector2{

public:
    // construtor 선언
    Vector2();
    Vector2(float x, float y);

    // Getter 선언 -> 멤버 변수 접근자
    float GetX() const;
    float GetY() const;

    // Operator Overloading
    Vector2 operator+(const Vector2 other) const; // 덧셈
    Vector2 operator-(const Vector2 other) const; // 뺄셈
    Vector2 operator*(const float k) const; // 벡터 * 실수
    float operator*(const Vector2 other) const;  // 벡터 * 벡터

private:
    float x, y;

};

Vector2 operator*(const float k, const Vector2 v); // 실수 * 벡터는 전역으로 따로 빼서 선언

int main(){

    // 가장 처음 main함수에서 할 건 필요한 객체 생성하는 것
    Vector2 a(2, 3), b(5, 7);
    Vector2 c1 = a * 3.1;
    Vector2 c2 = 3.1 * a;
    float c3 = a * b;

    // 출력
    cout << "c1 = (" << c1.GetX() << ", " << c1.GetY() << ")" << endl;
    cout << "c2 = (" << c2.GetX() << ", " << c2.GetY() << ")" << endl;

}

/************************ 함수 정의 ***************************/

// Constructor
Vector2::Vector2() : x(0), y(0) {}
Vector2::Vector2(float x, float y) : x(x), y(y) {}

// Getter
float Vector2::GetX() const {return x;}
float Vector2::GetY() const {return y;}

// Operator Overloading

Vector2 Vector2::operator+(const Vector2 other) const {

    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2 other) const {
        
    return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(const float k) const {
        
    return Vector2(x * k, y * k);
}

Vector2 operator*(const float k, Vector2 v)  {
        
    return Vector2(k*v.GetX(), k*v.GetY());
}

float Vector2::operator*(const Vector2 other) const{
    
    return float(x*other.x + y*other.y);
}