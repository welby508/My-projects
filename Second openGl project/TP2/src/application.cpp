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

	is_key_press_up = false;
	is_key_press_down = false;
	is_key_press_left = false;
	is_key_press_right = false;
}
void Application::setupGUI()
{
	//====================== main menu ======================
	gui.setup("Interface");
	button_collection.setup("Collection");
	button_collection.addListener(this, &Application::button_pressed_collection);
	gui.add(&button_collection);

	button_threed.setup("3D");
	button_threed.addListener(this, &Application::button_pressed_3D);
	gui.add(&button_threed);

	button_curves.setup("Curves");
	button_curves.addListener(this, &Application::button_pressed_curves);
	gui.add(&button_curves);

	button_surfaces.setup("Surfaces");
	button_surfaces.addListener(this, &Application::button_pressed_surfaces);
	gui.add(&button_surfaces);


	//====================== card preview ======================
	cardPreviewGUI.setup("Card preview");
	cardPreviewGUI.setPosition(225, 10);

	button_tone_filter.set("Tone Mapping");
	button_tone_filter.addListener(this, &Application::button_pressed_tone_filter);
	cardPreviewGUI.add(button_tone_filter);

	button_image_filter.set("Image Filtering");
	button_image_filter.addListener(this, &Application::button_pressed_image_Filter);
	cardPreviewGUI.add(button_image_filter);


	//====================== tone mapping ========================
	
	toneMapping.setup("Tone Mapping");
	toneMapping.setPosition(10, 250);

	slider_exposure.set("Exposure", 1, 0.0f, 5.0f);
	toneMapping.add(slider_exposure);
	slider_gamma.set("Gamma", 2.2f, 0.0f, 5.0f);
	toneMapping.add(slider_gamma);
	slider_mapping_type.set("Aces-Filmic", 1, 1, 5);
	toneMapping.add(slider_mapping_type);

	//====================== filtering ========================
	imageFilter.setup("Image Filter");
	imageFilter.setPosition(10, 250);

	button_filter1.set("Exposure filter");
	button_filter1.addListener(this, &Application::button_pressed_ExposureFilter);

	button_filter2.set("Invert color filter");
	button_filter2.addListener(this, &Application::button_pressed_InvertFilter);

	button_convolution_identity.set("Convolution: identity");
	button_convolution_identity.addListener(this, &Application::button_pressed_convolution_identity);

	button_convolution_emboss.set("Convolution: emboss");
	button_convolution_emboss.addListener(this, &Application::button_pressed_convolution_emboss);

	button_convolution_sharpen.set("Convolution: sharpen");
	button_convolution_sharpen.addListener(this, &Application::button_pressed_convolution_sharpen);

	button_convolution_edge_detect.set("Convolution: edge_detect");
	button_convolution_edge_detect.addListener(this, &Application::button_pressed_convolution_edge_detect);

	button_convolution_blur.set("Convolution: blur");
	button_convolution_blur.addListener(this, &Application::button_pressed_convolution_blur);


	imageFilter.add(button_filter1);
	imageFilter.add(color_picker.set("teinte", renderer.tint, ofColor(0, 0), ofColor(255, 255)));
	imageFilter.add(slider.set("mix", renderer.mix_factor, 0.0f, 1.0f));

	imageFilter.add(button_filter2);

	imageFilter.add(button_convolution_identity);
	imageFilter.add(button_convolution_emboss);
	imageFilter.add(button_convolution_sharpen);
	imageFilter.add(button_convolution_edge_detect);
	imageFilter.add(button_convolution_blur);



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

	button_help.setup("Help (H)");
	button_help.addListener(this, &Application::button_pressed_help);
	gui3D.add(&button_help);

	slider_shader_type.set("Default", 1, 1, 4);
	gui3D.add(slider_shader_type);

	button_pbr_toggle.set("Toggle PBR");
	button_pbr_toggle.addListener(this, &Application::button_pressed_pbr);
	gui3D.add(button_pbr_toggle);


	//PBR
	pbr_panel.setup("pbr material");
	pbr_panel.setPosition(750, 10);

	reset_pbr();

	group_material_color.setup("color");
	group_material_color.add(color_picker_ambient);
	group_material_color.add(color_picker_diffuse);
	group_material_color.add(color_picker_specular);
	pbr_panel.add(&group_material_color);

	group_material_factor.setup("factor");
	group_material_factor.add(slider_metallic);
	group_material_factor.add(slider_roughness);
	group_material_factor.add(slider_occlusion);
	group_material_factor.add(slider_brightness);
	pbr_panel.add(&group_material_factor);

	group_material_factor.add(slider_fresnel_ior);

	group_light.setup("light");
	group_light.add(slider_light_intensity);
	pbr_panel.add(&group_light);

	group_tone_mapping.setup("tone mapping");
	group_tone_mapping.add(slider_exposure_3D);
	group_tone_mapping.add(slider_gamma_3D);
	pbr_panel.add(&group_tone_mapping);

	//reset PBR button
	button_reset_pbr.set("Reset PBR");
	button_reset_pbr.addListener(this, &Application::button_pressed_reset_pbr);
	pbr_panel.add(button_reset_pbr);

	//interface util3d
	util3D.setup("Util");
	util3D.setPosition(10, 185);

	geo_group.setup("Geo");

	button_box.setup("Box");
	button_box.addListener(this, &Application::button_addbox);
	geo_group.add(&button_box);

	button_cylinder.setup("Cylinder");
	button_cylinder.addListener(this, &Application::button_addcylinder);
	geo_group.add(&button_cylinder);

	button_cone.setup("Cone");
	button_cone.addListener(this, &Application::button_addcone);
	geo_group.add(&button_cone);

	button_sphere.setup("Sphere");
	button_sphere.addListener(this, &Application::button_addsphere);
	geo_group.add(&button_sphere);

	util3D.add(&geo_group);

	add_geo_button.setup("Add Geo (G)");
	add_geo_button.addListener(this, &Application::button_pressed_addgeo);
	util3D.add(&add_geo_button);

	//interface transform 3D
	transform3D.setup("Transformation");
	transform3D.setPosition(10, 400);

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

	// ==================== light // material ===========================
	gui_Lights.setup("General Lights");
	guiLightPosition.set("X,Z=SpotLight/Y=SideLights", ofVec3f(0, 50, 0), ofVec3f(-500, -500, -500), ofVec3f(500, 500, 500));
	gui_Lights.add(guiLightPosition);
	gui_Lights.setPosition(215, 10);
	gui_Lights.add(ambientColor.set("Ambient", ofFloatColor(0.1f, 0.1f, 0.1f)));
	gui_Lights.add(directionalColor.set("Directional", ofColor(127, 127, 127, 127)));

	spot_light_group.setup("Spot Light Settings");
	spot_light_group.add(concentration.set("Concentration", 20, 0, 100));
	spot_light_group.add(cutoff.set("Cutoff", 45, 0, 50));

	gui_Lights.add(&spot_light_group);

	gui_Material.setup("Material");
	gui_Material.setPosition(750, 10);
	gui_Material.add(shininess.set("Shininess", 20, 1, 120));
	gui_Material.add(diffuse_color.set("Diffuse Color", ofFloatColor(1.0f, 0.0f, 0.0f)));
	gui_Material.add(specular_color.set("Specular Color", ofColor(255, 90, 0, 255)));
	gui_Material.add(ambient_color.set("Ambient Color", ofFloatColor(0.1f, 0.1f, 0.1f)));

}

void Application::reset_pbr()
{
	renderer3d.resetPBR();

	color_picker_ambient.set("ambient", renderer3d.material_color_ambient, ofColor(0, 0), ofColor(255, 255));
	color_picker_diffuse.set("diffuse", renderer3d.material_color_diffuse, ofColor(0, 0), ofColor(255, 255));
	color_picker_specular.set("specular", renderer3d.material_color_specular, ofColor(0, 0), ofColor(255, 255));

	slider_metallic.set("metallic", renderer3d.material_metallic, 0.0f, 1.0f);
	slider_roughness.set("roughness", renderer3d.material_roughness, 0.0f, 1.0f);
	slider_occlusion.set("occlusion", renderer3d.material_occlusion, 0.0f, 5.0f);
	slider_brightness.set("brightness", renderer3d.material_brightness, 0.0f, 5.0f);

	slider_fresnel_ior.set("fresnel ior", renderer3d.material_fresnel_ior, glm::vec3(0.0f), glm::vec3(1.0f));

	slider_light_intensity.set("intensity", renderer3d.light_intensity, 0.0f, 10.0f);

	slider_exposure_3D.set("exposure", 1.0f, 0.0f, 5.0f);
	slider_gamma_3D.set("gamma", 2.2f, 0.0f, 5.0f);

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
		renderer.update();
		updateRendererVars();
		time_current = ofGetElapsedTimef();
		time_elapsed = time_current - time_last;
		time_last = time_current;
		if (renderer.state == curves)
		{
			if (is_key_press_up)
				renderer.selected_ctrl_point->y -= renderer.delta_y * time_elapsed;
			if (is_key_press_down)
				renderer.selected_ctrl_point->y += renderer.delta_y * time_elapsed;
			if (is_key_press_left)
				renderer.selected_ctrl_point->x -= renderer.delta_x * time_elapsed;
			if (is_key_press_right)
				renderer.selected_ctrl_point->x += renderer.delta_x * time_elapsed;
		}
		if (renderer.state == surfaces)
		{
			if (is_key_press_up)
				renderer.selected_ctrl_point_coon->z -= renderer.delta_y * time_elapsed;
			if (is_key_press_down)
				renderer.selected_ctrl_point_coon->z += renderer.delta_y * time_elapsed;
;
		}


		break;
	case state3D:
		updateRenderer3DVars();
		renderer3d.update();
		break;
	default:
		break;
	}
}
void Application::updateRendererVars() {

	renderer.tone_mapping_exposure = slider_exposure;
	renderer.tone_mapping_gamma = slider_gamma;
	renderer.tone_mapping_style = slider_mapping_type;
	updateSliderType();

	renderer.tint = color_picker;
	renderer.mix_factor = slider;
}

void Application::updateSliderType()
{
	switch (slider_mapping_type)
	{
	case 1:
		slider_mapping_type.set("Aces-Filmic", 1, 1, 5);
		break;
	case 2:
		slider_mapping_type.set("Reinhard", 2, 1, 5);
		break;
	case 3:
		slider_mapping_type.set("Aces-Filmic Hill", 3, 1, 5);
		break;
	case 4:
		slider_mapping_type.set("SRGB", 4, 1, 5);
		break;
	case 5:
		slider_mapping_type.set("Burgess Dawson Filmic", 5, 1, 5);
		break;
	default:
		break;
	}
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
	renderer3d.cam_speed = cam_speed;
	renderer3d.cam_fov = cam_fov;

	renderer3d.light_pos.x = guiLightPosition->x;
	renderer3d.light_pos.y = guiLightPosition->y;
	renderer3d.light_pos.z = guiLightPosition->z;
	renderer3d.ambientLight = ambientColor;
	renderer3d.directionalColor = directionalColor;
	renderer3d.shininess = shininess;
	renderer3d.specular_color = specular_color;
	renderer3d.diffuse_color = diffuse_color;
	renderer3d.ambient_color = ambient_color;
	renderer3d.concentration = concentration;
	renderer3d.cutoff = cutoff;

	renderer3d.material_color_ambient = color_picker_ambient;
	renderer3d.material_color_diffuse = color_picker_diffuse;
	renderer3d.material_color_specular = color_picker_specular;

	renderer3d.material_metallic = slider_metallic;
	renderer3d.material_roughness = slider_roughness;
	renderer3d.material_occlusion = slider_occlusion;
	renderer3d.material_brightness = slider_brightness;

	renderer3d.material_fresnel_ior = slider_fresnel_ior;

	renderer3d.light_intensity = slider_light_intensity;

	renderer3d.tone_mapping_exposure = slider_exposure_3D;
	renderer3d.tone_mapping_gamma = slider_gamma_3D;

	updateSliderShader3D();
}

void Application::updateSliderShader3D()
{
	switch (slider_shader_type)
	{
	case 1:
		slider_shader_type.set("Default", 1, 1, 4);
		renderer3d.shader_active = ShaderType::none;
		break;
	case 2:
		slider_shader_type.set("Lambert", 2, 1, 4);
		renderer3d.shader_active = ShaderType::lambert;
		break;
	case 3:
		slider_shader_type.set("Gouraud", 3, 1, 4);
		renderer3d.shader_active = ShaderType::gouraud;
		break;
	case 4:
		slider_shader_type.set("Phong", 4, 1, 4);
		renderer3d.shader_active = ShaderType::phong;
		break;
	default:
		break;
	}
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
		if (help_on)
		{
			renderer3d.drawhelp();
		}
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
			if (renderer.toneMapping)
			{
				toneMapping.draw();
			}
			if (renderer.imageFilters)
			{
				imageFilter.draw();
			}
			break;

		case collection:
			break;

		case curves:
			break;

		default:
			break;
		}
		break;

		case state3D:
			gui3D.draw();
			gui_Lights.draw();
			if (renderer3d.pbr_toggle == false)
			{
				util3D.draw();
				if (renderer3d.preview_mode != objectType3D::none)
				{
					transform3D.draw();
					gui_Material.draw();
				}
			}
			else
			{
				pbr_panel.draw();
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

}
void Application::keyPressed(int key)
{
	if (astate == state2D)
	{
		switch (key)
		{
		case OF_KEY_LEFT:
			is_key_press_left = true;
			break;

		case OF_KEY_UP:
			is_key_press_up = true;
			break;

		case OF_KEY_RIGHT:
			is_key_press_right = true;
			break;

		case OF_KEY_DOWN:
			is_key_press_down = true;
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
		case OF_KEY_LEFT: // touche
			is_key_press_left = false;
			break;

		case OF_KEY_UP: // touche
			is_key_press_up = false;
			break;

		case OF_KEY_RIGHT: // touche
			is_key_press_right = false;
			break;

		case OF_KEY_DOWN: // touche
			is_key_press_down = false;
			break;
		default:
			break;
		}

		if (renderer.state == curves)
		{
			switch (key)
			{
			case 49: // touche 1
				renderer.selected_ctrl_point = &renderer.ctrl_point1;
				break;

			case 50: // touche 2
				renderer.selected_ctrl_point = &renderer.ctrl_point2;
				break;

			case 51: // touche 3
				renderer.selected_ctrl_point = &renderer.ctrl_point3;
				break;

			case 52: // touche 4
				renderer.selected_ctrl_point = &renderer.ctrl_point4;
				break;
			case 53: // touche 5
				renderer.selected_ctrl_point = &renderer.ctrl_point5;
				break;
			default:
				break;
			}
		}
		if (renderer.state == surfaces)
		{
			switch (key)
			{
			case 49: // touche 1
				renderer.selected_ctrl_point_coon = &renderer.ctrl_point1_coon;
				break;

			case 50: // touche 2
				renderer.selected_ctrl_point_coon = &renderer.ctrl_point2_coon;
				break;

			case 51: // touche 3
				renderer.selected_ctrl_point_coon = &renderer.ctrl_point3_coon;
				break;

			case 52: // touche 4
				renderer.selected_ctrl_point_coon = &renderer.ctrl_point4_coon;
				break;


			case 53: // touche 5
				renderer.selected_ctrl_point_coon = &renderer.ctrl_point12_coon;
				break;
			case 54: // touche 6
				renderer.selected_ctrl_point_coon = &renderer.ctrl_point13_coon;
				break;
			case 55: // touche 7
				renderer.selected_ctrl_point_coon = &renderer.ctrl_point24_coon;
				break;
			case 56: // touche 8
				renderer.selected_ctrl_point_coon = &renderer.ctrl_point34_coon;
				break;
			default:
				break;
			}
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
				gui_Material.setPosition(1230, 10);
				pbr_panel.setPosition(1230, 10);
				return;
			}

			if (renderer3d.camstate == freecam)
			{
				renderer3d.camstate = multi;
				gui_Material.setPosition(750, 10);
				pbr_panel.setPosition(750, 10);
				return;
			}
		}

			case 103: //g
				renderer3d.addGeo(renderer3d.preview_mode);
				break;

			case 104: //h
				if (!help_on)
				{
					help_on = true;
				}

				else
				{
					help_on = false;
				}
				break;

		case 108: // l
			if (renderer3d.directionalToggle == true)
			{
				renderer3d.directionalToggle = false;
			}
			else
			{
				renderer3d.directionalToggle = true;
			}
			break;

		case 111: // o
			if (renderer3d.sideLightToggle == true)
			{
				renderer3d.sideLightToggle = false;
			}
			else
			{
				renderer3d.sideLightToggle = true;
			}
			break;

		case 112: // p
			if (renderer3d.spotToggle == true)
			{
				renderer3d.spotToggle = false;
			}
			else
			{
				renderer3d.spotToggle = true;
			}
			break;

			case 116: // t


			case OF_KEY_LEFT: // left arrow
				renderer3d.directionalLight.setOrientation({ 0, 90, 0 });
				break;

			case OF_KEY_RIGHT: //right arrow 
				renderer3d.directionalLight.setOrientation({ 0, -90, 0 });
				break;

			case 49: // 1 
			case 50: // 2
			case 51: // 3
			case 52: // 4
			case 53: // 5
				renderer3d.iMainCamera = key - '1';
				break;

		case 54: // 6
			renderer3d.mainLight = 0;
			break;

		case 55: // 7
			renderer3d.mainLight = 1;
			break;

		case 56: // 8
			renderer3d.mainLight = 2;
			break;

		case 57: // 9
			renderer3d.mainLight = 3;
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

void Application::button_pressed_curves()
{
	renderer.state = curves;
	renderer.resetBezier();
}

void Application::button_pressed_surfaces()
{
	renderer.state = surfaces;
	renderer.resetCoonSurface();
}

void Application::button_pressed_3D()
{
	astate = state3D;
}


//====================== filter and tone mapping ===================
void Application::button_pressed_tone_filter()
{
	renderer.imageFilters = false;
	renderer.toneMapping = !renderer.toneMapping;
}

void Application::button_pressed_image_Filter()
{
		renderer.toneMapping = false;
		renderer.imageFilters = !renderer.imageFilters;
}

void Application::button_pressed_ExposureFilter()
{
	renderer.shader1_IsActive = !renderer.shader1_IsActive;
	renderer.updateFiltedImage = true;
}

void Application::button_pressed_InvertFilter()
{
	renderer.shader2_IsActive = !renderer.shader2_IsActive;
	renderer.updateFiltedImage = true;
}


//convolution filters
void Application::button_pressed_convolution_identity()
{
	renderer.convolutionFilter_IsActive[0] = !renderer.convolutionFilter_IsActive[0];
	renderer.updateFiltedImage = true;
}

void Application::button_pressed_convolution_emboss()
{
	renderer.convolutionFilter_IsActive[1] = !renderer.convolutionFilter_IsActive[1];
	renderer.updateFiltedImage = true;
}

void Application::button_pressed_convolution_sharpen()
{
	renderer.convolutionFilter_IsActive[2] = !renderer.convolutionFilter_IsActive[2];
	renderer.updateFiltedImage = true;
}

void Application::button_pressed_convolution_edge_detect()
{
	renderer.convolutionFilter_IsActive[3] = !renderer.convolutionFilter_IsActive[3];
	renderer.updateFiltedImage = true;
}

void Application::button_pressed_convolution_blur()
{
	renderer.convolutionFilter_IsActive[4] = !renderer.convolutionFilter_IsActive[4];
	renderer.updateFiltedImage = true;
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

void Application::button_pressed_help()
{
	if (!help_on)
	{
		help_on = true;
	}

	else
	{
		help_on = false;
	}
}

void Application::button_pressed_none()
{
	renderer3d.preview_mode = objectType3D::none;
}

void Application::button_addbox()
{
	renderer3d.preview_mode = objectType3D::box;
}

void Application::button_addcylinder()
{
	renderer3d.preview_mode = objectType3D::cylinder;
}

void Application::button_addcone()
{
	renderer3d.preview_mode = objectType3D::cone;
}

void Application::button_addsphere()
{
	renderer3d.preview_mode = objectType3D::sphere;
}

void Application::button_pressed_reset_cam()
{
	renderer3d.resetcam();
}

void Application::button_pressed_addgeo()
{
	renderer3d.addGeo(renderer3d.preview_mode);
}

void Application::button_pressed_2D()
{
	astate = state2D;
}

void Application::button_pressed_pbr()
{
	if (renderer3d.pbr_toggle == false)
	{
		renderer3d.pbr_toggle = true;
	}
	else
	{
		renderer3d.pbr_toggle = false;
	}
}

void Application::button_pressed_reset_pbr()
{
	reset_pbr();
}