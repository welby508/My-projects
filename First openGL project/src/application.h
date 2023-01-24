#pragma once

#include "ofMain.h"
#include "renderer.h"
#include "renderer3d.h"
#include "ofxGui.h"
#include "utils.h"

class Application : public ofBaseApp{

	public:

		bool debug_ON = false;

		Renderer renderer;
		Renderer3D renderer3d;
		ofImage hscursor;

		appStates  astate = state2D;

		//==================== Core Functions =================
		void draw();
		void drawGUI();
		void DrawHScursor();

		void setup();
		void setupGUI();

		void update();
		void updateRendererVars();
		void updateRenderer3DVars();

		//====================== events ======================
		void keyPressed(int key);
		void keyReleased(int key);
		void dragEvent(ofDragInfo dragInfo);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseScrolled(int x, int y, float scrollX, float scrollY);

		//====================== Mainmenu ======================
		ofxPanel gui;
		ofxButton button_addcard;
		ofxButton button_collection;
		ofxButton button_createcard;
		ofxButton button_random;
		ofxButton button_threed;

		void button_pressed_collection();
		void button_pressed_createcard();
		void button_pressed_randomcard();
		void button_pressed_3D();

		//====================== Collection ======================
		ofxButton button_SaveCollectionToFolder;

		void button_pressed_saveCardToCollection();

		ofxPanel canvasGUI;
		ofxGuiGroup group_color;
		ofParameter<ofColor> color_canvas;
		ofParameter<ofColor> color_shape;
		ofParameter<ofColor> color_stroke;
		ofParameter<float> slider_stroke_weight;

		//shapes
		ofxGuiGroup group_shape;
		ofxButton button_line;
		ofxButton button_rectangle;
		ofxButton button_parral;
		ofxButton button_triangle;
		ofxButton button_triangle_rectangle;
		ofxButton button_ellipse;

		//util
		ofxPanel group_utilCanvas;
		ofxButton button_reset;
		ofxButton button_undo;
		ofxButton button_redo;
	
		// ====================== Random Card ======================
		ofxPanel randomCardGUI;
		ofParameter<void> button_saveCardToCollection;
		ofParameter<void> button_saveToPath;

		// ====================== Card Preview ======================
		ofxPanel cardPreviewGUI;
		ofxButton button_histo;
		ofParameter<int> slider_hist;

		// histograms
		void button_pressed_hist();
		
		//====================== Create card ======================
		void selectShape();
		void button_pressed_line();
		void button_pressed_rectangle();
		void button_pressed_parral();
		void button_pressed_triangle();
		void button_pressed_triangler();
		void button_pressed_ellipse();
		void button_pressed_reset();
		void button_pressed_undo();
		void button_pressed_redo();
		
		// ====================== Utils =====================
		void button_pressed_saveToFolder(); // used in Collection, Custom Card and Random Card
		void button_pressed_saveCustomCard();
		void button_pressed_saveRandomCard();
		void setupDebug();

		
		// ====================== 3D Interface ======================
		ofxButton button_twod;
		ofxButton button_none;
		ofxButton button_box;
		ofxButton button_tetra;
		ofxButton button_octa;
		ofxButton button_dodeca;
		ofxButton button_cylinder;
		ofxButton button_cone;
		ofxButton button_sphere;
		ofxPanel gui3D;
		ofxButton button_cam_mode;
		ofxButton button_reset_cam;
		ofxButton button_help;
		ofxButton button_3DModel;

		ofxPanel transform3D;
		ofxPanel util3D;
		ofxGuiGroup geo_group;
		ofParameter<ofColor> geo_color;
		ofParameter<float> slider_radius;
		ofParameter<float> slider_height;

		ofxGuiGroup group_pos;
		ofParameter<float> slider_x;
		ofParameter<float> slider_y;
		ofParameter<float> slider_z;

		ofxGuiGroup group_scale;
		ofParameter<float> scale_x;
		ofParameter<float> scale_y;
		ofParameter<float> scale_z;

		ofxGuiGroup group_rotat;
		ofParameter<int> rotat_x;
		ofParameter<int> rotat_y;
		ofParameter<int> rotat_z;
		ofParameter<int> model;
		ofxButton add_geo_button;
		ofxButton show_delim;
		ofxButton snap;
		ofxButton undo3d;
	
		ofParameter<float> cam_speed;
		ofParameter<int> cam_fov;
		ofxButton activ_snap;
		ofParameter<int> cam_snap;

		ofMesh mesh;

		ofxButton saveToPath;

		void selectGeo();
		void SnapGeo();
		void button_pressed_2D();
		void button_pressed_camswitch();
		void button_pressed_addgeo();
		void button_pressed_show_delim();
		void button_pressed_snap();
		void button_pressed_none();
		void button_addbox();
		void button_addtetra();
		void button_addocta();
		void button_adddodeca();
		void button_addcylinder();
		void button_addcone();
		void button_addsphere();
		void button_addmodel();
		void button_pressed_reset_cam();
		void button_pressed_snap_toggle();
		void button_pressed_help();
		void updateSnapCamSlider();
		void resetSelectionUIValues();

		// ====================== Batch transformation ======================
		ofxPanel batch_transformation;
		ofxGuiGroup batch_group_dimension;
		ofParameter<float> batch_slider_radius;
		ofParameter<float> batch_slider_height;

		ofxGuiGroup batch_group_pos;
		ofParameter<float> batch_slider_x;
		ofParameter<float> batch_slider_y;
		ofParameter<float> batch_slider_z;

		ofxGuiGroup batch_group_scale;
		ofParameter<float> batch_scale_x;
		ofParameter<float> batch_scale_y;
		ofParameter<float> batch_scale_z;

		ofxGuiGroup batch_group_rotat;
		ofParameter<int> batch_rotat_x;
		ofParameter<int> batch_rotat_y;
		ofParameter<int> batch_rotat_z;
};