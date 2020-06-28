A C++ project where I used my previous functions (maths and OpenGL related) to create a simple graphing application. The controls for this project are listed within it (when it's executed and the user holds down the CTRL key).  
  
All graphs must be written in Reverse Polish Notation (e.g sin(3x) + 5 becomes X 3 MULT SIN 5 ADD (as we take X and 3, multiply them together, sin the answer, and then add 5) and of the form f(x). The value of x can either represent an x coordinate or an angle from the origin, depending on whether the graph is cartesian or polar (can be changed with the 1 and 2 keys respectively).  
  
Once the graph has been created the user can enlarge, stretch, translate (cartesian only), or rotate (polar only) the graph.  
  
The syntax accepted by the program is as follows:  
Numerical values:  
-Any positive integer (negative integers can be achieved via SUB and decimal values can be achieved via DIV)  
-A (a constant which can have it's value changed using the numkeys)  
-E  
-PI  
-X  
  
Requires a single parameter:  
-SIN  
-SINH  
-COS  
-COSH  
-TAN  
-TANH  
-EXP  
-LN  
-MOD  
  
Requires two parameters:  
-MULT  
-DIV  
-ADD  
-SUB  
-POW (exponent must be a positive integer)  
-ROOT  
-LOG  
