#include "renderer3d.h"

void Renderer3D::setup() {

	ofSetFrameRate(60);
	fontText.load(PATH_FONTS + "BelweBold.otf", 20);
	setupPlane();
	setupGeo();
	setupModel();
	setupMulticam();
	setupViewports();
	resetcam();
}

//Function that sets the plane dimension/rotation and load the texture that will be used later on
void Renderer3D::setupPlane() {

	ofDisableArbTex();
	ofLoadImage(board, PATH_ASSETS + "board.png");
	plane.set(250, 140);
	plane.setPosition(0, 0, 0);
	plane.rotateDeg(90, 1, 0, 0);
	plane.setResolution(2, 2);

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
	oldGeo = (object3D*)std::malloc(bufferSize);
	geoCount = 0;
	resetGeoSelection();

	for (size_t i = 0; i < bufferCount; i++)
	{
		geo[i].type = objectType3D::none;
		geo[i].model_type = 0;
		for (size_t j = 0; j < 4; j++)
		{
			geo[i].color[j] = 0;
		}
		geo[i].radius = 0;
		geo[i].height = 0;
		geo[i].vector_position = { 0,0,0 };
		geo[i].vector_proportion = { 0,0,0 };
		geo[i].vector_rotation = { 0,0,0 };
		geo[i].selected = false;
		
	}

}

//Function that loads and push the model in a vector to ease selection
void Renderer3D::setupModel() {

	model.loadModel(PATH_ASSETS + "medieval house.obj");
	vmodel.push_back(model);
	model2.loadModel(PATH_ASSETS + "dom 1.obj");
	vmodel.push_back(model2);
	model3.loadModel(PATH_ASSETS + "Stone.obj");
	vmodel.push_back(model3);
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
	ofFill();
	ofSetColor(100, 100, 100);
	ofDrawRectangle(viewGrid[iMainCamera]);

	cameras[iMainCamera]->begin(viewMain);
	drawScene();
	cameras[iMainCamera]->end();


	for (int i = 0; i < N_CAMERAS; i++) {
		cameras[i]->begin(viewGrid[i]);
		drawScene();
		cameras[i]->end();
	}
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

	//Switch between orthogonal or perspective view depending on the bool
	if (projection_o)
	{
		for (size_t i = 0; i != N_CAMERAS; ++i) {
			cameras[i]->enableOrtho();

		}
	}

	if (!projection_o)
	{
		for (size_t i = 0; i != N_CAMERAS; ++i) {
			cameras[i]->disableOrtho();
		}
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
	snap_active = false;

	cam_fov = 60;
}

//Function that sets the look at of the snap camera at the nap cam target
void Renderer3D::snapcamera()
{
	if (geoCount == 0)
	{
		snap_active = false;
		resetcam();
		return;
	}

	current_snap = cam_snap;
	freecamera.lookAt(geo[cam_snap].vector_position);
}

//Function that ensure that the camera position is set around the Snap cam target
void Renderer3D::updatecam()
{
	//Condition so that the position gets updated when you change snap cam target
	if (current_snap != previous_snap)
	{
		//Position and distance of the snap camera
		freecamera.setPosition(0, geo[cam_snap].vector_position.y + 50, geo[cam_snap].vector_position.z + 100);
		previous_snap = current_snap;
	}
}

//Function to switch between orthogonal or perspective view for the free cam depending on the bool
void Renderer3D::fcamproject()
{
	if (projection_o)
	{
		freecamera.enableOrtho();
	}

	if (!projection_o)
	{
		freecamera.disableOrtho();
	}
}

void Renderer3D::update() {

	updateBatchTransformation();

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

		//Calls the proper function when the Snap Cam mode gets selected
		if (snap_active)
		{
			updatecam();
		}
	}
}

//Funtion that updates the transform of selected geos
void Renderer3D::updateBatchTransformation()
{
	for (size_t i = 0; i < bufferCount; i++)
	{
		if (geo[i].selected)
		{
			geo[i].height = oldGeo[i].height + batch_slider_height;
			geo[i].radius = oldGeo[i].radius + batch_slider_radius;

			geo[i].vector_position.x = oldGeo[i].vector_position.x + batch_vector_position.x;
			geo[i].vector_position.y = oldGeo[i].vector_position.y + batch_vector_position.y;
			geo[i].vector_position.z = oldGeo[i].vector_position.z + batch_vector_position.z;

			geo[i].vector_proportion.x = oldGeo[i].vector_proportion.x + batch_vector_proportion.x;
			geo[i].vector_proportion.y = oldGeo[i].vector_proportion.y + batch_vector_proportion.y;
			geo[i].vector_proportion.z = oldGeo[i].vector_proportion.z + batch_vector_proportion.z;

			geo[i].vector_rotation.x = oldGeo[i].vector_rotation.x + batch_vector_rotation.x;
			geo[i].vector_rotation.y = oldGeo[i].vector_rotation.y + batch_vector_rotation.y;
			geo[i].vector_rotation.z = oldGeo[i].vector_rotation.z + batch_vector_rotation.z;
		}
	}
}

void Renderer3D::draw() {
	switch (camstate)
	{
	case multi:
		drawViewportCamera();
		break;

	case freecam:
		freecamera.begin();
		if (snap_active)
		{
			snapcamera();
		}
		fcamproject();
		drawScene();
		freecamera.end();
		break;
	default:
		break;
	}
	if (help_on)
	{
		drawhelp();
	}
}

//Function that draws the content of the scene (Plane, Geo and Preview)
void Renderer3D::drawScene()
{
	ofSetColor(255, 255, 255);
	board.bind();
	ofEnableDepthTest();
	plane.draw();
	board.unbind();
	drawGeo();
	previewGeo();
	ofDisableDepthTest();
}

//Function that draws all the geo
void Renderer3D::drawGeo()
{
	for (index = 0; index < bufferCount; ++index)
	{
		ofPushMatrix();
		ofTranslate(geo[index].vector_position);
		switch (geo[index].type)
		{
		case objectType3D::box:
			ofSetColor(geo[index].color[0], geo[index].color[1], geo[index].color[2], geo[index].color[3]);
			box.set(geo[index].height);
			box.setScale(geo[index].vector_proportion);
			box.setOrientation(geo[index].vector_rotation);	
			box.setPosition(0,0,0);
			box.draw();
			break;

		case objectType3D::cylinder:
			ofSetColor(geo[index].color[0], geo[index].color[1], geo[index].color[2], geo[index].color[3]);
			cylinder.set(geo[index].radius, geo[index].height);
			cylinder.setScale(geo[index].vector_proportion);
			cylinder.setOrientation(geo[index].vector_rotation);
			cylinder.setPosition(0,0,0);
			cylinder.draw();
			break;

		case objectType3D::cone:
			ofSetColor(geo[index].color[0], geo[index].color[1], geo[index].color[2], geo[index].color[3]);
			cone.set(geo[index].radius, geo[index].height);
			cone.setScale(geo[index].vector_proportion);
			cone.setOrientation(geo[index].vector_rotation);
			cone.setPosition(0,0,0);
			cone.draw();
			break;

		case objectType3D::sphere:
			ofSetColor(geo[index].color[0], geo[index].color[1], geo[index].color[2], geo[index].color[3]);
			sphere.set(geo[index].radius, 10);
			sphere.setScale(geo[index].vector_proportion);
			sphere.setOrientation(geo[index].vector_rotation);
			sphere.setPosition(0,0,0);
			sphere.draw();
			break;

		case objectType3D::tetra:
			ofSetColor(geo[index].color[0], geo[index].color[1], geo[index].color[2], geo[index].color[3]);
			ofRotateZDeg(geo[index].vector_rotation.z);
			ofRotateYDeg(geo[index].vector_rotation.y);
			ofRotateXDeg(geo[index].vector_rotation.x);
			ofScale(geo[index].vector_proportion);
			ofFill();
			drawTetrahedron(0, 0, 0, geo[index].radius, geo[index].height);
			break;

		case objectType3D::octa:
			ofSetColor(geo[index].color[0], geo[index].color[1], geo[index].color[2], geo[index].color[3]);
			ofRotateZDeg(geo[index].vector_rotation.z);
			ofRotateYDeg(geo[index].vector_rotation.y);
			ofRotateXDeg(geo[index].vector_rotation.x);
			ofScale(geo[index].vector_proportion);
			ofFill();
			drawOctahedron(0, 0, 0, geo[index].radius, geo[index].height);
			break;

		case objectType3D::dode:
			ofSetColor(geo[index].color[0], geo[index].color[1], geo[index].color[2], geo[index].color[3]);
			ofRotateZDeg(geo[index].vector_rotation.z);
			ofRotateYDeg(geo[index].vector_rotation.y);
			ofRotateXDeg(geo[index].vector_rotation.x);
			ofScale(geo[index].vector_proportion);
			ofFill();
			drawDodecahedron(0, 0, 0, geo[index].radius);
			break;

		case objectType3D::model:
			vmodel[model_selection_slider].setRotation(0, 180, 0, 0, 1);
			ofSetColor(255, 255, 255);
			ofRotateZDeg(geo[index].vector_rotation.z);
			ofRotateYDeg(geo[index].vector_rotation.y);
			ofRotateXDeg(geo[index].vector_rotation.x);
			vmodel[geo[index].model_type].setScale(geo[index].vector_proportion.x, geo[index].vector_proportion.y, geo[index].vector_proportion.z);
			vmodel[geo[index].model_type].setPosition(0,0,0);
			vmodel[geo[index].model_type].setScaleNormalization(false);
			vmodel[geo[index].model_type].drawFaces();
			break;
		}
		ofPopMatrix();

		//Debug to see the delimitation box
		if (delim_on or geo[index].selected)
		{
			drawDelim(geo[index].type);
		}
		drawGeoContour(index);
	}
}

//Function that draws the contour of certain shape drwan in DrawGeo()
void Renderer3D::drawGeoContour(int i)
{
	//Translation/Scale/Rotation are applied first
	ofPushMatrix();
	ofSetColor(255, 255, 255, 127);
	ofTranslate(geo[i].vector_position);
	ofRotateZDeg(geo[i].vector_rotation.z);
	ofRotateYDeg(geo[i].vector_rotation.y);
	ofRotateXDeg(geo[i].vector_rotation.x);
	ofScale(geo[i].vector_proportion);
	ofNoFill();
	ofSetLineWidth(2);

	//Results on Cone and Cylinder were not looking good so not added to this function
	switch (geo[i].type)
	{
	case objectType3D::none:
		break;
	case objectType3D::box:
		ofDrawBox(0, 0, 0, geo[i].height);
		break;
	case objectType3D::tetra:
		drawTetrahedron(0, 0, 0, geo[i].radius, geo[i].height);
		break;
	case objectType3D::octa:
		drawOctahedron(0, 0, 0, geo[i].radius, geo[i].height);
		break;
	case objectType3D::dode:
		drawDodecahedron(0, 0, 0, geo[i].radius);
		break;
	default:
		break;
	}
	ofPopMatrix();
}

//Function that draws a preview of the current geo selected for placement
void Renderer3D::previewGeo()
{
	ofPushMatrix();
	ofTranslate(vector_position);
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

	case objectType3D::tetra:
		ofSetColor(geo_color.r, geo_color.g, geo_color.b, geo_color.a);
		ofRotateZDeg(vector_rotation.z);
		ofRotateYDeg(vector_rotation.y);
		ofRotateXDeg(vector_rotation.x);
		ofScale(vector_proportion);
		ofFill();
		drawTetrahedron(0, 0, 0, geo_radius, geo_height);;
		break;

	case objectType3D::octa:
		ofSetColor(geo_color.r, geo_color.g, geo_color.b, geo_color.a);
		ofRotateZDeg(vector_rotation.z);
		ofRotateYDeg(vector_rotation.y);
		ofRotateXDeg(vector_rotation.x);
		ofScale(vector_proportion);
		ofFill();
		drawOctahedron(0, 0, 0, geo_radius, geo_height);
		break;

	case objectType3D::dode:
		ofRotateZDeg(vector_rotation.z);
		ofRotateYDeg(vector_rotation.y);
		ofRotateXDeg(vector_rotation.x);
		ofScale(vector_proportion);
		ofSetColor(geo_color.r, geo_color.g, geo_color.b, geo_color.a);
		ofFill();
		drawDodecahedron(0, 0, 0, geo_radius);
		break;

	case objectType3D::model:
		vmodel[model_selection_slider].setRotation(0, 180, 0, 0, 1);
		ofSetColor(255, 255, 255);
		ofRotateZDeg(vector_rotation.z);
		ofRotateYDeg(vector_rotation.y);
		ofRotateXDeg(vector_rotation.x);
		vmodel[model_selection_slider].setPosition(0, 0, 0);
		vmodel[model_selection_slider].setScale(vector_proportion.x, vector_proportion.y, vector_proportion.z);
		vmodel[model_selection_slider].setScaleNormalization(false);
		vmodel[model_selection_slider].drawFaces();

	default:
		break;
	}
	ofPopMatrix();
	previewContour();
}

//Function that draws the contour of the prevew shape
void Renderer3D::previewContour()
{
	ofPushMatrix();
	ofSetColor(255, 255, 255, 127);
	ofTranslate(vector_position);
	ofRotateZDeg(vector_rotation.z);
	ofRotateYDeg(vector_rotation.y);
	ofRotateXDeg(vector_rotation.x);
	ofScale(vector_proportion);
	ofNoFill();
	ofSetLineWidth(2);
	switch (preview_mode)
	{
	case objectType3D::none:
		break;
	case objectType3D::box:
		ofDrawBox(0, 0, 0, geo_height);
		break;
	case objectType3D::tetra:
		drawTetrahedron(0, 0, 0, geo_radius, geo_height);
		break;
	case objectType3D::octa:
		drawOctahedron(0, 0, 0, geo_radius, geo_height);
		break;
	case objectType3D::dode:
		drawDodecahedron(0, 0, 0, geo_radius);
		break;
	default:
		break;
	}
	ofPopMatrix();
}

//Function that draws a delimitation box around the shapes, works with selection and the debug
void Renderer3D::drawDelim(objectType3D type) {

	//Variable used for Tetrahedron and Octahedron
	float d1 = ((sqrt(3) / 6) * geo[index].radius);
	float d2 = d1 + ((sqrt(3) / 3) * geo[index].radius);
	float d3 = (sqrt(3) / 2 * geo[index].height);

	ofNoFill();
	ofSetLineWidth(2);
	ofSetColor(255, 0, 0);
	ofPushMatrix(); 
	ofTranslate(geo[index].vector_position);
	ofRotateZDeg(geo[index].vector_rotation.z);
	ofRotateYDeg(geo[index].vector_rotation.y);
	ofRotateXDeg(geo[index].vector_rotation.x);
	ofScale(geo[index].vector_proportion);
	switch (type)
	{
	case objectType3D::none:
		break;
	case objectType3D::box:
		ofDrawBox(0,0,0, (geo[index].height));
		break;
	case objectType3D::cylinder:
		ofDrawBox(0, 0, 0, (geo[index].radius * 2), (geo[index].height), (geo[index].radius * 2));
		break;
	case objectType3D::sphere:
		ofDrawBox(0, 0, 0, (geo[index].radius*2), (geo[index].radius * 2), (geo[index].radius * 2) );
		break;
	case objectType3D::cone:
		ofDrawBox(0, 0, 0, (geo[index].radius * 2), (geo[index].height), (geo[index].radius * 2) );
		break;

	case objectType3D::tetra:
		ofDrawBox(0, 0 + geo[index].height / 2, 0 + (d2) / 2 - d1, geo[index].radius, geo[index].height, d2);
		break;

	case objectType3D::octa:
		ofDrawBox(0, 0, 0, geo[index].radius, (d3) * 2, geo[index].radius);
		break;

	case objectType3D::dode:
		ofDrawBox(0, 0, 0, (geo[index].radius / 3)*3.25);
		break;

	case objectType3D::model:
		float mx, my, mz;
		mx = vmodel[geo[index].model_type].getSceneMax().x - vmodel[geo[index].model_type].getSceneMin().x;
		my = vmodel[geo[index].model_type].getSceneMax().y - vmodel[geo[index].model_type].getSceneMin().y;
		mz = vmodel[geo[index].model_type].getSceneMax().z - vmodel[geo[index].model_type].getSceneMin().z;
		ofDrawBox((vmodel[geo[index].model_type].getSceneCenter().x),(vmodel[geo[index].model_type].getSceneCenter().y), (vmodel[geo[index].model_type].getSceneCenter().z), mx , my , mz );
		break;

	default:
		break;

	}
	ofPopMatrix();
}

//Function that draws a Terahedron  using 4 triangles
void Renderer3D::drawTetrahedron(float x, float y, float z, float s, float h)
{
	float d1 = ((sqrt(3) / 3) * s); //Upper Height of the triangle from it's center
	float d2 = ((sqrt(3) / 6) * s); //Lower Height of the triangle from it's center
	float d3 = s / 2; //Equation to find the vertex of the base of the triangle

	//Base Face
	ofDrawTriangle(x, y, z + d1, x - d3, y, z - d2, x + d3, y, z - d2);
	//Back Face
	ofDrawTriangle(x, y + h, z, x - d3, y, z - d2, x + d3, y, z - d2);
	//Left Face
	ofDrawTriangle(x, y + h, z, x - d3, y, z - d2, x, y, z + d1);
	//Right Face
	ofDrawTriangle(x, y + h, z, x + d3, y, z - d2, x, y, z + d1);
}

//Function that draws an Octahedron  using 4 triangles
void Renderer3D::drawOctahedron(float x, float y, float z, float s, float h)
{
	float d1 = (sqrt(3) / 2 * h); //Height of one half of the Octahedron
	float d2 = s / 2; 

	//Upper Back Face
	ofDrawTriangle(x, y + d1, z, x - d2, y, z - d2, x + d2, y, z - d2);
	//Upper Front Face
	ofDrawTriangle(x, y + d1, z, x - d2, y, z + d2, x + d2, y, z + d2);
	//Upper Right Face
	ofDrawTriangle(x, y + d1, z, x + d2, y, z - d2, x + d2, y, z + d2);
	//Upper Left Face
	ofDrawTriangle(x, y + d1, z, x - d2, y, z + d2, x - d2, y, z - d2);

	//Bottom Back Face
	ofDrawTriangle(x, y - d1, z, x - d2, y, z - d2, x + d2, y, z - d2);
	//Bottom Front Face
	ofDrawTriangle(x, y - d1, z, x - d2, y, z + d2, x + d2, y, z + d2);
	//Bottom Right Face
	ofDrawTriangle(x, y - d1, z, x + d2, y, z - d2, x + d2, y, z + d2);
	//Bottom Left Face
	ofDrawTriangle(x, y - d1, z, x - d2, y, z + d2, x - d2, y, z - d2);
}

//Function that draws a Dodecahedron using multiple shapes
void Renderer3D::drawDodecahedron(float x, float y, float z, float r)
{
	r = r / 3; //Normalizing the size of the Dodecadron to other geo

	float GR = (1 + sqrt(5)) / 2; //Golden Ratio

	//Position of vertexes for the Dodecadron
	vector<ofVec3f> v =
	{
		//Front Top Right [0]
		{1, 1, 1},
		//Front Top Left [1]
		{-1, 1, 1},
		//Front Bot Right [2]
		{1, -1, 1},
		//Front Bot Left [3]
		{-1, -1, 1},
		//Back Top Right [4]
		{1, 1, -1},
		//Back Top Left [5]
		{-1, 1, -1}, 
		//Back Bot Right [6]
		{1, -1, -1},
		//Back Bot Left [7]
		{-1, -1, -1},

		//Top Front [8]
		{0, GR, 1 / GR},
		//Bot Front [9]
		{0, -GR, 1 / GR},
		//Top Back [10]
		{0, GR, -1 / GR},
		//Bot Back [11]
		{0, -GR, -1 / GR},

		//Front Right [12]
		{1 / GR, 0, GR},
		//Front Left [13]
		{-1 / GR, 0, GR},
		//Back Right [14]
		{1 / GR, 0, -GR},
		//Back Left [15]
		{-1 / GR, 0, -GR},

		//Top Right [16]
		{GR, 1 / GR, 0},
		//Top Left [17]
		{-GR, 1 / GR, 0},
		//Bot Right [18]
		{GR, -1 / GR, 0},
		//Bot Left [19]
		{-GR, -1 / GR, 0},
	};

	ofVec3f vp{ x,y,z }; //Vector that hold the position of the Geo

	//Creating shapes with 3 loops that starts from a vertex and creates 3 pentagon faces for the geo
	ofBeginShape();
	for (int i = 0, idx[6] = { 8,0,12,13,1,8 }; i < 6; i++) {
		ofVertex(vp + v[idx[i]] * r);
	}

	for (int i = 0, idx[6] = { 8,10,5,17,1,8 }; i < 6; i++) {
		ofVertex(vp + v[idx[i]] * r);
	}

	for (int i = 0, idx[6] = { 8,10,4,16,0,8 }; i < 6; i++) {
		ofVertex(vp + v[idx[i]] * r);
	}
	ofEndShape();

	ofBeginShape();
	for (int i = 0, idx[6] = { 14,15,7,11,6,14 }; i < 6; i++) {
		ofVertex(vp + v[idx[i]] * r);
	}

	for (int i = 0, idx[6] = { 14,6,18,16,4,14 }; i < 6; i++) {
		ofVertex(vp + v[idx[i]] * r);
	}

	for (int i = 0, idx[6] = { 14,4,10,5,15,14 }; i < 6; i++) {
		ofVertex(vp + v[idx[i]] * r);
	}
	ofEndShape();

	ofBeginShape();
	for (int i = 0, idx[6] = { 19,7,15,5,17,19 }; i < 6; i++) {
		ofVertex(vp + v[idx[i]] * r);
	}

	for (int i = 0, idx[6] = { 19,17,1,13,3,19 }; i < 6; i++) {
		ofVertex(vp + v[idx[i]] * r);
	}

	for (int i = 0, idx[6] = { 19,7,11,9,3,19 }; i < 6; i++) {
		ofVertex(vp + v[idx[i]] * r);
	}
	ofEndShape();

	ofBeginShape();
	for (int i = 0, idx[6] = { 2,12,0,16,18,2 }; i < 6; i++) {
		ofVertex(vp + v[idx[i]] * r);
	}
	for (int i = 0, idx[6] = { 2,9,11,6,18,2 }; i < 6; i++) {
		ofVertex(vp + v[idx[i]] * r);
	}
	ofEndShape();

	//Last Pentagon needs to be drawn alone if not some faces dissapear
	ofBeginShape();
	for (int i = 0, idx[6] = { 2,9,3,13,12,2 }; i < 6; i++) {
		ofVertex(vp + v[idx[i]] * r);
	}
	ofEndShape();

	//Debug loop that draws color coded sphere with their index at all the positions which helped us find the info needed for the idx above.

	//ofSetColor(255, 255, 255);
	//int num = 0;
	//for (int i = 0; i < 8; i++)
	//{
	//	ofSphere(v[i] * r, 1);
	//	ofDrawBitmapString(num, v[i] * r *1.1);
	//	num++;
	//}

	//ofSetColor(255, 0, 0);
	//for (int i = 8; i < 12; i++)
	//{
	//	ofSphere(v[i] * r, 1);
	//	ofDrawBitmapString(num, v[i] * r * 1.1);
	//	num++;
	//}

	//ofSetColor(0, 255, 0);
	//for (int i = 12; i < 16; i++)
	//{
	//	ofSphere(v[i] * r, 1);
	//	ofDrawBitmapString(num, v[i] * r * 1.1);
	//	num++;
	//}

	//ofSetColor(0, 0, 255);
	//for (int i = 16; i < 20; i++)
	//{
	//	ofSphere(v[i]* r, 1);
	//	ofDrawBitmapString(num, v[i] * r * 1.1);
	//	num++;
	//}
}

//Function that gets the lowest vertex position, used for snapGeo() in application
float Renderer3D::getMiny(ofxAssimpModelLoader model) {
	float min_y;
	min_y = 0;
	for (int i = 0; i < model.getNumMeshes(); ++i) {
		{
			if (model.getMesh(i).getNumVertices() > 0) {
				for (int i = 0; i < mesh.getNumVertices(); i++)
				{
					if (mesh.getVertex(i).y < min_y) min_y = mesh.getVertex(i).y;
				}
			}
		}
	}
	return min_y;
}

//Function that gets the lowest vertex position, used for snapGeo() in application
float Renderer3D::getMinyGeo(ofMesh mesh) {
	float min_y;
	min_y = 0;
	if (mesh.getNumVertices() > 0) {
		min_y = mesh.getVertex(0).y;

		for (int i = 0; i < mesh.getNumVertices(); i++) {
			if (mesh.getVertex(i).y < min_y) min_y = mesh.getVertex(i).y;
		}
	}
	return min_y;
}

//Function that adds a geo in geo* with the current selected transform
void Renderer3D::addGeo(objectType3D type)
{

	if (preview_mode == objectType3D::none)
	{
		return;
	}

	geo[bufferHead].type = type;
	geo[bufferHead].model_type = model_selection_slider;

	geo[bufferHead].color[0] = geo_color.r;
	geo[bufferHead].color[1] = geo_color.g;
	geo[bufferHead].color[2] = geo_color.b;
	geo[bufferHead].color[3] = geo_color.a;

	geo[bufferHead].height = geo_height;
	geo[bufferHead].radius = geo_radius;

	geo[bufferHead].vector_position = { vector_position.x, vector_position.y, vector_position.z };
	geo[bufferHead].vector_proportion = { vector_proportion.x, vector_proportion.y, vector_proportion.z };
	geo[bufferHead].vector_rotation = { vector_rotation.x, vector_rotation.y, vector_rotation.z };

	bufferHead = ++bufferHead >= bufferCount ? 0 : bufferHead;

	if(geoCount != bufferCount)
		geoCount++;

	preview_mode = objectType3D::none;
}

//Function that undo the previous geo placed
void Renderer3D::undo()
{
	if (bufferHead)
	{
		geo[bufferHead - 1].type = objectType3D::none;
		geoCount--;
		bufferHead--;
	}
}

//Function that draws a help panel that contains hotkeys info
void Renderer3D::drawhelp() 
{

	int midscreenX = ofGetWindowWidth() / 2;
	int midscreenY = (ofGetWindowHeight() / 2) - 25;

	int helpwindowW = 1000;
	int helpwindowH = 675;
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
						"Left Up Down Righ Arrows: Move selected Geo/Model around\n"
						"B: Preview Mode: Snap Geo/Model to the Board\n"
						"G: Preview Mode: Add Current Selected Geo/Model\n"
						"P: Toggle Orthogonal Projection\n"
						"T: Show Delimitation Boxes of placed Geo/Models\n"
						"Del: Delete current selected Geos\n"
						"H: Toggle Help Panel\n"
						"** Rotations hotkeys do not work in Snap Cam mode**";

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

//Function that iterate through all the geos to find if one is currently selected
bool Renderer3D::isGeoSelected()
{
	for (int i = 0; i < bufferCount; i++)
	{
		if (geo[i].selected)
			return true;
	}
	return false;
}

//Function that selects the closest geo to the cursor, used in application
void Renderer3D::selectClosestGeo()
{
	//Condition so that selection cannot occur if no geo are placed
	if (geoCount > 0)
	{
		if (!isGeoSelected())
		{
			copyGeoVector();
		}

		geo[getClosestGeoIndex()].selected = true;
	}
}

//Function that copies the selected geo transform in a different container
void Renderer3D::copyGeoVector()
{

	for (size_t i = 0; i < bufferCount - 1; i++)
	{
		oldGeo[i].height = geo[i].height;
		oldGeo[i].radius = geo[i].radius;

		oldGeo[i].vector_position.x = geo[i].vector_position.x;
		oldGeo[i].vector_position.y = geo[i].vector_position.y;
		oldGeo[i].vector_position.z = geo[i].vector_position.z;

		oldGeo[i].vector_proportion.x = geo[i].vector_proportion.x;
		oldGeo[i].vector_proportion.y = geo[i].vector_proportion.y;
		oldGeo[i].vector_proportion.z = geo[i].vector_proportion.z;

		oldGeo[i].vector_rotation.x = geo[i].vector_rotation.x;
		oldGeo[i].vector_rotation.y = geo[i].vector_rotation.y;
		oldGeo[i].vector_rotation.z = geo[i].vector_rotation.z;
	}
}

//Function that gets the index of the closest geo to the cursor
int Renderer3D::getClosestGeoIndex()
{
	
	float xOffset = ofGetWidth() / 3;
	float test = ofGetWidth();
	float mainViewPortW = xOffset * 2;

	float nearestDistance = 0;
	int nearestIndex = -1;
	glm::vec3 mousePos;

	ofCamera tempCam;
	switch (camstate)
	{
	case multi:
		tempCam = *cameras[iMainCamera];
		mousePos = { mouse_current_x / mainViewPortW * ofGetWidth() , mouse_current_y, 0};
		break;
	case freecam:
		tempCam = freecamera;
		mousePos = { mouse_current_x, mouse_current_y, 0 };
		break;
	default:
		break;
	}

	for (size_t i = 0; i < bufferHead; i++)
	{
		glm::vec3 camPos = tempCam.worldToScreen(geo[i].vector_position);
		float distance = glm::distance(camPos, mousePos);

		if (distance < nearestDistance or nearestDistance == 0) {
			nearestDistance = distance;
			nearestIndex = i;
		}
	}
	return nearestIndex;
}

//Function that resets a geo status to none
void Renderer3D::resetGeo(int i)
{
	geo[i].type = objectType3D::none;
	geo[i].selected = false;
}

//Function that causes all current selected geo to not be anymore
void Renderer3D::resetGeoSelection()
{
	for (int i = 0; i < bufferCount; ++i)
		geo[i].selected = false;
}

//Function that gets the index of the first empty geo (Where the bufferhead -1 should be)
int Renderer3D::getIndexFirstEmptyGeo()
{
	for (int i = 0; i < bufferCount; i++)
	{
		if (geo[i].type == objectType3D::none)
		{
			/*cout << "getIndexFirstEmptyShape: " << i << endl;*/
			return i;
		}
	}
	/*cout << "getIndexFirstEmptyShape: " << bufferCount << endl;*/
	return bufferCount;
}

//Function that deletes selected geos
void Renderer3D::deleteGeo()
{

	for (int i = 0; i < bufferCount; ++i)
		if (geo[i].selected)
		{
			resetGeo(i);
			geoCount--;
		}

	do
	{
		for (int i = getIndexFirstEmptyGeo(); i < bufferCount - 1; i++)
		{
			object3D temp = geo[i];
			geo[i] = geo[i + 1];
			geo[i + 1] = temp;
		}
	} while (getIndexFirstEmptyGeo() != geoCount);
	bufferHead = geoCount;
	preview_mode = objectType3D::none;
}

//Destructor that frees the geo* and oldgeo*
Renderer3D::~Renderer3D()
{
	std::free(geo);
	std::free(oldGeo);
}
