#ifndef VECTOR2_H
#define VECTOR2_H
#include<math.h>
class Vector2 {
public:
	Vector2()=default;
	Vector2(float m, float n) :x(m), y(n) {};
	~Vector2() = default;
	Vector2 operator+(const Vector2& other) const {
		return Vector2(x + other.x, y + other.y);
	}//const在后面的作用使得，函数不能修改类的成员变量
	Vector2 operator-(const Vector2& other) const {
		return Vector2(x - other.x, y - other.y);
	}
	Vector2 operator*(float scalar) const {
		return Vector2(x * scalar, y * scalar);
	}
	void operator+=(const Vector2& other) {
		x += other.x;
		y += other.y;
	}
	void operator-=(const Vector2 & other) {
		x -= other.x;
		y -= other.y;
	}
	void operator*=(float val) {
		x *= val,y *= val;

	}
	Vector2 operator/(float scalar) const {
		return Vector2(x / scalar, y / scalar);
	}
	void operator/=(float val) {
		x /= val, y /= val;
	}
	float operator*(const Vector2& vec) const{
		return x * vec.x + y * vec.y;
	}
	float length() {
		return sqrt(x * x + y * y);
	}
	void set_position(float m, float n) {
		x = m;
		y = n;
	}
	Vector2 get_position() {
		return *this;
	}
	Vector2 normalize() {
		float len = length();
		if (len == 0)return Vector2(0, 0);
		return Vector2(x / len, y / len);
	}
	bool approx(const Vector2& target) {
		return  (*this - target).length() <=0.001f;
	}
	friend class camera;
	float x = 0.0f;
	float y = 0.0f;
private:

};


#endif