#include <node.h>
#include <vector>
#include <iostream>
#include <nan.h>
#include "datamatrix.h"

using namespace std;

template<class T>
std::vector<T> getArray(const v8::Local<v8::Object> &obj, const std::string &name) {
    std::vector<T> vect;
    v8::Local<v8::Value> val = obj->Get(Nan::New<v8::String>(name.c_str()).ToLocalChecked());
    v8::Local<v8::Array> array = v8::Local<v8::Array>::Cast(val);
    for (unsigned int i=0; i<array->Length(); ++i) {
        v8::Local<v8::Number> n = v8::Local<v8::Number>::Cast(array->Get(i));
        vect.push_back(n->Value());
    }
    return vect;
}

template<class T>
T getNumber(const v8::Local<v8::Object> &obj, const std::string &name) {
    v8::Local<v8::Value> val = obj->Get(Nan::New<v8::String>(name.c_str()).ToLocalChecked());
    T number = v8::Local<v8::Number>::Cast(val)->NumberValue();
    return number;
}


std::string getString(const v8::Local<v8::Object> &obj, const std::string &name) {
    v8::Local<v8::Value> val = obj->Get(Nan::New<v8::String>(name.c_str()).ToLocalChecked());
    v8::String::Utf8Value str(val->ToString());
    return string(*str);
}

void CreateDm(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    DataMatrix dm;

    if (info[0]->IsObject()) {
        v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(info[0]);
        if (obj->Has(Nan::New<v8::String>("data").ToLocalChecked())) {
            string text = getString(obj, "data");
            dm_data data;
            bool done = dm.generate(text, data);
            v8::Local<v8::Object> result = Nan::New<v8::Object>();
            result->Set(Nan::New<v8::String>("success").ToLocalChecked(), Nan::New<v8::Boolean>(done));
            if (done) {
                result->Set(Nan::New<v8::String>("width").ToLocalChecked(), Nan::New<v8::Number>(data.width));
                result->Set(Nan::New<v8::String>("height").ToLocalChecked(), Nan::New<v8::Number>(data.height));
                result->Set(Nan::New<v8::String>("channels").ToLocalChecked(), Nan::New<v8::Number>(data.channels));
                int nPixels = (int)data.pixels.size();
                v8::Handle<v8::Array> pixels = Nan::New<v8::Array>(nPixels);
                for (int i=0; i<nPixels; ++i) {
                    pixels->Set(i, Nan::New<v8::Integer>(data.pixels[i]));
                }
                result->Set(Nan::New<v8::String>("pixels").ToLocalChecked(), pixels);
            }
            info.GetReturnValue().Set(result);
        }
    }

}

void DecodeDm(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    DataMatrix dm;

    bool done=false;
    string decodedText;

    if (info[0]->IsObject()) {
        v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(info[0]);
        dm_image image;
        image.rows = obj->Get(Nan::New<v8::String>("rows").ToLocalChecked())->NumberValue();
        image.cols = obj->Get(Nan::New<v8::String>("cols").ToLocalChecked())->NumberValue();
        image.channels = obj->Get(Nan::New<v8::String>("channels").ToLocalChecked())->NumberValue();
        image.data = (unsigned char *) node::Buffer::Data(info[1]->ToObject());
        done = dm.decode(image, decodedText);
        //image.data = obj->Get(Nan::New<v8::String>("data"));
        //if (obj->Has(Nan::New<v8::String>("path").ToLocalChecked())) {
        //    string path = getString(obj, "path");
        //    done = dm.decode(path, decodedText);
        //}
    }

    v8::Local<v8::Object> result = Nan::New<v8::Object>();
    result->Set(Nan::New<v8::String>("success").ToLocalChecked(), Nan::New<v8::Boolean>(done));
    result->Set(Nan::New<v8::String>("text").ToLocalChecked(), Nan::New<v8::String>(decodedText).ToLocalChecked());
    info.GetReturnValue().Set(result);
}

void Init(v8::Handle<v8::Object> exports) {
    Nan::SetMethod(exports, "generateDm", CreateDm);
    /*
     * image:
     *  - rows
     *  - cols
     *  - channels
     * data (pixels)
     */
    Nan::SetMethod(exports, "decodeDm", DecodeDm);
}

NODE_MODULE(dmCreator, Init)
