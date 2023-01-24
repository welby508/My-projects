#include "ofMain.h"
#include "utils.h"
#include "ofxAssimpModelLoader.h"

#define N_CAMERAS 5

class Renderer3D
{
public:


	void setup();
	void update();
	void draw();

	ofTrueTypeFont fontText;

	//Plane	
	ofPlanePrimitive plane;
	ofTexture board;

	void setupPlane();

	//Setup Geo
	objectType3D preview_mode;
	object3D* geo;
	object3D* oldGeo;
	int index;
	int bufferCount;
	int bufferStride;
	int bufferSize;
	int bufferHead;

	void setupGeo();

	//Setup Model
	ofxAssimpModelLoader model;
	ofxAssimpModelLoader model2;
	ofxAssimpModelLoader model3;
	ofxAssimpModelLoader model4;
	int model_selection_slider;
	vector<ofxAssimpModelLoader> vmodel;

	void setupModel();


	//Setup MultipleViewPort Cam
	ofVec3f camera_target;
	ofVec3f box_pos;
	ofCamera camFront;
	ofCamera camBack;
	ofCamera camTop;
	ofCamera camLeft;
	ofCamera camRight;
	ofCamera* cameras[N_CAMERAS];
	int iMainCamera;

	void setupMulticam();

	//Setup//Draw Viewports
	ofRectangle viewMain;
	ofRectangle viewGrid[N_CAMERAS];

	void setupViewports();
	void drawViewportCamera();

	//Setup/Reset FreeCam
	ofCamera freecamera;
	float cam_speed;
	int cam_fov;
	int cam_snap;
	bool is_camera_move_left;
	bool is_camera_move_right;
	bool is_camera_move_forward;
	bool is_camera_move_backward;
	bool is_camera_move_up;
	bool is_camera_move_down;
	bool is_camera_pan_left;
	bool is_camera_pan_right;
	bool is_camera_tilt_up;
	bool is_camera_tilt_down;

	void resetcam();

	//Snap Cam / Projection Style / Cam Mode
	bool snap_active;
	int current_snap = 0;
	int previous_snap = 1;
	bool projection_o = false;
	camState camstate = multi;

	void snapcamera();
	void updatecam();
	void fcamproject();

	// Update Batch Selection
	void updateBatchTransformation();

	//Draw Scene / Draw Geo
	ofBoxPrimitive box;
	ofCylinderPrimitive cylinder;
	ofConePrimitive cone;
	ofSpherePrimitive sphere;
	ofColor geo_color;
	float geo_height;
	float geo_radius;
	ofVec3f vector_position;
	ofVec3f vector_proportion;
	ofVec3f vector_rotation;
	bool delim_on = false;

	void drawScene();
	void drawGeo();
	void drawGeoContour(int i);
	void previewGeo();
	void previewContour();
	void drawDelim(objectType3D type);
	void drawTetrahedron(float x, float y, float z, float s, float h);
	void drawOctahedron(float x, float y, float z, float s, float h);
	void drawDodecahedron(float x, float y, float z, float r);


	//Util 
	ofMesh mesh;
	bool help_on = false;

	float Renderer3D::getMinyGeo(ofMesh mesh);
	float Renderer3D::getMiny(ofxAssimpModelLoader model);
	void addGeo(objectType3D type);
	void undo();
	void drawhelp();

	//Selection
	int mouse_current_x;
	int mouse_current_y;
	int geoCount;
	float batch_slider_radius;
	float batch_slider_height;
	ofVec3f batch_vector_position;
	ofVec3f batch_vector_proportion;
	ofVec3f batch_vector_rotation;

	void selectClosestGeo();
	bool isGeoSelected();
	void copyGeoVector();
	int getClosestGeoIndex();
	void resetGeo(int i);
	void resetGeoSelection();
	int getIndexFirstEmptyGeo();
	void deleteGeo();

	~Renderer3D();
};