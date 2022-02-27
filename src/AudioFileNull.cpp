#include <Gosu/Platform.hpp>

#if defined(GOSU_IS_EMSCRIPTEN)

#include "AudioFile.hpp"
#include "AudioImpl.hpp"
#include <vector>

struct Gosu::AudioFile::Impl : Gosu::Noncopyable {};

Gosu::AudioFile::AudioFile(const std::string& filename)
: pimpl(new Impl) {}

Gosu::AudioFile::AudioFile(Reader reader)
: pimpl(new Impl) {}

Gosu::AudioFile::~AudioFile() {}

ALenum Gosu::AudioFile::format() const
{
    return AL_FORMAT_MONO8;
}

ALuint Gosu::AudioFile::sample_rate() const
{
    return 1;
}

void Gosu::AudioFile::rewind() {}

std::size_t Gosu::AudioFile::read_data(void* dest, size_t length)
{
    return 0;
}

const std::vector<char>& Gosu::AudioFile::decoded_data()
{
   return std::vector<char>();
}

#endif
