#ifndef QUEUE_ROCKET_H
#define QUEUE_ROCKET_H

#include <Arduino.h>

struct Queue_struct {
  String data;
  struct Queue_struct *next;
};

class Queue {
  private:
  Queue_struct *head;
  Queue_struct *tail;

  public:
  Queue();
  void push(const String & value);
  void print();
  String pop();
  unsigned int getNumberOfElements();
  ~Queue();
};

#endif