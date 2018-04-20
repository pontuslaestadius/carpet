/*
Copyright 2018 @ Pontus Laestadius
*/
var mid = 9999;
var size = 128;

// Create a bool table with keys.
var table = new Array(size);

// table[code][field, messageid, down value, up value]
for (var i = 0; i < size; i++) {
  table[i] = new Array(4);
}

var mock_random = new Array(3);
mock_randomVals();

/**

Register a key, these are listed in layout.js to handle user input.

**/
function registerkey(character, field, messageid, values, label) {
  if (character == null) {
    addIndicator();
    return;
  }
  if (isLetter(character)) {
    character = character.charCodeAt(0) - 32;
  }
  table[character] = [field, messageid, values[0], values[1]];
  addIndicator(character, label);
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
    node.innerText = label;
  }
  document.getElementById("box").appendChild(node);

  if (isRangeSlider(code)) {
    node.appendChild(newRangeSlider());
  }
}

/**

Creates a range slider and returns it.

**/
function newRangeSlider() {
  var range = document.createElement("input");
    range.type = "range";
    range.min = "0";
    range.max = "0.5";
    range.value = "0.2";
    range.step = "0.01";
    return range;
}

/**

Validates that the provided key is registered and sends it forward.

**/
function validateKey(k, s) {
  if (table[Math.abs(k)][0] == null || 
    document.getElementById(Math.abs(k)).style.backgroundColor == s) {
    return;
  }
  document.getElementById(Math.abs(k)).style.backgroundColor = s;
  send(k);
}

/**

Converts a data to that of a simulated message on the canvas.

**/
function convertMessageReadingsToSimulation(val, type) {
  type = parseInt(type);
  switch(type) {
    case 1039:
    mock_simulate(val, front);
        break;
    case 1041:
    mock_simulate(val, accel);
        break;
    case 1045:
    mock_simulate(val, angle);
        break;
  } 
}


/**

Processes the given value and converts it to a JSON and sends it forward.

**/
function send(k) {
  if (k == null) {
    console.log("Can't process null.");
    return;
  }
  var add = 0;
  if (k < 0) {
    k = -k;
    add = 1;
  }

  var val = table[k][2+add];
  if (val == "range") {
    var qs = document.querySelector("#\\3" + parseInt(k/10) + " " + k%10 + " input");
    val = qs.value;
  }

  var jsonMessageToBeSent = "{\"" + table[k][0] + "\":" + val + "}";
  if (table[k][0] == null || table[k][2+add] == null) {
    jsonMessageToBeSent = "{}";
  } 

  __send(ws, jsonMessageToBeSent, table[k][1]);
}

/**

Transmits the msg provided to the socket with specific message id.

**/
function __send(socket, msg, id) {

  if (socket == null || msg == null || id == null) {
    console.log("Received null parameters for sending libcluon envolope. {" + socket + ", " + msg + ", " + id + "}");
    return;
  } else {
    console.log("envolope {" + socket + ", " + msg + ", " + id + ", 0}");
  }

  var protoEncodedPayload = lc.encodeEnvelopeFromJSONWithoutTimeStamps(msg, id, 0);
  strToAB = str =>
    new Uint8Array(str.split('')
      .map(c => c.charCodeAt(0))).buffer;
  ws.send(strToAB(protoEncodedPayload), { binary: true });

  increment('messages-sent');
}


function mock_randomVals() {
  for (var i = 0; i < mock_random.length; i++) {
    mock_random[i] = parseInt(Math.random() * 30);
  }
}

function render() {
  updateCanvas();
}

function mock_onInterval() {

  mock_simulate(mock_random[0] *1.1, front);
  mock_simulate((mock_random[1] / 19.32) -1, accel);
  mock_simulate(mock_random[2] -15, angle);
  updateCanvas();
}

function mock_simulate(id, fun) {
    fun(id);
}

function simulate(id, fun) {
  var dom = document.getElementById(id + "_field0_value");

  if (dom == null) {
    return;
  }

  var text = dom.innerText;
  fun(parseInt(text));
}

/**

Increment the inner value of an element.

**/
function increment(id) {
  let tmp = document.getElementById(id).innerText;
  document.getElementById(id).innerText = parseInt(tmp) +1;
}

window.onkeydown = function(e) {
   validateKey(e.keyCode ? e.keyCode : e.which, "darkred");
}

window.onkeyup = function(e) {
   validateKey(-(e.keyCode ? e.keyCode : e.which), "coral");
}

function isRangeSlider(code) {
  return table[code] != null && table[code][2] == "range";
}

function isLetter(str) {
  return str.length === 1 && str.match(/[a-z]/i);
}