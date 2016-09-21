#include "maestro_servo_controller.h"

Nan::Persistent<v8::Function> Maestro::constructor;

// Constructor
Maestro::Maestro(unsigned char addr, int baud_rate) {
    _maestro_address = addr;
    _maestro_baud_rate = baud_rate;
}

// Deconstructor
Maestro::~Maestro() {
}

// Init
void Maestro::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Maestro").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "connect", Connect);
  Nan::SetPrototypeMethod(tpl, "disconnect", Disconnect);

  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("Maestro").ToLocalChecked(), tpl->GetFunction());
}

// New
void Maestro::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.IsConstructCall()) {
    Maestro* maestro = new Maestro();
    maestro->Wrap(info.This());
    // set maestro board address
    maestro->_maestro_address = info[0]->IsUndefined() ? maestro->_maestro_address : (unsigned char)(info[0]->NumberValue());

    info.GetReturnValue().Set(info.This());
  } else {
    // Invoked as plain function `Maestro(...)`, turn into construct call.
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = { info[0] };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

// Connect
void Maestro::Connect(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Maestro* maestro = ObjectWrap::Unwrap<Maestro>(info.Holder());
  v8::String::Utf8Value device_path(info[0]->ToString());
  maestro->_maestro_path = (const char*)(*device_path);
  // drive control communication configuration
  printf("\nMaestro Virtual COM port: %s", maestro->_maestro_path);
  printf("\nMaestro Bus address: %i", maestro->_maestro_address);
  printf("\nConnecting to Maestro board...");
  maestro->_maestro_device = open(maestro->_maestro_path, O_RDWR | O_NOCTTY | O_NDELAY);
  if (maestro->_maestro_device == -1)
  {
    printf("\nFailed to connect!");
    perror(maestro->_maestro_path);
    info.GetReturnValue().Set(false);
    return;
  }
  printf("\nConnected!\n");
  struct termios options;
  tcgetattr(maestro->_maestro_device, &options);
  options.c_cflag = maestro->_maestro_baud_rate | CS8 | CLOCAL | CREAD;   //<Set baud rate
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;
  options.c_lflag = 0;
  tcflush(maestro->_maestro_device, TCIFLUSH);
  tcsetattr(maestro->_maestro_device, TCSANOW, &options);

  info.GetReturnValue().Set(true);
}

// Disconnect
void Maestro::Disconnect(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Maestro* maestro = ObjectWrap::Unwrap<Maestro>(info.Holder());

  close(maestro->_maestro_device);
}

// convertIntegerToBytes
int Maestro::convertIntegerToBytes(int value, unsigned char* buf, int offset) {
  buf[offset]   = value>>24;
  buf[offset+1] = value>>16;
  buf[offset+2] = value>>8;
  buf[offset+3] = value;

  return offset + 4;
}

// Read from maestro board
bool Maestro::maestro_read(int device, unsigned char* buf, int length) {
  int r_result = read(device, buf, 1);
  if (r_result == -1) {
    return false;
  }

  return true;
}

// Write to maestro board
bool Maestro::maestro_write(int device, unsigned char address, unsigned char command, unsigned char* data, int length) {
  unsigned char _data[2 + length + 2];  // addr(1) + cmd(1) + data(length) + checksum(2)
  // device address
  _data[0] = address;
  // device command
  _data[1] = command;
  // fill data bytes
  for(int i =0; i < length; i++) {
    _data[i+2] = data[i];
  }
  // write to device
  int w_result = write(device, &_data[0], sizeof(_data));
  if (w_result == -1 || w_result != (int)(sizeof(_data))) {
    return false;
  }

  return true;
}
