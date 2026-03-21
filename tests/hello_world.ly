/* Testing block comments 
   and complex strings 
*/

var message char* = "Hello, Compiler!";
var newline char = '\n';
var tab char = '\t';

func print_msg(s char*) -> void {
    var first_char char*  = s;
    print(s);
}
