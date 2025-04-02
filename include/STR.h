#include <string>
#include <sstream>
// Suppose you have a function:
//   void func(std::string c);
// Then you can write:
//   func(STR("foo"<<1<<"bar"));
#define STR(X) static_cast<std::ostringstream&>(std::ostringstream().flush() << X).str() 