#include "renderer3d.h"

void Renderer3D::setup() {

	ofSetFrameRate(60);
	fontText.load(PATH_FONTS + "BelweBold.otf", 20);
	setupPlane();
	setupGeo();
	setupMulticam();
	setupViewports();
	setupLights();
	setupShaders();
	setupMaterials();
	setupPBR();
	resetcam();

}

//Function that sets the plane dimension/rotation and load the texture that will be used later on
void Renderer3D::setupPlane() {

	ofDisableArbTex();
	ofLoadImage(board, PATH_ASSETS + "board.png");
	plane.set(250, 140);
	plane.setPosition(0, 0, 0);
	plane.rotateDeg(-90, 1, 0, 0);

}

//Function that sets up the necessary variable for geos to be added
void Renderer3D::setupGeo()
{
	preview_mode = objectType3D::none;
	bufferCount = 100;
	bufferHead = 0;
	geoCount = 0;
	bufferStride = sizeof(object3D);
	bufferSize = bufferCount * bufferStride;
	geo = (object3D*)std::malloc(bufferSize);
	geoCount = 0;

	for (size_t i = 0; i < bufferCount; i++)
	{
		geo[i].type = objectType3D::none;
		geo[i].model_type = 0;
		geo[i].radius = 0;
		geo[i].height = 0;
		geo[i].vector_position = { 0,0,0 };
		geo[i].vector_proportion = { 0,0,0 };
		geo[i].vector_rotation = { 0,0,0 };
		
	}

}

//Function that sets the Multicam, their target and position in the scene
void Renderer3D::setupMulticam()
{
	iMainCamera = 0; //Default Cam set to 0 - camFront

	//Camera Positions
	camFront.panDeg(-45);
	camFront.setPosition(0, 150, 90);
	camBack.panDeg(45);
	camBack.setPosition(0, 150, -90);
	camTop.setPosition(0, 150, 0);
	camLeft.setPosition(-165, 65, 0);
	camRight.setPosition(165, 65, 0);


	//Setting the camera lookAt on the center of the scene
	camera_target = { 0,0,0 };

	camFront.lookAt(camera_target);
	camBack.lookAt(camera_target);
	camTop.lookAt(camera_target);
	camLeft.lookAt(camera_target);
	camRight.lookAt(camera_target);

	//Adding the camera in an array and setting their clip distance
	cameras[0] = &camFront;
	cameras[1] = &camBack;
	cameras[2] = &camTop;
	cameras[3] = &camLeft;
	cameras[4] = &camRight;

	for (size_t i = 0; i != N_CAMERAS; ++i) {
		cameras[i]->setNearClip(0.1);
		cameras[i]->setFarClip(100000);
	}

}

//Function that sets the dimension of the viewport (Main and Grid)
void Renderer3D::setupViewports() {

	float xOffset = ofGetWidth() / 3;
	float yOffset = ofGetHeight() / N_CAMERAS;

	//Sets the Main viewport
	viewMain.x = 0;
	viewMain.y = 0;
	viewMain.width = xOffset * 2;
	viewMain.height = ofGetHeight();

	//Sets the Grid viewports
	for (int i = 0; i < N_CAMERAS; i++) {

		viewGrid[i].x = ofGetWidth() - xOffset;
		viewGrid[i].y = yOffset * i;
		viewGrid[i].width = xOffset;
		viewGrid[i].height = yOffset;
	}

}

//Function to draw the view port camera and its scene
void Renderer3D::drawViewportCamera()
{
	//Draws the Scene in all the multiple viewport
	ofPushStyle();
	cameras[iMainCamera]->begin(viewMain);
	drawScene();
	cameras[iMainCamera]->end();


	for (int i = 0; i < N_CAMERAS; i++) {
		cameras[i]->begin(viewGrid[i]);
		drawScene();
		cameras[i]->end();
	}

	ofFill();
	ofSetColor(255, 255, 255, 50);
	ofDrawRectangle(viewGrid[iMainCamera]);
	ofPopStyle();

	//Draws the "HUD" interface around all the viewports and name them
	ofPushStyle();
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Player 1", viewGrid[0].x + 20, viewGrid[0].y + 30);
	ofDrawBitmapString("Player 2", viewGrid[1].x + 20, viewGrid[1].y + 30);
	ofDrawBitmapString("Top", viewGrid[2].x + 20, viewGrid[2].y + 30);
	ofDrawBitmapString("Left", viewGrid[3].x + 20, viewGrid[3].y + 30);
	ofDrawBitmapString("Right", viewGrid[4].x + 20, viewGrid[4].y + 30);

	ofSetLineWidth(5);
	ofNoFill();
	ofSetColor(255, 255, 255);

	for (int i = 0; i < N_CAMERAS; i++) {
		ofDrawRectangle(viewGrid[i]);
	}
	ofDrawRectangle(viewMain);
	ofPopStyle();


	for (size_t i = 0; i != N_CAMERAS; ++i) {
		cameras[i]->disableOrtho();
	}

}

//Function that sets the Free Cam at it's initial position/fov in the scene, used in Setup and other funcs
void Renderer3D::resetcam()
{
	freecamera.panDeg(-45);
	freecamera.setPosition(0, 150, 90);
	camera_target = { 0,0,0 };
	freecamera.lookAt(camera_target);

	is_camera_move_left = false;
	is_camera_move_right = false;
	is_camera_move_forward = false;
	is_camera_move_backward = false;
	is_camera_move_up = false;
	is_camera_move_down = false;
	is_camera_pan_left = false;
	is_camera_pan_right = false;
	is_camera_tilt_up = false;
	is_camera_tilt_down = false;

	cam_fov = 60;
}

//Function that sets the Directional Light attribute, sets the Light pointer array and Spotlight orientation
void Renderer3D::setupLights()
{
	directionalToggle = true;
	directionalLight.setDiffuseColor(ofColor(0.f, 255.f, 0.f));
	directionalLight.setSpecularColor(ofColor(0.f, 255.f, 0.f));
	directionalLight.setDirectional();
	directionalLight.setOrientation({ 0, 90, 0 });

	sideLightToggle = true;
	mainLight = 0;
	lights[0] = &light_back_left;
	lights[1] = &light_back_right;
	lights[2] = &light_front_left;
	lights[3] = &light_front_right;

	spotToggle = true;
	spotLight.setOrientation(ofVec3f(-90.0f, 0.0f, 0.0f));
	spotLight.setSpotlight();
}

//Function that sets up the Material used on the board
void Renderer3D::setupMaterials()
{
	boardMaterial.setShininess(120);
	boardMaterial.setSpecularColor(ofColor(255, 255, 255, 255));
	boardMaterial.setDiffuseColor(ofColor(255, 255, 255, 255));
}

//Function that loads the Shader
void Renderer3D::setupShaders()
{
	lambert.load(
		PATH_SHADERS + "Illum\\lambert_330_vs.glsl", 
		PATH_SHADERS + "Illum\\lambert_330_fs.glsl");

	gouraud.load(
		PATH_SHADERS + "Illum\\gouraud_330_vs.glsl",
		PATH_SHADERS + "Illum\\gouraud_330_fs.glsl");

	phong.load(
		PATH_SHADERS + "Illum\\phong_330_vs.glsl",
		PATH_SHADERS + "Illum\\phong_330_fs.glsl");

	pbr_shader.load(
		PATH_SHADERS + "PBR\\pbr_330_vs.glsl",
		PATH_SHADERS + "PBR\\pbr_330_fs.glsl");
}

//Function that sets up the basic different attribute for the PBR function and loads the Material
void Renderer3D::setupPBR()
{

	pbr_toggle = false;

	resetPBR();

	texture_diffuse1.load(PATH_ASSETS + "diffuse1.jpg");
	texture_metal_rough_occ1.load(PATH_ASSETS + "metal_rough_occ1.jpg");
	texture_diffuse1.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	texture_metal_rough_occ1.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);

	texture_diffuse2.load(PATH_ASSETS + "diffuse2.jpg");
	texture_metal_rough_occ2.load(PATH_ASSETS + "metal_rough_occ2.jpg");
	texture_diffuse2.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	texture_metal_rough_occ2.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);

	texture_diffuse3.load(PATH_ASSETS + "diffuse3.jpg");
	texture_metal_rough_occ3.load(PATH_ASSETS + "metal_rough_occ3.jpg");
	texture_diffuse3.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	texture_metal_rough_occ3.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
}


//Function that sets up attribute for the PBR and allows to reset them later on
void Renderer3D::resetPBR()
{
	material_color_ambient = ofColor(63, 63, 63);
	material_color_diffuse = ofColor(255, 255, 255);
	material_color_specular = ofColor(255, 255, 255);

	material_metallic = 0.5f;
	material_roughness = 0.5f;
	material_occlusion = 1.0f;
	material_brightness = 1.0f;

	material_fresnel_ior = glm::vec3(0.04f, 0.04f, 0.04f);

	light_color = ofColor(255, 255, 255);
	light_intensity = 0.15f;

}

//Function to determine which light is enabled
void Renderer3D::activateLights()
{
	ofSetGlobalAmbientColor(ambientLight);
	if (spotToggle == true)
	{
		spotLight.enable();
	}
	if (directionalToggle == true)
	{
		directionalLight.enable();
	}
	if (sideLightToggle == true)
	{
		lights[mainLight]->enable();
	}

}

//Function to disable lights at the end of the scene
void Renderer3D::deactivateLights()
{
	ofSetGlobalAmbientColor(ofColor(0, 0, 0, 0));
	spotLight.disable();
	directionalLight.disable();
	light_front_right.disable();
	light_front_left.disable();
	light_back_right.disable();
	light_back_left.disable();
}

void Renderer3D::update() {

	updateLights();

	//Updates the Fov for the Multicam view
	if (camstate == multi)
	{
		camFront.setFov(cam_fov);

		camBack.setFov(cam_fov);
		camTop.setFov(cam_fov);
		camLeft.setFov(cam_fov);
		camRight.setFov(cam_fov);
	}

	//Updates the position of the FreeCam in the scene
	if (camstate == freecam)
	{
		if (is_camera_move_left)
			freecamera.truck(-cam_speed);
		if (is_camera_move_right)
			freecamera.truck(cam_speed);
		if (is_camera_move_forward)
			freecamera.dolly(-cam_speed);
		if (is_camera_move_backward)
			freecamera.dolly(cam_speed);
		if (is_camera_move_up)
			freecamera.boom(cam_speed);
		if (is_camera_move_down)
			freecamera.boom(-cam_speed);
		if (is_camera_pan_left)
			freecamera.panDeg(cam_speed / 2);
		if (is_camera_pan_right)
			freecamera.panDeg(-cam_speed / 2);
		if (is_camera_tilt_up)
			freecamera.tiltDeg(cam_speed / 2);
		if (is_camera_tilt_down)
			freecamera.tiltDeg(-cam_speed / 2);

		freecamera.setFov(cam_fov);
	}
}

//Function that updates the lights attribute that are changed through the UI
void Renderer3D::updateLights()
{
	directionalLight.setDiffuseColor(directionalColor);
	directionalLight.setSpecularColor(directionalColor);

	spotLight.setPosition(light_pos.x, 150, light_pos.z);

	spotLight.setSpotConcentration(concentration);
	spotLight.setSpotlightCutOff(cutoff);

	light_front_right.setPosition(100, light_pos.y, 90);
	light_front_left.setPosition(-100, light_pos.y, 90);
	light_back_right.setPosition(100, light_pos.y, -90);
	light_back_left.setPosition(-100, light_pos.y, -90);

}

void Renderer3D::draw() {

	switch (camstate)
	{
	case multi:
		drawViewportCamera();
		break;

	case freecam:
		freecamera.begin();
		drawScene();
		freecamera.end();
		break;
	default:
		break;
	}
}

//Function that draws the content of the scene (Plane, Geo and Preview)
void Renderer3D::drawScene()
{
	ofEnableDepthTest();
	ofSetColor(255, 255, 255);
	activateLights();
	
	//Draws the basic board/scene
	if (!pbr_toggle)
	{
		boardMaterial.begin();
		board.bind();
		plane.draw();
		board.unbind();
		boardMaterial.end();
		drawGeo();
		previewGeo();
	}

	//Draws the PBR setup with 3 sphere
	if (pbr_toggle)
	{
		ofPushMatrix();
		addPBRUniform(1);
		pbr_shader.begin();
		ofDrawSphere(-65,0,0,30);
		pbr_shader.end();
		addPBRUniform(2);
		pbr_shader.begin();
		ofDrawSphere(0, 0, 0, 30);
		pbr_shader.end();
		addPBRUniform(3);
		pbr_shader.begin();
		ofDrawSphere(65, 0, 0, 30);
		pbr_shader.end();
		ofPopMatrix();
	}

	deactivateLights();
	ofDisableDepthTest();
}

//Function that draws all the geo
void Renderer3D::drawGeo()
{
	for (index = 0; index < bufferCount; ++index)
	{
		ofPushMatrix();
		ofTranslate(geo[index].vector_position);

		//Check which Shader/Material to use
		if (geo[index].type != objectType3D::none)
		{
			geoShaders(index);

			if (shader_active == ShaderType::none)
			{
				geoMaterial.begin();
			}
			else
			{
				shader->begin();

			}
		}

		switch (geo[index].type)
		{
		case objectType3D::box:
			box.set(geo[index].height);
			box.setScale(geo[index].vector_proportion);
			box.setOrientation(geo[index].vector_rotation);	
			box.setPosition(0,0,0);
			box.draw();
			break;

		case objectType3D::cylinder:
			cylinder.set(geo[index].radius, geo[index].height);
			cylinder.setScale(geo[index].vector_proportion);
			cylinder.setOrientation(geo[index].vector_rotation);
			cylinder.setPosition(0,0,0);
			cylinder.draw();
			break;

		case objectType3D::cone:
			cone.set(geo[index].radius, geo[index].height);
			cone.setScale(geo[index].vector_proportion);
			cone.setOrientation(geo[index].vector_rotation);
			cone.setPosition(0,0,0);
			cone.draw();
			break;

		case objectType3D::sphere:
			sphere.set(geo[index].radius, 10);
			sphere.setScale(geo[index].vector_proportion);
			sphere.setOrientation(geo[index].vector_rotation);
			sphere.setPosition(0,0,0);
			sphere.draw();
			break;
		}
		if (geo[index].type != objectType3D::none)
		{
			geoMaterial.end();
			shader->end();
		}
		ofPopMatrix();
	}
}

//Assigns uniform/material to the shader that is needed for existing placed geo
void Renderer3D::geoShaders(int i)
{
	switch (shader_active)
	{
	case ShaderType::none:
		geoMaterial.setShininess(geo[i].shininess);
		geoMaterial.setSpecularColor(geo[i].specular);
		geoMaterial.setDiffuseColor(geo[i].diffuse);
		geoMaterial.setAmbientColor(geo[i].ambient);
		break;
	case ShaderType::lambert:
		shader = &lambert;
		shader->begin();
		addUniformShaders(i);
		shader->end();
		break;

	case ShaderType::gouraud:
		shader = &gouraud;
		shader->begin();
		addUniformShaders(i);
		shader->setUniform3f("color_specular", geo[i].specular.r, geo[i].specular.g, geo[i].specular.b);
		shader->setUniform1f("brightness", geo[i].shininess);
		shader->end();
		break;

	case ShaderType::phong:
		shader = &phong;
		shader->begin();
		addUniformShaders(i);
		shader->setUniform3f("color_specular", geo[i].specular.r, geo[i].specular.g, geo[i].specular.b);
		shader->setUniform1f("brightness", geo[i].shininess);
		shader->end();
		break;

	default:
		break;
	}
}

//Function to consolidate lines that were repeated in the above function
void Renderer3D::addUniformShaders(int i)
{
	shader->setUniform3f("color_ambient", geo[i].ambient.r, geo[i].ambient.g, geo[i].ambient.b);
	shader->setUniform3f("color_diffuse", geo[i].diffuse.r, geo[i].diffuse.g, geo[i].diffuse.b);
	shader->setUniform3f("ambient_light", ambientLight.r, ambientLight.g, ambientLight.b);
	addLightsUniformShaders();
}

//Function that draws a preview of the current geo selected for placement
void Renderer3D::previewGeo()
{
	ofPushMatrix();
	ofTranslate(vector_position);

	previewShaders();

	if (shader_active == ShaderType::none)
	{
		previewMaterial.begin();
	}
	else
	{
		shader->begin();

	}

	switch (preview_mode)
	{

	case objectType3D::box:
		ofSetColor(geo_color.r, geo_color.g, geo_color.b, geo_color.a);
		box.set(geo_height);
		box.setScale(vector_proportion);
		box.setOrientation(vector_rotation);
		box.setPosition(0, 0, 0);
		box.draw();
		break;

	case objectType3D::cylinder:
		ofSetColor(geo_color.r, geo_color.g, geo_color.b, geo_color.a);
		cylinder.set(geo_radius, geo_height);
		cylinder.setScale(vector_proportion);
		cylinder.setOrientation(vector_rotation);
		cylinder.setPosition(0, 0, 0);
		cylinder.draw();
		break;

	case objectType3D::cone:
		ofSetColor(geo_color.r, geo_color.g, geo_color.b, geo_color.a);
		cone.set(geo_radius, geo_height);
		cone.setScale(vector_proportion);
		cone.setOrientation(vector_rotation);
		cone.setPosition(0, 0, 0);
		cone.draw();
		break;


	case objectType3D::sphere:
		ofSetColor(geo_color.r, geo_color.g, geo_color.b, geo_color.a);
		sphere.set(geo_radius, 10);
		sphere.setScale(vector_proportion);
		sphere.setOrientation(vector_rotation);
		sphere.setPosition(0, 0, 0);
		sphere.draw();
		break;

	default:
		break;
	}
	lambert.end();
	previewMaterial.end();
	ofPopMatrix();
}


//Assigns uniform/material to the shader that is needed for preview geo
void Renderer3D::previewShaders()
{
	switch (shader_active)	
	{
	 case ShaderType::none:
		 previewMaterial.setShininess(shininess);
		 previewMaterial.setSpecularColor(specular_color);
		 previewMaterial.setDiffuseColor(diffuse_color);
		 previewMaterial.setAmbientColor(ambient_color);
		 break;

	 case ShaderType::lambert:
		 shader = &lambert;
		 shader->begin();
		 addPreviewUniformShaders();
		 shader->end();
		 break;

	 case ShaderType::gouraud:
		 shader = &gouraud;
		 shader->begin();
		 addPreviewUniformShaders();
		 shader->setUniform3f("color_specular", specular_color.r, specular_color.g, specular_color.b);
		 shader->setUniform1f("brightness", shininess);
		 shader->end();
		 break;

	 case ShaderType::phong:
		 shader = &phong;
		 shader->begin();
		 addPreviewUniformShaders();
		 shader->setUniform3f("color_specular", specular_color.r, specular_color.g, specular_color.b);
		 shader->setUniform1f("brightness", shininess);
		 shader->end();
		 break;

	default:
		break;
	}
}

//Function to consolidate lines that were repeated in the above function
void Renderer3D::addPreviewUniformShaders()
{
	shader->setUniform3f("color_ambient", ambient_color.r, ambient_color.g, ambient_color.b);
	shader->setUniform3f("color_diffuse", diffuse_color.r, diffuse_color.g, diffuse_color.b);
	shader->setUniform3f("ambient_light", ambientLight.r, ambientLight.g, ambientLight.b);
	addLightsUniformShaders();
}

//Function to consolidate the light positions and lights on/off condition for shaders used in the shaders
void Renderer3D::addLightsUniformShaders()
{
	shader->setUniform3f("spot_light_position", glm::vec4(spotLight.getGlobalPosition(), 0.0f));
	shader->setUniform3f("side_light_position", glm::vec4(lights[mainLight]->getGlobalPosition(), 0.0f));
	if (spotToggle == false and sideLightToggle == false)
	{
		shader->setUniform1f("lights_on", 0);
	}
	if (sideLightToggle == true and spotToggle == false)
	{
		shader->setUniform1f("lights_on", 1);
	}
	if (sideLightToggle == false and spotToggle == true)
	{
		shader->setUniform1f("lights_on", 2);
	}
	if (spotToggle == true and sideLightToggle == true)
	{
		shader->setUniform1f("lights_on", 3);
	}
}

//Function that assigns Uniform to the PBR shader and also assigns the texture depending on the condition
void Renderer3D::addPBRUniform(int i)
{
	pbr_shader.begin();

	pbr_shader.setUniform3f("material_color_ambient", material_color_ambient.r / 255.0f, material_color_ambient.g / 255.0f, material_color_ambient.b / 255.0f);
	pbr_shader.setUniform3f("material_color_diffuse", material_color_diffuse.r / 255.0f, material_color_diffuse.g / 255.0f, material_color_diffuse.b / 255.0f);
	pbr_shader.setUniform3f("material_color_specular", material_color_specular.r / 255.0f, material_color_specular.g / 255.0f, material_color_specular.b / 255.0f);

	pbr_shader.setUniform1f("material_brightness", material_brightness);
	pbr_shader.setUniform1f("material_metallic", material_metallic);
	pbr_shader.setUniform1f("material_roughness", material_roughness);
	pbr_shader.setUniform1f("material_occlusion", material_occlusion);

	pbr_shader.setUniform3f("material_fresnel_ior", material_fresnel_ior);

	switch (i)
	{
	case 1:
		pbr_shader.setUniformTexture("texture_diffuse", texture_diffuse1.getTexture(), 1);
		pbr_shader.setUniformTexture("texture_metallic", texture_metal_rough_occ1.getTexture(), 2);
		pbr_shader.setUniformTexture("texture_roughness", texture_metal_rough_occ1.getTexture(), 3);
		pbr_shader.setUniformTexture("texture_occlusion", texture_metal_rough_occ1.getTexture(), 4);
		break;

	case 2:
		pbr_shader.setUniformTexture("texture_diffuse", texture_diffuse2.getTexture(), 1);
		pbr_shader.setUniformTexture("texture_metallic", texture_metal_rough_occ2.getTexture(), 2);
		pbr_shader.setUniformTexture("texture_roughness", texture_metal_rough_occ2.getTexture(), 3);
		pbr_shader.setUniformTexture("texture_occlusion", texture_metal_rough_occ2.getTexture(), 4);
		break;

	case 3:
		pbr_shader.setUniformTexture("texture_diffuse", texture_diffuse3.getTexture(), 1);
		pbr_shader.setUniformTexture("texture_metallic", texture_metal_rough_occ3.getTexture(), 2);
		pbr_shader.setUniformTexture("texture_roughness", texture_metal_rough_occ3.getTexture(), 3);
		pbr_shader.setUniformTexture("texture_occlusion", texture_metal_rough_occ3.getTexture(), 4);
		break;

	default:
		break;
	}

	pbr_shader.setUniform1f("light_intensity", light_intensity);
	pbr_shader.setUniform3f("light_color", light_color.r / 255.0f, light_color.g / 255.0f, light_color.b / 255.0f);

	pbr_shader.setUniform3f("spot_light_position", glm::vec4(spotLight.getGlobalPosition(), 0.0f));
	pbr_shader.setUniform3f("side_light_position", glm::vec4(lights[mainLight]->getGlobalPosition(), 0.0f));

	//Condition for On/Off lights
	if (spotToggle == false and sideLightToggle == false)
	{
		pbr_shader.setUniform1f("lights_on", 0);
	}
	if (sideLightToggle == true and spotToggle == false)
	{
		pbr_shader.setUniform1f("lights_on", 1);
	}
	if (sideLightToggle == false and spotToggle == true)
	{
		pbr_shader.setUniform1f("lights_on", 2);
	}
	if (spotToggle == true and sideLightToggle == true)
	{
		pbr_shader.setUniform1f("lights_on", 3);
	}

	pbr_shader.setUniform1f("tone_mapping_exposure", tone_mapping_exposure);
	pbr_shader.setUniform1f("tone_mapping_gamma", tone_mapping_gamma);

	pbr_shader.end();
}

//Function that adds a geo in geo* with the current selected transform
void Renderer3D::addGeo(objectType3D type)
{

	if (preview_mode == objectType3D::none)
	{
		return;
	}

	geo[bufferHead].type = type;

	geo[bufferHead].height = geo_height;
	geo[bufferHead].radius = geo_radius;

	//Container for the Material
	geo[bufferHead].shininess = shininess;
	geo[bufferHead].specular = specular_color;
	geo[bufferHead].diffuse = diffuse_color;
	geo[bufferHead].ambient = ambient_color;

	geo[bufferHead].vector_position = { vector_position.x, vector_position.y, vector_position.z };
	geo[bufferHead].vector_proportion = { vector_proportion.x, vector_proportion.y, vector_proportion.z };
	geo[bufferHead].vector_rotation = { vector_rotation.x, vector_rotation.y, vector_rotation.z };

	bufferHead = ++bufferHead >= bufferCount ? 0 : bufferHead;

	if(geoCount != bufferCount)
		geoCount++;

	preview_mode = objectType3D::none;
}

//Function that draws a help panel that contains hotkeys info
void Renderer3D::drawhelp() 
{

	int midscreenX = ofGetWindowWidth() / 2;
	int midscreenY = (ofGetWindowHeight() / 2) - 25;

	int helpwindowW = 1000;
	int helpwindowH = 575;
	int offsetX2 = helpwindowW / 2;
	int offsetY2 = helpwindowH / 2;

	string helpstring = "Command List: \n"
						"C: Switch Multi/Free Cam\n"
						"A: Move Free Camera Left // Snap Cam: Rotate Left around Geo\n"
						"D: Move Free Camera Right // Snap Cam: Rotate Right around Geo\n"
						"W: Move Free/Snap Camera Forward\n"
						"S: Move Free/Snap Camera Backward\n"
						"Z: Move Free Camera Upward // Snap Cam: Rotate Upward around Geo\n"
						"X: Move Free Camera Downward // Snap Cam: Rotate Downward around Geo\n"
						"Mouse Wheel Up/Down: Change the FOV of any Cam\n"
						"Q: Rotate Free Camera Left\n"
						"E: Rotate Free Camera Right\n"
						"R: Rotate Free Camera Upward\n"
						"F: Rotate Free Camera Downard\n"
						"6/7/8/9: Change the position of the Side Light\n"
						"L: Toggle Directional Light On/Off\n"
						"Left/Right Arrows: Change orientation of directional light\n"
						"O: Toggle Side Light On/Off\n"
						"P: Toggle Spot Light On/Off";

	ofPushStyle();
	ofFill();
	ofSetLineWidth(0);
	ofSetColor(220,220,220);
	ofDrawRectangle(midscreenX - offsetX2, midscreenY - offsetY2, helpwindowW, helpwindowH);

	ofNoFill();
	ofSetLineWidth(5);
	ofSetColor(0, 0, 0);
	ofDrawRectangle(midscreenX - offsetX2, midscreenY - offsetY2, helpwindowW, helpwindowH);

	ofPushMatrix();
	ofTranslate(midscreenX - offsetX2, midscreenY - offsetY2);
	fontText.drawString(helpstring, 5, 25);
	ofPopMatrix();
	ofPopStyle();
}

//Destructor that frees the geo*
Renderer3D::~Renderer3D()
{
	std::free(geo);
}
