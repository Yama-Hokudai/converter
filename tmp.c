#include <iostream>
#include <string>
#include <stdio.h>

int getChar(std::string str,char **c);
int getSize(std::string);

int getSize(std::string str){
  return str.length();
}

int getChar(std::string str, char **c){
  std::cout << "getChar" << std::endl;
  char *tmp = (char*)str.c_str();
  int size = str.length();
  *c = new char[size];
  for(int i = 0; i < size; i++){
    (*c)[i] = tmp[i];
  }

  return size;
}

int main(void){
  std::string str = "Hello World!";
  char **c = new char*;
  /* std::cout << "ok" << std::endl; */
  int size = getChar(str, c);
  std::cout << "ok" << std::endl;
  if(c == NULL){
    std::cout << "NULL" << std::endl;
  }else{
    std::cout << size << ":" << *c << std::endl;
  }
  std::string str2;
  str2 = *c;
  std::cout << str2 << std::endl;
  delete[] *c;
  delete[] c;

  return 0;
}
