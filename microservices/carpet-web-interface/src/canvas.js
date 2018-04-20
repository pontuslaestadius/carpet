/*
Copyright 2018 @ Pontus Laestadius
*/

const car_width = 200;
const car_height = 300;

var y_offset = 0;
var front_reading = 30;
var deg = 0;

// Image resources
var canvas = document.getElementById("canvas");
var vehicle = document.getElementById("image");
var cone = document.getElementById("cone");

// Ideal positions from the last readings.
var head_angle = 0;
var head_offset = 0;
var head_reading = 0;

function front(val) {
	head_reading = val*5;
}

function accel(val) {
	head_offset = -val*60;
}

function angle(val) {
	head_angle = val*2;
}

function updateCanvas() {
	
	deg = partialEquation(head_angle, deg);
	y_offset = partialEquation(head_offset, y_offset);
	front_reading = partialEquation(head_reading, front_reading);

	clear();
	var ctx = getCtx();

	ctx.translate( getWidth()/2, getHeight()/2);
	ctx.rotate(deg*Math.PI/180);

	draw();

	ctx.rotate(-deg*Math.PI/180);
	ctx.translate( -getWidth()/2, -getHeight()/2);
}

/**

Smooths out actions over a set of frames by the use of math.

**/
function partialEquation(ideal, current) {
	if (ideal == current) {
		return ideal;
	}

	var ite = 8;
	var dif = Math.abs(ideal -current);
	var one_ite = ((ideal -current)/ite)+1;

	if (dif <= 5) {
		current = ideal;
	} else {
		current += one_ite;
	}

	return current;
}

/**

Draws the components on the canvas.

**/
function draw() {
	var ctx = getCtx();
 	ctx.fillRect(0,0,5,5); 	
 	ctx.drawImage(vehicle, 
 		-car_width/2, -car_height/2 +y_offset, 
 		car_width, car_height);
	ctx.drawImage(cone, 
		-car_width/2 +6, -car_height/2 +y_offset -front_reading +25, 
		car_width, front_reading);
}

function clear() {
	getCtx().clearRect(0, 0, getWidth(), getHeight());
}

function getWidth() { return canvas.width; }
function getHeight() { return canvas.height; }
function getCtx() { return canvas.getContext('2d'); }
