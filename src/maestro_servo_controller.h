#ifndef MAESTRO_SERVO_CONTROLLER_H
#define MAESTRO_SERVO_CONTROLLER_H

#include <nan.h>

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

// Maestro
class Maestro : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);

 private:
  explicit Maestro(unsigned char addr = 0x80, int baud_rate = 115200);
  ~Maestro();

  static Nan::Persistent<v8::Function> constructor;

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Connect(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void Disconnect(const Nan::FunctionCallbackInfo<v8::Value>& info);

  bool maestro_write(int device, unsigned char address, unsigned char command, unsigned char* data, int length);
  bool maestro_read(int device, unsigned char* buf, int length);
  int convertIntegerToBytes(int value, unsigned char* buf, int offset = 0);

  int               _maestro_device;
  unsigned char     _maestro_address;
  const char*       _maestro_path;
  int               _maestro_baud_rate;
};

#endif
