#include <chrono>
#include <tuple>
#include <iostream>

#include "erd_lib.hpp"

static const char KEY_HELLO[] = "keyHello";
static const char KEY_TIME [] = "keyTime";
static const char KEY_INT  [] = "keyInt";

static const constexpr char KEY_FAILURE   [] = "key_failure";


static const char KEY_STR  [] = "keyStr";
static const char KEY_REF  [] = "keyRef";
static const char KEY_BOOL [] = "keyBool";

int main(int, char**)
{

    auto start = std::chrono::high_resolution_clock::now();
    erd::keys_tuple<erd::keys_t<KEY_HELLO, KEY_TIME, KEY_INT>, erd::types_t<std::string, decltype(start.time_since_epoch().count()), int>> foo( "Hello world", start.time_since_epoch().count(), 42);
    auto read = std::chrono::high_resolution_clock::now();
    int counter = 0;for(int i = 0 ; i < 1000000;i++)counter += foo.get<KEY_INT>();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout<<counter<<std::endl;


    foo.set<KEY_HELLO>("He??o WORLD");
    foo.set<KEY_INT>(43);

    std::cout<<foo.get<KEY_HELLO>()<<std::endl;
    std::cout<<"The Answer to the Ultimate Question of Life, the Universe, and Everything is "<<foo.get<KEY_INT>()<<std::endl;
    std::cout<<"Passed time for DataElement: ";
    std::cout<<"create:"<< std::chrono::duration_cast<std::chrono::nanoseconds>(read - start).count() << "ns ";
    std::cout<<"read:"<< std::chrono::duration_cast<std::chrono::nanoseconds>(end - read).count() << "ns" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    std::tuple<std::string, decltype(start), int> bar("Hello world", start, 42);
    read = std::chrono::high_resolution_clock::now();
    counter = 0;for(int i = 0 ; i < 1000000;i++)counter += std::get<2>(bar);
    end = std::chrono::high_resolution_clock::now();
    std::cout<<counter<<std::endl;
    std::cout<<"Passed time for  std::tuple: ";
    std::cout<<"create:"<< std::chrono::duration_cast<std::chrono::nanoseconds>(read - start).count() << "ns   ";
    std::cout<<"read:"<< std::chrono::duration_cast<std::chrono::nanoseconds>(end - read).count() << "ns" << std::endl;
#ifdef ERROR
    erd::get<KEY_FAILURE>(foo);
#endif


    return 0;
}
