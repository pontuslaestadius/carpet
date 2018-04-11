/*
Copyrighted probably, 2018 Pontus Laestadius

*/

var ws;
var lc = libcluon();
var box = document.getElementById("box");

$(document).ready(function(){
  setupViewer();
});

function setupViewer() {

  if ("WebSocket" in window) {
    body("grey");
    ws = new WebSocket("ws://" + window.location.host + "/");
    ws.binaryType = 'arraybuffer';

    ws.onopen = function() {
      body("LightGreen");
      box.style.opacity = "1.0";
      onStreamOpen(lc);
    }

    ws.onclose = function() {
      body("Tomato");
      box.style.opacity = "0.0";
    };

  } else {
    body("Yellow");
  }
}

function onStreamOpen(lc) {
  function getResourceFrom(url) {
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open("GET", url, false);
    xmlHttp.send(null);
    return xmlHttp.responseText;
  }
  var odvd = getResourceFrom("messages.odvd");
  console.log("Loaded " + lc.setMessageSpecification(odvd) + " messages from specification.");
}

function body(c) {
  var z = document.getElementById("status");
  z.style.backgroundColor = c;
}
