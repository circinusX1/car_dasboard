/*
 * Copyright 2009-2010 Cybozu Labs, Inc.
 * Copyright 2011-2014 Kazuho Oku
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifdef QT3_SUPPORT

#ifndef AKA_J
#define AKA_J

#   include <QDebug>
#else
#   include <iostream>
#endif
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <fstream>
#define OOO qDebug()


class AkaJson
{
public:

    class Aka
    {
    public:
        friend class AkaJson;
        enum E_TYPE{eNODE,eLEAF};
        Aka(Aka::E_TYPE t, const std::string& name):_name(name),_type(t){
            _parent=nullptr;
        }
        Aka(Aka::E_TYPE t):_type(t){
            _parent=nullptr;
        }
        ~Aka(){
            for(const auto& a : _values)
                delete a;
        }
        void name_it(std::string& name){
            _name = name;
            name.clear();
        }
        Aka* store_it(std::string& name){
            Aka* paka = new Aka(Aka::eLEAF, name);
            paka->_parent = this;
            _type = eNODE;
            this->_values.push_back(paka);
            name.clear();
            return paka;
        }

        const Aka& operator[](const char* key)const
        {
            if(key==_name)
                return *this;
            for(const auto& a : _values)
            {
                if(a->_name==key)
                    return *a;
            }
            static Aka Dummy(Aka::eLEAF);
            return Dummy;
        }

        const Aka* parent(){
            return _parent;
        }

        const std::vector<Aka*>& values(){
            return _values;
        }

        const std::string value(int index=0)const{
            static std::string empty="";
            if(_values.size()>=index)
                return _values[index]->_name;
            return empty;
        }

        size_t count()const{
            return _values.size();
        }

    private:
        std::string                 _name;
        std::vector<Aka*>           _values;
        Aka*                        _parent;
        Aka::E_TYPE                 _type;
    };

public:
    AkaJson(const char* fname):_pnode(nullptr)
    {
        _parse(fname);
    }
    // looup todo
    const AkaJson::Aka& operator[](const char* key)const
    {
        return _pnode->operator[](key);
    }

private:
    void _parse(const char* fname)
    {
        char p = 0;
        Aka* paka = nullptr;
        Aka* parent = nullptr;
        std::ifstream fi(fname);
        if(fi.is_open())
        {
            while (std::getline(fi, _line))
            {
                char prev = 0;
                std::istringstream iss(_line);
                if(_line.empty() || _line[0]=='#')
                    continue;
                for(const auto f : _line)
                {
                    switch(f)
                    {
                    case '\t':
                    case ' ':
                        break;
                    case '{':
                        parent = paka;
                        paka = _new();
                        paka->_name=_string;
                        paka->_parent = parent;
                        _string.clear();
                        if(paka!=parent && parent)
                            parent->_values.push_back(paka);
                        break;
                    case '}':
                        if(!_string.empty())
                        {
                            paka->store_it(_string);
                        }
                        paka = paka->_parent;
                        break;
                    case ',':
                        paka->store_it(_string);
                        break;
                    default:
                        _string+=f;
                        break;
                    }
                }
            }
        }

        print(_pnode, 0);


    }

    Aka* _new()
    {
        if(_pnode==nullptr)
        {
            _pnode=new Aka(Aka::eNODE);
            _pnode->_parent = _pnode;
            return _pnode;
        }
        return new Aka(Aka::eLEAF);
    }


    void _wipe()
    {
        OOO << _string.c_str() << "\n";
        _string.clear();
    }

    void print(const Aka* p, int depth)
    {
        ++depth;
        //OOO << "\n";
        for(int i=0;i<depth;i++) OOO<<" ";

        OOO  << p->_name.c_str() << "\n";
        if(p->_values.size())
        {
            //OOO << "\n";
            for(int i=0;i<depth;i++) OOO<<" ";
            OOO << "{\n";
            for(const auto& a : p->_values)
            {
                print(a,depth);
            }
            //OOO << "\n";
            for(int i=0;i<depth;i++) OOO<<" ";
            OOO << "}\n";
        }
        --depth;
    }

    //

private:

    std::string             _string;
    std::string             _line;
    Aka*                    _pnode = nullptr;
    bool                    _escap=false;
    bool                    _toking=false;
    bool                    _refon=false;
};

#ifndef QT3_SUPPORT

int main(void)
{
    AkaJson aj("./test.comar");
    return 1;
}

#else

#   endif //

#endif



