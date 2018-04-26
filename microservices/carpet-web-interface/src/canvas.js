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

// Used for follower / leader mimicing.
var prev_head_angle = 0;
var prev_head_offset = 0;
var prev_head_reading = 0;

var follower = false;
var leader = false;

function front(val) {
	prev_head_reading = head_reading;
	head_reading = val*5;
}

function accel(val) {
	//prev_head_offset = head_offset;
	//head_offset = -val*10;
}

function angle(val) {
	prev_head_angle = head_angle;
	head_angle = -val*2;
}

function rotateDraw(deg, fun) {
	ctx.rotate(deg*Math.PI/180);
	fun();
	ctx.rotate(-deg*Math.PI/180);
}

function updateCanvas() {
		clear();

	
	deg = partialEquation(head_angle, deg);
	y_offset = partialEquation(head_offset, y_offset);
	front_reading = partialEquation(head_reading, front_reading);

	var ctx = getCtx();

	ctx.translate( getWidth()/2, getHeight()/2);
	ctx.rotate(deg*Math.PI/180);


	if (leader || follower) {
		if (follower && leader) {
			follower = false;
		}

		if (leader) {
			drawLeader();
		} else {
			drawFollower();
		}

	}
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

	var w = car_width;
	var h = car_height;
	var y = y_offset;

	if (follower || leader) {
		w = w/2;
		h = h/2;

		if (leader) {
			y = y-h/2;
		} else {
			y = y+h/2;
			deg = partialEquation(prev_head_angle, deg);
			y_offset = partialEquation(prev_head_offset, y_offset);

			ctx.rotate(deg*Math.PI/180);
		}

	}

 	ctx.drawImage(vehicle, -w/2, -h/2 +y, w, h);
	ctx.drawImage(cone, -w/2 +6, -h/2 +y -front_reading +25, w, front_reading);

	if (follower) {
		ctx.rotate(-deg*Math.PI/180);
	}
}

function drawLeader() {
	var ctx = getCtx();

	var w = car_width/2;
	var h = car_height/2;
	var y = y_offset - h/2;

	deg = partialEquation(prev_head_angle, deg);
	y_offset = partialEquation(prev_head_offset, y_offset);

	ctx.rotate(deg/2*Math.PI/180);

	ctx.drawImage(vehicle, -w/2, +h/2 +y, w, h);

	ctx.rotate(-deg/2*Math.PI/180);
}

function drawFollower() {
	var ctx = getCtx();

	var w = car_width/2;
	var h = car_height/2;
	var y = y_offset + h/2;

	ctx.drawImage(vehicle, -w/2, -h*1.5 +y, w, h);
}

function clear() {
	getCtx().clearRect(0, 0, getWidth(), getHeight());
}

function toggleFollow() { 
	if (leader) {
		leader = false;
	}
	follower = !follower; 
}
function toggleLead() { 
	if (follower) {
		follower = false;
	}
	leader = !leader; 
}

function getWidth() { return canvas.width; }
function getHeight() { return canvas.height; }
function getCtx() { return canvas.getContext('2d'); }

function stopFollow() { follower = false; }
function startFollow() { follower = true; }
function stopLead() { leader = false; }
function startLead() { leader = true; }
