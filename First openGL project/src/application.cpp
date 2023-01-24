#include "application.h"
#include "utils.h"


//==================== Core Functions =================
void Application::setup(){
	renderer.setup();
	renderer3d.setup();
	setupGUI();
	hscursor.load(PATH_ASSETS + "hscursor.png");

	if (debug_ON)
		setupDebug();
}
void Application::setupGUI()
{
	//====================== main menu ======================
	gui.setup("Interface");
	button_collection.setup("Collection");
	button_collection.addListener(this, &Application::button_pressed_collection);
	gui.add(&button_collection);

	button_createcard.setup("Create a card");
	button_createcard.addListener(this, &Application::button_pressed_createcard);
	gui.add(&button_createcard);

	button_random.setup("Create a Random card");
	button_random.addListener(this, &Application::button_pressed_randomcard);
	gui.add(&button_random);

	button_threed.setup("3D");
	button_threed.addListener(this, &Application::button_pressed_3D);
	gui.add(&button_threed);

	//====================== Collection ======================
	button_SaveCollectionToFolder.setup("Save to folder");
	button_SaveCollectionToFolder.addListener(this, &Application::button_pressed_saveToFolder);
	gui.add(&button_SaveCollectionToFolder);

	//====================== card preview ======================
	cardPreviewGUI.setup("Card preview");
	cardPreviewGUI.setPosition(225, 10);

	button_histo.setup("Histogram");
	button_histo.addListener(this, &Application::button_pressed_hist);
	cardPreviewGUI.add(&button_histo);

	slider_hist.set("R:1 G:2 B:3", 1, 1, 3);
	cardPreviewGUI.add(slider_hist);


	//====================== create custom card ======================
	canvasGUI.setup("Drawing Interface");
	canvasGUI.setPosition(10, 250);

	group_color.setup("Color/Line");

	color_canvas.set("Canvas color", ofColor(255), ofColor(0, 0), ofColor(255, 255));
	group_color.add(color_canvas);

	color_stroke.set("Line color", ofColor(0), ofColor(0, 0), ofColor(255, 255));
	group_color.add(color_stroke);

	slider_stroke_weight.set("Line width", 5.0f, 1.0f, 10.0f);
	group_color.add(slider_stroke_weight);

	color_shape.set("Shape color", ofColor(255), ofColor(0, 0), ofColor(255, 255));
	group_color.add(color_shape);

	canvasGUI.add(&group_color);

	group_shape.setup("Shape Type");

	button_line.setup("Line");
	button_line.addListener(this, &Application::button_pressed_line);
	group_shape.add(&button_line);

	button_rectangle.setup("Square / Rectangle");
	button_rectangle.addListener(this, &Application::button_pressed_rectangle);
	group_shape.add(&button_rectangle);

	button_parral.setup("Parallelogram");
	button_parral.addListener(this, &Application::button_pressed_parral);
	group_shape.add(&button_parral);

	button_triangle.setup("Triangle");
	button_triangle.addListener(this, &Application::button_pressed_triangle);
	group_shape.add(&button_triangle);

	button_triangle_rectangle.setup("Right triangle");
	button_triangle_rectangle.addListener(this, &Application::button_pressed_triangler);
	group_shape.add(&button_triangle_rectangle);

	button_ellipse.setup("Circle / Elipse");
	button_ellipse.addListener(this, &Application::button_pressed_ellipse);
	group_shape.add(&button_ellipse);

	canvasGUI.add(&group_shape);

	group_utilCanvas.setup("Util");

	button_reset.setup("Reset");
	button_reset.addListener(this, &Application::button_pressed_reset);
	group_utilCanvas.add(&button_reset);

	button_undo.setup("Undo");
	button_undo.addListener(this, &Application::button_pressed_undo);
	group_utilCanvas.add(&button_undo);

	button_redo.setup("Redo (Once)");
	button_redo.addListener(this, &Application::button_pressed_redo);
	group_utilCanvas.add(&button_redo);

	button_saveCardToCollection.set("Save to collection");
	button_saveCardToCollection.addListener(this, &Application::button_pressed_saveCardToCollection);
	group_utilCanvas.add(button_saveCardToCollection);

	button_saveToPath.set("Save to folder");
	button_saveToPath.addListener(this, &Application::button_pressed_saveToFolder);
	group_utilCanvas.add(button_saveToPath);

	canvasGUI.add(&group_utilCanvas);

	//====================== Create random card ======================
	randomCardGUI.setup("Util");
	randomCardGUI.setPosition(10, 100);
	randomCardGUI.add(button_saveCardToCollection);
	randomCardGUI.add(button_saveToPath);

	//====================== interface 3D ======================
	gui3D.setup("Interface");

	button_twod.setup("2D");
	button_twod.addListener(this, &Application::button_pressed_2D);
	gui3D.add(&button_twod);

	button_cam_mode.setup("MultiCam/Free Cam (C)");
	button_cam_mode.addListener(this, &Application::button_pressed_camswitch);
	gui3D.add(&button_cam_mode);

	cam_speed.set("Free Cam Speed", 1, 1, 5);
	gui3D.add(cam_speed);

	cam_fov.set("Cam Fov", 60, 0, 180);
	gui3D.add(cam_fov);

	button_reset_cam.setup("Reset Free Cam Pos");
	button_reset_cam.addListener(this, &Application::button_pressed_reset_cam);
	gui3D.add(&button_reset_cam);


	activ_snap.setup("Snap Camera (Free Cam)");
	activ_snap.addListener(this, &Application::button_pressed_snap_toggle);
	gui3D.add(&activ_snap);

	cam_snap.set("Snap Target", 0, 0, 0);
	gui3D.add(cam_snap);

	button_help.setup("Help (H)");
	button_help.addListener(this, &Application::button_pressed_help);
	gui3D.add(&button_help);

	//interface util3d
	util3D.setup("Util");
	util3D.setPosition(10, 185);

	button_none.setup("Batch Transform");
	button_none.addListener(this, &Application::button_pressed_none);
	util3D.add(&button_none);

	geo_group.setup("Geo");

	button_box.setup("Box");
	button_box.addListener(this, &Application::button_addbox);
	geo_group.add(&button_box);

	button_tetra.setup("Tetrahedron");
	button_tetra.addListener(this, &Application::button_addtetra);
	geo_group.add(&button_tetra);

	button_octa.setup("Octahedron");
	button_octa.addListener(this, &Application::button_addocta);
	geo_group.add(&button_octa);

	button_dodeca.setup("Dodecahedron");
	button_dodeca.addListener(this, &Application::button_adddodeca);
	geo_group.add(&button_dodeca);

	button_cylinder.setup("Cylinder");
	button_cylinder.addListener(this, &Application::button_addcylinder);
	geo_group.add(&button_cylinder);

	button_cone.setup("Cone");
	button_cone.addListener(this, &Application::button_addcone);
	geo_group.add(&button_cone);

	button_sphere.setup("Sphere");
	button_sphere.addListener(this, &Application::button_addsphere);
	geo_group.add(&button_sphere);

	button_3DModel.setup("3D Model");
	button_3DModel.addListener(this, &Application::button_addmodel);
	geo_group.add(&button_3DModel);

	model.set("Model Selection)", 0, 0, 2);
	geo_group.add(model);

	util3D.add(&geo_group);

	add_geo_button.setup("Add Geo (G)");
	add_geo_button.addListener(this, &Application::button_pressed_addgeo);
	util3D.add(&add_geo_button);

	show_delim.setup("Show Delim Box (T)");
	show_delim.addListener(this, &Application::button_pressed_show_delim);
	util3D.add(&show_delim);

	snap.setup("Snap Geo (B)");
	snap.addListener(this, &Application::button_pressed_snap);
	util3D.add(&snap);

	undo3d.setup("Undo");
	undo3d.addListener(this, &Application::button_pressed_undo);
	util3D.add(&undo3d);

	//interface transform 3D
	transform3D.setup("Transformation");
	transform3D.setPosition(10, 495);

	geo_color.set("Geo Color", ofColor(0, 0, 0));
	transform3D.add(geo_color);

	slider_height.set("Height", 10, 1, 40);
	transform3D.add(slider_height);

	slider_radius.set("Radius", 10, 1, 40);
	transform3D.add(slider_radius);

	//postion
	group_pos.setup("Position");

	slider_x.set("Position X", 0, -125, 125);
	group_pos.add(slider_x);

	slider_y.set("Position Y", 0, -25, 100);
	group_pos.add(slider_y);

	slider_z.set("Position Z", 0, -70, 70);
	group_pos.add(slider_z);

	transform3D.add(&group_pos);

	//scaling
	group_scale.setup("Scaling");

	scale_x.set("Scaling X", 1, 1, 15);
	group_scale.add(scale_x);

	scale_y.set("Scaling Y", 1, 1, 15);
	group_scale.add(scale_y);

	scale_z.set("Scaling Z", 1, 1, 15);
	group_scale.add(scale_z);

	transform3D.add(&group_scale);

	//rotations
	group_rotat.setup("Rotation");

	rotat_x.set("Rotation X", 0, -180, 180);
	group_rotat.add(rotat_x);

	rotat_y.set("Rotation Y", 0, -180, 180);
	group_rotat.add(rotat_y);

	rotat_z.set("Rotation Z", 0, -180, 180);
	group_rotat.add(rotat_z);

	transform3D.add(&group_rotat);

	//====================== batch transformation ======================

	batch_transformation.setup("Batch transformation");
	batch_transformation.setPosition(10, 495);

	//dimension
	batch_group_dimension.setup("Dimensions");

	batch_slider_radius.set("Radius", 0, -40, 40);
	batch_group_dimension.add(batch_slider_radius);

	batch_slider_height.set("Height", 0, -40, 40);
	batch_group_dimension.add(batch_slider_height);

	batch_transformation.add(&batch_group_dimension);

	//postion
	batch_group_pos.setup("Position");

	batch_slider_x.set("Position X", 0, -125, 125);
	batch_group_pos.add(batch_slider_x);

	batch_slider_y.set("Position Y", 0, -25, 100);
	batch_group_pos.add(batch_slider_y);

	batch_slider_z.set("Position Z", 0, -70, 70);
	batch_group_pos.add(batch_slider_z);

	batch_transformation.add(&batch_group_pos);

	//scaling
	batch_group_scale.setup("Scaling");

	batch_scale_x.set("Scaling X", 0, -15, 15);
	batch_group_scale.add(batch_scale_x);

	batch_scale_y.set("Scaling Y", 0, -15, 15);
	batch_group_scale.add(batch_scale_y);

	batch_scale_z.set("Scaling Z", 0, -15, 15);
	batch_group_scale.add(batch_scale_z);

	batch_transformation.add(&batch_group_scale);

	//rotations
	batch_group_rotat.setup("Rotation");

	batch_rotat_x.set("Rotation X", 0, -180, 180);
	batch_group_rotat.add(batch_rotat_x);

	batch_rotat_y.set("Rotation Y", 0, -180, 180);
	batch_group_rotat.add(batch_rotat_y);

	batch_rotat_z.set("Rotation Z", 0, -180, 180);
	batch_group_rotat.add(batch_rotat_z);

	batch_transformation.add(&batch_group_rotat);
}
void Application::setupDebug()
{
	ofDirectory dir(PATH_COLLECTION);
	dir.allowExt("png");
	dir.listDir();
	for (size_t i = 0; i < 2; i++)
	{
		ofImage temp;
		cout << dir.getPath(i) << endl;
		temp.load(dir.getPath(i));
		renderer.vCardCollection.push_back(temp);

	}

	renderer.state = debug;

}

void Application::update(){

	switch (astate)
	{
	case state2D:
		updateRendererVars();
		renderer.update();
		break;
	case state3D:
		updateRenderer3DVars();
		updateSnapCamSlider();
		renderer3d.update();
		break;
	default:
		break;
	}
}
void Application::updateRendererVars() {
	renderer.canvas = color_canvas;
	renderer.shape_color = color_shape;
	renderer.stroke_color = color_stroke;
	renderer.stroke_width = slider_stroke_weight;
	renderer.hist_slider = slider_hist;
}
void Application::updateRenderer3DVars() {
	renderer3d.geo_color = geo_color;
	renderer3d.geo_height = slider_height;
	renderer3d.geo_radius = slider_radius;
	renderer3d.vector_position.x = slider_x;
	renderer3d.vector_position.y = slider_y;
	renderer3d.vector_position.z = slider_z;
	renderer3d.vector_proportion.x = scale_x;
	renderer3d.vector_proportion.y = scale_y;
	renderer3d.vector_proportion.z = scale_z;
	renderer3d.vector_rotation.x = rotat_x;
	renderer3d.vector_rotation.y = rotat_y;
	renderer3d.vector_rotation.z = rotat_z;
	renderer3d.model_selection_slider = model;
	renderer3d.cam_speed = cam_speed;
	renderer3d.cam_fov = cam_fov;
	renderer3d.cam_snap = cam_snap;

	renderer3d.batch_slider_radius = batch_slider_radius;
	renderer3d.batch_slider_height = batch_slider_height;
	renderer3d.batch_vector_position.x = batch_slider_x;
	renderer3d.batch_vector_position.y = batch_slider_y;
	renderer3d.batch_vector_position.z = batch_slider_z;
	renderer3d.batch_vector_proportion.x = batch_scale_x;
	renderer3d.batch_vector_proportion.y = batch_scale_y;
	renderer3d.batch_vector_proportion.z = batch_scale_z;
	renderer3d.batch_vector_rotation.x = batch_rotat_x;
	renderer3d.batch_vector_rotation.y = batch_rotat_y;
	renderer3d.batch_vector_rotation.z = batch_rotat_z;
}

void Application::draw()
{
	switch (astate)
	{
	case state2D:
		renderer.draw();
		drawGUI();
		DrawHScursor();
		break;
	case state3D:
		renderer3d.draw();
		drawGUI();
		DrawHScursor();
		break;
	default:
		break;
	}
}
void Application::drawGUI()
{
	switch (astate)
	{
	case state2D:
		gui.draw();
		switch (renderer.state)
		{

		case idle:
			break;

		case cardPreview:
			cardPreviewGUI.draw();
			break;

		case collection:
			break;

		case createcard:
			canvasGUI.draw();
			break;

		case randomcard:
			randomCardGUI.draw();
			break;

		case hist:
			cardPreviewGUI.draw();
			break;

		default:
			break;
		}
		break;

		case state3D:
			gui3D.draw();
			util3D.draw();
			if (renderer3d.preview_mode != objectType3D::none)
			{
				transform3D.draw();
			}
			if (renderer3d.isGeoSelected())
			{
				batch_transformation.draw();
			}
			break;
		
	}
}

//Function to draw a custom cursor that also changes during vectorial drawing depending on the shapes 
void Application::DrawHScursor()
{
	ofHideCursor();
	ofSetColor(255, 255, 255);
	hscursor.draw(ofGetMouseX() - 10, ofGetMouseY() - 10);

	int xc = ofGetMouseX();
	int yc = ofGetMouseY();

	if (renderer.state == createcard and astate != state3D)
	{
		switch (renderer.draw_mode)
		{
		case VectorPrimitiveType::line:
			ofSetLineWidth(slider_stroke_weight);
			ofSetColor(color_stroke);
			ofDrawLine(xc + 20, yc + 35, xc + 25, yc + 30);
			break;


		case VectorPrimitiveType::rectangle:

			ofFill();
			ofSetLineWidth(0);
			ofSetColor(color_shape);
			ofDrawRectangle(xc + 20, yc + 30, 40, 20);

			ofNoFill();
			ofSetLineWidth(3);
			ofSetColor(color_stroke);
			ofDrawRectangle(xc + 20, yc + 30, 40, 20);
			break;

		case VectorPrimitiveType::parral:
			ofFill();
			ofSetLineWidth(0);
			ofSetColor(color_shape);
			renderer.drawParral(xc + 25, yc + 25, xc + 55, yc + 55);


			ofNoFill();
			ofSetLineWidth(3);
			ofSetColor(color_stroke);
			renderer.drawParral(xc + 25, yc + 25, xc + 55, yc + 55);

			break;

		case VectorPrimitiveType::triangle:
			ofFill();
			ofSetLineWidth(0);
			ofSetColor(color_shape);
			ofDrawTriangle(xc + 10, yc + 40, xc + 40, yc + 40, xc + 25, yc + 10);

			ofNoFill();
			ofSetLineWidth(3);
			ofSetColor(color_stroke);
			ofDrawTriangle(xc + 10, yc + 40, xc + 40, yc + 40, xc + 25, yc + 10);
			break;

		case VectorPrimitiveType::triangler:
			ofFill();
			ofSetLineWidth(0);
			ofSetColor(color_shape);
			ofDrawTriangle(xc + 20, yc + 50, xc + 50, yc + 50, xc + 20, yc + 20);

			ofNoFill();
			ofSetLineWidth(3);
			ofSetColor(color_stroke);
			ofDrawTriangle(xc + 20, yc + 50, xc + 50, yc + 50, xc + 20, yc + 20);
			break;

		case VectorPrimitiveType::ellipse:

			ofSetCircleResolution(10);
			ofFill();
			ofSetLineWidth(0);
			ofSetColor(color_shape);
			ofDrawEllipse(ofGetMouseX() + 30, ofGetMouseY() + 30, 25, 25);

			ofNoFill();
			ofSetLineWidth(3);

			ofSetColor(color_stroke);
			ofDrawEllipse(xc + 30, yc + 30, 25, 25);
			break;

		default:
			break;
		}
	}
}

//====================== events ======================
void Application::dragEvent(ofDragInfo dragInfo){

		ofImage temp;
		temp.load(dragInfo.files.at(0));
		renderer.vCardCollection.push_back(temp);
} 
void Application::mouseMoved(int x, int y)
{
	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;

	renderer3d.mouse_current_x = x;
	renderer3d.mouse_current_y = y;

}
void Application::mouseDragged(int x, int y, int button)
{
	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;
}
void Application::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
	if (scrollY == -1)
	{
		if (cam_fov >= 180)
			return;
		cam_fov += 2;
	}

	if (scrollY == 1)
	{
		if (cam_fov == 0)
			return;
		cam_fov -= 2;
	}
}
void Application::mousePressed(int x, int y, int button)
{
	renderer.is_mouse_button_pressed = true;

	renderer.mouse_press_x = x;
	renderer.mouse_press_y = y;

	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;
}
void Application::mouseReleased(int x, int y, int button)
{
	renderer.is_mouse_button_pressed = false;

	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;

	if (renderer.state == createcard)
	{
		renderer.addVectorShape(renderer.draw_mode);
	}

	if (astate == state3D and renderer3d.preview_mode == objectType3D::none)
	{
		resetSelectionUIValues();
		selectGeo();
	}

}
void Application::keyPressed(int key)
{
	if (astate == state3D)
	{
		switch (key)
		{

		case 97: // a
			renderer3d.is_camera_move_left = true;
			break;

		case 100: // d
			renderer3d.is_camera_move_right = true;
			break;

		case 119: // w
			renderer3d.is_camera_move_forward = true;
			break;

		case 115: // s
			renderer3d.is_camera_move_backward = true;
			break;

		case 122: //  z
			renderer3d.is_camera_move_up = true;
			break;

		case 120: //  x
			renderer3d.is_camera_move_down = true;
			break;

		case 113: //  q
			renderer3d.is_camera_pan_left = true;
			break;

		case 101: //  e
			renderer3d.is_camera_pan_right = true;
			break;

		case 114: //r 
			renderer3d.is_camera_tilt_up = true;
			break;

		case 102: //f 
			renderer3d.is_camera_tilt_down = true;
			break;

		case OF_KEY_LEFT: // left arrow
			slider_x -= 1;
			break;

		case OF_KEY_UP: // up arrow
			slider_z -= 1;
			break;

		case OF_KEY_RIGHT: //right arrow 
			slider_x += 1;
			break;

		case OF_KEY_DOWN: //down arrow
			slider_z += 1;
			break;

		default:
			break;
		}
	}
}
void Application::keyReleased(int key)
{
	if (astate == state2D)
	{
		switch (key)
		{
		case 115: //s
			selectShape();
			break;
		case 127: // delete
			renderer.deleteShape();
			break;
		default:
			break;
		}

	}
	if (astate == state3D)
	{
		switch (key)
		{
		case 97: // a
			renderer3d.is_camera_move_left = false;
			break;

		case 100: // d
			renderer3d.is_camera_move_right = false;
			break;

		case 119: // w
			renderer3d.is_camera_move_forward = false;
			break;

		case 115: // s
			renderer3d.is_camera_move_backward = false;
			break;

		case 122: //  z
			renderer3d.is_camera_move_up = false;
			break;

		case 120: //  x
			renderer3d.is_camera_move_down = false;
			break;

		case 113: //  q
			renderer3d.is_camera_pan_left = false;
			break;

		case 101: //  e
			renderer3d.is_camera_pan_right = false;
			break;

		case 114: //r 
			renderer3d.is_camera_tilt_up = false;
			break;

		case 102: //f 
			renderer3d.is_camera_tilt_down = false;
			break;

		case 99: // c
		{
			if (renderer3d.camstate == multi)
			{
				renderer3d.camstate = freecam;
				return;
			}

			if (renderer3d.camstate == freecam)
			{
				renderer3d.camstate = multi;
				return;
			}
		}

		case 98: //b
			SnapGeo();
			break;

		case 103: //g
			renderer3d.addGeo(renderer3d.preview_mode);
			break;

		case 104: //h
			if (!renderer3d.help_on)
			{
				renderer3d.help_on = true;
			}

			else
			{
				renderer3d.help_on = false;
			}
			break;

		case 112: // p
			if (!renderer3d.projection_o)
			{
				renderer3d.projection_o = true;
			}

			else
			{
				renderer3d.projection_o = false;
			}
			break;

		case 116: // t
			if (!renderer3d.delim_on)
			{
				renderer3d.delim_on = true;
			}

			else
			{
				renderer3d.delim_on = false;
			}
			break;

		case 49: // 1 
		case 50: // 2
		case 51: // 3
		case 52: // 4
		case 53: // 5
			renderer3d.iMainCamera = key - '1';
			break;

		case 127:
			renderer3d.deleteGeo();
			break;

		default:
			break;
		}
	}
}


//====================== Mainmenu ======================
void Application::button_pressed_collection()
{

	renderer.state = collection;
}
void Application::button_pressed_createcard()
{
	renderer.resetAllShapes();
	canvasGUI.draw();
	renderer.state = createcard;
}
void Application::button_pressed_3D()
{
	astate = state3D;
}
void Application::button_pressed_randomcard()
{
	renderer.updateRandomNumbers();
	renderer.state = randomcard;
}

// ====================== Create custom card ======================

void Application::button_pressed_line()
{
	renderer.draw_mode = VectorPrimitiveType::line;
}
void Application::button_pressed_rectangle()
{
	renderer.draw_mode = VectorPrimitiveType::rectangle;
}
void Application::button_pressed_parral()
{
	renderer.draw_mode = VectorPrimitiveType::parral;
}
void Application::button_pressed_triangle()
{
	renderer.draw_mode = VectorPrimitiveType::triangle;
}
void Application::button_pressed_triangler()
{
	renderer.draw_mode = VectorPrimitiveType::triangler;
}
void Application::button_pressed_ellipse()
{
	renderer.draw_mode = VectorPrimitiveType::ellipse;
}
void Application::button_pressed_reset()
{
	renderer.resetAllShapes();
}

void Application::button_pressed_undo()
{
	switch (astate)
	{
	case state2D:
		renderer.undo();
		break;
	case state3D:
		renderer3d.undo();
		break;
	default:
		break;
	}
}

void Application::button_pressed_redo()
{
	renderer.redo();
}

void Application::selectShape()
{
	if (renderer.indexMouseOverShape() != -1)
		renderer.shapes[renderer.indexMouseOverShape()].selected = true;
	else
		renderer.resetSelection();

	if (debug_ON)
	{
		system("CLS");
		for (int i = 0; i < renderer.bufferCount; i++)
		{
			cout << "Index : " << i;
			switch (renderer.shapes[i].type)
			{
			case VectorPrimitiveType::none:
				cout << " Shape : none ";
				break;
			case VectorPrimitiveType::line:
				cout << " Shape : line ";
				break;
			case VectorPrimitiveType::rectangle:
				cout << " Shape : rectangle ";
				break;
			case VectorPrimitiveType::parral:
				cout << " Shape : parral ";
				break;
			case VectorPrimitiveType::triangle:
				cout << " Shape : triangle ";
				break;
			case VectorPrimitiveType::triangler:
				cout << " Shape : triangler ";
				break;
			case VectorPrimitiveType::ellipse:
				cout << " Shape : ellipse ";
				break;
			default:
				break;
			}
			if (renderer.shapes[i].selected)
			{
				cout << " Selected : True ";
			}
			else
			{
				cout << " Selected : Flase ";
			}

			cout << endl;
		}
	}

}

// ====================== Utils =====================

void Application::button_pressed_saveCardToCollection()
{
	renderer.saveIMGtoCollection();
}

void Application::button_pressed_saveToFolder()
{
	renderer.saveToPath("card", "png");
}

// ====================== Card Preview ======================
void Application::button_pressed_hist()
{
	renderer.getcolor();
	renderer.state = hist;
}


// ====================== 3D Interface ======================
void Application::button_pressed_camswitch()
{
	if (renderer3d.camstate == multi)
	{
		renderer3d.camstate = freecam;
		return;
	}

	if (renderer3d.camstate == freecam)
	{
		renderer3d.camstate = multi;
		return;
	}
}

//Function to update the snap camera slider when more geos are added
void Application::updateSnapCamSlider()
{
	if (cam_snap.getMax() != renderer3d.bufferHead - 1 and renderer3d.geoCount != 0)
	{
		cam_snap.set("Snap Target", 0, 0, renderer3d.bufferHead - 1);
	}
}

void Application::button_pressed_snap_toggle()
{
	if (!renderer3d.snap_active)
	{
		renderer3d.snap_active = true;
	}

	else
	{
		renderer3d.snap_active = false;
		renderer3d.resetcam();
	}
}
void Application::button_pressed_help()
{
	if (!renderer3d.help_on)
	{
		renderer3d.help_on = true;
	}

	else
	{
		renderer3d.help_on = false;
	}
}

void Application::button_pressed_none()
{
	renderer3d.preview_mode = objectType3D::none;
}

void Application::button_addbox()
{
	renderer3d.preview_mode = objectType3D::box;
	renderer3d.resetGeoSelection();
}

void Application::button_addtetra()
{
	renderer3d.preview_mode = objectType3D::tetra;
	renderer3d.resetGeoSelection();
}

void Application::button_addocta()
{
	renderer3d.preview_mode = objectType3D::octa;
	renderer3d.resetGeoSelection();
}

void Application::button_adddodeca()
{
	renderer3d.preview_mode = objectType3D::dode;
	renderer3d.resetGeoSelection();
}

void Application::button_addcylinder()
{
	renderer3d.preview_mode = objectType3D::cylinder;
	renderer3d.resetGeoSelection();
}

void Application::button_addcone()
{
	renderer3d.preview_mode = objectType3D::cone;
	renderer3d.resetGeoSelection();
}

void Application::button_addsphere()
{
	renderer3d.preview_mode = objectType3D::sphere;
	renderer3d.resetGeoSelection();
}

void Application::button_addmodel()
{
	renderer3d.preview_mode = objectType3D::model;
	renderer3d.resetGeoSelection();
}

void Application::button_pressed_reset_cam()
{
	renderer3d.resetcam();
}

void Application::button_pressed_addgeo()
{
	renderer3d.addGeo(renderer3d.preview_mode);
	renderer3d.resetGeoSelection();
}

void Application::button_pressed_show_delim()
{
	if (!renderer3d.delim_on)
	{
		renderer3d.delim_on = true;
	}

	else
	{
		renderer3d.delim_on = false;
	}
}

void Application::button_pressed_snap()
{
	SnapGeo();
}

//Function that snaps the geo to the plane in the 3D mode, does not take in consideration rotations but works with scale
void Application::SnapGeo()
{
	switch (renderer3d.preview_mode)
	{

	case objectType3D::box:
		renderer3d.mesh = renderer3d.box.getMesh();
		slider_y = (renderer3d.getMinyGeo(renderer3d.mesh) * -1) * scale_y;
		break;

	case objectType3D::cylinder:
		renderer3d.mesh = renderer3d.cylinder.getMesh();
		slider_y = (renderer3d.getMinyGeo(renderer3d.mesh) * -1) * scale_y;
		break;

	case objectType3D::cone:
		renderer3d.mesh = renderer3d.cone.getMesh();
		slider_y = (renderer3d.getMinyGeo(renderer3d.mesh) * -1) * scale_y;
		break;

	case objectType3D::sphere:
		renderer3d.mesh = renderer3d.sphere.getMesh();
		slider_y = (renderer3d.getMinyGeo(renderer3d.mesh) * -1) * scale_y;
		break;

	case objectType3D::octa:
		slider_y = (sqrt(3) / 2 * slider_height) * scale_y;
		break;

	case objectType3D::dode:
		slider_y = slider_radius / 1.7 * scale_y;
		break;

	case objectType3D::model:
		//Does not work with the 2nd house model, didn't have time to fix.
		slider_y = (renderer3d.getMiny(renderer3d.vmodel[model])) * scale_y;
		break;
	}
}

void Application::selectGeo()
{

	renderer3d.selectClosestGeo();

	if (debug_ON)
	{
		system("CLS");
		for (int i = 0; i < renderer3d.bufferCount; i++)
		{
			cout << "Index : " << i;
			switch (renderer3d.geo[i].type)
			{
			case objectType3D::none:
				cout << " Shape : none ";
				break;
			case objectType3D::box:
				cout << " Shape : box ";
				break;
			case objectType3D::cylinder:
				cout << " Shape : cylinder ";
				break;
			case objectType3D::sphere:
				cout << " Shape : sphere ";
				break;
			case objectType3D::cone:
				cout << " Shape : cone ";
				break;
			case objectType3D::model:
				cout << " Shape : model ";
				break;

			default:
				break;
			}
			if (renderer3d.geo[i].selected)
			{
				cout << " Selected : True ";
			}
			else
			{
				cout << " Selected : Flase ";
			}

			cout << endl;
		}
	}
}
void Application::resetSelectionUIValues()
{
	batch_slider_height = 0;
	batch_slider_radius = 0;
	batch_slider_x = batch_slider_y = batch_slider_z = 0;
	batch_scale_x = batch_scale_y = batch_scale_z = 0;
	batch_rotat_x = batch_rotat_y = batch_rotat_z = 0;
}
void Application::button_pressed_2D()
{
	astate = state2D;
}