/**
 * Copyright (c) 2011, Tim Branyen @tbranyen <tim@tabdeveloper.com>
 * @author Michael Robinson @codeofinterest <mike@pagesofinterest.net>
 *
 * Dual licensed under the MIT and GPL licenses.
 */

#ifndef REF_H
#define REF_H

#include <v8.h>
#include <node.h>
#include <string>

#include "../vendor/libgit2/include/git2.h"

#include "repo.h"
#include "oid.h"

using namespace node;
using namespace v8;

class GitReference : public ObjectWrap {
  public:
    static Persistent<Function> constructor_template;
    static void Initialize(Handle<v8::Object> target);
    git_reference* GetValue();
    void SetValue(git_reference* ref);
    int Lookup(git_repository* repo, const char* name);
    const git_oid* Oid();

  protected:
    GitReference() {}
    ~GitReference() {}
    static Handle<Value> New(const Arguments& args);

    static Handle<Value> Lookup(const Arguments& args);
    static void EIO_Lookup(uv_work_t* req);
    static void EIO_AfterLookup(uv_work_t* req);

    static Handle<Value> Oid(const Arguments& args);

  private:
    git_reference *ref;

    struct lookup_request {
      GitReference* ref;
      GitRepo* repo;
      int err;
      std::string name;
      Persistent<Function> callback;
    };
};

#endif
