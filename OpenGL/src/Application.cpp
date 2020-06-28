#include "Graphics2D.h"
#include "maths.h"

#include <vector>
#include <iostream>
#include <string>

using namespace std;
//TODO - MAYBE ADD INVERSE HYPERBOLIC

const int xRes = 1920;
const int yRes = 1080;
const double polarIncrement = (8 * PI) / xRes; //0 to 8PI

string Atext = (string)"A IS ";

float xCoords[xRes];
float yCoords[xRes];

float polarXCoords[xRes];
float polarYCoords[xRes];

//int currentMode = 0;
bool isEditing = false; //Whether the user is inputting a new graph
bool isPolar = false;

int A = 1; //A constant which can be changed by the user
float xStretch = 10;
float yStretch = 0.1;
float xMove = 0;
float yMove = 0;

void reset() {
	xStretch = 10;
	yStretch = 0.1;
	xMove = 0;
	yMove = 0;
}




//vector<float> instructions = {-1, -2, 2, -10}; //currently sin^2x
//vector<float> instructions = {-1, -9}; //currently ln(x)
//vector<float> instructions = { -1, -8, -2 }; //sin(e^x)
vector<float> instructions = { 0, -1, -14, 0.1, -15, -8, -1, -2, -15 }; //e^-0.1x * sin(x)


/*		sin = 234 //COLLISION 2
		sinh = 306
		cos = 229
		cosh = 301
		tan = 227 //COLLISION 1
		tanh = 299
		exp = 237
		ln = 154
		pi = 153
		e = 69
		x = 88
		mult = 322
		div = 227 //COLLISION 1
		add = 201
		sub = 234 //COLLISION 2
		pow = 246
		root = 324
		log = 226
		mod = 224					*/

void convertToInstructions(string input, int length) { //takes an input string from the user and converts it into instructions
	input.append(" ");
	length += 1;

	instructions = {};
	//int beginning = 0; //index of beginning of word
	int currentVal = 0; //the idea behind this is that for each word the sum of all characters is obtained and used to determine the method to call
	vector<int> numsList = {};
	int numsListLength = 0;
	int multiplier = 1;
	for (int i = 0; i < length; i++) {
		if (input[i] != ' ' && input[i] != '\n') {
			if (input[i] < 'A') { //THIS MAY NEED TO BE CHANGED IN THE FUTURE IF MORE CHARACTERS GET ADDED
				numsListLength += 1;
				numsList.push_back(input[i] - '0');
			}
			else {
				currentVal += input[i];
			}
		}
		else {
			if (numsListLength == 0) {
				switch (currentVal) { //is always an integer in this case
				case 'A':
					instructions.push_back(-1);
					break;

				case 'E': //e
					instructions.push_back(e);
					break;

				case 'X': //x
					instructions.push_back(-2);
					break;

				case 153: //PI
					instructions.push_back(PI);
					break;

				case 154: //ln
					instructions.push_back(-10);
					break;

				case 201: //add
					instructions.push_back(-15);
					break;

				case 224: //mod
					instructions.push_back(-11);
					break;

				case 226: //log
					instructions.push_back(-14);
					break;

				case 227: //div or tan
					if (input[i - 1] == 'V') { //div
						instructions.push_back(-18);
					}
					else { //tan
						instructions.push_back(-5);
					}
					break;

				case 229: //cos
					instructions.push_back(-4);
					break;

				case 234: //sin or sub
					if (input[i - 1] == 'N') { //sin
						instructions.push_back(-3);
					}
					else { //sub
						instructions.push_back(-16);
					}
					break;

				case 237: //exp
					instructions.push_back(-9);
					break;

				case 246: //pow
					instructions.push_back(-12);
					break;

				case 299: //tanh
					instructions.push_back(-8);
					break;

				case 301: //cosh
					instructions.push_back(-7);
					break;

				case 306: //sinh
					instructions.push_back(-6);
					break;

				case 322: //mult
					instructions.push_back(-17);
					break;

				case 324: //root
					instructions.push_back(-13);
					break;
				}
			}
			else { //converts a string of integers into a single integer
				for (int i = numsListLength - 1; i >= 0; i--) {
					currentVal += numsList[i] * multiplier;
					multiplier *= 10;
				}
				//cout << currentVal << endl;
				instructions.push_back(currentVal);

				numsList = {};
				numsListLength = 0;
				multiplier = 1;
			}
			currentVal = 0;
			//isNum = false;
			//beginning = i + 1;
		}
	}
}



float reversePolishCalc(float x) { //reverse polish notation
	vector<float> stack;
	int stackSize = 0;
	float finalVal, beforeFinalVal;

	for (int i = 0; i < instructions.size(); i++) {

		if (instructions[i] < -2) { //must be a function if negative as the user can only input positive numbers
			if (instructions[i] > -12) { //determines number of function parameters
				if (stackSize > 0) {
					finalVal = stack.back();
					stack.pop_back();

					switch ((int)instructions.at(i)) { //all these functions take a single parameter

					case -3: //sin
						stack.push_back(sin(finalVal, 50));
						break;

					case -4: //cos
						stack.push_back(cos(finalVal, 50));
						break;

					case -5: //tan
						stack.push_back(tan(finalVal, 50));
						break;

					case -6: //sinh
						stack.push_back(sinh(finalVal, 1000));
						break;

					case -7: //cosh
						stack.push_back(cosh(finalVal, 1000));
						break;

					case -8: //tanh
						stack.push_back(tanh(finalVal, 1000));
						break;

					case -9: //exp
						stack.push_back(calcExp(finalVal, 1000));
						break;

					case -10: //ln
						stack.push_back(ln(finalVal, 1000));
						break;

					case -11: //mod
						if (finalVal >= 0) {
							stack.push_back(finalVal);
						}
						else {
							stack.push_back(-finalVal);
						}
					}
				}
			}

			else if (stackSize > 1) {
				finalVal = stack.back();
				stack.pop_back();
				beforeFinalVal = stack.back();
				stack.pop_back();
				stackSize -= 1;

				switch ((int)instructions[i]) { //all these functions take two parameters

				case -12: //pow
					//cout << "test " << endl;
					stack.push_back(intPow(beforeFinalVal, finalVal)); //double pow doesn't work for negative bases
					break;

				case -13: //root
					stack.push_back(root(beforeFinalVal, finalVal, 1000));
					break;

				case -14: //log
					stack.push_back(log(beforeFinalVal, finalVal, 1000));
					break;

				case -15: //add
					stack.push_back(beforeFinalVal + finalVal);
					break;

				case -16: //subtract
					stack.push_back(beforeFinalVal - finalVal);
					break;

				case -17: //multiply
					stack.push_back(beforeFinalVal * finalVal);
					break;

				case -18: //divide
					stack.push_back(beforeFinalVal / finalVal);
					break;
				}
			}
		}
		else {
			switch ((int)instructions[i]) { //must be an actual value
			case -1:
				stack.push_back(A);
				break;

			case -2:
				stack.push_back(x);
				break;

			default:
				stack.push_back(instructions[i]);
				break;
			}
			stackSize += 1;
		}
	}

	if (stackSize == 1) {
		return stack.back();
	}
	return 0; //invalid
}


void calcGraph(float xCoords[], float yCoords[]) {
	if (isPolar == false) {
		for (int i = 0; i < xRes; i++) {
			yCoords[i] = yStretch * reversePolishCalc(xStretch * ((double)xCoords[i] - xMove)) + yMove;
		}
	}
	else {
		float currentVal;
		float currentAngle;
		for (int i = 0; i < xRes; i++) {
			currentAngle = polarIncrement * i;
			currentVal = reversePolishCalc(currentAngle + xMove);
			xCoords[i] = ((float)(currentVal * cos(currentAngle)) / xStretch);
			yCoords[i] = (currentVal * sin(currentAngle) * yStretch);
		}
	}
}

bool input(Graphics2D& engine) {
	int key = engine.getCurrentKey();
	if (key != -1) {
		if (key >= '0' && key <= '9') {
			A = key - '0';
			Atext = (string)"A IS ";
			Atext.push_back(key);
			return true;
		}
		else {
			switch (key) {
			case 'C':
				isPolar = false;
				reset();
				calcGraph(xCoords, yCoords);
				return false;

			case 'P':
				isPolar = true;
				reset();
				calcGraph(polarXCoords, polarYCoords);
				return false;

			case 'W':
				yStretch += 0.001;
				return true;

			case 'S':
				yStretch -= 0.001;
				return true;

			case 'D':
				xStretch -= 0.1;
				return true;

			case 'A':
				xStretch += 0.1;
				return true;

			case 'Z':
				xStretch *= 0.99;
				yStretch *= 1.01;
				return true;

			case 'X':
				xStretch *= 1.01;
				yStretch *= 0.99;
				return true;

			case GLFW_KEY_UP:
				yMove += 0.01;
				return true;

			case GLFW_KEY_DOWN:
				yMove -= 0.01;
				return true;

			case GLFW_KEY_LEFT:
				xMove -= 0.01;
				return true;

			case GLFW_KEY_RIGHT:
				xMove += 0.01;
				return true;

			case 'R':
				reset();
				return true;

			case GLFW_KEY_ENTER:
				isEditing = true;
				return false;

			case GLFW_KEY_ESCAPE:
				engine.closeWindow();
				return false;
			}
		}
	}
	return false;
}

bool textInput(string& contents, int& stringLength, Graphics2D& engine) { //from my text input program
	int currentKey = engine.getCurrentKey();
	switch (currentKey) {
	case -1:
		break;

	case GLFW_KEY_BACKSPACE:
		if (stringLength > 0) {
			contents.pop_back();
			stringLength -= 1;
		}
		break;

	case GLFW_KEY_ENTER:
		contents.push_back('\n');
		stringLength += 1;
		break;

	case GLFW_KEY_ESCAPE:
		engine.closeWindow();
		break;

	case GLFW_KEY_LEFT_SHIFT:
		return true;

	default:
		contents.push_back(currentKey);
		stringLength += 1;
		break;
	}
	return false;
}



//for now the coordinates system goes from (-aspectRatio, -1) to (aspectRatio, 1)
//const string labels[10] = { "SIN X", "COS X", "TAN X", "SINH X", "COSH X", "TANH X", "EXP X", "LN X", "ARCSINH X", "ROOT X" };
char* controlText = (char*)"NUMKEYS TO CHANGE THE VALUE OF THE CONSTANT A \n\nW AND S STRETCH IN Y AXIS \nA AND D STRETCH IN X AXIS \n\nARROW KEYS TRANSLATE CARTESIAN OR ROTATE POLAR\n\nZ AND X TO ZOOM IN AND OUT RESPECTIVELY \n\nR TO RESET TRANSFORMATIONS \n\nENTER TO TYPE NEW GRAPH IN REVERSE POLISH NOTATION \nSHIFT TO CONFIRM NEW GRAPH \n\nC TO SELECT CARTESIAN GRAPH \nP TO SELECT POLAR GRAPH";
int controlTextLength = strlen(controlText);

int main() {
	//string function = "0 X SUB 10 DIV EXP X SIN MULT ";
	string function = "X SIN 3 POW";
	Atext.push_back('1');
	int functionLength = 11;
	convertToInstructions(function, function.length()); //TODO - MOVE THIS ELSEWHERE AND ALLOW FOR THE USER TO INPUT THIS STRING
	//THE SPACE ON THE END IS CURRENTLY REQUIRED


	Graphics2D engine = Graphics2D(xRes, yRes, "Graph", true);
	engine.setLineWidth(3);
	engine.resizeText(0.05);
	float aspectRatio = engine.getAspectRatio();
	double increment = (double)(2 * (double)aspectRatio) / xRes;

	for (int i = 0; i < xRes; i++) {
		xCoords[i] = (i * increment) - aspectRatio;
	}
	calcGraph(xCoords, yCoords);

	while (engine.shouldClose() == false) {
		if (engine.keyPress(GLFW_KEY_LEFT_CONTROL)) { //display controls
			engine.renderString(-aspectRatio + 0.01, 0.89, controlText, controlTextLength);
		}

		else if (isEditing == false) { //render graph
			engine.renderString(-aspectRatio + 0.01, -0.99, (char*)function.c_str(), functionLength);
			engine.renderString(-aspectRatio + 0.01, 0.85, (char*)"CTRL FOR CONTROLS", 17);
			engine.renderString(aspectRatio - 0.4, 0.85, (char*)Atext.c_str(), 6);
			engine.setLineWidth(1);
			engine.setLineColour(0.2, 0.2, 0.2, 1);
			engine.line(0, -1, 0, 1);
			engine.line(-aspectRatio, 0, aspectRatio, 0);
			engine.setLineColour(1, 1, 1, 1);
			if (isPolar == false) {
				for (int i = 0; i < xRes - 1; i++) {
					engine.line(xCoords[i], yCoords[i], xCoords[i + 1], yCoords[i + 1]);
				}
				if (input(engine) == true) {
					calcGraph(xCoords, yCoords);
				}
			}
			else {
				for (int i = 0; i < xRes - 1; i++) {
					engine.line(polarXCoords[i], polarYCoords[i], polarXCoords[i + 1], polarYCoords[i + 1]);
				}
				if (input(engine) == true) {
					calcGraph(polarXCoords, polarYCoords);
				}
			}


			engine.setLineWidth(4);
		}
		else { //allow for the user to input a new function
			if (textInput(function, functionLength, engine) == true) {
				isEditing = false;
				convertToInstructions(function, functionLength);
				reset();
				if (isPolar == false) {
					calcGraph(xCoords, yCoords);
				}
				else {
					calcGraph(polarXCoords, polarYCoords);
				}
			}
			engine.renderString(-aspectRatio + 0.01, 0.8, (char*)function.c_str(), functionLength);
		}

		engine.clear();
	}
}