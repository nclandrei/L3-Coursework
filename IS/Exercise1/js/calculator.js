// defining variables that will be used throughout the file

var operators = ["*", "/", "+", "-"];
var specialFunctions = ["log2", "sqrt", "cos", "sin", "pow", "tan", "exp"];
var presses = document.querySelectorAll("#calculator span");
var decimal = true;
var pressVal = document.querySelector("#pressVal");

/*  This section is for testing the user performance	
 *  - it automatically randomizes the number of computations the
 *  user has to undergo (between 1 and 5) and the number of numbers
 *  involved in a computation. 
*/
   
var numberOfNumbers = Math.floor(Math.random() * 6 + 2);
var numberOfComputations = Math.floor(Math.random() * 5 + 1);
var randomNumbers = [];
var computationStrings = [];
var currentOperator = "";
var correctPresses = 0;
var wrongPresses = 0;
var index = 0;
var stringIndex = 0;
var plotResultIndex = 0;
var currentPress = 0;
var prevPress = 0;
var buttonWidth = document.querySelectorAll("span")[0].offsetWidth;

// variables for calculating time between 2 clicks
var startClick = 0;
var endClick = 0;

// we are computing the random computations that the user has to undertake;
// they can be modified such that we can have special functions as well etc.
for (var j = 0; j < numberOfComputations; ++j) {
	computationStrings[j] = "";
	for (var z = 0; z < numberOfNumbers - 1; ++z) {
		randomNumbers[z] = Math.floor(Math.random() * 1000 + 1);
		pressVal.innerHTML += randomNumbers[z];
		computationStrings[j] += randomNumbers[z];
		pressVal.innerHTML += " ";
		currentOperator = operators[Math.floor(Math.random() * 3)];
		pressVal.innerHTML += currentOperator;
		computationStrings[j] += currentOperator;
		pressVal.innerHTML += " ";
	}
	randomNumbers[numberOfNumbers - 1] = Math.floor(Math.random() * 1000 + 1);
	pressVal.innerHTML += randomNumbers[numberOfNumbers - 1];
	computationStrings[j] += randomNumbers[numberOfNumbers - 1];
	pressVal.innerHTML += "<br> <br>";
}

// creating the array that will store all presses with the following properties: time taken until last click, 
// index of difficulty and if it was a correct press or a wrong one
var plotResults = Create2DArray(1000);

// going through keys pressed and doing computations
for(var i = 0; i < presses.length; ++i) {
	startClick = new Date().getTime();
	presses[i].onclick = function(x) {
		endClick = new Date().getTime();
		currentPress  = this;
		var input = document.querySelector(".resultBox");
		var inputValue = input.innerHTML;
		var buttonValue = this.innerHTML;
		
		// if start testing button has been pressed, testing starts and a left panel appears with tasks for the user to do
		if (document.getElementById("testPanel").style.display == "block") {
			if (stringIndex < computationStrings.length &&
				index < computationStrings[stringIndex].length) {
				if (buttonValue != computationStrings[stringIndex].charAt(index)) {
					wrongPresses++;
					plotResults[plotResultIndex][0] = 0;
				}
				else {
					correctPresses++;
					plotResults[plotResultIndex][0] = 1;
					index++;
				}
			}
			else {
				index = 0;
				++stringIndex;
			}
		}

		// clear the result box
		if (buttonValue == 'C') {
			input.innerHTML = "";
			decimal = false;
		}
		
		// getting the value of the equation and printing it out
		else if (buttonValue == '=') {
			var result = inputValue;
			var lastCh = result[result.length - 1];
			
			// checking now for last char, if it is a decimal or operator: if so, remove it
			if (lastCh == '.' || operators.indexOf(lastCh) > -1) {
				result = result.replace(/.$/, '');
			}

			// replacing the special functions with the corresponding javascript function
			result = result.replace(/log2/, function mathFun(x){return "Math." + x;});
			result = result.replace(/sin/, function mathFun(x){return "Math." + x;});
			result = result.replace(/cos/, function mathFun(x){return "Math." + x;});
			result = result.replace(/sqrt/, function mathFun(x){return "Math." + x;});
			result = result.replace(/tan/, function mathFun(x){return "Math." + x;});
			result = result.replace(/pow/, function mathFun(x){return "Math." + x;});
			result = result.replace(/exp/, function mathFun(x){return "Math." + x;});

			result = result.replace("PI", "Math.PI");

			if (result) {
				input.innerHTML = eval(result);
			}
			
			decimal = false;
		}
		
		// in this case, we press one of the operators
		else if(operators.indexOf(buttonValue) > -1) {
			var lastChar = inputValue[inputValue.length - 1];

			if(inputValue != '' && operators.indexOf(lastChar) == -1) 
				input.innerHTML += buttonValue;
			
			// we are allowing here to add - in front of every number
			else if(inputValue == '' && buttonValue == '-') 
				input.innerHTML += buttonValue;
			
			// if we decide to switch the operator currently pressed, we can do that
			// by replacing the current one
			if(operators.indexOf(lastChar) > -1 && inputValue.length > 1) {
				input.innerHTML = inputValue.replace(/.$/, buttonValue);
			}
			
			decimal = false;
		}

		// check if it is a special function (cos/sin/sqrt/log)

		else if(specialFunctions.indexOf(buttonValue) > -1) {
			input.innerHTML += buttonValue + "(";
		}
		
		// adding a decimal only if there isn't already one inserted
		else if(buttonValue == '.') {
			if(!decimal) {
				input.innerHTML += buttonValue;
				decimal = true;
			}
		}

		// analyzing when to add parantheses and when not
		else if(buttonValue == "(" || buttonValue == ")") {
			var lastChar = inputValue[inputValue.length - 1];
			if (buttonValue == "(" && (operators.indexOf(lastChar) > -1 || lastChar == "(" || inputValue.length == 0)) {
				input.innerHTML += buttonValue;
			}
			else if (buttonValue == ")" && operators.indexOf(lastChar) == -1) {
				input.innerHTML += buttonValue;
			}
		}
		
		// last case: we add the digits we are pressing
		else {
			input.innerHTML += buttonValue;
		}

		// monitoring the time between clicks
		plotResults[plotResultIndex][1] = (endClick - startClick) / 1000;
		startClick = endClick;
		console.log(computeID(prevPress, currentPress) + "    " + plotResults[plotResultIndex][1]);
		plotResults[plotResultIndex][2] = computeID(prevPress, currentPress);
		prevPress = currentPress;
		plotResultIndex ++;
		
	}
}

//creating a 2d array for storing the test results
function Create2DArray(rows) {
  var arr = [];

  for (var i=0;i<rows;i++) {
     arr[i] = [];
  }

  return arr;
}

// this function records and logs all mouse movement on the calculator
document.getElementById("calculator").onmousemove = function(e) {
	var positions = "(" + e.pageX + ", " + e.pageY + ")";
}

// compute distance between 2 spans

function getPosition(element) {
	var xPosition = 0;
	var yPosition = 0;

	while(element) {
		xPosition += (element.offsetLeft - element.scrollLeft + element.clientLeft);
		yPosition += (element.offsetTop - element.scrollTop + element.clientTop);
		element = element.offsetParent;
	}
	return { x: xPosition, y: yPosition };
}

function computeDistance(element1, element2) {
	var xs = 0;
	var ys = 0;

	xs = getPosition(element2).x - getPosition(element1).x;
	ys = getPosition(element2).y - getPosition(element1).y;

	xs = xs * xs;
	ys = ys * ys;

	return Math.sqrt(xs + ys);
}

// computes the index of difficulty
function computeID (element1, element2) {
	return (Math.log(2 * computeDistance(element1, element2) / buttonWidth));
}