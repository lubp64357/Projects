#include <iostream>
#include <string>

using namespace std;

#include "vm.h"
#include "parser.h"

void showVMRes(Optional<int> r) {
  if(r.isNothing())
    cout << "\nVM stack (top): empty";

  cout << "\nVM stack (top): " << r.fromJust();
  

}

void testVM() {

  {
    vector<Code> vc{
      newPush(1),
      newPush(2),
      newPush(3),
      newMult(),
      newPlus() };

    Optional<int> res = VM(vc).run();

    showVMRes(res);
  }
  
  {
    vector<Code> vc{
      newPush(2),
      newPush(3),
      newPush(5),
      newPlus(),
      newMult() };

    Optional<int> res = VM(vc).run();

    showVMRes(res);
  }
  {
		std::cout << std::endl;

		std::string userInput;
//		std::cin >> userInput;

     //Testcases 

		 userInput = "3+3*9";
    // userInput = "1*2 + 1";
    // userInput = "1+ 1*2*2";
    // userInput = "1*2 + 1*2";
    //  userInput = "(4+2)*3";
    //   userInput = "+ 1 1 ";

		Parser p(userInput);
		Optional<EXP> parsedValue = p.parse();                        //parsed String zu einem Token, in dem die Zahlen und die Operanten sind ( + * ( ))
		if(parsedValue.isJust()) {                                   //geht in Schleife wenn erlaubte Tokens enthalten sind (0-9 ()+*)
			cout << parsedValue.fromJust()->supperPretty() << std::endl; // entfernt �berfl�ssige Klammern
			vector<Code> vc = parsedValue.fromJust()-> translate(); // geht durch den Baum und "�bersetzt" diesen in die VM

			Optional<int> res = VM(vc).run();
			showVMRes(res);
		}else{
			cout<<"Input not correct. Valid values: 0-9, +, *"; // Falls falsche Werte in geparsten Objekt sind -> Error
		}



  }
}


int main() {

  testVM();
  
  return 1;
}
