/*
Copyrighted probably, 2018 @ Pontus laestadius
*/

var x = document.getElementById("indicator");
var mid = 9999;
var size = 128;
var table = new Array(size);
table[37] = "LeftArrow";
table[38] = "DownArrow";
table[39] = "RightArrow";
table[40] = "UpArrow";

window.onkeydown = function(e) {
   var key = e.keyCode ? e.keyCode : e.which;
   validateKey(key, "black");
}

window.onkeyup = function(e) {
   var key = e.keyCode ? e.keyCode : e.which;
   validateKey(-key, "grey");
}

function validateKey(k, s) {
   if (table[Math.abs(k)] == null && k[math.abs(k)]) {
      return;
   }
   var e = document.getElementById(Math.abs(k));
   e.style.backgroundColor = s;
   send(k);
}

function send(k) {
   var jsonMessageToBeSent = "{\"" + table[Math.abs(k)] + "\":" + k + "}";
   console.log(jsonMessageToBeSent);
   var protoEncodedPayload = lc.encodeEnvelopeFromJSONWithoutTimeStamps(jsonMessageToBeSent, mid, 0);
    strToAB = str =>
      new Uint8Array(str.split('')
        .map(c => c.charCodeAt(0))).buffer;
    ws.send(strToAB(protoEncodedPayload), { binary: true });
}