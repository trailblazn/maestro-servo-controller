#include "maestro_servo_controller.h"

Nan::Persistent<v8::Function> Maestro::constructor;

// Constructor
Maestro::Maestro() {
  _maestro_target_min = 2000;  // quarter-microseconds
  _maestro_target_max = 10000;

  _maestro_target_center = 6000;  // 1500us
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
  Nan::SetPrototypeMethod(tpl, "setTarget", SetTarget);


  constructor.Reset(tpl->GetFunction());
  exports->Set(Nan::New("Maestro").ToLocalChecked(), tpl->GetFunction());
}

// New
void Maestro::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.IsConstructCall()) {
    Maestro* maestro = new Maestro();
    maestro->Wrap(info.This());

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
  // control communication configuration
  printf("\nMaestro Virtual COM port: %s", maestro->_maestro_path);
    printf("\nConnecting to Maestro board...");
  maestro->_maestro_device = open(maestro->_maestro_path, O_RDWR | O_NOCTTY);
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
  options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
  options.c_oflag &= ~(ONLCR | OCRNL);
  options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  tcflush(maestro->_maestro_device, TCIFLUSH);
  tcsetattr(maestro->_maestro_device, TCSANOW, &options);

  info.GetReturnValue().Set(true);
}

// Disconnect
void Maestro::Disconnect(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Maestro* maestro = ObjectWrap::Unwrap<Maestro>(info.Holder());

  close(maestro->_maestro_device);
}

// SetTarget
void Maestro::SetTarget(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Maestro* maestro = ObjectWrap::Unwrap<Maestro>(info.Holder());
  unsigned char channel = (unsigned char)(info[0]->NumberValue());
  unsigned short target = (unsigned short)(info[1]->NumberValue());

  unsigned char packet[] = {
    SET_TARGET_COMMAND,                    // command
    channel,                               // channel
    (unsigned char)(target & 0x7F),        // target low byte
    (unsigned char)(target >> 7 & 0x7F)    // target high byte
  };

  if (write(maestro->_maestro_device, packet, sizeof(packet)) == -1) {
  	return info.GetReturnValue().Set(false);
  }

  return info.GetReturnValue().Set(true);
}
