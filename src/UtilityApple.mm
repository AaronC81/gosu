#include <Gosu/Utility.hpp>
#include <Gosu/Platform.hpp>
#include "AppleUtility.hpp"
#import <Foundation/Foundation.h>
#include <stdexcept>
#include <vector>
using namespace std;

#ifdef GOSU_IS_IPHONE // (but could also be used for OS X)
wstring Gosu::utf8ToWstring(const string& s)
{
    if (s.empty()) {
        return wstring();
    }
    
    NSString *str = [NSString stringWithUTF8String:s.c_str()];
    vector<wchar_t> buffer(s.size());
    NSUInteger usedBufferCount;
    if (![str getBytes:&buffer[0]
             maxLength:buffer.size() * sizeof(wchar_t)
            usedLength:&usedBufferCount
              encoding:NSUTF32LittleEndianStringEncoding
               options:0
                 range:NSMakeRange(0, str.length)
            remainingRange:NULL]) {
        throw std::runtime_error("String " + s + " could not be converted to Unicode");
    }
    return wstring(&buffer[0], &buffer[0] + usedBufferCount / sizeof(wchar_t));
}

string Gosu::wstringToUTF8(const std::wstring& ws)
{
    if (ws.empty()) {
        return string();
    }

    @autoreleasepool {
        NSString *str = [[NSString alloc] initWithBytes:ws.data()
                                                 length:ws.size() * sizeof(wchar_t)
                                               encoding:NSUTF32LittleEndianStringEncoding];
        return str.UTF8String ?: string();
    }
}

wstring Gosu::widen(const string& s)
{
    return utf8ToWstring(s);
}

string Gosu::narrow(const std::wstring& ws)
{
    return wstringToUTF8(ws);
}
#endif

string Gosu::language()
{
    @autoreleasepool {
        // Cannot use accessor syntax here without breaking compilation with OS X 10.7/Xcode 4.6.3.
        NSString *language = [[NSLocale preferredLanguages] objectAtIndex:0];
        return language.UTF8String ?: "en";
    }
}
