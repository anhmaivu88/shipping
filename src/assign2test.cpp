#include <string>
#include <set>
#include <ostream>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>
#include <sys/signal.h>
#include <boost/lexical_cast.hpp>
#include "Instance.h"

using namespace std;

#define STRINGIFY(a) #a
#define STRING(a) STRINGIFY(a)

#define ASSERT(a) {\
    if (!(a)) {\
        throw std::string("Assertion failed: "__FILE__ ":" STRING(__LINE__) " " #a);\
    }\
}

#define EQUAL(a, b) {\
    if ((a) != (b)) {\
      throw std::string("Assertion failed: "__FILE__":" STRING(__LINE__) " " + a + " == " + b);\
    }\
} 

#define MAYBETHROW(a) {\
    try {\
        a;\
    } catch (...) {\
    }\
}
        
#define TEST(testfn, points) {\
    test(&(testfn), (points), #testfn);\
}


/* This function runs a single test by forking the process.  This way,
   if there is a segfault during a test, the test program won't crash */
void test(void (*testfn)(), int points, const char *name) {
    pid_t pid = fork();
    
    if (pid > 0) {
        /* Check the child's return value */
        int status = 0;
        wait(&status);
        
        /* Non-zero value = test failed */
        cout << name << " " << (status ? 0 : points) << endl;
    } else if (pid == 0) {
        /* Run the test in the child process.  Exit with 0 for success,
           non-zero for failure */
        try {
            testfn();
            exit(0);
        } catch (std::string& error) {
            std::cout << error << endl;
            exit(1);
        } catch (...) {
            exit(1);
        }
    } else {
        cout << "Fork failed!!" << endl;
    }
}

/* Tokenizes the multi-line input into a set of strings */
set<string> tokenize(const std::string& output) {
    set<string> tokens; 
    static const char *delim = "\n";
    size_t last = output.find_first_not_of(delim, 0);
    size_t pos = output.find_first_of(delim, last);
    
    /* Find all lines that are non-empty */
    while (string::npos != pos || string::npos != last) {
        string token = output.substr(last, pos - last);
        for (size_t i = 0; i < token.size(); i++) {
            if (!isspace(token[i])) {
                /* Only insert if non-empty */
                cout << output.substr(last, pos - last) << endl;
                tokens.insert(output.substr(last, pos - last));
                break;
            }
        }

        last = output.find_first_not_of(delim, pos);
        pos = output.find_first_of(delim, last);
    }
    return tokens;
}

bool contains(const set<string>& s, const string& str) {
    return s.find(str) != s.end();
}

void testLocationAttributes() {
    Ptr<Instance::Manager> m = shippingInstanceManager();
    Ptr<Instance> fleet = m->instanceNew("fleet", "Fleet");
    Ptr<Instance> seg1 = m->instanceNew("seg1", "Truck segment");
    Ptr<Instance> seg2 = m->instanceNew("seg2", "Truck segment");
    Ptr<Instance> loc1 = m->instanceNew("loc1", "Port");
    Ptr<Instance> loc2 = m->instanceNew("loc2", "Truck terminal");

    seg1->attributeIs("source", "loc1");
    seg1->attributeIs("length", "10.5");
    seg1->attributeIs("return segment", "seg2");
    seg1->attributeIs("difficulty", "1.0");
    seg1->attributeIs("expedite support", "no");

    seg2->attributeIs("source", "loc2");
    seg2->attributeIs("length", "3.0");
    seg2->attributeIs("return segment", "seg1");
    seg2->attributeIs("difficulty", "5.0");
    seg2->attributeIs("expedite support", "yes");

    EQUAL(seg1->attribute("source"), "loc1");
    EQUAL(seg1->attribute("length"), "10.50");
    EQUAL(seg1->attribute("return segment"), "seg2");
    EQUAL(seg1->attribute("difficulty"), "1.00");
    EQUAL(seg1->attribute("expedite support"), "no");
    
    EQUAL(seg2->attribute("source"), "loc2");
    EQUAL(seg2->attribute("length"), "3.00");
    EQUAL(seg2->attribute("return segment"), "seg1"); 
    EQUAL(seg2->attribute("difficulty"), "5.00");
    EQUAL(seg2->attribute("expedite support"), "yes");

}


void testExplore1() {

    /* Simple network, arranged in a line */
    Ptr<Instance::Manager> m = shippingInstanceManager();
    Ptr<Instance> fleet = m->instanceNew("fleet", "Fleet");

    m->instanceNew("1", "Truck terminal");
    m->instanceNew("2", "Port");
    m->instanceNew("3", "Port");
    m->instanceNew("4", "Boat terminal");

    Ptr<Instance> s12 = m->instanceNew("s12", "Truck segment");
    Ptr<Instance> s21 = m->instanceNew("s21", "Truck segment");
    Ptr<Instance> s23 = m->instanceNew("s23", "Plane segment");
    Ptr<Instance> s32 = m->instanceNew("s32", "Plane segment");
    Ptr<Instance> s34 = m->instanceNew("s34", "Boat segment");
    Ptr<Instance> s43 = m->instanceNew("s43", "Boat segment");

    s12->attributeIs("length", "10");
    s21->attributeIs("length", "10");
    s23->attributeIs("length", "20");
    s32->attributeIs("length", "20");
    s34->attributeIs("length", "30"); 
    s43->attributeIs("length", "30");

    s12->attributeIs("source", "1");
    s12->attributeIs("return segment", "s21");
    s21->attributeIs("source", "2");

    s23->attributeIs("source", "2");
    s23->attributeIs("return segment", "s32");
    s32->attributeIs("source", "3");
    
    s34->attributeIs("source", "3");
    s34->attributeIs("return segment", "s43");
    s43->attributeIs("source", "4");

    Ptr<Instance> conn = m->instanceNew("conn", "Conn");
    set<string> t = tokenize(conn->attribute("explore 1 : distance 100000000"));
    ASSERT(contains(t, "1(s12:10.00:s21) 2"));
    ASSERT(contains(t, "1(s12:10.00:s21) 2(s23:20.00:s32) 3"));
    ASSERT(contains(t, "1(s12:10.00:s21) 2(s23:20.00:s32) 3(s34:30.00:s43) 4"));
}


void testStressTest() {
  Ptr<Instance::Manager> m = shippingInstanceManager();

  for (int i = 0; i < 100000; i++) {

    m->instanceNew("conn", "Conn");
    m->instanceDel("conn");

    m->instanceNew("fleet", "Fleet");
    m->instanceDel("fleet");
  
    m->instanceNew("seg", "Truck segment");
    m->instanceDel("seg");
  }
}

void sigsegv(int signal) {
    cerr << "Segmentation fault" << endl;
    exit(1);
}

int main(int argc, char *argv[]) {
    signal(SIGSEGV, sigsegv);
    TEST(testLocationAttributes, 1);
    TEST(testExplore1, 1);
    TEST(testStressTest, 1);

    return 0;
}


