#pragma once

#include <vector>
#include<iostream>
#include<Windows.h>



#ifdef TESTDLL_EXPORTS
#define TESTDLL_API __declspec(dllexport)
#else
#define TESTDLL_API __declspec(dllimport)
#endif

namespace testdll {

	class TESTDLL_API cScene;
	struct TESTDLL_API cMatrix;


	// Constants
	const double Pi = 3.1415926;

	struct TESTDLL_API Color {
		unsigned short R;
		unsigned short G;
		unsigned short B;
	};

	// auxiliary structs

	struct TESTDLL_API cPoint {
		double m_x = 0.0;
		double m_y = 0.0;
		double m_z = 0.0;

		cPoint() {};
		cPoint(double x, double y, double z);
		bool is_equal(const cPoint& pt2) const;
		void ApplyMatrix(const cMatrix& mat);
	};


	// cVector of 4 elements X,Y,Z,w
	struct TESTDLL_API cVector {
		double m_i = 0.0;
		double m_j = 0.0;
		double m_k = 0.0;
		double m_w = 1.0;

		cVector() {}
		cVector(double x, double y, double z, double w);
		cVector(const cPoint& begin, const cPoint& end);
		cVector(const cPoint& pt, double w);
		double Length() const;
		static double Length(const cVector& vector);
		void Normalize();
		cVector& operator=(const cVector& vec);

		// cVector production
		cVector prod(const cVector& second) const;
		// Scalar vector production
		double ScalarProd(const cVector& second) const;

		cVector operator*(double val) const;
		double GetAngleBetween(const cVector& second) const;
		cPoint GetPoint() const;
		void ApplyMatrix(const cMatrix& mat);
	};

	struct TESTDLL_API cTriangle {
		cVector p1{};
		cVector p2{};
		cVector p3{};

		cTriangle() {};
		cTriangle(const cVector& pt1, const cVector& pt2, const cVector& pt3) :p1{ pt1 }, p2{ pt2 }, p3{ pt3 }{};
		void ApplyMatrix(const cMatrix& mat);
		cVector GetNormal() const;
	};


	struct TESTDLL_API cMatrix {
		double m[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };

		cMatrix operator*(const cMatrix& m2) const;
		cVector operator*(const cVector& vec) const;
		cMatrix& operator=(const cMatrix& m2);

		// Add moving to matrix
		void Move(const cPoint& distance);
		void Move(double x, double y, double z);
		// Add to matrix rotation around zero position
		void Rotate(double A, double B, double C);
		// Add to matrix rotation around set vector
		void Rotate(const cVector& vec, double angle);
		// Add to matrix rotation around vector and point
		void Rotate(const cVector& vec, const cPoint& pt, double angle);
		// Get inversed matrix
		cMatrix get_invert() const;
	};



	// Base class for all geometry classes in the hierarchy
	class TESTDLL_API cBaseGeometry
	{
	protected:
		static int common_Id;

		// Id of geometry. Every geometry has an individual ID
		int m_id = 0;

		// Name of geometry
		std::string* m_name;

		// cMatrix of position and rotation in the scene
		cMatrix m_matrix = cMatrix();

		//geometry_types m_type = geometry_types::T_Undefined;

		//Points of mesh
		std::vector<cTriangle*>* m_triangles;

		// Edge vectors
		cVector m_edge1, m_edge2;

		// Temp normal vector of triangle
		cVector m_norm;

		// Vector of camera
		// Always equal to Z axis vector (at screen view)
		cVector m_view_v = cVector(0.0, 0.0, 1.0, 1);


		// Properties for show
		// Width of pen
		int m_pen_width = 1;

		// Color of shape
		COLORREF m_color = RGB(0, 0, 0);

		// Highlight color

		COLORREF m_highlight = RGB(255, 202, 66);

		// Is geometry highlight
		bool m_ishighlight = false;

		// Base class creation disabled
		cBaseGeometry();

		// CreateMesh function creates all points of geometry triangles
		// and apply a position matrix to it.
		virtual void CreateMesh() = 0;

		// Remove all triangles from mesh array.
		void ClearMesh();

	public:
		int GetId();
		const std::string& GetName();
		void SetHighlight(bool state);
		bool IsHighlight();
		void SetColor(int R, int G, int B);
		// Object rotation around origin
		virtual void Rotate(double A, double B, double C);
		// Object rotation around given vector and origin 
		virtual void Rotate(const cVector& vec, double angle);
		// Object rotation around given vector and given point 
		virtual void Rotate(const cVector& vec, const cPoint& pt, double angle);
		// Moving the project
		virtual void Move(const cPoint& distance);
		// Draw object
		virtual void Draw(HDC hdc, cScene& scene);

		virtual ~cBaseGeometry();
	};

	// cLSYS - Local coordinate system
	class TESTDLL_API cLSYS : public cBaseGeometry {
	private:
		cVector m_outpoints[4]{};
		int m_length = 200;
		cVector m_vectors[3];
		virtual void CreateMesh() override;
	public:
		cLSYS(const cPoint& orig, const cPoint& X, const cPoint& Y);
		virtual void Draw(HDC hdc, cScene& scene) override;
	};

	// cSphere geometry
	class TESTDLL_API cSphere : public cBaseGeometry {
	private:
		cPoint m_center = cPoint();
		double m_radius = 0.0;
		virtual void CreateMesh() override;
	public:
		cSphere(const cPoint& center, const double radius);
		cSphere(const cPoint& center, const cPoint& s_point);


	};

	// cBox class
	class TESTDLL_API cBox : public cBaseGeometry {
	private:
		double m_length = 0.0;
		double m_width = 0.0;
		double m_height = 0.0;
		virtual void CreateMesh() override;
	public:
		// Set the parameters of new cBox
		// The first and the second point should be 
		// on a Z plane. cBox sides are paraller to the
		// world coondinate system axis.
		cBox(const cPoint& p1, const cPoint& p2, const cPoint& p3);
	};


	// cCamera class
	// Convert 3d points to view plane
	class TESTDLL_API cCamera :public cBaseGeometry {
		// cMatrix of view
		cMatrix m_view{};

		// Parameters of camera
		int m_camera_plan = 60;
		double m_scale = 1;
		double m_observe_angle = (30 * Pi) / 180.0;
		cPoint m_camera_position;
		// View parameters
		int m_screen_width = 0;
		int m_screen_height = 0;
		// Create view matrix
		void create_view_matrix();
		// Update view matrix of camera 
		virtual void CreateMesh() override;

	public:
		cCamera(cPoint& pos, int dist);
		cCamera(double x, double y, double z, int dist);
		cCamera(double x, double y, double z, int dist, int observe);

		void SetScreenRect(int width, int height);

		virtual void Move(const cPoint& distance) override;
		virtual void Rotate(double A, double B, double C) override;
		virtual void Rotate(const cVector& vec, double angle) override;
		virtual void Rotate(const cVector& vec, const cPoint& pt, double angle) override;
		cVector ScreenProjection(const cVector& vec) const;
		void SetObserveAngle(int angle);
		void SetCameraPlan(int distance);
		int GetObserveAngle();

		int GetDistance();
		int GetScreenWidth();
		int GetScreenHeight();


	};



	// Class cScene represent 3d world, includes all geometry objects
	// and functions for adding, removing and show objects
	class TESTDLL_API cScene {
	private:
		cCamera m_screen_cam{ 2300,-80,150,50 };
		int m_screen_width = 0;
		int m_screen_height = 0;
		int m_camera_plan = 1;

		// Memory DC
		HDC m_memDC;
		HBITMAP m_backbuffer;
		HDC GetBuffer(HDC hdc);


		std::vector<cBaseGeometry*> *m_geometry;
		//std::vector<cBaseGeometry*>::iterator* m_current;
		cBaseGeometry* m_curptr;
		size_t m_curpos = 0;




	public:
		cScene();
		~cScene();

		// Work with iterator of current geometry
		//cBaseGeometry* GetCurrent();
		void SetNext();
		void SetPrev();
		void GetAt(int id);
		void GetAt(std::string name);
		void DeleteCurrent();
		void SetColor(int R, int G, int B);
		void MoveCurrent(const cPoint& point);
		void RotateCurrent(const cPoint& point); 
		void RotateAround(const cVector& vec, const cPoint& pt, double angle);

		// Adding geometry
		void AddLSys(const cPoint& origin, const cPoint& X, const cPoint& Y);
		void AddSphere(const cPoint& orig, double rad);
		void AddSphere(const cPoint& orig, const cPoint& on_surf);
		void AddBox(const cPoint& first, const cPoint& second, const cPoint& third);

		// Work with camera
		void SetScreenRest(int width, int height);
		void SetCameraPlane(int z);
		void MoveCameraPlane(int delta);
		void SetCameraObserveAngle(int angle);// Degrees
		void ChangeCameraObserveAngle(int delta);//Degrees
		cCamera& GetCamera();
		void MoveCamera(const cPoint& point);
		void RotateCamera(const cPoint& angles);

		// Draw all geometry
		void DrawGeometry(HDC hdc);

		cVector ScreenProjection(const cVector& vec);
	};


}

