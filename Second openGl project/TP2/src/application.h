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
		void updateRenderer3DVars();
		void updateRendererVars();

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
		ofxButton button_collection;
		ofxButton button_curves;
		ofxButton button_surfaces;
		ofxButton button_threed;

		void button_pressed_collection();
		void button_pressed_curves();
		void button_pressed_surfaces();
		void button_pressed_3D();


		// ====================== Card Preview ======================
		ofxPanel cardPreviewGUI;
		ofParameter<void> button_tone_filter;
		ofParameter<void> button_image_filter;


		//Tone Mapping
		void button_pressed_tone_filter();
		void updateSliderType();

		ofxPanel toneMapping;
		ofParameter<float> slider_exposure;
		ofParameter<float> slider_gamma;
		ofParameter<int> slider_mapping_type;

		//Filtering
		void button_pressed_image_Filter();
		void button_pressed_ExposureFilter();
		void button_pressed_InvertFilter();
		void button_pressed_convolution_identity();
		void button_pressed_convolution_emboss();
		void button_pressed_convolution_sharpen();
		void button_pressed_convolution_edge_detect();
		void button_pressed_convolution_blur();

		ofxPanel imageFilter;
		ofParameter<void> button_filter1;
		ofParameter<void> button_filter2;
		ofParameter<void> button_convolution_identity;
		ofParameter<void> button_convolution_emboss;
		ofParameter<void> button_convolution_sharpen;
		ofParameter<void> button_convolution_edge_detect;
		ofParameter<void> button_convolution_blur;

		ofParameter<ofColor> color_picker;
		ofParameter<float> slider;

		// ====================== Utils =====================
		void setupDebug();

		// ================= Lights ==================
		ofxPanel gui_Lights;
		ofParameter <ofVec3f> guiLightPosition;
		ofParameter <ofFloatColor> ambientColor;
		ofParameter<ofColor> directionalColor;
		ofxGuiGroup spot_light_group;
		ofParameter<int> concentration;
		ofParameter<int> cutoff;

		ofxPanel gui_Material;
		ofParameter<int> shininess;
		ofParameter<ofColor> specular_color;
		ofParameter<ofFloatColor> diffuse_color;
		ofParameter<ofFloatColor> ambient_color;

		// ================== Shaders ================
		ofParameter<int> slider_shader_type;
		void updateSliderShader3D();

		// ================== PBR =====================
		void Application::reset_pbr();
		ofParameter<void> button_pbr_toggle;
		void button_pressed_pbr();

		ofxPanel pbr_panel;

		ofxGuiGroup group_material_color;
		ofxGuiGroup group_material_factor;
		ofxGuiGroup group_light;
		ofxGuiGroup group_tone_mapping;

		ofParameter<ofColor> color_picker_ambient;
		ofParameter<ofColor> color_picker_diffuse;
		ofParameter<ofColor> color_picker_specular;

		ofParameter<float> slider_metallic;
		ofParameter<float> slider_roughness;
		ofParameter<float> slider_occlusion;
		ofParameter<float> slider_brightness;

		ofParameter<glm::vec3> slider_fresnel_ior;
		ofParameter<float> slider_light_intensity;

		ofParameter<float> slider_exposure_3D;
		ofParameter<float> slider_gamma_3D;

		ofParameter<void> button_reset_pbr;
		void button_pressed_reset_pbr();

		// ====================== 3D Interface ======================
		ofxButton button_twod;
		ofxButton button_box;
		ofxButton button_cylinder;
		ofxButton button_cone;
		ofxButton button_sphere;
		ofxPanel gui3D;
		ofxButton button_cam_mode;
		ofxButton button_reset_cam;
		ofxButton button_help;

		bool help_on = false;

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
		ofxButton add_geo_button;
	
		ofParameter<float> cam_speed;
		ofParameter<int> cam_fov;

		ofMesh mesh;

		void button_pressed_2D();
		void button_pressed_camswitch();
		void button_pressed_addgeo();
		void button_pressed_none();
		void button_addbox();
		void button_addcylinder();
		void button_addcone();
		void button_addsphere();
		void button_pressed_reset_cam();
		void button_pressed_help();

		//============= curves ====================

		float time_current;
		float time_last;
		float time_elapsed;

		bool is_key_press_up;
		bool is_key_press_down;
		bool is_key_press_left;
		bool is_key_press_right;



};