var servo = require("../../index");
/* ---------------------------------------------------------------------------------
  The following script will connect to a Micro Maestro 6-Channel Servo Controller
------------------------------------------------------------------------------------ */

/* ---------------------------------------------------
  connect(device_path)
    - device_path: the path to the Maestro device

        Mac: '/dev/cu.usbmodem00115481'
        Ubuntu: '/dev/ttyAMA0'
        Raspi: '/dev/ttyACM0'

  NOTE: The Maestro's serial mode must be set to "USB Dual Port".
------------------------------------------------------ */
servo.connect('/dev/cu.usbmodem00115481');


/* ---------------------------------------------------
  setSpeed(channel, speed)
    - channel: servo number
    - speed: servo speed
------------------------------------------------------ */
servo.setSpeed(0, 25);


/* ---------------------------------------------------
  setAccel(channel, accel)
    - channel: servo number
    - accel: servo acceleration
------------------------------------------------------ */
servo.setAccel(0, 2);


/* ---------------------------------------------------
  setTarget(channel, target)
    - channel: servo number
    - target: the target servo posistion in quarter-microseconds

  servo protections:
    - servo min: 2000 (500us)
    - servo max: 10000 (2500us)

  for reference:
    - servo center: ~6000 (1500us)
------------------------------------------------------ */
servo.setTarget(0, 6000);


/* ---------------------------------------------------
  getPosition(channel)
    - channel: servo number
------------------------------------------------------ */
console.log('Servo position:', servo.getPosition(0));


/* ---------------------------------------------------
  disconnect()
    - close connection to Maestro device
------------------------------------------------------ */
servo.disconnect();
