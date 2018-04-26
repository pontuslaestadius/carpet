/*
Copyright 2018 @ Pontus Laestadius

interfaces between events and the code.

*/
var mid = 9999;
var size = 128;

// Create a bool table with keys.
var table = new Array(size);

// table[code][field, messageid, down value, up value]
for (var i = 0; i < size; i++) {
  table[i] = new Array(4);
}


var delay = -1;
var maxDelay = 4;

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

Validates that the provided key is registered and sends it forward.

**/
async function validateKey(k, s) {
  if (table[Math.abs(k)][0] == null) {
    return;
  }

  // Range sliders ignore if the key is alread pressed.
  var val = table[Math.abs(k)][2];
  if (val == "range") {
    var kabs = Math.abs(k);
    var qs = document.querySelector("#\\3" + parseInt(kabs/10) + " " + kabs%10 + " input");
    var element = document.querySelector("#\\3" + parseInt(kabs/10) + " " + kabs%10 + " b");


    if (k < 0) {
      element.innerText = table[-k][0];
      qs.value = 0.0;
      delay = -1;
    } else {
      if (delay-- <= 0) {
        delay = maxDelay;
      } else {
        return;
      }

      var rangevalue = parseFloat(qs.value);

      if ((qs.value >= parseFloat(qs.max))) {
        return;
      }


      var rmax = 0.10;
      if (parseFloat(qs.value) < rmax) {
        delay = -1;
        setBackground(k, s);
        for (var i = 0; i < rmax; i += parseFloat(qs.step)) {
          qs.value = i;
          element.innerText = qs.value;
          send(k);
          await sleep(25 +(rmax - i)*3);
        }

      } else if (rangevalue < 0.18) {
        delay--;
        qs.value = rangevalue + parseFloat(qs.step)*2;
      } else {
        qs.value = rangevalue + parseFloat(qs.step);
      }

      element.innerText = qs.value;

    }

  }

  setBackground(k, s);
  send(k);
}

/**

Converts a data to that of a simulated message on the canvas.

**/
function convertMessageReadingsToSimulation(val, type) {
  type = parseInt(type);
  switch(type) {
    case 1004:
    mock_simulate(null, stopFollow);
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
    jsonMessageToBeSent = "{\"nothing\": 0}";
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

function render() {
  updateCanvas();
}

function simulate(id, fun) {
  var dom = document.getElementById(id + "_field0_value");

  if (dom == null) {
    return;
  }

  var text = dom.innerText;
  fun(parseInt(text));
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

function delm(k) {
  return document.getElementById(Math.abs(k));
}

function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}