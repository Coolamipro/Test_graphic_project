
#include "pch.h"

#include <cmath>
#include <string>

#include "TestDll.h"
using namespace testdll;

// Initialize ID for geometry
///////////////////////////
int testdll::cBaseGeometry::common_Id = 0;


// cPoint class functions
///////////////////////////
testdll::cPoint::cPoint(double x, double y, double z) :m_x{ x }, m_y{ y }, m_z{ z } {}

bool testdll::cPoint::is_equal(const cPoint& pt2) const
{
	return ((m_x == pt2.m_x) && (m_y == pt2.m_y) && (m_z == pt2.m_z));
}

void testdll::cPoint::ApplyMatrix(const cMatrix& mat)
{
	cVector temp = mat * cVector(*this, 1.0);
	m_x = temp.m_i;
	m_y = temp.m_j;
	m_z = temp.m_k;
}

// cTriangle class functions
///////////////////////////
void testdll::cTriangle::ApplyMatrix(const cMatrix& mat)
{
	p1.ApplyMatrix(mat);
	p2.ApplyMatrix(mat);
	p3.ApplyMatrix(mat);
}

cVector testdll::cTriangle::GetNormal() const
{
	cVector v1 = cVector(p2.GetPoint(), p1.GetPoint());
	cVector v2 = cVector(p2.GetPoint(), p3.GetPoint());
	return v1.prod(v2);
}

// cVector class functions
///////////////////////////


testdll::cVector::cVector(double x, double y, double z, double w) : m_i{ x }, m_j{ y }, m_k{ z }, m_w{ w }{}

testdll::cVector::cVector(const cPoint& begin, const cPoint& end)
{
	m_i = end.m_x - begin.m_x;
	m_j = end.m_y - begin.m_y;
	m_k = end.m_z - begin.m_z;

}

testdll::cVector::cVector(const cPoint& pt, double w)
{
	m_i = pt.m_x;
	m_j = pt.m_y;
	m_k = pt.m_z;
	m_w = w;
}

double testdll::cVector::Length() const
{
	return std::sqrt(m_i * m_i + m_j * m_j + m_k * m_k);
}

double testdll::cVector::Length(const cVector& vector)
{
	return std::sqrt(vector.m_i * vector.m_i + vector.m_j * vector.m_j + vector.m_k * vector.m_k);
}

void testdll::cVector::Normalize()
{
	double len = Length();
	m_i /= len;
	m_j /= len;
	m_k /= len;
}

cVector& testdll::cVector::operator=(const cVector& vec)
{
	m_i = vec.m_i;
	m_j = vec.m_j;
	m_k = vec.m_k;
	m_w = vec.m_w;

	return *this;
}

cVector testdll::cVector::prod(const cVector& second) const
{
	cVector temp;
	temp.m_i = m_j * second.m_k - m_k * second.m_j;
	temp.m_j = m_k * second.m_i - m_i * second.m_k;
	temp.m_k = m_i * second.m_j - m_j * second.m_i;
	return temp;
}

double testdll::cVector::ScalarProd(const cVector& second) const
{
	return m_i * second.m_i + m_j * second.m_j + m_k * second.m_k;
}

cVector testdll::cVector::operator*(double val) const
{
	return cVector(m_i * val, m_j * val, m_k * val, 1);
}

double testdll::cVector::GetAngleBetween(const cVector& second) const
{
	// Get scalar production of vectors
	double p = ScalarProd(second);

	// Calculate length of vectors
	double l1 = Length();
	double l2 = Length(second);

	// Test for zero
	if (l1 == 0.0 || l2 == 0.0)return 0.0;

	// return angle value

	return acos(p / (l1 * l2));
}

cPoint testdll::cVector::GetPoint() const
{
	return cPoint(m_i, m_j, m_k);
}

void testdll::cVector::ApplyMatrix(const cMatrix& mat)
{
	cVector temp = mat * (*this);
	m_i = temp.m_i;
	m_j = temp.m_j;
	m_k = temp.m_k;
}


// cMatrix class functions
///////////////////////////


cMatrix testdll::cMatrix::operator*(const cMatrix& m2) const
{
	cMatrix mat;

	// Output - first line; product first line of m1 to rows of m2
	mat.m[0] = m[0] * m2.m[0] + m[1] * m2.m[4] + m[2] * m2.m[8] + m[3] * m2.m[12];
	mat.m[1] = m[0] * m2.m[1] + m[1] * m2.m[5] + m[2] * m2.m[9] + m[3] * m2.m[13];
	mat.m[2] = m[0] * m2.m[2] + m[1] * m2.m[6] + m[2] * m2.m[10] + m[3] * m2.m[14];
	mat.m[3] = m[0] * m2.m[3] + m[1] * m2.m[7] + m[2] * m2.m[11] + m[3] * m2.m[15];

	// Output - second line; product second line of m1 to rows of m2
	mat.m[4] = m[4] * m2.m[0] + m[5] * m2.m[4] + m[6] * m2.m[8] + m[7] * m2.m[12];
	mat.m[5] = m[4] * m2.m[1] + m[5] * m2.m[5] + m[6] * m2.m[9] + m[7] * m2.m[13];
	mat.m[6] = m[4] * m2.m[2] + m[5] * m2.m[6] + m[6] * m2.m[10] + m[7] * m2.m[14];
	mat.m[7] = m[4] * m2.m[3] + m[5] * m2.m[7] + m[6] * m2.m[11] + m[7] * m2.m[15];

	// Output - third line; product third line of m1 to rows of m2
	mat.m[8] = m[8] * m2.m[0] + m[9] * m2.m[4] + m[10] * m2.m[8] + m[11] * m2.m[12];
	mat.m[9] = m[8] * m2.m[1] + m[9] * m2.m[5] + m[10] * m2.m[9] + m[11] * m2.m[13];
	mat.m[10] = m[8] * m2.m[2] + m[9] * m2.m[6] + m[10] * m2.m[10] + m[11] * m2.m[14];
	mat.m[11] = m[8] * m2.m[3] + m[9] * m2.m[7] + m[10] * m2.m[11] + m[11] * m2.m[15];

	// Output - fouth line; product fouth line of m1 to rows of m2
	mat.m[12] = m[12] * m2.m[0] + m[13] * m2.m[4] + m[14] * m2.m[8] + m[15] * m2.m[12];
	mat.m[13] = m[12] * m2.m[1] + m[13] * m2.m[5] + m[14] * m2.m[9] + m[15] * m2.m[13];
	mat.m[14] = m[12] * m2.m[2] + m[13] * m2.m[6] + m[14] * m2.m[10] + m[15] * m2.m[14];
	mat.m[15] = m[12] * m2.m[3] + m[13] * m2.m[7] + m[14] * m2.m[11] + m[15] * m2.m[15];

	return mat;
}

cVector testdll::cMatrix::operator*(const cVector& vec) const
{
	cVector out;
	out.m_i = m[0] * vec.m_i + m[1] * vec.m_j + m[2] * vec.m_k + m[3] * vec.m_w;
	out.m_j = m[4] * vec.m_i + m[5] * vec.m_j + m[6] * vec.m_k + m[7] * vec.m_w;
	out.m_k = m[8] * vec.m_i + m[9] * vec.m_j + m[10] * vec.m_k + m[11] * vec.m_w;
	out.m_w = m[12] * vec.m_i + m[13] * vec.m_j + m[14] * vec.m_k + m[15] * vec.m_w;

	return out;
}

cMatrix& testdll::cMatrix::operator=(const cMatrix& m2)
{
	for (int i = 0; i < 16; i++) {
		m[i] = m2.m[i];
	}
	return *this;
}

void testdll::cMatrix::Move(const cPoint& distance)
{
	Move(distance.m_x, distance.m_y, distance.m_z);
}

void testdll::cMatrix::Move(double x, double y, double z)
{
	m[3] += x;
	m[7] += y;
	m[11] += z;
}

void testdll::cMatrix::Rotate(double A, double B, double C)
{
	cMatrix rot{};

	double A_rad = (A * Pi) / 180;
	double B_rad = (B * Pi) / 180;
	double C_rad = (C * Pi) / 180;

	rot.m[0] = cos(B_rad) * cos(C_rad);
	rot.m[1] = -1 * cos(B_rad) * sin(C_rad);
	rot.m[2] = sin(B_rad);
	rot.m[4] = cos(A_rad) * sin(C_rad) + cos(C_rad) * sin(A_rad) * sin(B_rad);
	rot.m[5] = cos(A_rad) * cos(C_rad) - sin(A_rad) * sin(B_rad) * sin(C_rad);
	rot.m[6] = -1 * cos(B_rad) * sin(A_rad);

	rot.m[8] = sin(A_rad) * sin(C_rad) - cos(A_rad) * cos(C_rad) * sin(B_rad);
	rot.m[9] = cos(A_rad) * sin(B_rad) * sin(C_rad) + cos(C_rad) * sin(A_rad);
	rot.m[10] = cos(A_rad) * cos(B_rad);

	cMatrix temp{};
	temp = rot * (*this);
	*this = temp;

}

void testdll::cMatrix::Rotate(const cVector& vec, double angle)
{
	cVector v{ vec };
	v.Normalize();
	cMatrix rot{};
	double A = (angle * Pi) / 180;

	rot.m[0] = cos(A) + v.m_i * v.m_i * (1 - cos(A));
	rot.m[1] = v.m_i * v.m_j * (1 - cos(A)) - v.m_k * sin(A);
	rot.m[2] = v.m_i * v.m_k * (1 - cos(A)) + v.m_j * sin(A);
	rot.m[3] = 0;
	rot.m[4] = v.m_j * v.m_i * (1 - cos(A)) + v.m_k * sin(A);
	rot.m[5] = cos(A) + v.m_j * v.m_j * (1 - cos(A));
	rot.m[6] = v.m_j * v.m_k * (1 - cos(A)) - v.m_i * sin(A);
	rot.m[7] = 0;
	rot.m[8] = v.m_k * v.m_i * (1 - cos(A)) - v.m_j * sin(A);
	rot.m[9] = v.m_k * v.m_j * (1 - cos(A)) + v.m_i * sin(A);
	rot.m[10] = cos(A) + v.m_k * v.m_k * (1 - cos(A));
	rot.m[11] = 0;

	cMatrix temp{};
	temp = rot * (*this);
	*this = temp;
}

void testdll::cMatrix::Rotate(const cVector& vec, const cPoint& pt, double angle)
{
	Move(-pt.m_x, -pt.m_y, -pt.m_z);
	Rotate(vec, angle);
	Move(pt.m_x, pt.m_y, pt.m_z);
}

cMatrix testdll::cMatrix::get_invert() const
{
	cMatrix r{}, mov{};

	// invert rotation part of matrix
	r.m[0] = m[0];
	r.m[4] = m[1];
	r.m[8] = m[2];
	r.m[12] = 0.0;


	r.m[1] = m[4];
	r.m[5] = m[5];
	r.m[9] = m[6];
	r.m[13] = 0.0;

	r.m[2] = m[8];
	r.m[6] = m[9];
	r.m[10] = m[10];
	r.m[14] = 0.0;

	r.m[3] = 0.0;
	r.m[7] = 0.0;
	r.m[11] = 0.0;
	r.m[15] = 1;

	// invert translation part of matrix

	mov.m[3] = -m[3];
	mov.m[7] = -m[7];
	mov.m[11] = -m[11];

	mov.m[0] = 1.0;
	mov.m[5] = 1.0;
	mov.m[10] = 1.0;
	mov.m[15] = 1.0;

	cMatrix res = r * mov;

	return res;
}


// cBaseGeometry class functions
///////////////////////////

int testdll::cBaseGeometry::GetId()
{
	return m_id;
}

const std::string& testdll::cBaseGeometry::GetName()
{
	return *m_name;
}

void testdll::cBaseGeometry::SetHighlight(bool state)
{
	m_ishighlight = state;
}

bool testdll::cBaseGeometry::IsHighlight()
{
	return m_ishighlight;
}

void testdll::cBaseGeometry::SetColor(int R, int G, int B)
{
	if (R > 255 || G > 255 || B > 255) return;
	m_color = RGB(R, G, B);
}

void testdll::cBaseGeometry::Rotate(double A, double B, double C)
{
	m_matrix.Rotate(A, B, C);
	CreateMesh();
}

void testdll::cBaseGeometry::Rotate(const cVector& vec, double angle)
{
	m_matrix.Rotate(vec, angle);
	CreateMesh();
}

void testdll::cBaseGeometry::Rotate(const cVector& vec, const cPoint& pt, double angle)
{
	m_matrix.Rotate(vec, pt, angle);
	CreateMesh();
}

void testdll::cBaseGeometry::Move(const cPoint& distance)
{
	m_matrix.Move(distance);
	CreateMesh();
}

void testdll::cBaseGeometry::Draw(HDC hdc, cScene& scene)
{
	HPEN pen;
	if (m_ishighlight) {
		pen = CreatePen(PS_SOLID, m_pen_width, m_highlight);
	}
	else {
		pen = CreatePen(PS_SOLID, m_pen_width, m_color);
	}

	HGDIOBJ prev = SelectObject(hdc, pen);

	for (auto i : *m_triangles) {

		cVector v1 = scene.ScreenProjection(i->p1);
		cVector v2 = scene.ScreenProjection(i->p2);
		cVector v3 = scene.ScreenProjection(i->p3);

		MoveToEx(hdc, (int)v1.m_i, (int)v1.m_j, NULL);
		LineTo(hdc, (int)v2.m_i, (int)v2.m_j);
		LineTo(hdc, (int)v3.m_i, (int)v3.m_j);
		LineTo(hdc, (int)v1.m_i, (int)v1.m_j);
	}

	SelectObject(hdc, prev);
}

testdll::cBaseGeometry::~cBaseGeometry()
{
	ClearMesh();
}

testdll::cBaseGeometry::cBaseGeometry()
{
	m_id = common_Id++;
	m_name = new std::string("");
	m_triangles = new std::vector<cTriangle*>;
}

void testdll::cBaseGeometry::CreateMesh()
{
	delete m_name;
	delete m_triangles;
}

void testdll::cBaseGeometry::ClearMesh()
{
	while (!m_triangles->empty())
	{
		delete m_triangles->back();
		m_triangles->pop_back();
	}
}

// cLSYS class functions
///////////////////////////

void testdll::cLSYS::CreateMesh()
{
	m_outpoints[0] = cVector(0, 0, 0, 1);
	m_outpoints[1] = cVector(m_length, 0, 0, 1);
	m_outpoints[2] = cVector(0, m_length, 0, 1);
	m_outpoints[3] = cVector(0, 0, m_length, 1);

	for (int i = 0; i < 4; i++) {
		m_outpoints[i].ApplyMatrix(m_matrix);
	}
}

testdll::cLSYS::cLSYS(const cPoint& orig, const cPoint& X, const cPoint& Y)
{
	m_vectors[0] = cVector(orig, X);
	m_vectors[1] = cVector(orig, Y);
	m_vectors[2] = m_vectors[0].prod(m_vectors[1]);

	m_vectors[0].Normalize();
	m_vectors[1].Normalize();
	m_vectors[2].Normalize();

	m_matrix.m[0] = m_vectors[0].m_i;
	m_matrix.m[4] = m_vectors[0].m_j;
	m_matrix.m[8] = m_vectors[0].m_k;

	m_matrix.m[1] = m_vectors[1].m_i;
	m_matrix.m[5] = m_vectors[1].m_j;
	m_matrix.m[9] = m_vectors[1].m_k;

	m_matrix.m[2] = m_vectors[2].m_i;
	m_matrix.m[6] = m_vectors[2].m_j;
	m_matrix.m[10] = m_vectors[2].m_k;

	m_matrix.m[3] = orig.m_x;
	m_matrix.m[7] = orig.m_y;
	m_matrix.m[11] = orig.m_z;

	CreateMesh();
	*m_name = "LSYS" + std::to_string(m_id);

	m_pen_width = 4;
}

void testdll::cLSYS::Draw(HDC hdc, cScene& scene)
{
	cVector v1 = scene.ScreenProjection(m_outpoints[0]);
	cVector v2 = scene.ScreenProjection(m_outpoints[1]);
	cVector v3 = scene.ScreenProjection(m_outpoints[2]);
	cVector v4 = scene.ScreenProjection(m_outpoints[3]);

	HPEN pen = CreatePen(PS_SOLID, m_pen_width, RGB(255, 0, 0));
	HGDIOBJ prev = SelectObject(hdc, pen);

	MoveToEx(hdc, (int)v1.m_i, (int)v1.m_j, NULL);
	LineTo(hdc, (int)v2.m_i, (int)v2.m_j);

	pen = CreatePen(PS_SOLID, m_pen_width, RGB(0, 255, 0));
	SelectObject(hdc, pen);

	MoveToEx(hdc, (int)v1.m_i, (int)v1.m_j, NULL);
	LineTo(hdc, (int)v3.m_i, (int)v3.m_j);


	pen = CreatePen(PS_SOLID, m_pen_width, RGB(0, 0, 255));
	SelectObject(hdc, pen);

	MoveToEx(hdc, (int)v1.m_i, (int)v1.m_j, NULL);
	LineTo(hdc, (int)v4.m_i, (int)v4.m_j);

	SelectObject(hdc, prev);
}

// cSphere class functions
///////////////////////////

void testdll::cSphere::CreateMesh()
{
	ClearMesh();

	cVector top = cVector(0.0, 0.0, m_radius, 1);
	cVector bottom = cVector(0.0, 0.0, -m_radius, 1);

	cVector pt[72];

	const int sec = 10; // Degrees of sector

	const double delta = (sec * Pi) / 180;

	const int horiz = 180 / sec;
	const int vert = 360 / sec;

	cVector t1[horiz];
	cVector t2[horiz];


	for (int i = 0; i <= vert; i++) {
		for (int j = 0; j < horiz; j++) {
			t1[j].m_i = m_radius * sin(j * delta + delta) * cos(i * delta);
			t1[j].m_j = m_radius * sin(j * delta + delta) * sin(i * delta);
			t1[j].m_k = m_radius * cos(j * delta + delta);
			if (i == 0)continue;
			if (j == 0) {
				m_triangles->push_back(new cTriangle(top, t1[j], t2[j]));
				continue;
			}
			if (j == (horiz - 1)) {
				m_triangles->push_back(new cTriangle(bottom, t2[j], t1[j]));
				continue;
			}
			m_triangles->push_back(new cTriangle(t1[j - 1], t2[j - 1], t2[j]));
			m_triangles->push_back(new cTriangle(t1[j - 1], t2[j], t1[j]));
		}
		for (int k = 0; k < horiz; k++) {
			t2[k] = t1[k];
		}
	}

	for (auto i : *m_triangles) {
		i->ApplyMatrix(m_matrix);
	}
}

testdll::cSphere::cSphere(const cPoint& center, double radius)
{
	m_center = center;
	m_radius = radius;
	*m_name = "Sphere" + std::to_string(m_id);
	m_matrix.Move(center);
	CreateMesh();

}

testdll::cSphere::cSphere(const cPoint& center, const cPoint& s_point)
{
	m_center = center;
	cVector temp = cVector(m_center, s_point);
	m_radius = temp.Length();
	*m_name = "Sphere" + std::to_string(m_id);
	m_matrix.Move(center);
	CreateMesh();
}

// cBox class functions
///////////////////////////

void testdll::cBox::CreateMesh()
{
	ClearMesh();

	// Botton
	m_triangles->push_back(new cTriangle(cVector(0.0, 0.0, 0.0, 1.0), cVector(m_length, 0.0, 0.0, 1.0), cVector(0.0, m_width, 0.0, 1.0)));
	m_triangles->push_back(new cTriangle(cVector(0.0, m_width, 0.0, 1.0), cVector(m_length, 0.0, 0.0, 1.0), cVector(m_length, m_width, 0.0, 1.0)));
	// Top
	m_triangles->push_back(new cTriangle(cVector(0.0, 0.0, m_height, 1.0), cVector(0.0, m_width, m_height, 1.0), cVector(m_length, 0.0, m_height, 1.0)));
	m_triangles->push_back(new cTriangle(cVector(0.0, m_width, m_height, 1.0), cVector(m_length, m_width, m_height, 1.0), cVector(m_length, 0.0, m_height, 1.0)));

	// Side 1
	m_triangles->push_back(new cTriangle(cVector(0.0, 0.0, 0.0, 1.0), cVector(0.0, m_width, 0.0, 1.0), cVector(0.0, 0.0, m_height, 1.0)));
	m_triangles->push_back(new cTriangle(cVector(0.0, m_width, 0.0, 1.0), cVector(0.0, m_width, m_height, 1.0), cVector(0.0, 0.0, m_height, 1.0)));

	// Side 2
	m_triangles->push_back(new cTriangle(cVector(0.0, 0.0, 0.0, 1.0), cVector(0.0, 0.0, m_height, 1.0), cVector(m_length, 0.0, m_height, 1.0)));
	m_triangles->push_back(new cTriangle(cVector(0.0, 0.0, 0.0, 1.0), cVector(m_length, 0.0, m_height, 1.0), cVector(m_length, 0.0, 0.0, 1.0)));

	// Side 3
	m_triangles->push_back(new cTriangle(cVector(m_length, 0.0, 0.0, 1.0), cVector(m_length, 0.0, m_height, 1.0), cVector(m_length, m_width, 0.0, 1.0)));
	m_triangles->push_back(new cTriangle(cVector(m_length, m_width, 0.0, 1.0), cVector(m_length, 0.0, m_height, 1.0), cVector(m_length, m_width, m_height, 1.0)));

	// Side 4
	m_triangles->push_back(new cTriangle(cVector(m_length, m_width, 0.0, 1.0), cVector(m_length, m_width, m_height, 1.0), cVector(0.0, m_width, 0.0, 1.0)));
	m_triangles->push_back(new cTriangle(cVector(0.0, m_width, 0.0, 1.0), cVector(m_length, m_width, m_height, 1.0), cVector(0.0, m_width, m_height, 1.0)));

	for (auto i : *m_triangles) {
		i->ApplyMatrix(m_matrix);
	}

}

testdll::cBox::cBox(const cPoint& p1, const cPoint& p2, const cPoint& p3)
{
	m_length = p2.m_x - p1.m_x;
	m_width = p2.m_y - p1.m_y;
	m_height = p3.m_z - p1.m_z;
	m_matrix.Move(p1);
	*m_name = "Box" + std::to_string(m_id);
	CreateMesh();
}

// cCamera class functions
///////////////////////////

testdll::cCamera::cCamera(cPoint& pos, int dist) :cCamera(pos.m_x, pos.m_y, pos.m_z, dist) {}

testdll::cCamera::cCamera(double x, double y, double z, int dist)
{
	m_camera_position = cPoint(x, y, z);
	m_camera_plan = dist;

	create_view_matrix();
}

testdll::cCamera::cCamera(double x, double y, double z, int dist, int observe) :cCamera(x, y, z, dist)
{
	m_observe_angle = (observe * Pi) / 180.0;
}

void testdll::cCamera::CreateMesh() {

	create_view_matrix();

}

void testdll::cCamera::create_view_matrix()
{
	cMatrix temp = m_matrix;
	temp.Rotate(-90, -90, 0);
	temp.Move(m_camera_position);
	m_view = temp.get_invert();
}




cVector testdll::cCamera::ScreenProjection(const cVector& vec) const
{
	cVector temp = m_view * vec;
	double delta = (abs(temp.m_k) > 0.000001) ? (m_camera_plan / temp.m_k) * m_scale : 1.0 * m_scale;
	temp.m_i = temp.m_i * delta + m_screen_width / 2.0;
	temp.m_j = temp.m_j * delta + m_screen_height / 2.0;
	temp.m_k = 0.0;


	return temp;
}

void testdll::cCamera::SetScreenRect(int width, int height)
{
	m_screen_width = width;
	m_screen_height = height;
	m_scale = m_screen_width / ((2.0 * m_camera_plan) * tan(m_observe_angle / 2));
}

void testdll::cCamera::Move(const cPoint& distance)
{
	m_camera_position.m_x += distance.m_x;
	m_camera_position.m_y += distance.m_y;
	m_camera_position.m_z += distance.m_z;

	create_view_matrix();
}

void testdll::cCamera::Rotate(double A, double B, double C)
{
	m_matrix.Rotate(A, B, C);
	create_view_matrix();
}

void testdll::cCamera::Rotate(const cVector& vec, double angle)
{
	m_matrix.Rotate(vec, angle);
	create_view_matrix();
}

void testdll::cCamera::Rotate(const cVector& vec, const cPoint& pt, double angle)
{
	m_matrix.Rotate(vec, pt, angle);
	create_view_matrix();
}

void testdll::cCamera::SetObserveAngle(int angle)
{
	m_observe_angle = (angle * Pi) / 180.0;
}

void testdll::cCamera::SetCameraPlan(int distance)
{
	if (m_camera_plan <= 0) {
		m_camera_plan = 0;
		return;
	}
	m_camera_plan = distance;
}

int testdll::cCamera::GetObserveAngle()
{
	return (int)((m_observe_angle * 180.0) / Pi);
}

int testdll::cCamera::GetDistance()
{
	return m_camera_plan;
}

int testdll::cCamera::GetScreenWidth()
{
	return m_screen_width;
}

int testdll::cCamera::GetScreenHeight()
{
	return m_screen_height;
}

// cScene class functions
///////////////////////////

HDC testdll::cScene::GetBuffer(HDC hdc)
{
	if (m_memDC) {
		return m_memDC;
	}

	m_memDC = CreateCompatibleDC(hdc);

	m_backbuffer = CreateCompatibleBitmap(hdc, m_screen_cam.GetScreenWidth(), m_screen_cam.GetScreenHeight());

	SelectObject(m_memDC, m_backbuffer);

	return m_memDC;
}

testdll::cScene::cScene() {
	m_geometry = new std::vector<cBaseGeometry*>();
	m_curptr = nullptr;
	m_curpos = -1;
}


testdll::cScene::~cScene()
{
	for (auto& i : *m_geometry) {
		delete i;
	}
	m_geometry->clear();
	delete m_geometry;

	if (m_memDC) {
		DeleteDC(m_memDC);
		m_memDC = NULL;
	}
	if (m_backbuffer) {
		DeleteObject(m_backbuffer);
		m_backbuffer = NULL;
	}
}

void testdll::cScene::SetNext()
{
	if (m_geometry->empty()) return;
	m_curptr->SetHighlight(false);
	m_curpos++;
	if (m_curpos == m_geometry->size()) {
		m_curpos = 0;
	}
	m_curptr = m_geometry->at(m_curpos);
	m_curptr->SetHighlight(true);
}

void testdll::cScene::SetPrev()
{
	if (m_geometry->empty()) return;
	m_curptr->SetHighlight(false);
	m_curpos--;
	if (m_curpos < 0) {
		m_curpos = m_geometry->size() - 1;
	}
	m_curptr = m_geometry->at(m_curpos);
	m_curptr->SetHighlight(true);
}

void testdll::cScene::GetAt(int id)
{
	size_t i = 0;
	m_curptr->SetHighlight(false);
	for (i = 0; i < m_geometry->size(); i++) {
		if (m_geometry->at(i)->GetId() == id)break;
	}
	m_curpos = i;
	m_curptr = m_geometry->at(i);

}

void testdll::cScene::GetAt(std::string name)
{
	size_t i = 0;
	m_curptr->SetHighlight(false);
	for (i = 0; i < m_geometry->size(); i++) {
		if (m_geometry->at(i)->GetName().compare(name) == 0)break;
	}
	m_curpos = i;
	m_curptr = m_geometry->at(i);
}

void testdll::cScene::DeleteCurrent()
{

	if (m_curpos < 0)return;
	if (m_curptr->GetId() == 1)return;
	if (m_curptr) {
		m_curptr->SetHighlight(false);
		delete m_curptr;
	}
	m_geometry->erase(m_geometry->begin() + m_curpos);
	if (m_geometry->empty()) {
		m_curptr = nullptr;
		m_curpos = -1;
		return;
	}

	m_curpos = 0;
	m_curptr = m_geometry->at(0);

}

void testdll::cScene::SetColor(int R, int G, int B)
{
	m_curptr->SetColor(R, G, B);
}

void testdll::cScene::AddLSys(const cPoint& orig, const cPoint& X, const cPoint& Y)
{
	if (orig.is_equal(X) || orig.is_equal(Y) || X.is_equal(Y))return;

	if (m_curptr) {
		m_curptr->SetHighlight(false);
	}
	m_curptr = new cLSYS(orig, X, Y);
	m_geometry->push_back(m_curptr);
	m_curpos = m_geometry->size() - 1;
	m_curptr->SetHighlight(true);

}

void testdll::cScene::AddSphere(const cPoint& orig, double rad)
{
	if (rad == 0)return;
	if (m_curptr) {
		m_curptr->SetHighlight(false);
	}
	m_curptr = new cSphere(orig, rad);
	m_geometry->push_back(m_curptr);
	m_curpos = m_geometry->size() - 1;
	m_curptr->SetHighlight(true);
}

void testdll::cScene::AddSphere(const cPoint& orig, const cPoint& on_surf)
{
	if (orig.is_equal(on_surf))return;
	if (m_curptr) {
		m_curptr->SetHighlight(false);
	}
	m_curptr = new cSphere(orig, on_surf);
	m_geometry->push_back(m_curptr);
	m_curpos = m_geometry->size() - 1;
	m_curptr->SetHighlight(true);
}

void testdll::cScene::AddBox(const cPoint& first, const cPoint& second, const cPoint& third)
{
	if (first.is_equal(second) || first.is_equal(third) || second.is_equal(third))return;
	if (m_curptr) {
		m_curptr->SetHighlight(false);
	}
	m_curptr = new cBox(first, second, third);
	m_geometry->push_back(m_curptr);
	m_curpos = m_geometry->size() - 1;
	m_curptr->SetHighlight(true);
}

void testdll::cScene::SetScreenRest(int width, int height)
{
	m_screen_width = width;
	m_screen_height = height;
	m_screen_cam.SetScreenRect(m_screen_width, m_screen_height);
}

void testdll::cScene::SetCameraPlane(int z)
{
	m_camera_plan = z;
	m_screen_cam.SetCameraPlan(m_camera_plan);
}

void testdll::cScene::MoveCameraPlane(int delta)
{
	m_screen_cam.SetCameraPlan(m_screen_cam.GetDistance() + delta);
}

void testdll::cScene::SetCameraObserveAngle(int angle)
{
	m_screen_cam.SetObserveAngle(angle);
}

void testdll::cScene::ChangeCameraObserveAngle(int delta)
{
	m_screen_cam.SetObserveAngle(m_screen_cam.GetObserveAngle() + delta);
}

cCamera& testdll::cScene::GetCamera()
{
	return m_screen_cam;
}


void testdll::cScene::MoveCamera(const cPoint& point)
{
	m_screen_cam.Move(point);
}

void testdll::cScene::RotateCamera(const cPoint& angles)
{
	m_screen_cam.Rotate(angles.m_x, angles.m_y, angles.m_z);
}

void testdll::cScene::DrawGeometry(HDC hdc)
{
	if (m_screen_cam.GetScreenWidth() == 0 || m_screen_cam.GetScreenHeight() == 0) return;

	// Create compatible DC for drawing
	HDC DC = GetBuffer(hdc);
	//
	RECT rt = {
		0,0,m_screen_cam.GetScreenWidth(),m_screen_cam.GetScreenHeight()
	};
	FillRect(DC, &rt, (HBRUSH)::GetStockObject(WHITE_BRUSH));

	for (auto& i : *m_geometry) {
		i->Draw(DC/*hdc*/, *this);
	}

	BitBlt(hdc, 0, 0, m_screen_cam.GetScreenWidth(), m_screen_cam.GetScreenHeight(), DC, 0, 0, SRCCOPY);
}

cVector testdll::cScene::ScreenProjection(const cVector& vec)
{
	return m_screen_cam.ScreenProjection(vec);
}


void testdll::cScene::MoveCurrent(const cPoint& point) {
	if (m_curptr) {
		m_curptr->Move(point);
	}
}


void testdll::cScene::RotateCurrent(const cPoint& point) {
	if (m_curptr) {
		m_curptr->Rotate(point.m_x, point.m_y, point.m_z);
	}
}

void testdll::cScene::RotateAround(const cVector& vec, const cPoint& pt, double angle) {
	if (m_curptr) {
		m_curptr->Rotate(vec,pt,angle);
	}
}