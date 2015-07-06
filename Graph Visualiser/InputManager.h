class InputManager{
public:
	int mouseX, mouseY; // holds the coordinates of the mouse
	bool inDrag = false; // holds whether or not the mouse is being dragged
	unsigned char keyState[255]; // array holding the states of all keys
	unsigned char mouseState[5]; // holds the state of the mouse

	void keyboard(unsigned char key, int x, int y);
	void keyboardUp(unsigned char key, int x, int y);
	void mouse(int button, int state, int x, int y);
	void mousemove(int x, int y);
	void mousepassive(int x, int y);
};