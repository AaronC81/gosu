#include <Gosu/Font.hpp>
#include <Gosu/Graphics.hpp>
#include <Gosu/Image.hpp>
#include <Gosu/Math.hpp>
#include <Gosu/Text.hpp>
#include <Gosu/Utility.hpp>
#include "GraphicsImpl.hpp"
#include "MarkupParser.hpp"
#include <array>
#include <map>
#include <stdexcept>
#include <utf8proc.h>

static const int FONT_RENDER_SCALE = 2;

struct Gosu::Font::Impl : Gosu::Noncopyable
{
    std::string name;
    int height;
    unsigned base_flags;

    // The most common characters are stored directly in an array for maximum performance.
    // (This is the start of the Basic Multilingual Plane, up until the part where right-to-left
    // languages begin, which don't really work with Gosu yet.)
    std::array<std::array<Image, 0x58f>, FF_COMBINATIONS> fast_glyphs;
    // Everything else is looked up through a map...
    std::array<std::map<utf8proc_int32_t, Image>, FF_COMBINATIONS> other_glyphs;

    Image& image(char32_t codepoint, unsigned font_flags)
    {
        Image* image;
        if (codepoint < fast_glyphs[font_flags].size()) {
            image = &fast_glyphs[font_flags][codepoint];
        }
        else {
            image = &other_glyphs[font_flags][codepoint];
        }

        // If this codepoint has not been rendered before, do it now.
        if (image->width() == 0 && image->height() == 0) {
            auto scaled_height = height * FONT_RENDER_SCALE;

            std::u32string string(1, codepoint);
            Bitmap bitmap(scaled_height, scaled_height);
            auto required_width = ceil(Gosu::draw_text(bitmap, 0, 0, Color::WHITE, string, name,
                                                       scaled_height, font_flags));
            if (required_width > bitmap.width()) {
                // If the character was wider than high, we need to render it again.
                Bitmap(required_width, scaled_height).swap(bitmap);
                Gosu::draw_text(bitmap, 0, 0, Color::WHITE, string, name, scaled_height,
                                font_flags);
            }

            *image = Image(bitmap, 0, 0, required_width, scaled_height);
        }

        return *image;
    }
};

Gosu::Font::Font(int font_height, const std::string& font_name, unsigned font_flags)
: m_impl{new Impl}
{
    m_impl->name = font_name;
    m_impl->height = font_height;
    m_impl->base_flags = font_flags;
}

const std::string& Gosu::Font::name() const
{
    return m_impl->name;
}

int Gosu::Font::height() const
{
    return m_impl->height;
}

unsigned Gosu::Font::flags() const
{
    return m_impl->base_flags;
}

double Gosu::Font::text_width(const std::string& text) const
{
    return markup_width(escape_markup(text));
}

double Gosu::Font::markup_width(const std::string& markup) const
{
    double width = 0;

    // Split the text into lines (split_words = false) because Font doesn't implement word-wrapping.
    MarkupParser parser(m_impl->base_flags, false, [&](std::vector<FormattedString>&& line) {
        double line_width = 0;
        for (auto& part : line) {
            for (auto codepoint : part.text) {
                auto& image = m_impl->image(codepoint, part.flags);
                double image_scale = 1.0 * height() / image.height();
                line_width += image_scale * image.width();
            }
        }
        width = std::max(width, line_width);
    });
    parser.parse(markup);

    return width;
}

void Gosu::Font::draw_text(const std::string& text, double x, double y, ZPos z, //
                           double scale_x, double scale_y, Color c, BlendMode mode) const
{
    draw_markup(escape_markup(text), x, y, z, scale_x, scale_y, c, mode);
}

void Gosu::Font::draw_markup(const std::string& markup, double x, double y, ZPos z, //
                             double scale_x, double scale_y, Color c, BlendMode mode) const
{
    double current_y = y;

    // Split the text into lines (split_words = false) because Font doesn't implement word-wrapping.
    MarkupParser parser(m_impl->base_flags, false, [&](std::vector<FormattedString>&& line) {
        double current_x = x;
        for (auto& part : line) {
            for (auto codepoint : part.text) {
                auto& image = m_impl->image(codepoint, part.flags);
                double image_scale = 1.0 * height() / image.height();
                image.draw(current_x, current_y, z, image_scale * scale_x, image_scale * scale_y,
                           multiply(c, part.color), mode);
                current_x += image_scale * scale_x * image.width();
            }
        }
        current_y += scale_y * height();
    });
    parser.parse(markup);
}

void Gosu::Font::draw_text_rel(const std::string& text, double x, double y, ZPos z,        //
                               double rel_x, double rel_y, double scale_x, double scale_y, //
                               Color c, BlendMode mode) const
{
    if (rel_x) x -= text_width(text) * scale_x * rel_x;
    if (rel_y) y -= height() * scale_y * rel_y;

    draw_text(text, x, y, z, scale_x, scale_y, c, mode);
}

void Gosu::Font::draw_markup_rel(const std::string& markup, double x, double y, ZPos z, //
                                 double rel_x, double rel_y, double scale_x, double scale_y,
                                 Color c, BlendMode mode) const
{
    if (rel_x) x -= markup_width(markup) * scale_x * rel_x;
    if (rel_y) y -= height() * scale_y * rel_y;

    draw_markup(markup, x, y, z, scale_x, scale_y, c, mode);
}

void Gosu::Font::set_image(std::string codepoint, unsigned font_flags, const Gosu::Image& image)
{
    auto utc4 = utf8_to_composed_utc4(codepoint);
    if (utc4.length() != 1) {
        throw std::invalid_argument{"Could not compose '" + codepoint + "' into single codepoint"};
    }

    if (utc4[0] < m_impl->fast_glyphs[font_flags].size()) {
        m_impl->fast_glyphs[font_flags][utc4[0]] = image;
    }
    else {
        m_impl->other_glyphs[font_flags][utc4[0]] = image;
    }
}

void Gosu::Font::set_image(std::string codepoint, const Gosu::Image& image)
{
    for (unsigned font_flags = 0; font_flags < FF_COMBINATIONS; ++font_flags) {
        set_image(codepoint, font_flags, image);
    }
}
