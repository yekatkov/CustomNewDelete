#include <string>

std::string* str = nullptr;

void fillString( const char* in ) {
    str = new std::string( in );
}

void foo()
{
    // Use very long string in order to prevent SSO
    fillString( "stl::foooooooooooooooooooooooooooooooooooooooooooooooo" );
    delete str;
}
