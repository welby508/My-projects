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
	int index;
	int bufferCount;
	int bufferStride;
	int bufferSize;
	int bufferHead;

	void setupGeo();


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

	//Setup Lighting
	void setupLights();
	void setupMaterials();
	void activateLights();
	void deactivateLights();
	void updateLights();
	ofVec3f light_pos;

	//Material
	int shininess;
	ofColor specular_color;
	ofFloatColor diffuse_color;
	ofFloatColor ambient_color;
	ofMaterial boardMaterial;
	ofMaterial previewMaterial;
	ofMaterial geoMaterial;

	//Lights
	bool sideLightToggle;
	ofLight* lights[4];
	int mainLight;
	ofLight light_front_right;
	ofLight light_front_left;
	ofLight light_back_right;
	ofLight light_back_left;
	ofLight spotLight;
	bool spotToggle;
	ofFloatColor ambientLight;
	ofColor directionalColor;
	bool directionalToggle;
	ofLight directionalLight;

	int concentration;
	int cutoff;

	//Shader Illumination
	void setupShaders();
	void geoShaders(int i);
	void addUniformShaders(int i);
	void previewShaders();
	void addPreviewUniformShaders();
	void addLightsUniformShaders();
	ShaderType shader_active;
	ofShader* shader;
	ofShader lambert;
	ofShader gouraud;
	ofShader phong;

	//PBR
	void setupPBR();
	void resetPBR();
	void addPBRUniform(int i);
	bool pbr_toggle;
	ofShader pbr_shader;
	ofColor material_color_ambient;
	ofColor material_color_diffuse;
	ofColor material_color_specular;

	float material_metallic;
	float material_roughness;
	float material_occlusion;
	float material_brightness;

	glm::vec3 material_fresnel_ior;

	ofImage texture_diffuse1;
	ofImage texture_metal_rough_occ1;

	ofImage texture_diffuse2;
	ofImage texture_metal_rough_occ2;

	ofImage texture_diffuse3;
	ofImage texture_metal_rough_occ3;

	ofLight light;
	ofColor light_color;
	float light_intensity;

	float tone_mapping_exposure;
	float tone_mapping_gamma;

	//Snap Cam / Projection Style / Cam Mode
	camState camstate = multi;

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

	void drawScene();
	void drawGeo();
	void previewGeo();

	//Util 
	ofMesh mesh;

	void addGeo(objectType3D type);
	void drawhelp();

	//Selection
	int mouse_current_x;
	int mouse_current_y;
	int geoCount;


	~Renderer3D();
};