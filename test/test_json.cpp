#include <fstream>
#include <iostream>
#include "person.pb.h"
//#include "../pb2json.h"
#include <pb2json.h>

using namespace std;

int main(int argc,char *argv[])
{
 	// Test 1: read binary PB from a file and convert it to JSON
	ifstream fin("dump",ios::binary);
	fin.seekg(0,ios_base::end);
	size_t len = fin.tellg();
	fin.seekg(0,ios_base::beg);
	char *buf = new char [len];

	fin.read(buf,len);
	Message *p = new Person();
        
	string json = pb2json(p,buf,len);
	cout<<json<<endl;
	delete p;

	// Test 2: convert PB to JSON directly
	Person p2;
	string json2 = pb2json(&p2);
	cout<<json2<<endl;
	
	return 0;
}
