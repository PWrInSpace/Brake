#include "queue.h"

Queue::Queue(){
  head = nullptr;
  tail = nullptr;
  numberOfElements = 0;
}

Queue::~Queue(){
  while(head != nullptr){
    Queue_struct *tmp = head->next;
    delete head;
    head = tmp;
  }
}

void Queue::push(const String & value){
  try{
    Queue_struct *temp = new Queue_struct;
    temp->data = value;
    temp->next = nullptr;

    if (head == nullptr){
    head = temp;
    tail = temp;
    }else{
      tail->next = temp;
      tail = temp;
    }
  }
  catch(std::bad_alloc & e){
    return;
  }
  
  numberOfElements += 1;
}

String Queue::pop(){
  String value = "";
  if(head != nullptr){
    Queue_struct *temp = head->next;
    value = head->data;
    delete head;
  
    head = temp;
    if(temp == nullptr){  //gdy ściągniety został ostatni element
      tail = temp; 
    }
    numberOfElements -= 1;
  }else{
    //Serial.println("Brak elementow!");
  }
  
  return value;
}

void Queue::print(){
  Queue_struct *current = head;
  if(current == nullptr){
    Serial.println("Brak elementow!");
    return;
  }
  Serial.println("=====================================");
  while (current != nullptr) {
    Serial.println(current->data);
    current = current->next;
  }
  Serial.println("=====================================");
}

unsigned int Queue::getNumberOfElements(){
  return numberOfElements;
}
