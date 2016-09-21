// Maestro object
var maestro = require("../../index").Maestro();

/*
  The following script will connect to a Micro Maestro 6-Channel Servo Controller

  - device path:
      Mac: '/dev/cu.usbmodem00115481'
      Ubuntu: '/dev/ttyAMA0'
      Raspi: '/dev/ttyACM0'

  - baud rate:
      defaults to 115200
*/

// connect
maestro.connect('/dev/cu.usbmodem00115481', 115200);

// disconnect
maestro.disconnect();
