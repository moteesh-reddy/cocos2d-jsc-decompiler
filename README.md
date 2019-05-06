# SpiderMonkey VM

[SpiderMonkey](https://developer.mozilla.org/en-US/docs/SpiderMonkey) is [Mozilla](http://www.mozilla.org)'s JavaScript engine.

## About this repository

 - This is NOT the official SpiderMonkey standalone project. Official repo is [here](https://developer.mozilla.org/en-US/docs/SpiderMonkey)
 - Only contains the SpiderMonkey source code and needed files to compile it
 - Contains a few patches to make it compile on iOS (device and simulator)
 - Contains build scripts for iOS, Android, Win32 and OS X
 - Add code for decoding jsc file

## About build jsc decompiler
 - cd js/src/
 - autoconf-2.13
 - cd build-linux
 - ../configure --enable-debug --disable-optimize
 - make
 More info:https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey 
 The program decjsc will been created in dist/bin.

## About run jsc decoder
 - ./decjsc -e jsfile     encode jsfile to jscfile named jsfile.jsc
 - ./decjsc -d jscfile    decode jscfile to assemblefile named jscfile.jsdec

## About builds

### iOS
 - JIT is disabled
 - Device only: compiled in RELEASE mode
 - Simulator only: compiled in DEBUG mode

### Android

 - JIT is enabled
 - compiled in RELEASE mode
 

### OS X

 - JIT is enabled
 - compiled in DEBUG mode
 

### Windows

 - JIT is enabled
 - compiled in RELEASE mode

### Linux

 - JIT is enabled
 - compiled in RELEASE mode

## About the patches
 
 - Patches for [SpiderMonkey](https://github.com/ricardoquesada/Spidermonkey/wiki/)
 


 
