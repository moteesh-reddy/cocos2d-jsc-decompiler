/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

//#include "decjsc.h"
#include "jsapi.h"
#include "jsfriendapi.h"

#include "jsscript.h"
#include "jsstr.h"

#include "jsapi-tests/tests.h"

#include "jsscriptinlines.h"

using namespace JS;

/* The class of the global object. */
const JSClass global_class = {
    "global", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub,  JS_DeletePropertyStub, JS_PropertyStub,  JS_StrictPropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub,
    nullptr, nullptr, nullptr, nullptr,
    JS_GlobalObjectTraceHook
};

// Just a wrapper around JSPrincipals that allows static construction.
/*class TestJSPrincipals : public JSPrincipals
{
  public:
    explicit TestJSPrincipals(int rc = 0)
      : JSPrincipals()
    {
        refcount = rc;
    }
};
*/
template<typename T>
static inline T *
checkPtr(T *ptr)
{
  if (! ptr)
    abort();
  return ptr;
}

static void
checkBool(bool success)
{
  if (! success)
    abort();
}

/* The error reporter callback. */
void reportError(JSContext *cx, const char *message, JSErrorReport *report)
{
    fprintf(stderr, "%s:%u: %s\n",
            report->filename ? report->filename : "<no filename>",
            (unsigned int) report->lineno,
            message);
}



char *load_string_from_file(char* filename,int *datalen)
{
		 if(filename ==NULL or datalen==NULL)
		 {
		 		return nullptr;		 		
		 }
		 
     FILE* f = fopen(filename, "rb");
     if(f==NULL)
     {
     	*datalen = 0;
     	return nullptr;   	
     }

     fseek(f, 0, SEEK_END);
     int len = ftell(f);
     fseek(f, 0, SEEK_SET);
     char* data = (char *)malloc(len);
     fread(data, 1, len, f);
     fclose(f);
     *datalen = len;
    return data;
}

bool write_jsc_file(char* filename, char *filedata, int datalen)
{
		 if(filename ==NULL or filedata==NULL or datalen==0)
		 {
		 		return false;		 		
		 }
		 
     FILE* f = fopen(filename, "wb");
     if(f==NULL)
     	return false;
     fwrite(filedata, 1, datalen, f);
     fclose(f);
     return true;
}

static TestJSPrincipals testPrincipal1(1);

static JSScript *
CompileScriptForPrincipalsVersionOrigin(JSContext *cx, JS::HandleObject obj,
                                        JSPrincipals *originPrincipals,
                                        const char *bytes, size_t nbytes,
                                        const char *filename, unsigned lineno,
                                        JSVersion version)
{
    size_t nchars;
    if (!JS_DecodeBytes(cx, bytes, nbytes, nullptr, &nchars))
        return nullptr;
    jschar *chars = static_cast<jschar *>(JS_malloc(cx, nchars * sizeof(jschar)));
    if (!chars)
        return nullptr;
    JS_ALWAYS_TRUE(JS_DecodeBytes(cx, bytes, nbytes, chars, &nchars));
    JS::CompileOptions options(cx);
    options.setOriginPrincipals(originPrincipals)
           .setFileAndLine(filename, lineno)
           .setVersion(version);
    JS::RootedScript script(cx);
    JS::Compile(cx, obj, options, chars, nchars, &script);
    free(chars);
    return script;
}


bool encode_js(JSContext *cx, char *js_file_path)
{
	 uint32_t datalen = 0;
	 char *bytecode_data = load_string_from_file(js_file_path,(int *)&datalen);
	 
	 if(bytecode_data==NULL||datalen==0)
	 	return false;
	 
	 JS::RootedObject global(cx, JS::CurrentGlobalOrNull(cx));
	 JS::RootedScript script(cx, CompileScriptForPrincipalsVersionOrigin(cx, global, &testPrincipal1,
                                                                        bytecode_data,datalen, "test", 1,
                                                                        JSVERSION_DEFAULT));
	 
	 	uint32_t nbytes;
    void *memory = JS_EncodeScript(cx, script, &nbytes);
    if (!memory)
        return false;
    char outfilename[255]={0};
    sprintf(outfilename,"%s.jsc",js_file_path);
	 	write_jsc_file(outfilename, (char *)memory, nbytes);
	 
	 free(bytecode_data);
	 free(memory);	
	 	return true;
	
}

bool decode_jsc(JSContext *cx,char *jsc_file_path) {

	 uint32_t datalen = 0;
	 void *bytecode_data = load_string_from_file(jsc_file_path,(int *)&datalen);

   if (bytecode_data!=NULL)
   {
                                                                        
     JS::RootedScript *script = new JS::RootedScript(cx);
     *script = JS_DecodeScript(cx, bytecode_data, datalen,nullptr);
     char outfilename[255]={0};
     sprintf(outfilename,"%s.jsdec",jsc_file_path);
	   if(js_DumpJSC(cx,*script,outfilename)) //js_DumpJSCFile(cx,*script,outfilename)
		 {
	  	   printf( "run ok.");
		 }
   }
   
   return true;
}

int
main (int argc, const char **argv)
{
	//decjsc -d filename
	//decjsc -e filename
	
    char *filename=NULL;//"jsb.jsc";
    int flag = 0;
    
    if(argc!=3)
    {
    	printf("useage: -d/e filename\n");
    	return 0;
    }
    
    if(strcmp(argv[1],"-d")==0)
    {
    	flag = 0;
    
    }
    else if(strcmp(argv[1],"-e")==0)
    {
    	flag = 1;
    }
    else
    	return 0;
    	
    	filename = (char*)argv[2];
    
    if (!JS_Init()) return 1;
    JSRuntime *runtime = checkPtr(JS_NewRuntime(32*1024 * 1024));
    JS_SetGCParameter(runtime, JSGC_MAX_BYTES, 0xffffffff);
    JS_SetNativeStackQuota(runtime, 5000000);

    JSContext *cx = checkPtr(JS_NewContext(runtime, 32*1024));
    JS_SetErrorReporter(cx, reportError);

    JSAutoRequest ar(cx);

    /* Create the global object. */
    JS::CompartmentOptions options;
    options.setVersion(JSVERSION_LATEST);
    RootedObject global(cx, checkPtr(JS_NewGlobalObject(cx, &global_class,
                        nullptr, JS::FireOnNewGlobalHook, options)));
    js::SetDefaultObjectForContext(cx, global);

    JSAutoCompartment ac(cx, global);

    /* Populate the global object with the standard globals,
       like Object and Array. */
    checkBool(JS_InitStandardClasses(cx, global));
    
    if(flag == 0)	
    	decode_jsc(cx, filename);
    else
    	encode_js(cx,filename);
    return 0;
}
