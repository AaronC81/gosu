#include <Gosu/Platform.hpp>
#if defined(GOSU_IS_EMSCRIPTEN)

#include "TrueTypeFont.hpp"
#include <Gosu/Text.hpp>
#include <iostream>

const unsigned char* Gosu::ttf_data_by_name(const std::string& font_name, unsigned font_flags)
{
    std::cout << "Tried to load a font using TTFNull" << std::endl;
    return NULL;
}

static const unsigned char* ttf_data_of_default_sans_serif_font()
{
    std::cout << "Tried to load default sans serif font using TTFNull" << std::endl;
    return NULL;
}

const unsigned char* Gosu::ttf_fallback_data()
{
    std::cout << "Tried to load fallback data using TTFNull" << std::endl;
    return NULL;
}

std::string Gosu::default_font_name()
{
    std::cout << "Tried to get default font using TTFNull" << std::endl;
    return "Arial";
}

#endif
