/*
Copyright 2018 @ Pontus Laestadius
*/

var canvas;
var car_width = 70;
var car_height = 70;
var y_offset = 0;
var front_reading = 30;
var deg = 0;


var head_angle = 0;
var head_offset = 0;
var head_reading = 0;

function drawAction() {
    getCtx().drawImage(getImage(), -car_width/2,-car_height/2 +y_offset, car_width, car_height);
}

function front_inner() {
	let cone = document.getElementById("cone");
	getCtx().drawImage(cone, -car_width/2,-car_height/2 +y_offset -car_height/1.55 +(50-front_reading), car_width, front_reading);
}

function front(val) {
	head_reading = val;
}

function accel(val) {
	head_offset = val;
}

function angle(val) {
	head_angle = val;
}

function updateCanvas() {
	
	deg = partialEquation(head_angle, deg);
	y_offset = partialEquation(head_offset, y_offset);
	front_reading = partialEquation(head_reading, front_reading);

	clear();
	var ctx = getCtx();
	front_inner();

	ctx.translate( getWidth()/2 +car_width/2, getHeight()/2  +car_height/2);
	ctx.rotate(deg*Math.PI/180);

	__updateCanvas();

	ctx.rotate(deg*Math.PI/180);
	ctx.translate( -(getWidth()/2 +car_width/2), -(getHeight()/2  +car_height/2));
}

function partialEquation(ideal, current) {
	if (Math.abs(ideal + current) < 3) {
		current = ideal;
	} else {
		current += ideal/10;
	}
	return current;
}

function __updateCanvas() {
	drawAction();
	front_inner();
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
	if (canvas == null) {
		console.log("Unable to locate canvas");
	}
}
