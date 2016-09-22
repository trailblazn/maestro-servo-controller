// Maestro object
var maestro = require("../../index").Maestro();

/*
  The following script will connect to a Micro Maestro 6-Channel Servo Controller

  - device path:
      Mac: '/dev/cu.usbmodem00115481'
      Ubuntu: '/dev/ttyAMA0'
      Raspi: '/dev/ttyACM0'
*/

// connect
// NOTE: The Maestro's serial mode must be set to "USB Dual Port".
maestro.connect('/dev/cu.usbmodem00115481');

maestro.setTarget(0, 6000);

// disconnect
maestro.disconnect();
