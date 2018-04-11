/*
Copyright 2018 @ Pontus Laestadius
*/
var mid = 9999;
var size = 128;
var table = new Array(size);
for (var i = 0; i < size; i++) {
  table[i] = new Array(4);
}

window.onkeydown = function(e) {
   validateKey(e.keyCode ? e.keyCode : e.which, "darkred");
}

window.onkeyup = function(e) {
   validateKey(-(e.keyCode ? e.keyCode : e.which), "coral");
}
  
function isLetter(str) {
  return str.length === 1 && str.match(/[a-z]/i);
}

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
}

function validateKey(k, s) {
  if (table[Math.abs(k)][0] == null || 
    document.getElementById(Math.abs(k)).style.backgroundColor == s) {
    return;
  }
  document.getElementById(Math.abs(k)).style.backgroundColor = s;
  send(k);
}

function send(k) {
  var add = 0;
  if (k < 0) {
    k = -k;
    add = 1;
  }
  var jsonMessageToBeSent = "{\"" + table[k][0] + "\":" + table[k][2+add] + "}";
  console.log(jsonMessageToBeSent + " -> " + table[k][1]);
  var protoEncodedPayload = lc.encodeEnvelopeFromJSONWithoutTimeStamps(jsonMessageToBeSent, table[k][1], 0);
  strToAB = str =>
    new Uint8Array(str.split('')
      .map(c => c.charCodeAt(0))).buffer;
  ws.send(strToAB(protoEncodedPayload), { binary: true });
}
