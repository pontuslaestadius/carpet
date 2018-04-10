/*
Copyrighted probably, 2018 @ Pontus laestadius
*/
var mid = 9999;
var size = 128;
var table = new Array(size);
for (var i = 0; i < size; i++) {
  table[i] = new Array(2);
}

// These are the valid keys, along with the command and the message id.
table[37] = ["command", 2101];
table[38] = ["command", 2101];
table[39] = ["command", 2101];
table[40] = ["command", 2101];

window.onkeydown = function(e) {
   validateKey(e.keyCode ? e.keyCode : e.which, "black");
}

window.onkeyup = function(e) {
   validateKey(-(e.keyCode ? e.keyCode : e.which), "grey");
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
   var jsonMessageToBeSent = "{\"" + table[Math.abs(k)][0] + "\":" + k + "}";
   console.log(jsonMessageToBeSent);
   var protoEncodedPayload = lc.encodeEnvelopeFromJSONWithoutTimeStamps(jsonMessageToBeSent, table[Math.abs(k)][1], 0);
    strToAB = str =>
      new Uint8Array(str.split('')
        .map(c => c.charCodeAt(0))).buffer;
    ws.send(strToAB(protoEncodedPayload), { binary: true });
}