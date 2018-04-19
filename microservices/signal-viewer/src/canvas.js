/*
Copyright 2018 @ Pontus Laestadius
*/

var canvas;
var car_width = 70;
var car_height = 70;
var y_offset = 0;
var front_reading = 30;
var last_deg = 0;

function drawAction() {
    getCtx().drawImage(getImage(), -car_width/2,-car_height/2 +y_offset, car_width, car_height);
}

function front_inner() {
	let cone = document.getElementById("cone");
	getCtx().drawImage(cone, -car_width/2,-car_height/2 +y_offset -car_height/1.55 +(50-front_reading), car_width, front_reading);
}

function front(distance) {
	front_reading = distance;
	//turn(last_deg, drawAction);
}

function accel(val) {
	y_offset = accel;
}

function angle(val) {
	last_deg = val;
}

function updateCanvas() {

	clear();
	var ctx = getCtx();
	front_inner();

	ctx.translate( getWidth()/2 +car_width/2, getHeight()/2  +car_height/2);
	ctx.rotate(last_deg*Math.PI/180);

	lamb();
	front_inner();

	ctx.rotate(-last_deg*Math.PI/180);
	ctx.translate( -(getWidth()/2 +car_width/2), -(getHeight()/2  +car_height/2));
}

function getImage() {
	return document.getElementById("image");
}

function getWidth() {
	__if_null();
	return canvas.width;
}

function getHeight() {
	__if_null();
	return canvas.height;
}

function getCtx() {
	__if_null();
	return canvas.getContext('2d');
}

function clear() {
	__if_null();
	getCtx().clearRect(0, 0, getWidth(), getHeight());
}

function __if_null() {
	if (canvas == null) {
		canvas = document.getElementById('canvas');
	}
}