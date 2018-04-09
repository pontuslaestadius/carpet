/*
Copyrighted probably, 2018 Pontus Laestadius

*/


var g_data = new Map();
var ws;
var lc = libcluon();

$(document).ready(function(){
  setupViewer();
});

function setupViewer() {

  if ("WebSocket" in window) {

    ws = new WebSocket("ws://" + window.location.host + "/");
    ws.binaryType = 'arraybuffer';

    ws.onopen = function() {
      onStreamOpen(lc);
    }

    ws.onclose = function() {
      onStreamClosed();
    };

  } else {
    body("Yellow");
  }
}

function onStreamOpen(lc) {
  body("LightGreen");
  function getResourceFrom(url) {
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open("GET", url, false);
    xmlHttp.send(null);
    return xmlHttp.responseText;
  }

  var odvd = getResourceFrom("messages.odvd");

  console.log("Connected to stream.");
  console.log("Loaded " + lc.setMessageSpecification(odvd) + " messages from specification.");
  
  setInterval(onInterval, 250);
}

function onStreamClosed() {
  body("Tomato");
}

function body(c) {
  var z = document.getElementById("body");
  z.style.backgroundColor = c;
}
