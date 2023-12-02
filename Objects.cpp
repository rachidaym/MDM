#include "Objects.h"

Rect::Rect(char* n,Vector2 position,float w, float h) {
	width = w;
	height = h;
	pos.x = position.x;
	pos.y = position.y + h;
	name = n;
	type = RECT;
}

void Rect::CalculateCenter() {
	center.x = pos.x + width / 2;
	center.y = pos.y - height / 2;
}

void Rect::CalculateArea() {
	area = mass*width * height;
}

void Rect::CalculateMOI() {
	Ix = (width * pow(height, 3)) / 12;
	Iy = (height * pow(width, 3)) / 12;
	Ixy = 0;
}

void Rect::Draw(const Axes& axes, Color color) {
	float s = axes._scale;
	DrawRectangle(axes.origin.x+s*pos.x, axes.origin.y-s*pos.y, s*width, s*height, color);
}

Triangle::Triangle(char* n,Vector2 position, float h, float totalBase, float diffBase) {
	pos = position;
	height = h;
	b1 = totalBase;
	b2 = diffBase;
	name = n;
	type = TRIANGLE;
}

void Triangle::CalculateCenter() {
	center.x = (3*pos.x + b1 + b2)/3;
	center.y = pos.y + height / 3;
}

void Triangle::CalculateArea() {
	area = mass*b1 * height / 2;
}

void Triangle::CalculateMOI() {
	Ix = (b1 * pow(height, 3)) / 36;
	Iy = (pow(b1,3)*height - b1*b1*height*b2 + b1*height*b2*b2) / 36;
	Ixy = -(b1 * height * height) * (b1 - 2 * b2) / 72;
}

void Triangle::Draw(const Axes& axes, Color color) {
	float s = axes._scale;
	Vector2 v1 = { axes.origin.x+s*pos.x,axes.origin.y-s*pos.y };
	Vector2 v2 = { axes.origin.x+s*pos.x+s*b1,axes.origin.y-s*pos.y };
	Vector2 v3 = { axes.origin.x+s*pos.x+s*b2, axes.origin.y-s*pos.y-s*height };
	DrawTriangle(v1, v2, v3, color);
}

Circle::Circle(char* n,Vector2 position, float inner, float outer, float ang1, float ang2) {
	pos = position;
	innerRadius = inner;
	outerRadius = outer;
	firstAngle = ang1;
	secondAngle = ang2;
	name = n;
	type = CIRCLE;
}

void Circle::CalculateCenter() {
	float ax = (sin(2 * PI - (secondAngle * PI) / 180) - sin(2 * PI - (firstAngle * PI) / 180));
	float ay = (-cos(2 * PI - (secondAngle * PI) / 180) + cos(2 * PI - (firstAngle * PI) / 180));
	float b = pow(outerRadius, 3) - pow(innerRadius, 3);
	if (secondAngle > firstAngle) {
		center.x =pos.x - (ax * b) / (3 * area);
		center.y =pos.y - (ay * b) / (3 * area);
	}
	else {
		center.x = pos.x + (ax * b) / (3 * area);
		center.y = pos.y + (ay * b) / (3 * area);
	}
}

void Circle::CalculateArea() {
	area = mass*((PI * outerRadius * outerRadius) - (PI * innerRadius * innerRadius)) * ((fabs(secondAngle - firstAngle)) / 360);
}

void Circle::CalculateMOI() {
	float a = (2 * PI - (firstAngle * PI) / 180);
	float b = (2 * PI - (secondAngle * PI) / 180);
	int sign = 1;
	if (secondAngle > firstAngle) {
		sign = -1;
	}
	Iy = sign*(pow(outerRadius,4)-pow(innerRadius,4))*(b-a+0.5*(sin(2*b)-sin(2*a))) / 8;
	Ix = sign*(pow(outerRadius, 4) - pow(innerRadius, 4)) * (b - a - 0.5 * (sin(2 * b) - sin(2 * a))) / 8;
	Ixy = sign*(pow(outerRadius, 4) - pow(innerRadius, 4))*(sin(b)*sin(b) - sin(a)*sin(a)) / 8;

	Ix -= area * center.y * center.y;
	Iy -= area * center.x * center.x;
	Ixy -= area * center.x * center.y;
}

void Circle::Draw(const Axes& axes, Color color) {
	float s = axes._scale;
	DrawRing(Vector2{axes.origin.x+s*pos.x, axes.origin.y-s*pos.y}, s*innerRadius, s*outerRadius, firstAngle, secondAngle, 50, color);
}

Trapezoid::Trapezoid(char* n,Vector2 position, float height, float bB, float tB) {
	h = height;
	bottomBase = bB;
	topBase = tB;
	pos.x = position.x;
	pos.y = position.y;
	name = n;
	type = TRAPEZOID;
}

void Trapezoid::CalculateCenter() {
	center.y = pos.y + (h/3)*(2*topBase+bottomBase)/(topBase+bottomBase);
	center.x = pos.x + bottomBase/2;
}

void Trapezoid::CalculateArea() {
	area = mass*(bottomBase + topBase) * h / 2;
}

void Trapezoid::CalculateMOI() {
	Ixy = 0;
	Ix = pow(h,3)*(3*topBase+bottomBase)/ 12;
	Iy = h*(topBase+bottomBase)*(topBase*topBase+7*bottomBase*bottomBase)/ 48;

	Ix -= area * (center.y-pos.y) * (center.y-pos.y);
	Iy -= area * (center.x-pos.x) * (center.x-pos.x);
}

void Trapezoid::Draw(const Axes& axes, Color color) {
	float s = axes._scale;
	float a = (bottomBase - topBase) / 2;
	DrawRectangle(axes.origin.x+s*pos.x + s * a + 0.02, axes.origin.y - s*pos.y - s * h, s*topBase, s*h, color);

	Vector2 v1 = Vector2{axes.origin.x+s*pos.x, axes.origin.y - s*pos.y};
	Vector2 v2 = Vector2{ axes.origin.x + s*pos.x + s * a, axes.origin.y -s*pos.y };
	Vector2 v3 = Vector2{ axes.origin.x+s*pos.x + s * a,axes.origin.y- s*pos.y - s * h };

	Vector2 v4 = Vector2{axes.origin.x+s*pos.x + s*bottomBase-a*s,axes.origin.y- s*pos.y};
	Vector2 v5 = Vector2{ axes.origin.x + s * pos.x + s * bottomBase, axes.origin.y - s * pos.y };
	Vector2 v6 = Vector2{ axes.origin.x + s * pos.x + s * bottomBase - s * a, axes.origin.y - s * pos.y - s * h };

	DrawTriangle(v1, v2, v3, color);
	DrawTriangle(v4, v5, v6, color);
}