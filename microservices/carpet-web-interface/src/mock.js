var mock_max = 3;
var mock_size = 0;
var mock_random = new Array(mock_max);
mock_randomVals(3);


/**

Recursively sets a count mock data based on the number provided.

**/
function mock_randomVals(c) {
  mock_random[mock_size++] = parseInt(Math.random() * 30);

  if (mock_size >= mock_max) {
    mock_size = 0;
  }

  if (c == null || c == 0) {
    return;
  } else {
    mock_randomVals(c-1);
  }
}

/**

Used for updaing the mocked canvas to prodoce a simulation.

**/
function mock_onInterval() {

  mock_simulate(mock_random[0] *1.1, front);
  mock_simulate((mock_random[1] / 19.32) -1, accel);
  mock_simulate(mock_random[2] -15, angle);
  updateCanvas();
}

function mock_simulate(id, fun) {
    if (id == null) {
      fun();
    } else {
      fun(id);
    }
}