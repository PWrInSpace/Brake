#include "queue.h"

Queue::Queue(){
  head = nullptr;
  tail = nullptr;
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
    //error handling
  }
}

String Queue::pop(){
  String value = "";
  if(head != nullptr){
    Queue_struct *temp = head->next;
    value = head->data;
    delete head;
  
    head = temp;
    if(temp == nullptr){  //if it was last element
      tail = temp; 
    }

  }else{
    //error handling - list is empty
  }
  
  return value;
}

unsigned int Queue::getNumberOfElements(){
  unsigned int elements = 0;
  
  Queue_struct *current = head;
  if(current == nullptr){
    return elements;
  }

  while(current != nullptr){
    current = current->next;
    elements++;
  }

  return elements;
}

void Queue::print(){
  Queue_struct *current = head;
  if(current == nullptr){
    //Serial.println("Brak elementow!"); 
    return;
  }
  //Serial.println("=====================================");
  while (current != nullptr) {
    //Serial.println(current->data);
    current = current->next;
  }
  //Serial.println("=====================================");
}