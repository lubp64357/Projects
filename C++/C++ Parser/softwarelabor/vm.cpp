#include "utility.h"
#include "vm.h"


Code newPush(int i) {
  return Code(VM_PUSH, i);
}

Code newPlus() {
  return Code(VM_PLUS);
}

Code newMult() {
  return Code(VM_MULT);
}


Optional<int> VM::run() {

      // always start with an empty stack
      stack<int> d;
      s.swap(d);

      for(unsigned int i = 0; i < code.size(); i++) {
    switch(code[i].kind) {
    case VM_PUSH:
      s.push(code[i].val);
        break;
    case VM_MULT: {
      int right = s.top();
      s.pop();
      int left = s.top();
      s.pop();
      s.push(left * right);
      break;
    }
    case VM_PLUS: {
      int right = s.top();
      s.pop();
      int left = s.top();
      s.pop();
      s.push(left + right);
      break;
    }
    }
      }

      if(s.empty())
    return nothing<int>();

      return just<int>(s.top());
} // run
