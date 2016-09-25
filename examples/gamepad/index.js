var servo = require("../../index");
// gamepad package for reading gamepad controller
var gamepad = require("gamepad");
/* ---------------------------------------------------------------------------------
  The following script will connect to a Micro Maestro 6-Channel Servo Controller,
    and set servo positions for a pan/tilt rig with gamepad joystick events
------------------------------------------------------------------------------------ */
var SERVO_PAN_CENTER_POSITION = 6200;
var SERVO_TILT_CENTER_POSITION = 5675;
var SERVO_PAN_MAX_DEVIATION = 2500;
var SERVO_TILT_MAX_DEVIATION = 2000;

var AxisX = 0;
var AxisY = 0;

var pan = SERVO_PAN_CENTER_POSITION;
var tilt = SERVO_TILT_CENTER_POSITION;

servo.connect('/dev/cu.usbmodem00115481');
servo.setSpeed(0, 40);
servo.setAccel(0, 10);

var calculateTargets = function() {
  pan = (AxisX * SERVO_PAN_MAX_DEVIATION) + SERVO_PAN_CENTER_POSITION;
  tilt = (AxisY * SERVO_TILT_MAX_DEVIATION) + SERVO_TILT_CENTER_POSITION;
};

var setTargets = function() {
  servo.setTarget(0, pan);
  servo.setTarget(1, tilt);
};

// apply servo default targets
setTargets();

// Initialize the library
gamepad.init();
// Create a game loop and poll for events
setInterval(gamepad.processEvents, 50);
// Scan for new gamepads as a slower rate
setInterval(gamepad.detectDevices, 5000);
// Listen for move events on all gamepads
gamepad.on("move", function (id, axis, value) {
  if (axis === 2) AxisX = value;
  if (axis === 3) AxisY = value;
  // calculate servo targets based on gamepad axes positions
  calculateTargets();
  // apply servo targets
  setTargets();
});

// close maestro and application, when any button pushed
gamepad.on("down", function (id, num) {
  // print servo positions to terminal
  if (num === 16) { // 'A' button on the XBox One controller
    console.log("Servo position (pan):", servo.getPosition(0));
    console.log("Servo position (tilt):", servo.getPosition(1));
  }
  // disconnect and terminate the application
  if (num === 17) { // 'B' button on the XBox One controller
    // IO stream must be closed before terminating the application
    servo.disconnect();
    // termiate application
    setTimeout(function() {
      process.exit();
    }, 250);
  }
});
