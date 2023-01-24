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
	ofImage canvasback;
	ofImage canvasbackground;
	ofImage selectBorder;
	ofImage previewCard;
	ofImage cardSelected;

	//State handling
	rendererStates state = idle;
	bool updateOnce = false;

	//Not working because of OpenGL version
	//ofShader shaderBlurX;
	//ofShader shaderBlurY;
	//ofFbo fboBlurOnePass;
	//ofFbo fboBlurTwoPass;
	//void updateBackground();

	//---------------- Core functions ----------------
	void setup();
	void update();
	void updateRandomNumbers();
	void draw();

	~Renderer();

	//---------------- collection and draw single card ----------------
	vector<ofImage> vCardCollection;

	void drawCardPreview();
	void LoadCardsFromFolder();
	void drawCollection(const int x0, const int y0, const int w, const int h, const int row, const int col, const vector<ofImage> &vCardCollection, const int padding = 0);
	void drawCard(const ofImage &image, const int x, const int y, const int w, const int h);
	bool mouseOnCard(const int x0, const int y0, const int x1, const int y1, const int h, const int w);

	//---------------- random card ----------------
	static const int PARAM_NUM = 7;
	int randomNumbers[7];
	int randomCardNumber;

	void drawRandomCard(const int randomNumbers[]);
	void drawRandomCard(const int randomNumbers[], const ofImage img);


	//---------------- Create a custom card ----------------
	VectorPrimitiveType draw_mode;
	VectorPrimitiveType last_shape;
	VectorPrimitive* shapes;

	int shapeCount;
	int bufferCount;
	int bufferStride;
	int bufferSize;
	int bufferHead;

	float stroke_width;

	void createCardSetup();
	void drawShapes();
	void drawCanvas();

	//DrawShapes
	void draw_zone(const float x1, const float y1, const float x2, const float y2);
	void addVectorShape(const VectorPrimitiveType type);
	void drawLine(const int x1, const int y1, const int x2, const int y2);
	void drawRectangle(const int x1, const int y1, const int x2, const int y2);
	void drawParral(const int x1, const int y1, const int x2, const int y2);
	void draw_triangle(const int x1, const int y1, const int x2, const int y2);
	void drawEllipse(const int x1, const int y1, const int x2, const int y2);
	void drawTriangler(const int x1, const int y1, const int x2, const int y2);

	//Shape selection detection 
	bool isInsideTriangle(const int mouseX, const int mouseY, const int x, const int y, const int x0, const int y0);
	bool isInsideTriangler(const int mouseX, const int mouseY, const int xs, const int ys, const int xd, const int yd);
	bool isInsideRectangle(const int x, const int y, const int x1, const int y1, const int x2, const int y2);
	bool isInsideElipse(const int x, const int y, const int x1, const int y1, const int w, const int h);
	bool isInsideParal(const int x, const int y, const int x1, const int y1, const int x2, const int y2);
	bool isInsideLine(const int x, const int y, const int x0, const int y0, const int x1, const int y1);
	float distanceBetweenTwoPoints(const int x0, const int y0, const int x1, const int y1);
	float triangleArea(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3);

	//Shape selection handling
	bool isInsideShape(VectorPrimitive shape);
	int indexMouseOverShape();
	void deleteShape();
	int getIndexFirstEmptyShape();
	void resetAllShapes();
	void resetShape(const int index);
	void resetSelection();

	//utils
	void undo();
	void redo();



	//---------------- Histograms ----------------
	int hist_slider;
	int arrayr[256]{ 0 };
	int arrayg[256]{ 0 };
	int arrayb[256]{ 0 };

	vector<ofColor> vRedColors;
	vector<ofColor> vGreenColors;
	vector<ofColor> vBlueColors;

	ofTrueTypeFont fontTitle;
	ofTrueTypeFont fontText;


	void drawHistograms(const int x, const int y);
	void drawColorHist(const vector<ofColor> &v, const int array[], const int x, const int y);
	void getcolor();


	//---------------- Utils ----------------

	// Mouse handling
	bool is_mouse_button_pressed = false;
	int mouse_press_x;
	int mouse_press_y;
	int mouse_current_x;
	int mouse_current_y;

	//Exporting to folder
	void saveIMGtoCollection();
	ofImage customCardtoIMG();
	ofImage randomCardToImg();
	ofImage collectionToImg();
	void saveToPath(const string name, const string extension);

};

