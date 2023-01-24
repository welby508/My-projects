#include "ofMain.h"
#include "utils.h"


class Renderer
{
public:

	//Static images
	ofImage background;
	ofImage empty;
	ofColor canvas;
	ofColor shape_color;
	ofColor stroke_color;
	ofImage selectBorder;
	ofImage previewCard;
	ofImage cardSelected;

	//State handling
	rendererStates state = idle;
	bool updateOnce = false;

	//---------------- Core functions ----------------
	void setup();
	void update();
	void draw();

	~Renderer();

	//---------------- collection and draw single card ----------------
	vector<ofImage> vCardCollection;

	void drawCardPreview();
	void LoadCardsFromFolder();
	void drawCollection(const int x0, const int y0, const int w, const int h, const int row, const int col, const vector<ofImage> &vCardCollection, const int padding = 0);
	void drawCard(const ofImage &image, const int x, const int y, const int w, const int h);
	bool mouseOnCard(const int x0, const int y0, const int x1, const int y1, const int h, const int w);

	//---------------- tone_mapping ---------------------
	bool toneMapping = false;
	void applyToneMapping();

	ofShader shaderMap;
	float tone_mapping_exposure;
	float tone_mapping_gamma;
	int tone_mapping_style;


	//--------------- Filters -------------------------

	bool imageFilters = false;
	bool updateFiltedImage = true;
	void renderFilteredImage();
	ofImage filteredImage;

	float mix_factor;
	ofColor tint;

	ofShader shader1;
	ofShader shader2;


	bool shader1_IsActive = false;
	bool shader2_IsActive = false;
	bool shader3_IsActive = false;
	bool convolutionFilter_IsActive[5] = { false,false,false,false,false };

	ConvolutionKernel kernel_type[5] = { ConvolutionKernel::identity, 
										 ConvolutionKernel::emboss,
										 ConvolutionKernel::sharpen,
										 ConvolutionKernel::edge_detect,
										 ConvolutionKernel::blur};

	//---------------- Utils ----------------

	// Mouse handling
	bool is_mouse_button_pressed = false;
	int mouse_press_x;
	int mouse_press_y;
	int mouse_current_x;
	int mouse_current_y;

	//-------------- Curves ------------------

	void setupBezier();
	void updateBezier();
	void drawBezier();
	void resetBezier();
	void bezier_quadratic(float t, float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float p3x, float p3y, float p3z, float& x, float& y, float& z);

	ofPolyline line_renderer;

	ofVec3f* selected_ctrl_point;

	ofVec3f ctrl_point1;
	ofVec3f ctrl_point2;
	ofVec3f ctrl_point3;
	ofVec3f ctrl_point4;
	ofVec3f ctrl_point5;

	ofVec3f initial_position1;
	ofVec3f initial_position2;
	ofVec3f initial_position3;
	ofVec3f initial_position4;
	ofVec3f initial_position5;

	ofVec3f position;

	float radius;

	int framebuffer_width;
	int framebuffer_height;

	int line_resolution;

	int index;

	float delta_x;
	float delta_y;

	//============= surfaces ====================


	void bezier_cubic(float t, float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float p3x, float p3y, float p3z, float p4x, float p4y, float p4z, float p5x, float p5y, float p5z, float& x, float& y, float& z);

	//ofVec3f bezier_cubic_X(float t, float s);
	//ofVec3f bezier_cubic_Y(float t, float s);
	//ofVec3f bezier_cubic_corners(float s, float t);
	//ofVec3f bezier_cubic_corners();
	//ofVec3f bezier_cubic_bilinearBlend(float t, float s);
	//ofVec3f bezier_cubic_bilinearBlend();
	void setupCoonSurface();
	void updateCoonSurface();
	void drawCoonSurface();
	void resetCoonSurface();

	ofMesh mainMesh;
	ofEasyCam mainCam;

	ofVec3f* selected_ctrl_point_coon;

	//initial position of the 4 corners
	ofVec3f initial_position1_coon;
	ofVec3f initial_position2_coon;
	ofVec3f initial_position3_coon;
	ofVec3f initial_position4_coon;

	//Points in between each corners
	ofVec3f initial_position12_coon;
	ofVec3f initial_position13_coon;
	ofVec3f initial_position24_coon;
	ofVec3f initial_position34_coon;

	//Actual postion of each corners
	ofVec3f ctrl_point1_coon;
	ofVec3f ctrl_point2_coon;
	ofVec3f ctrl_point3_coon;
	ofVec3f ctrl_point4_coon;

	//Actual postion of the control points between each corners
	ofVec3f ctrl_point12_coon;
	ofVec3f ctrl_point13_coon;
	ofVec3f ctrl_point24_coon;
	ofVec3f ctrl_point34_coon;

	float width, height;

};

