#include <nan.h>

#include "maestro_servo_controller.h"

void InitAll(v8::Local<v8::Object> exports){
  Maestro::Init(exports);
}

NODE_MODULE(maestro, InitAll)
