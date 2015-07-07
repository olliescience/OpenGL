namespace InputManager{
	extern int mouseX, mouseY; // holds the coordinates of the mouse
	extern bool inDrag; // holds whether or not the mouse is being dragged
	extern unsigned char keyState[255]; // array holding the states of all keys
	extern unsigned char mouseState[5]; // holds the state of the mouse

	extern void keyboard(unsigned char key, int x, int y);
	extern void keyboardUp(unsigned char key, int x, int y);
	extern void mouse(int button, int state, int x, int y);
	extern void mousemove(int x, int y);
	extern void mousepassive(int x, int y);
}