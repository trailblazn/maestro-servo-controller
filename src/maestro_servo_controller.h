#ifndef MAESTRO_SERVO_CONTROLLER_H
#define MAESTRO_SERVO_CONTROLLER_H

#include <nan.h>

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#define SET_TARGET_COMMAND    0x84
#define GET_POSITION_COMMAND  0x90
#define SET_SPEED_COMMAND     0x87
#define SET_ACCEL_COMMAND     0x89

// Maestro
class Maestro : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);

 private:
  explicit Maestro();
  ~Maestro();

  static Nan::Persistent<v8::Function> constructor;

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Connect(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Disconnect(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void SetTarget(const Nan::FunctionCallbackInfo<v8::Value>& info);

  int               _maestro_device;
  const char*       _maestro_path;
  int               _maestro_target_min;
  int               _maestro_target_max;
  int               _maestro_target_center;
};

#endif
