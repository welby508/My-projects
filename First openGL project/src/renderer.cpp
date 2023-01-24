#include "renderer.h"


//================================================
//---------------- Core functions ----------------
//================================================

void Renderer::setup()
{
	ofSetFrameRate(60);

	background.load(PATH_ASSETS + "background.PNG");
	canvasback.load(PATH_ASSETS + "canvasback.png");
	canvasback.setCompression(OF_COMPRESS_SRGB);

	canvasbackground.load(PATH_ASSETS + "backgroundcanvas.png");

	createCardSetup();

	cardSelected.loadImage(PATH_ASSETS + "gradient.png");
	selectBorder.loadImage(PATH_ASSETS + "selectBorder.png");

	 fontTitle.load(PATH_FONTS + "BelweBold.otf", 50);
	 fontText.load(PATH_FONTS + "BelweBold.otf", 20);

	 LoadCardsFromFolder();
}

void Renderer::update()
{}

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

	case createcard:
		drawCanvas();
		break;

	case randomcard:
		drawRandomCard(randomNumbers);
		break;

	case hist:

		drawHistograms(200, 800);
		break;
	case debug:
		break;
	default:
		break;

	}
}

Renderer::~Renderer()
{
	std::free(shapes);
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
	drawCard(previewCard, 50, 50);
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


//================================================
//---------------- random card -------------------
//================================================

//Function that crops the proper section of a card to then draw a randomized one with them
void Renderer::drawRandomCard(const int randomNumbers[])
{
	int midscreenX = ofGetWindowWidth() / 2;
	int midscreenY = (ofGetWindowHeight() / 2) - 25;
	int offsetX = ofGetWindowWidth() / 8;
	int offsetY = ofGetWindowHeight() / 3;

	const paramCoords rCoords[PARAM_NUM] = {
		{77, 0, 250, 500},		//portrait
		{35, 270, 284, 60},		//name
		{41, 330, 271, 176},	//text
		{168, 310, 26, 37},		//rarity
		{5, 40, 85, 87},		//mana cost
		{0, 440, 92, 56},		//attack
		{289, 446, 43, 57}		//health
	};

	vCardCollection[randomCardNumber].draw(midscreenX - offsetX, midscreenY - offsetY);

	for (int i = 0; i < PARAM_NUM; i++)
	{
		ofImage temp;
		temp.cropFrom(vCardCollection[randomNumbers[i]], rCoords[i].x, rCoords[i].y, rCoords[i].w, rCoords[i].h);
		temp.draw(midscreenX - offsetX + rCoords[i].x, midscreenY - offsetY + rCoords[i].y, rCoords[i].w, rCoords[i].h);
	}
}

//Overloaded function that is used to initialize the random section to avoid some bugs/crashes
void Renderer::drawRandomCard(const int randomNumbers[], ofImage img)
{
	int midscreenX = ofGetWindowWidth() / 2;
	int midscreenY = (ofGetWindowHeight() / 2) - 25;
	int offsetX = ofGetWindowWidth() / 8;
	int offsetY = ofGetWindowHeight() / 3;

	const paramCoords rCoords[PARAM_NUM] = {
		{77, 0, 250, 500},		//portrait
		{35, 270, 284, 60},		//name
		{41, 330, 271, 176},	//text
		{168, 310, 26, 37},		//rarity
		{5, 40, 85, 87},		//mana cost
		{0, 440, 92, 56},		//attack
		{289, 446, 43, 57}		//health
	};

	img.draw(0, 0);

	for (int i = 1; i < PARAM_NUM; i++)
	{
		ofImage temp;
		temp.cropFrom(vCardCollection[randomNumbers[i]], rCoords[i].x, rCoords[i].y, rCoords[i].w, rCoords[i].h);
		temp.draw(0 + rCoords[i].x, 0 + rCoords[i].y, rCoords[i].w, rCoords[i].h);
	}
}

//================================================
//------------- Create a custom card -------------
//================================================

//Function that updates the random values for random card creation
void Renderer::updateRandomNumbers()
{
	if (vCardCollection.size() > 0)
	{
		randomCardNumber = rand() % vCardCollection.size();
		for (int i = 0; i < PARAM_NUM; i++)
		{
			randomNumbers[i] = rand() % vCardCollection.size();
		}
	}
}

//Function that sets up the necessary variable for the createcard mode (vectorial drawing)
void Renderer::createCardSetup()
{
	shapeCount = 0;
	bufferCount = 100;
	bufferHead = 0;
	bufferStride = sizeof(VectorPrimitive);
	bufferSize = bufferCount * bufferStride;
	shapes = (VectorPrimitive*)std::malloc(bufferSize);
	draw_mode = VectorPrimitiveType::none;
}

//Function that draws a canvas in the middle of the app screen
void Renderer::drawCanvas()
{
	int midscreenX = ofGetWindowWidth() / 2;
	int midscreenY = (ofGetWindowHeight() / 2) - 25;

	int canvasW = 650;
	int canvasH = 755;
	int offsetX2 = canvasW / 2;
	int offsetY2 = canvasH / 2;

	ofFill();
	ofSetLineWidth(0);
	ofSetColor(canvas);
	ofDrawRectangle(midscreenX - offsetX2, midscreenY - offsetY2, canvasW, canvasH);
	//canvasback.draw(midscreenX - offsetX2, midscreenY - offsetY2, canvasW, canvasH);

	ofNoFill();
	ofSetLineWidth(5);
	ofSetColor(0, 0, 0);
	ofDrawRectangle(midscreenX - offsetX2, midscreenY - offsetY2, canvasW, canvasH);

	//Redrawing the background with a hole that fits the canvas size so that shapes do are not drawn outside of the canvas
	drawShapes();
	ofSetColor(255, 255, 255);
	canvasbackground.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

	if (is_mouse_button_pressed)
	{
		draw_zone(
			mouse_press_x,
			mouse_press_y,
			mouse_current_x,
			mouse_current_y);
	}
}

//Function that draws all the current shapes that were added with their proper component
void Renderer::drawShapes()
{
	for (int index = 0; index < bufferCount; ++index)
	{
		switch (shapes[index].type)
		{

		case VectorPrimitiveType::line:

			ofNoFill();
			ofSetLineWidth(shapes[index].stroke_width);
			if (shapes[index].selected)
			{
				ofSetColor(255,0,0);
			}
			else
			{
				ofSetColor(
					shapes[index].stroke_color[0],
					shapes[index].stroke_color[1],
					shapes[index].stroke_color[2],
					shapes[index].stroke_color[3]);
			}
			
			drawLine(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			break;

		case VectorPrimitiveType::rectangle:

			ofFill();
			ofSetLineWidth(0);
			ofSetColor(shapes[index].fill_color[0], shapes[index].fill_color[1], shapes[index].fill_color[2], shapes[index].fill_color[3]);
			drawRectangle(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			ofNoFill();
			ofSetLineWidth(shapes[index].stroke_width);
			if (shapes[index].selected)
			{
				ofSetColor(255, 0, 0);
			}
			else
			{
				ofSetColor(
					shapes[index].stroke_color[0],
					shapes[index].stroke_color[1],
					shapes[index].stroke_color[2],
					shapes[index].stroke_color[3]);
			}
			drawRectangle(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			break;

		case VectorPrimitiveType::parral:

			ofFill();
			ofSetLineWidth(0);
			ofSetColor(shapes[index].fill_color[0], shapes[index].fill_color[1], shapes[index].fill_color[2], shapes[index].fill_color[3]);
			drawParral(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			ofNoFill();
			ofSetLineWidth(shapes[index].stroke_width);
			if (shapes[index].selected)
			{
				ofSetColor(255, 0, 0);
			}
			else
			{
				ofSetColor(
					shapes[index].stroke_color[0],
					shapes[index].stroke_color[1],
					shapes[index].stroke_color[2],
					shapes[index].stroke_color[3]);
			}
			drawParral(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			break;

		case VectorPrimitiveType::triangle:

			ofFill();
			ofSetLineWidth(0);
			ofSetColor(shapes[index].fill_color[0], shapes[index].fill_color[1], shapes[index].fill_color[2], shapes[index].fill_color[3]);
			draw_triangle(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			ofNoFill();
			ofSetLineWidth(shapes[index].stroke_width);
			if (shapes[index].selected)
			{
				ofSetColor(255, 0, 0);
			}
			else
			{
				ofSetColor(
					shapes[index].stroke_color[0],
					shapes[index].stroke_color[1],
					shapes[index].stroke_color[2],
					shapes[index].stroke_color[3]);
			}
			draw_triangle(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			break;

		case VectorPrimitiveType::triangler:
			ofFill();
			ofSetLineWidth(0);
			ofSetColor(shapes[index].fill_color[0], shapes[index].fill_color[1], shapes[index].fill_color[2], shapes[index].fill_color[3]);
			drawTriangler(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			ofNoFill();
			ofSetLineWidth(shapes[index].stroke_width);
			if (shapes[index].selected)
			{
				ofSetColor(255, 0, 0);
			}
			else
			{
				ofSetColor(
					shapes[index].stroke_color[0],
					shapes[index].stroke_color[1],
					shapes[index].stroke_color[2],
					shapes[index].stroke_color[3]);
			}
			drawTriangler(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			break;

		case VectorPrimitiveType::ellipse:

			ofFill();
			ofSetLineWidth(0);
			ofSetCircleResolution(1000);
			ofSetColor(
				shapes[index].fill_color[0],
				shapes[index].fill_color[1],
				shapes[index].fill_color[2]);
			drawEllipse(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			ofNoFill();
			ofSetLineWidth(shapes[index].stroke_width);
			if (shapes[index].selected)
			{
				ofSetColor(255, 0, 0);
			}
			else
			{
				ofSetColor(
					shapes[index].stroke_color[0],
					shapes[index].stroke_color[1],
					shapes[index].stroke_color[2],
					shapes[index].stroke_color[3]);
			}
			drawEllipse(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			break;
		}

	}
}

//----------- DrawShapes -------------

//Function that draws a different draw zone depending on the shape that is selected
void Renderer::draw_zone(const float x1, const float y1, const float x2, const float y2)
{

	switch (draw_mode)
	{

	case VectorPrimitiveType::line:
		ofSetLineWidth(stroke_width);
		ofSetColor(0);
		drawLine(x1, y1, x2, y2);
		break;

	case VectorPrimitiveType::rectangle:
		ofSetLineWidth(stroke_width);
		ofSetColor(0);
		ofNoFill();
		drawRectangle(x1, y1, x2, y2);
		break;

	case VectorPrimitiveType::parral:
		ofSetLineWidth(stroke_width);
		ofSetColor(0);
		ofNoFill();
		drawParral(x1, y1, x2, y2);
		break;


	case VectorPrimitiveType::triangle:
		ofSetLineWidth(stroke_width);
		ofSetColor(0);
		ofNoFill();
		draw_triangle(x1, y1, x2, y2);
		break;

	case VectorPrimitiveType::triangler:
		ofSetLineWidth(stroke_width);
		ofSetColor(0);
		ofNoFill();
		drawTriangler(x1, y1, x2, y2);
		break;

	case VectorPrimitiveType::ellipse:
		ofSetCircleResolution(1000);
		ofSetLineWidth(stroke_width);
		ofSetColor(0);
		ofNoFill();
		drawEllipse(x1, y1, x2, y2);
		break;

	default:
		break;
	}
}

//Function that addes a shape and its component to shapes* 
void Renderer::addVectorShape(const VectorPrimitiveType type)
{
	shapes[bufferHead].type = type;

	shapes[bufferHead].position1[0] = mouse_press_x;
	shapes[bufferHead].position1[1] = mouse_press_y;

	shapes[bufferHead].position2[0] = mouse_current_x;
	shapes[bufferHead].position2[1] = mouse_current_y;

	shapes[bufferHead].fill_color[0] = shape_color.r;
	shapes[bufferHead].fill_color[1] = shape_color.g;
	shapes[bufferHead].fill_color[2] = shape_color.b;
	shapes[bufferHead].fill_color[3] = shape_color.a;


	shapes[bufferHead].stroke_color[0] = stroke_color.r;
	shapes[bufferHead].stroke_color[1] = stroke_color.g;
	shapes[bufferHead].stroke_color[2] = stroke_color.b;
	shapes[bufferHead].stroke_color[3] = stroke_color.a;

	shapes[bufferHead].stroke_width = stroke_width;

	last_shape = VectorPrimitiveType::none;

	bufferHead = ++bufferHead >= bufferCount ? 0 : bufferHead;

	if (shapeCount != bufferCount)
		shapeCount++;
	resetSelection();
}

//Function to draw a line
void Renderer::drawLine(const int x1, const int y1, const int x2, const int y2)
{
	ofDrawLine(x1, y1, x2, y2);
}

//Function to draw a rectangle
void Renderer::drawRectangle(const int x1, const int y1, const int x2, const int y2)
{
	ofDrawRectangle(x1, y1, x2 - x1, y2 - y1);
}

//Function to draw a triangle
void Renderer::draw_triangle(const int x1, const int y1, const int x2, const int y2)
{
	ofDrawTriangle(x1, y2, x2, y2, x1 + (x2 - x1) / 2, y1);
}

//Function that draws that draws a parralleogram
void Renderer::drawParral(const int x1, const int y1, const int x2, const int y2)
{
	ofDrawTriangle(x1, y1, (x1 + (x2 - x1) / 2), y2, (x1 + (x2 - x1) / 2), y1);
	ofDrawTriangle((x1 + (x2 - x1) / 2), y1, x2, y2, (x1 + (x2 - x1) / 2), y2);
}

//Function that draws a right triangle
void Renderer::drawTriangler(const int x1, const int y1, const int x2, const int y2)
{
	ofDrawTriangle(x1, y2, x2, y2, x1, y1);
}

//Function that draw an ellipse
void Renderer::drawEllipse(const int x1, const int y1, const int x2, const int y2)
{
	ofDrawEllipse(x1 + (x2 - x1) / 2, y1 + (y2 - y1) / 2, x2 - x1, y2 - y1);
}

//Function that removes the last shape added
void Renderer::undo()
{
	if (bufferHead != 0)
	{
		last_shape = shapes[bufferHead - 1].type;
		shapes[bufferHead - 1].type = VectorPrimitiveType::none;
		shapeCount--;
		bufferHead--;
	}
}

//Function that replace the previously "undo" shape that was removed
void Renderer::redo()
{
	if (last_shape != VectorPrimitiveType::none)
	{
		shapes[bufferHead].type = last_shape;
		last_shape = VectorPrimitiveType::none;
		shapeCount++;
		bufferHead++;
	}
}


//------------- Shape selection detection -------------

//Function to return if the current mouse position is inside a shape
bool Renderer::isInsideShape(VectorPrimitive shape)
{
	switch (shape.type)

	{
	case VectorPrimitiveType::none:
		return false;
		break;
	case VectorPrimitiveType::line:
		return isInsideLine(mouse_current_x, mouse_current_y,
			shape.position1[0], shape.position1[1],
			shape.position2[0], shape.position2[1]);
		break;
	case VectorPrimitiveType::rectangle:
		return isInsideRectangle(mouse_current_x, mouse_current_y,
			shape.position1[0], shape.position1[1],
			shape.position2[0], shape.position2[1]);
		break;
	case VectorPrimitiveType::parral:
		return isInsideParal(mouse_current_x, mouse_current_y,
			shape.position1[0], shape.position1[1],
			shape.position2[0], shape.position2[1]);
		break;
	case VectorPrimitiveType::triangle:
		return isInsideTriangle(mouse_current_x, mouse_current_y,
			shape.position1[0], shape.position1[1],
			shape.position2[0], shape.position2[1]);
		break;
	case VectorPrimitiveType::triangler:
		return isInsideTriangler(mouse_current_x, mouse_current_y,
			shape.position1[0], shape.position1[1],
			shape.position2[0], shape.position2[1]);
		break;
	case VectorPrimitiveType::ellipse:
		return isInsideElipse(mouse_current_x, mouse_current_y,
			shape.position1[0], shape.position1[1],
			shape.position2[0], shape.position2[1]);
		break;
	default:
		return false;
		break;
	}
}


//Function to return if the cursor is inside a line
bool Renderer::isInsideLine(const int x, const int y, const int x0, const int y0, const int x1, const int y1)
{
	return (distanceBetweenTwoPoints(x, y, x0, y0) + distanceBetweenTwoPoints(x, y, x1, y1) < distanceBetweenTwoPoints(x0, y0, x1, y1) + 1 and
		distanceBetweenTwoPoints(x, y, x0, y0) + distanceBetweenTwoPoints(x, y, x1, y1) > distanceBetweenTwoPoints(x0, y0, x1, y1) - 1);
}

//Function to return if the cursor is inside a triangle
// source: https://www.geeksforgeeks.org/check-whether-a-given-point-lies-inside-a-triangle-or-not/
bool Renderer::isInsideTriangle(const int mouseX, const int mouseY, const int xs, const int ys, const int xd, const int yd)
{

	const int x = mouseX;
	const int y = mouseY;
	const int x1 = xs;
	const int y1 = yd;
	const int x2 = xd;
	const int y2 = yd;
	const int x3 = xs + (xd - xs) / 2;
	const int y3 = ys;

	/* Calculate area of triangle ABC */
	float A = triangleArea(x1, y1, x2, y2, x3, y3);

	/* Calculate area of triangle PBC */
	float A1 = triangleArea(x, y, x2, y2, x3, y3);

	/* Calculate area of triangle PAC */
	float A2 = triangleArea(x1, y1, x, y, x3, y3);

	/* Calculate area of triangle PAB */
	float A3 = triangleArea(x1, y1, x2, y2, x, y);

	/* Check if sum of A1, A2 and A3 is same as A */
	return (A == A1 + A2 + A3);
}

//Function to return if the cursor is inside a right triangle
bool Renderer::isInsideTriangler(const int mouseX, const int mouseY, const int xs, const int ys, const int xd, const int yd)
{

	const int x = mouseX;
	const int y = mouseY;
	const int x1 = xs;
	const int y1 = yd;
	const int x2 = xd;
	const int y2 = yd;
	const int x3 = xs;
	const int y3 = ys;

	/* Calculate area of triangle ABC */
	float A = triangleArea(x1, y1, x2, y2, x3, y3);

	/* Calculate area of triangle PBC */
	float A1 = triangleArea(x, y, x2, y2, x3, y3);

	/* Calculate area of triangle PAC */
	float A2 = triangleArea(x1, y1, x, y, x3, y3);

	/* Calculate area of triangle PAB */
	float A3 = triangleArea(x1, y1, x2, y2, x, y);

	/* Check if sum of A1, A2 and A3 is same as A */
	return (A == A1 + A2 + A3);
}

//Function to return if the cursor is inside a rectangle
bool Renderer::isInsideRectangle(const int x, const int y, const int x1, const int y1, const int x2, const int y2)
{
	if (x1 < x2 and y1 < y2)
	{
		return (x >= x1 and x <= x2 and y >= y1 and y <= y2);
	}
	if (x1 > x2 and y1 > y2)
	{
		return (x <= x1 and x >= x2 and y <= y1 and y >= y2);
	}
	if (x1 < x2 and y1 > y2)
	{
		return (x >= x1 and x <= x2 and y <= y1 and y >= y2);
	}
	if (x1 > x2 and y1 < y2)
	{
		return (x <= x1 and x >= x2 and y >= y1 and y <= y2);
	}
}

//Function to return if the cursor is inside an ellipse
// source: https://www.geeksforgeeks.org/check-if-a-point-is-inside-outside-or-on-the-ellipse/
bool Renderer::isInsideElipse(const int x0, const int y0, const int xS, const int yS, const int xD, const int yD)
{
	const int x = x0;
	const int y = y0;
	const int x1 = xS + (xD - xS) / 2;
	const int y1 = yS + (yD - yS) / 2;
	const int w = xD - xS;
	const int h = yD - yS;


	return ((pow((x - x1), 2) / pow(w, 2)) + (pow((y - y1), 2) / pow(h, 2)) <= 0.3);

}

//Function to return if the cursor is inside a parralelogram
bool Renderer::isInsideParal(const int mouseX, const int mouseY, const int x1, const int y1, const int x2, const int y2)
{

	return (isInsideTriangler(mouseX, mouseY, (x1 + (x2 - x1) / 2), y2, x1, y1) or
		isInsideTriangler(mouseX, mouseY, (x1 + (x2 - x1) / 2), y1, x2, y2));
}

//Function that detects the distance between two points, used to find if cursor is on a line
float Renderer::distanceBetweenTwoPoints(const int x0, const int y0, const int x1, const int y1)
{
	float w = abs(x0 - x1);
	float h = abs(y0 - y1);
	return sqrt(pow(w, 2) + pow(h, 2));
}

//Function that calculates the area of a triangle, used to find if cursor is inside a triangle
// source: https://www.geeksforgeeks.org/check-whether-a-given-point-lies-inside-a-triangle-or-not/
float Renderer::triangleArea(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3)
{
	return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

//---------------Shape selection handling---------------

//Function that returns the index of the shape the mouse is currently over
int Renderer::indexMouseOverShape()
{
	for (int index = 0; index != bufferCount; ++index)
	{
		if (isInsideShape(shapes[index]))
		{
			return index;
		}
	}
	return -1;
}

//Function that currently deletes the selected shapes
void Renderer::deleteShape()
{

	for (int i = 0; i < bufferCount; ++i)
		if (shapes[i].selected)
		{
			resetShape(i);
			shapeCount--;
		}

	do
	{
		for (int i = getIndexFirstEmptyShape(); i < bufferCount - 1; i++)
		{
			VectorPrimitive temp = shapes[i];
			shapes[i] = shapes[i + 1];
			shapes[i + 1] = temp;
		}
	} while (getIndexFirstEmptyShape() != shapeCount);

	bufferHead = shapeCount;

}

//Function that unselect all the currently selected shapes
void Renderer::resetSelection()
{
	for (int i = 0; i < bufferCount; ++i)
		shapes[i].selected = false;

}

//Function that resets (deletes) all the current shapes and sets back the variable to 0
void Renderer::resetAllShapes()
{
	for (int i = 0; i < bufferCount; ++i)
	{
		shapes[i].type = VectorPrimitiveType::none;
		shapes[i].selected = false;
	}

	shapeCount = 0;
	bufferHead = 0;
}

//Function that resets the current component of a shape in order to remove it
void Renderer::resetShape(const int i)
{
	shapes[i].type = VectorPrimitiveType::none;
	shapes[i].selected = false;
}

//Function that returns the index of the first empty shape in order to return the proper buffercount
int Renderer::getIndexFirstEmptyShape()
{
	for (int i = 0; i < bufferCount; i++)
	{
		if (shapes[i].type == VectorPrimitiveType::none)
		{
			/*cout << "getIndexFirstEmptyShape: " << i << endl;*/
			return i;
		}
	}
	/*cout << "getIndexFirstEmptyShape: " << bufferCount << endl;*/
	return bufferCount;
}


//================================================
//------------------ Histograms ------------------
//================================================

//Function that draws the histograms depending on which color is selected in the slider
void Renderer::drawHistograms(int x, int y)
{

	switch (hist_slider)
	{
	case 1:
		ofSetColor(0, 0, 0);
		fontTitle.drawString("Red dominant colors", 300, 250);
		drawColorHist(vRedColors, arrayr, x, y);
		break;

	case 2:
		ofSetColor(0, 0, 0);
		fontTitle.drawString("Green dominant colors", 300, 250);
		drawColorHist(vGreenColors, arrayg, x, y);
		break;

	case 3:
		ofSetColor(0, 0, 0);
		fontTitle.drawString("Blue dominant colors", 300, 250);
		drawColorHist(vBlueColors, arrayb, x, y);
	default:
		break;
	}

}

//Function that draws an histogram with the color and color count given in argument
void Renderer::drawColorHist(const vector<ofColor>& v, const int array[], const int StartX, const int StartY)
{
	int x = StartX;
	int y = StartY;

	if (v.size() == 0)
	{
		ofSetColor(0, 0, 0);
		fontTitle.drawString("No color for this component", 300, 320);
		return;
	}

	//find max value
	float max = 0;
	for (size_t i = 0; i < 256; i++)
	{
		if (max < array[i])
		{
			max = array[i];
		}
	}

	//Padding and dimensions
	const float rightPad = 200.0;
	const float columnPad = 1.0;
	const float maxHeight = 500.0;
	const float histWidth = ofGetWindowWidth() - x - rightPad;
	const float columnSize = histWidth - (v.size() * columnPad) < 1.0 ? 1.0 : (histWidth - (v.size() * columnPad)) / v.size(); //columnSize width based on the number of elements

	//draw background
	ofFill();
	ofSetColor(255, 255, 255);
	ofDrawRectangle(x, y - maxHeight - 10, histWidth, maxHeight + 10);
	ofSetColor(0, 0, 0);

	//draw Y values
	float textPad = 75;
	ofSetLineWidth(4);
	fontText.drawString(ofToString(max), x - textPad, y - maxHeight);
	fontText.drawString(ofToString(max / 2), x - textPad, y - maxHeight / 2);
	fontText.drawString("0", x - textPad + 50, y);

	//draw axis
	ofDrawLine(x, y + 3, x + histWidth, y + 3); // X
	ofDrawLine(x - 3, y, x - 3, y - maxHeight - 10); // Y

	//draw histogram
	ofSetLineWidth(1);
	for (size_t i = 0; i < v.size() and x < histWidth + rightPad; i++)
	{
		ofSetColor(0, 0, 0);
		ofDrawRectangle(x, y, columnSize + columnPad, array[i] / max * maxHeight * -1.0);
		ofSetColor(v[i]);
		ofDrawRectangle(x, y, columnSize, array[i] / max * maxHeight * -1.0);
		x += columnSize + columnPad;

	}
}

//Function that goes over all the pixel of an image and adds the color in their proper vector depending of their RGB values
void Renderer::getcolor()
{
	ofColor ColorAtXY;

	//Initializing the vector and array for color count to 0
	vRedColors.clear();
	vGreenColors.clear();
	vBlueColors.clear();
	fill_n(arrayr, 256, 0);
	fill_n(arrayg, 256, 0);
	fill_n(arrayb, 256, 0);

	for (size_t i = 0; i < previewCard.getWidth(); i++)
	{
		for (size_t j = 0; j < previewCard.getHeight(); j++)
		{

			ColorAtXY = previewCard.getColor(i, j);
			if (ColorAtXY.a == 255)
			{
				if (ColorAtXY.r > ColorAtXY.g and ColorAtXY.r > ColorAtXY.b)
				{
					vector<ofColor>::iterator it = find(vRedColors.begin(), vRedColors.end(), ColorAtXY);
					int indexA = distance(vRedColors.begin(), it);
					if (it == vRedColors.end())
					{
						//Pushes the color and add one count of the color being used to the array
						vRedColors.push_back(ColorAtXY);
						arrayr[vRedColors.size() - 1]++;
					}
					else
					{
						//If the color is already present in the vector we simply update the color count
						arrayr[indexA]++;
					}
				}
				if (ColorAtXY.g > ColorAtXY.r and ColorAtXY.g > ColorAtXY.b)
				{
					vector<ofColor>::iterator it = find(vGreenColors.begin(), vGreenColors.end(), ColorAtXY);
					int indexA = distance(vGreenColors.begin(), it);
					if (it == vGreenColors.end())
					{
						vGreenColors.push_back(ColorAtXY);
						arrayg[vGreenColors.size() - 1]++;
					}
					else
					{
						arrayg[indexA]++;
					}
				}
				if (ColorAtXY.b > ColorAtXY.g and ColorAtXY.b > ColorAtXY.r)
				{
					vector<ofColor>::iterator it = find(vBlueColors.begin(), vBlueColors.end(), ColorAtXY);
					int indexA = distance(vBlueColors.begin(), it);
					if (it == vBlueColors.end())
					{
						vBlueColors.push_back(ColorAtXY);
						arrayb[vBlueColors.size() - 1]++;
					}
					else
					{
						arrayb[indexA]++;
					}
				}
			}
		}
	}
}



//================================================
//------------------- Utils ----------------------
//================================================

//------------------ Export ------------------
ofImage Renderer::customCardtoIMG()
{
	int midscreenX = ofGetWindowWidth() / 2;
	int midscreenY = (ofGetWindowHeight() / 2) - 25;

	int canvasW = 650;
	int canvasH = 755;
	int offsetX2 = canvasW / 2;
	int offsetY2 = canvasH / 2;

	ofImage img, emptyCard;
	ofFbo fbo;
	ofPixels pixels;

	fbo.allocate(1440, 900, GL_RGBA8);
	pixels.allocate(1440, 900, OF_IMAGE_COLOR_ALPHA);

	emptyCard.load("..\\data\\Assets\\emptyCard2.png");
	emptyCard.setCompression(OF_COMPRESS_SRGB);

	//draw canvas into FBO
	ofSetColor(255, 255, 255, 255);
	fbo.begin();
	drawCanvas();
	fbo.end();

	//Transfer to image, crop and resize
	fbo.readToPixels(pixels);
	img = ofImage(pixels);
	img.crop(midscreenX - offsetX2, midscreenY - offsetY2, canvasW, canvasH);
	img.resize(197, 229);

	//clear fbo and pixels
	fbo.clear();
	pixels.clear();
	fbo.allocate(375, 518, GL_RGBA8);
	pixels.allocate(375, 518, OF_IMAGE_COLOR_ALPHA);

	//Assemble canvas and empty card
	ofSetColor(255, 255, 255, 255);
	fbo.begin();
	img.draw(80, 40);
	emptyCard.draw(0, 0);
	fbo.end();
	fbo.readToPixels(pixels);
	img = ofImage(pixels);

	//clear fbo and pixels
	fbo.clear();
	pixels.clear();
	fbo.allocate(375, 518, GL_RGBA8);
	pixels.allocate(375, 518, OF_IMAGE_COLOR_ALPHA);

	//Randomize the card
	ofSetColor(255, 255, 255, 255);
	updateRandomNumbers();
	fbo.begin();
	drawRandomCard(randomNumbers, img);
	fbo.end();

	fbo.readToPixels(pixels);
	img = ofImage(pixels);

	return img;
}

//Draw the current random card to an FBO and returns it as an OFimage
ofImage Renderer::randomCardToImg()
{
	int midscreenX = ofGetWindowWidth() / 2;
	int midscreenY = (ofGetWindowHeight() / 2) - 25;
	int offsetX = ofGetWindowWidth() / 8;
	int offsetY = ofGetWindowHeight() / 3;

	ofFbo fbo;
	ofPixels pixels;
	ofImage img;

	fbo.allocate(1440, 900, GL_RGBA8);
	pixels.allocate(1440, 900, OF_IMAGE_COLOR_ALPHA);

	ofSetColor(255, 255, 255, 255);
	fbo.begin();
	drawRandomCard(randomNumbers);
	fbo.end();

	fbo.readToPixels(pixels);
	img = ofImage(pixels);
	img.crop(midscreenX - offsetX, midscreenY - offsetY, 375, 518);
	
	return img;
}

//Draw the collection to an FBO and returns it as an OFimage
ofImage Renderer::collectionToImg()
{

	ofFbo fbo;
	ofPixels pixels;
	ofImage img;

	fbo.allocate(1440, 900, GL_RGBA8);
	pixels.allocate(1440, 900, OF_IMAGE_COLOR_ALPHA);

	ofSetColor(255, 255, 255, 255);
	fbo.begin();
	drawCollection(0, 0, 1440, 900, 375 * 0.5, 518 * 0.5, vCardCollection);
	fbo.end();

	fbo.readToPixels(pixels);
	img = ofImage(pixels);

	return img;
}

//Depending on the renderer state, save the collection or the card to a folder
void Renderer::saveToPath(const string name, const string extension)
{

	// extraire des données temporelles formatées
	string time_stamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");

	// générer un nom de fichier unique et ordonné
	string file_name = name + time_stamp + "." + extension;

	switch (state)
	{
	case idle:
		break;
	case cardPreview:
		break;
	case collection:
		try
		{
			collectionToImg().save(file_name);
		}
		catch (const std::exception&)
		{
			cout << "Unable to save " << file_name << endl;
		}
		break;
	case createcard:
		try
		{
			customCardtoIMG().save(file_name);
		}
		catch (const std::exception&)
		{
			cout << "Unable to save " << file_name << endl;
		}
		break;
	case randomcard:
		try
		{
			randomCardToImg().save(file_name);
		}
		catch (const std::exception&)
		{
			cout << "Unable to save " << file_name << endl;
		}
		break;
	case hist:
		break;
	case debug:
		break;
	default:
		break;
	}
}

//Save the image to the collection acording to the renderer state
void Renderer::saveIMGtoCollection()
{
	switch (state)
	{
	case idle:
		break;
	case cardPreview:
		break;
	case collection:
		break;
	case createcard:
		vCardCollection.push_back(customCardtoIMG());
		state = collection;
		break;
	case randomcard:
		vCardCollection.push_back(randomCardToImg());
		state = collection;
		break;
	case hist:
		break;
	case debug:
		break;
	default:
		break;
	}
}