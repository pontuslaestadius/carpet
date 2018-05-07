/*
Copyright 2018 @ Pontus Laestadius

Handles HTML elements.

*/

/**

Creates a range slider and returns it.

**/
function newRangeSlider() {
  var range = document.createElement("input");
    range.type = "range";
    range.min = "0.0";
    range.max = "0.16";
    range.value = "0.1";
    range.step = "0.01";
    return range;
}

function setBackground(key, color) {
	var element = delm(key);
	if (element.style.backgroundColor != color) {
		element.style.backgroundColor = color;
	}
}

/**

Increment the inner value of an element.

**/
function increment(id) {
  let tmp = document.getElementById(id).innerText;
  document.getElementById(id).innerText = parseInt(tmp) +1;
}

/**

Generates HTML code for the indicator.

**/
function addIndicator(code, label) {
  var node = document.createElement("LI");

  if (code == null) {
    node.style.opacity = "0.0";
  } else {
    if (label == null || label == "") {
      label = table[code][0];
    }
    node.id = code;

    var text_node = document.createElement("b");
    text_node.innerText = label;


    node.appendChild(text_node);
  }
  document.getElementById("box").appendChild(node);

  if (isRangeSlider(code)) {
    node.appendChild(newRangeSlider());
  }
}