#include <node.h>
#include <vector>
#include <iostream>
#include "datamatrix.h"

using namespace v8;
using namespace std;

template<class T>
std::vector<T> getArray(const Local<Object> &obj, const std::string &name) {
	std::vector<T> vect;
	Local<Value> val = obj->Get(String::New(name.c_str()));
	Local<Array> array = Local<Array>::Cast(val);
	for (unsigned int i=0; i<array->Length(); ++i) {
		Local<Number> n = Local<Number>::Cast(array->Get(i));
		vect.push_back(n->Value());
	}
	return vect;
}

template<class T>
T getNumber(const Local<Object> &obj, const std::string &name) {
	Local<Value> val = obj->Get(String::New(name.c_str()));
	T number = Local<Number>::Cast(val)->NumberValue();
	return number;
}


std::string getString(const Local<Object> &obj, const std::string &name) {
	Local<Value> val = obj->Get(String::New(name.c_str()));
	String::Utf8Value str(val->ToString());
	return string(*str);
}

Handle<Value> CreateDm(const Arguments &args) {
	HandleScope scope;
	DataMatrix dm;

	if (args[0]->IsObject()) {
            Local<Object> obj = Local<Object>::Cast(args[0]);
            if (obj->Has(String::New("data"))) {
                string data = getString(obj, "data");
                dm.setData(data);
            }
	}

        dm_data data;
	bool done = dm.generate(data);
	Local<Object> result = Object::New();
	result->Set(String::New("success"), Boolean::New(done));
        if (done) {
            result->Set(String::New("width"), Number::New(data.width));
            result->Set(String::New("height"), Number::New(data.height));
            result->Set(String::New("channels"), Number::New(data.channels));
            int nPixels = (int)data.pixels.size();
            Handle<Array> pixels = Array::New(nPixels);
            for (int i=0; i<nPixels; ++i) {
                pixels->Set(i,Integer::New(data.pixels[i]));
            }
            result->Set(String::New("pixels"), pixels);
        }
	return scope.Close(result);
}

Handle<Value> DecodeDm(const Arguments &args) {
    HandleScope scope;
    DataMatrix dm;

    bool done=false;
    string decodedText;

    if (args[0]->IsObject()) {
        Local<Object> obj = Local<Object>::Cast(args[0]);
        if (obj->Has(String::New("path"))) {
            string path = getString(obj, "path");
            done = dm.decode(path, decodedText);
        }
    }

    Local<Object> result = Object::New();
    result->Set(String::New("success"), Boolean::New(done));
    result->Set(String::New("text"), String::New(decodedText.c_str()));
    return scope.Close(result);
}

void Init(Handle<Object> exports) {
	exports->Set(String::NewSymbol("generateDm"), FunctionTemplate::New(CreateDm)->GetFunction());
        exports->Set(String::NewSymbol("decodeDm"), FunctionTemplate::New(DecodeDm)->GetFunction());
}

NODE_MODULE(dmCreator, Init)
