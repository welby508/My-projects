#include "renderer.h"


//================================================
//---------------- Core functions ----------------
//================================================

void Renderer::setup()
{
	ofSetFrameRate(60);

	background.load(PATH_ASSETS + "background.PNG");


	cardSelected.loadImage(PATH_ASSETS + "gradient.png");
	selectBorder.loadImage(PATH_ASSETS + "selectBorder.png");

	 LoadCardsFromFolder();
	 ofDisableArbTex();
	 shaderMap.load(PATH_SHADERS + "Tone_Mapping\\tone_mapping_330_vs.glsl", PATH_SHADERS + "Tone_Mapping\\tone_mapping_330_fs.glsl");

	 tint.set(255, 255, 255);
	 mix_factor = 0.618f;
	 shader1.load(PATH_SHADERS + "Tint\\image_filter_330_vs.glsl", PATH_SHADERS +  "Tint\\image_filter_330_fs.glsl");
	 shader2.load(PATH_SHADERS + "Invert\\invert_color_330_vs.glsl", PATH_SHADERS + "Invert\\invert_color_330_fs.glsl");

	 //setupBezier();
	 setupCoonSurface();

}

void Renderer::update()
{
	switch (state)
	{
	case idle:
		break;
	case cardPreview:
		if (updateFiltedImage)
		{
			filteredImage = previewCard;
			renderFilteredImage();
			updateFiltedImage = false;
		}
		break;
	case collection:
		break;
	case curves:
		updateBezier();
		break;
	case surfaces:
		updateCoonSurface();
		break;
	case debug:
		break;
	default:
		break;
	}
}

void Renderer::draw()
{
	ofSetColor(255, 255, 255);
	background.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	switch (state)
	{

	case idle:

		break;

	case cardPreview:

		drawCardPreview();
		break;

	case collection:
		
		drawCollection(240, 0, 1200, 800, 375 * 0.5, 518 * 0.5, vCardCollection);
		break;

	case curves:
		drawBezier();
		break;

	case surfaces:
		drawCoonSurface();
		break;

	case debug:
		break;

	default:
		break;
	}
}

Renderer::~Renderer()
{
}

//================================================
//------- collection and draw single card --------
//================================================

//Function  that draws the current collection on screen
void Renderer::drawCollection(const int x0, const int y0, const int w, const int h, const int cardW, const int cardH, const vector<ofImage>& vCardCollection, const int padding)
{

	int currentX;
	int currentY;
	int maxX = x0 + w - cardW - padding;
	int maxY = y0 + h - cardH - padding;

	int i = 0;
	currentY = y0 + padding;

	while (currentY < maxY)
	{
		currentX = x0 + padding;
		while (currentX < maxX)
		{
			drawCard(vCardCollection[i], currentX, currentY, cardW, cardH);
			i++;
			currentX += cardW + padding;

			if (i == vCardCollection.size())
				return;
		}
		currentY += cardH + padding;
	}
}

//Function that sets the dimension and variable for preview mode of a card
void Renderer::drawCard(const ofImage& image, const int x, const int y, const int w = 375, const int h = 518)
{
	int padding = 0.2 * w;

	if (mouseOnCard(mouse_current_x, mouse_current_y, x, y, w, h))
	{
		cardSelected.draw(x - padding, y - padding, w + padding * 2, h + padding * 2);

		cardSelected.draw(x - padding, y - padding, w + padding * 2, h + padding * 2);
		selectBorder.draw(x, y, w, h);

		if (is_mouse_button_pressed and state != cardPreview)
		{
			previewCard = image;
			state = cardPreview;
			updateOnce = true;
		}
	}

	image.draw(x, y, w, h);
}

//Function that detects when the mouse is on a card during collection or preview moode
bool Renderer::mouseOnCard(const int mouseX, const int mouseY, const int x, const int y, const int w = 375, const int h = 518)
{
	return (mouseX > x and mouseX < x + w and mouseY > y and mouseY < y + h);
}

//Function that draws the selected card in preview mode
void Renderer::drawCardPreview()
{
	drawCard(previewCard, (ofGetWindowWidth() / 2) - 375, (ofGetWindowHeight() / 2) - 259);
	if (toneMapping)
	{
		applyToneMapping();
	}
	if (imageFilters)
	{
		drawCard(filteredImage, (ofGetWindowWidth() / 2), (ofGetWindowHeight() / 2) - 259);
	}
}

//Render the filtered image
void Renderer::renderFilteredImage()
{
	ofImage image = filteredImage;
	ofFbo fbo;
	ofPixels pixels;

	ofSetColor(255, 255, 255, 255);
	fbo.allocate(375, 518);

	if (shader1_IsActive)
	{
		shader1.begin();
		fbo.begin();
		shader1.setUniformTexture("image", image.getTexture(), 1);
		shader1.setUniform3f("tint", tint.r / 255.0f, tint.g / 255.0f, tint.b / 255.0f);
		shader1.setUniform1f("factor", mix_factor);
		image.draw(0, 0);
		fbo.end();
		shader1.end();

		fbo.readToPixels(pixels);
		image = ofImage(pixels);
	}

	if (shader2_IsActive)
	{
		shader2.begin();
		fbo.begin();
		image.draw(0, 0);
		fbo.end();
		shader2.end();

		fbo.readToPixels(pixels);
		image = ofImage(pixels);
	}

	

	//Source module 6: Exemple 3 (IFT3100H21_Convolution)
	//Ne fonctionne pas avec openGL 3
	for (size_t k = 0; k < 5; k++)
	{
		if (convolutionFilter_IsActive[k])
		{
			// dimensions de l'image source
			int image_width = image.getWidth();
			int image_height = image.getHeight();

			// résolution du kernel de convolution
			const int kernel_size = 3;

			// décalage à partir du centre du kernel
			const int kernel_offset = kernel_size / 2;

			// nombre de composantes de couleur (RGBA)
			const int color_component_count = 4;

			// indices de l'image
			int x, y;

			// indices du kernel
			int i, j;

			// index des composantes de couleur
			int c;

			// index du pixel de l'image source utilisé pour le filtrage
			int pixel_index_img_src;

			// index du pixel de l'image de destination en cours de filtrage
			int pixel_index_img_dst;

			// index du pixel de l'image de destination en cours de filtrage
			int kernel_index;

			// valeur à un des indices du kernel de convolution
			float kernel_value;

			// extraire les pixels de l'image source
			ofPixels pixel_array_src = image.getPixels();

			// extraire les pixels de l'image de destination
			ofPixels pixel_array_dst = image.getPixels();

			// couleur du pixel lu dans l'image source
			ofColor pixel_color_src;

			// couleur du pixel à écrire dans l'image de destination
			ofColor pixel_color_dst;

			// somme du kernel appliquée à chaque composante de couleur d'un pixel
			float sum[color_component_count];

			// itération sur les rangées des pixels de l'image source
			for (y = 0; y < image_height; ++y)
			{
				// itération sur les colonnes des pixels de l'image source
				for (x = 0; x < image_width; ++x)
				{
					// initialiser le tableau où les valeurs de filtrage sont accumulées
					for (c = 0; c < color_component_count; ++c)
						sum[c] = 0;

					// déterminer l'index du pixel de l'image de destination
					pixel_index_img_dst = (image_width * y + x) * color_component_count;

					// itération sur les colonnes du kernel de convolution
					for (j = -kernel_offset; j <= kernel_offset; ++j)
					{
						// itération sur les rangées du kernel de convolution
						for (i = -kernel_offset; i <= kernel_offset; ++i)
						{
							// déterminer l'index du pixel de l'image source à lire
							pixel_index_img_src = (image_width * (y - j) + (x - i)) * color_component_count;

							// lire la couleur du pixel de l'image source
							pixel_color_src = pixel_array_src.getColor(pixel_index_img_src);

							// déterminer l'indice du facteur à lire dans le kernel de convolution
							kernel_index = kernel_size * (j + kernel_offset) + (i + kernel_offset);

							// extraction de la valeur à cet index du kernel
							switch (kernel_type[k])
							{
							case ConvolutionKernel::identity:
								kernel_value = convolution_kernel_identity.at(kernel_index);
								break;

							case ConvolutionKernel::emboss:
								kernel_value = convolution_kernel_emboss.at(kernel_index);
								break;

							case ConvolutionKernel::sharpen:
								kernel_value = convolution_kernel_sharpen.at(kernel_index);
								break;

							case ConvolutionKernel::edge_detect:
								kernel_value = convolution_kernel_edge_detect.at(kernel_index);
								break;

							case ConvolutionKernel::blur:
								kernel_value = convolution_kernel_blur.at(kernel_index);
								break;

							default:
								kernel_value = convolution_kernel_identity.at(kernel_index);
								break;
							}

							// itération sur les composantes de couleur
							for (c = 0; c < color_component_count; ++c)
							{
								// accumuler les valeurs de filtrage en fonction du kernel de convolution
								sum[c] = sum[c] + kernel_value * pixel_color_src[c];
							}
						}
					}

//Function that assign Uniform to the Tone Mapping Shader and draws the card with the changes
					// déterminer la couleur du pixel à partir des valeurs de filtrage accumulées pour chaque composante
					for (c = 0; c < color_component_count; ++c)
					{
						// conversion vers entier et validation des bornes de l'espace de couleur
						pixel_color_dst[c] = (int)ofClamp(sum[c], 0, 255);
					}

					// écrire la couleur à l'index du pixel en cours de filtrage
					pixel_array_dst.setColor(pixel_index_img_dst, pixel_color_dst);
				}
			}
			// écrire les pixels dans l'image de destination
			image.setFromPixels(pixel_array_dst);
		}
	}

	filteredImage = image;
}

void Renderer::applyToneMapping()
{
	shaderMap.begin();

	shaderMap.setUniformTexture("image", previewCard.getTexture(), 1);

	shaderMap.setUniform1f("tone_mapping_exposure", tone_mapping_exposure);
	shaderMap.setUniform1f("tone_mapping_gamma", tone_mapping_gamma);
	shaderMap.setUniform1i("tone_mapping_style", tone_mapping_style);

	drawCard(previewCard, (ofGetWindowWidth() / 2), (ofGetWindowHeight() / 2) - 259);


	shaderMap.end();
}


void Renderer::LoadCardsFromFolder()
{
	ofDirectory dir(PATH_COLLECTION);
	dir.allowExt("png");
	dir.listDir();
	for (size_t i = 0; i < dir.size(); i++)
	{
		ofImage temp;
		/*cout << dir.getPath(i) << endl;*/
		temp.load(dir.getPath(i));
		vCardCollection.push_back(temp);

	}
}


//============== curves =========================
// Source: IFT3100H21_CurveBezier/renderer.cpp
void Renderer::bezier_cubic(
	float t,
	float p1x, float p1y, float p1z,
	float p2x, float p2y, float p2z,
	float p3x, float p3y, float p3z,
	float p4x, float p4y, float p4z,
	float p5x, float p5y, float p5z,
	float& x, float& y, float& z)
{
	float u = 1 - t;
	float uu = u * u;
	float uuu = uu * u;
	float uuuu = uuu * u;
	float tt = t * t;
	float ttt = tt * t;
	float tttt = ttt * t;

	x = uuuu * p1x + 4 * uuu * t * p2x + 6 * uu * tt * p3x + 4 * u * ttt * p4x + tttt * p5x;
	y = uuuu * p1y + 4 * uuu * t * p2y + 6 * uu * tt * p3y + 4 * u * ttt * p4y + tttt * p5y;
	z = uuuu * p1z + 4 * uuu * t * p2z + 6 * uu * tt * p3z + 4 * u * ttt * p4z + tttt * p5z;

}

void Renderer::setupBezier()
{
	ofSetBackgroundColor(0);

	// paramètres
	line_resolution = 500;

	radius = 32.0f;

	// initialisation des sommets de la ligne
	for (index = 0; index <= line_resolution; ++index)
		line_renderer.addVertex(ofPoint());


	// initialisation de la scène
	delta_x = 250.0f;
	delta_y = 250.0f;
	resetBezier();
}

void Renderer::updateBezier()
{
	// approximation de la courbe avec une séquence de lignes
	for (index = 0; index <= line_resolution; ++index)
	{

		bezier_cubic(
			index / (float)line_resolution,
			ctrl_point1.x, ctrl_point1.y, ctrl_point1.z,
			ctrl_point2.x, ctrl_point2.y, ctrl_point2.z,
			ctrl_point3.x, ctrl_point3.y, ctrl_point3.z,
			ctrl_point4.x, ctrl_point4.y, ctrl_point4.z,
			ctrl_point5.x, ctrl_point5.y, ctrl_point5.z,
			position.x, position.y, position.z);


		// affecter la position du point sur la courbe
		line_renderer[index] = position;
	}
}

void Renderer::drawBezier()
{
	// dessiner les positions initiales
	ofSetColor(63, 63, 63);
	ofDrawEllipse(initial_position1.x, initial_position1.y, radius / 2.0f, radius / 2.0f);
	ofDrawEllipse(initial_position2.x, initial_position2.y, radius / 2.0f, radius / 2.0f);
	ofDrawEllipse(initial_position3.x, initial_position3.y, radius / 2.0f, radius / 2.0f);
	ofDrawEllipse(initial_position4.x, initial_position4.y, radius / 2.0f, radius / 2.0f);
	ofDrawEllipse(initial_position5.x, initial_position5.y, radius / 2.0f, radius / 2.0f);


	// dessiner la courbe
	ofSetColor(0, 255, 0);
	line_renderer.draw();

	// dessiner les points de contrôle
	ofSetColor(255, 0, 0);
	ofDrawEllipse(ctrl_point1.x, ctrl_point1.y, radius, radius);
	ofDrawEllipse(ctrl_point2.x, ctrl_point2.y, radius, radius);
	ofDrawEllipse(ctrl_point3.x, ctrl_point3.y, radius, radius);
	ofDrawEllipse(ctrl_point4.x, ctrl_point4.y, radius, radius);
	ofDrawEllipse(ctrl_point5.x, ctrl_point5.y, radius, radius);

	ofSetColor(255, 255, 255);

}

void Renderer::resetBezier()
{
	// initialisation des variables
	framebuffer_width = ofGetWidth();
	framebuffer_height = ofGetHeight();

	// ratios de positionnement dans la fenêtre
	float w_1_8 = framebuffer_width / 8.0f;
	float w_1_4 = framebuffer_width / 4.0f;
	float w_1_2 = framebuffer_width / 2.0f;
	float w_3_4 = framebuffer_width * 3.0f / 4.0f;
	float w_7_8 = framebuffer_width * 7.0f / 8.0f;
	float h_1_5 = framebuffer_height / 5.0f;
	float h_1_3 = framebuffer_height / 3.0f;
	float h_4_5 = framebuffer_height * 4.0f / 5.0f;

	initial_position1 = { w_1_8, h_4_5, 0 };
	initial_position2 = { w_1_4, h_1_3, 0 };
	initial_position3 = { w_1_2, h_1_5, 0 };
	initial_position4 = { w_3_4, h_1_3, 0 };
	initial_position5 = { w_7_8, h_4_5, 0 };

	ctrl_point1 = initial_position1;
	ctrl_point2 = initial_position2;
	ctrl_point3 = initial_position3;
	ctrl_point4 = initial_position4;
	ctrl_point5 = initial_position5;

	selected_ctrl_point = &ctrl_point1;
}

//============== surface =========================
// Code inspiré d'un tutoriel en ligne sur les mesh
//Source: https://github.com/danbz/art-and-code/blob/master/openFrameworks/meshExample/src/ofApp.cpp

void Renderer::bezier_quadratic(
	float t,
	float p1x, float p1y, float p1z,
	float p2x, float p2y, float p2z,
	float p3x, float p3y, float p3z,
	float& x, float& y, float& z)
{
	float u = 1 - t;
	float uu = u * u;
	float tt = t * t;

	x = uu * p1x + 2 * u * t * p2x + tt * p3x;
	y = uu * p1y + 2 * u * t * p2y + tt * p3y;
	z = uu * p1z + 2 * u * t * p2z + tt * p3z;

}

//La surface ne fonctionne pas à 100%, j'ai laissé mon essais de code plus bas.
//Source des formules : https://en.wikipedia.org/wiki/Coons_patch

//ofVec3f Renderer::bezier_cubic_X(float t, float s)
//{
//	float u = 1 - t;
//
//	ofVec3f posUP;
//	ofVec3f posDOWN;
//
//	bezier_cubic2(
//		s / (float)line_resolution,
//		ctrl_point1_coon.x, ctrl_point1_coon.y, ctrl_point1_coon.z,
//		ctrl_point12_coon.x, ctrl_point12_coon.y, ctrl_point12_coon.z,
//		ctrl_point2_coon.x, ctrl_point2_coon.y, ctrl_point2_coon.z,
//		posDOWN.x, posDOWN.y, posDOWN.z);
//
//	bezier_cubic2(
//		s / (float)line_resolution,
//		ctrl_point3_coon.x, ctrl_point3_coon.y, ctrl_point3_coon.z,
//		ctrl_point34_coon.x, ctrl_point34_coon.y, ctrl_point34_coon.z,
//		ctrl_point4_coon.x, ctrl_point4_coon.y, ctrl_point4_coon.z,
//		posUP.x, posUP.y, posUP.z);
//
//
//	return (u*posDOWN + t*posUP);
//
//}
//
//ofVec3f Renderer::bezier_cubic_Y(float t, float s)
//{
//	float u = 1 - s;
//
//	ofVec3f posLEFT;
//	ofVec3f posRIGHT;
//
//	bezier_cubic2(
//		t / (float)line_resolution,
//		ctrl_point1_coon.x, ctrl_point1_coon.y, ctrl_point1_coon.z,
//		ctrl_point13_coon.x, ctrl_point13_coon.y, ctrl_point13_coon.z,
//		ctrl_point3_coon.x, ctrl_point3_coon.y, ctrl_point3_coon.z,
//		posLEFT.x, posLEFT.y, posLEFT.z);
//
//	bezier_cubic2(
//		t / (float)line_resolution,
//		ctrl_point2_coon.x, ctrl_point2_coon.y, ctrl_point2_coon.z,
//		ctrl_point24_coon.x, ctrl_point24_coon.y, ctrl_point24_coon.z,
//		ctrl_point4_coon.x, ctrl_point4_coon.y, ctrl_point4_coon.z,
//		posRIGHT.x, posRIGHT.y, posRIGHT.z);
//
//
//	return (u * posLEFT + s * posRIGHT);
//
//}
//
//ofVec3f Renderer::bezier_cubic_corners(float s, float t)
//{
//	float u = 1 - t;
//	float v = 1 - s;
//	return (mainMesh.getVertex(0)*v*u + mainMesh.getVertex(width-1)*s*u + 
//		mainMesh.getVertex(width*height - width - 1)*v*t, mainMesh.getVertex(width * height - 1)*s*t);
//
//}
//
//ofVec3f Renderer::bezier_cubic_bilinearBlend(float t, float s)
//{
//	return (bezier_cubic_X(s,t) + bezier_cubic_Y(s,t) - bezier_cubic_corners(s,t));
//
//}


void Renderer::setupCoonSurface()
{
	
	ofSetBackgroundColor(0);

	// paramètres
	line_resolution = 500;

	radius = 5.0f;

	// initialisation des sommets de la ligne
	for (index = 0; index <= line_resolution; ++index)
		line_renderer.addVertex(ofPoint());


	// initialisation de la scène
	delta_x = 250.0f;
	delta_y = 250.0f;
	resetBezier();



	mainMesh.setMode(OF_PRIMITIVE_TRIANGLES);


	//set the width and height for our mesh and initial rendering values
	width = 500;
	height = 500;

	mainCam.setPosition(0, 0, 500); // set initial position for our easyCam 3D viewer

   // here we make the points inside our mesh
   // add one vertex to the mesh across our width and height
   // we use these x and y values to set the x and y co-ordinates of the mesh, adding a z value of zero to complete the 3d location of each vertex

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			mainMesh.addVertex(ofPoint(x - width / 2, y - height / 2, 0)); // mesh index = x + y*width
		}
	}

	// here we loop through and join the vertices together as indices to make rows of triangles to make the wireframe grid

	for (int y = 0; y < height - 1; y++) {
		for (int x = 0; x < width - 1; x++) {
			mainMesh.addIndex(x + y * width);                // 0
			mainMesh.addIndex((x + 1) + y * width);            // 1
			mainMesh.addIndex(x + (y + 1) * width);            // 10

			mainMesh.addIndex((x + 1) + y * width);            // 1
			mainMesh.addIndex((x + 1) + (y + 1) * width);        // 11
			mainMesh.addIndex(x + (y + 1) * width);            // 10
		}
	}

	resetCoonSurface();

}

void Renderer::updateCoonSurface()
{
	// ============================ approximation des 4 courbes =============================
	//C1 (horizontal bas)
	for (int index = 0, resStep = 0; index <= width; index++, resStep++)
	{

		bezier_quadratic(
			resStep / (float)line_resolution,
			ctrl_point1_coon.x, ctrl_point1_coon.y, ctrl_point1_coon.z,
			ctrl_point12_coon.x, ctrl_point12_coon.y, ctrl_point12_coon.z,
			ctrl_point2_coon.x, ctrl_point2_coon.y, ctrl_point2_coon.z,
			position.x, position.y, position.z);


		// affecter la position du point sur la surface
		mainMesh.setVertex(index, position);
	}

	//C2 (horizontal haut)
	for (int index = width*(height-1), resStep = 0; index < width*height; index++, resStep++)
	{

		bezier_quadratic(
			resStep / (float)line_resolution,
			ctrl_point3_coon.x, ctrl_point3_coon.y, ctrl_point3_coon.z,
			ctrl_point34_coon.x, ctrl_point34_coon.y, ctrl_point34_coon.z,
			ctrl_point4_coon.x, ctrl_point4_coon.y, ctrl_point4_coon.z,
			position.x, position.y, position.z);


		// affecter la position du point sur la surface
		mainMesh.setVertex(index, position);
	}

	//C3 (vertical gauche)
	for (int index = 0, resStep = 0; index < width * height; index+=height, resStep++)
	{

		bezier_quadratic(
			resStep / (float)line_resolution,
			ctrl_point1_coon.x, ctrl_point1_coon.y, ctrl_point1_coon.z,
			ctrl_point13_coon.x, ctrl_point13_coon.y, ctrl_point13_coon.z,
			ctrl_point3_coon.x, ctrl_point3_coon.y, ctrl_point3_coon.z,
			position.x, position.y, position.z);


		// affecter la position du point sur la surface
		mainMesh.setVertex(index, position);
	}

	//C4 (vertical droite)
	for (int index = width-1, resStep = 0; index < width * height; index += height, resStep++)
	{

		bezier_quadratic(
			resStep / (float)line_resolution,
			ctrl_point2_coon.x, ctrl_point2_coon.y, ctrl_point2_coon.z,
			ctrl_point24_coon.x, ctrl_point24_coon.y, ctrl_point24_coon.z,
			ctrl_point4_coon.x, ctrl_point4_coon.y, ctrl_point4_coon.z,
			position.x, position.y, position.z);


		// affecter la position du point sur la surface
		mainMesh.setVertex(index, position);
	}

	// ============================ Calcul des positions =============================
	float newZ;
	float cornerZ;
	ofVec3f refPos_Left;
	ofVec3f refPos_Right;
	ofVec3f refPos_Up;
	ofVec3f refPos_Down;
	ofVec3f oldPos;

	float Z_1;
	float Z_2;

	float refXPercentage;
	float refYPercentage;

	for (int y = 1; y < height-1; y++) {
		for (int x = 1; x < width-1; x++) {
			refPos_Left = mainMesh.getVertex(y * width);
			refPos_Right = mainMesh.getVertex(y * width + width-1);
			refPos_Up= mainMesh.getVertex(x + (width-1 * width));
			refPos_Down = mainMesh.getVertex(x);
			refXPercentage = x / width;
			refYPercentage = y / height;


			Z_1 = refPos_Down.z * (1 - refYPercentage) + refPos_Up.z * refYPercentage;
			Z_2 = refPos_Left.z * (1 - refXPercentage) + refPos_Right.z * refXPercentage;


			cornerZ = (mainMesh.getVertex(0).z * (1 - refXPercentage) * (1 - refYPercentage) +
				mainMesh.getVertex(width - 1).z * refXPercentage * (1 - refYPercentage) +
				mainMesh.getVertex(width * height - width - 1).z * (1 - refXPercentage) * refYPercentage +
				mainMesh.getVertex(width * height - 1).z * refXPercentage * refYPercentage);

			newZ = Z_1 + Z_2 - cornerZ;

			oldPos = mainMesh.getVertex(x + (y * width));
			oldPos.z = newZ;
			mainMesh.setVertex(x + (y * width), oldPos);
		}
	}

}


void Renderer::drawCoonSurface()
{
	glPointSize(10);
	mainCam.begin();
	ofSetColor(255,0,0);

	mainMesh.drawVertices();


	// Draw control points
	ofSetColor(255, 255, 255);
	ofDrawSphere(ctrl_point1_coon.x, ctrl_point1_coon.y, ctrl_point1_coon.z, radius);
	ofDrawSphere(ctrl_point2_coon.x, ctrl_point2_coon.y, ctrl_point2_coon.z, radius);
	ofDrawSphere(ctrl_point3_coon.x, ctrl_point3_coon.y, ctrl_point3_coon.z, radius);
	ofDrawSphere(ctrl_point4_coon.x, ctrl_point4_coon.y, ctrl_point4_coon.z, radius);

	ofDrawSphere(ctrl_point12_coon.x, ctrl_point12_coon.y, ctrl_point12_coon.z, radius);
	ofDrawSphere(ctrl_point13_coon.x, ctrl_point13_coon.y, ctrl_point13_coon.z, radius);
	ofDrawSphere(ctrl_point24_coon.x, ctrl_point24_coon.y, ctrl_point24_coon.z, radius);
	ofDrawSphere(ctrl_point34_coon.x, ctrl_point34_coon.y, ctrl_point34_coon.z, radius);

	mainCam.end();

}

void Renderer::resetCoonSurface()
{


	initial_position1_coon = { -width/2, -height/2, 0 };
	initial_position2_coon = { width/2, - height/2, 0 };
	initial_position3_coon = { -width/2, height/2, 0 };
	initial_position4_coon = { width/2, height/2, 0 };

	initial_position12_coon = {0, -height/2 ,100};
	initial_position13_coon = {-width/2 , 0 , 100};
	initial_position24_coon = {width/2 , 0 , 100};
	initial_position34_coon = {0, height/2 , 100};

	ctrl_point1_coon = initial_position1_coon;
	ctrl_point2_coon = initial_position2_coon;
	ctrl_point3_coon = initial_position3_coon;
	ctrl_point4_coon = initial_position4_coon;

	ctrl_point12_coon = initial_position12_coon;
	ctrl_point13_coon = initial_position13_coon;
	ctrl_point24_coon = initial_position24_coon;
	ctrl_point34_coon = initial_position34_coon;

	selected_ctrl_point_coon = &ctrl_point1_coon;
}