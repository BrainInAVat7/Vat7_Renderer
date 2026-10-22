/* The software renderer for the engine */

#include "engine_libc/eng_stdbool.h"  //#include <stdbool.h>
#include "platform/render_target.h"
#include "engine_core/eng_render.h"
#include "util/debug_text.h"


static void set_pixel (uint8_t *pixel, ENG_Color color)
{
	pixel[0] = color.blue;
	pixel[1] = color.green;
	pixel[2] = color.red;
	pixel[3] = color.alpha;
}


static void render_debug_glyph (ENG_RenderTarget rt, char ascii,
	size_t x, size_t y, size_t scale, ENG_Color color, ENG_Color bg_color)
{
	(void)color;
	(void)bg_color;
	const uint8_t *glyph;
	if (!(ascii >= 32 && ascii <= 126))
	{
		glyph = DEBUG_NULL;
	}
	else
	{
		glyph = DEBUG_CHARS[ascii - 32];
	}

	size_t stride = (size_t)rt.width * rt.pixel_bytes;
	size_t offset = x * rt.pixel_bytes + y * stride;
	uint8_t *pb = rt.pixel_buffer;
	for (size_t i = 0; i < 10 * scale; i++)
	{
		for (size_t j = 0; j < 8 * scale; j++)
		{
			int set = ((1 << (7 - j / scale))
				& glyph[i / scale]);
			uint8_t *pixel = pb + offset
				+ j * rt.pixel_bytes
				+ i * stride;
			for (size_t k = 0; k < scale; k++)
			{
				if (set) set_pixel(pixel, color);
				else set_pixel(pixel, bg_color);
			}
		}
	}
}


static void render_debug_text (ENG_RenderTarget rt, char *str, size_t x,
	size_t y, size_t scale, ENG_Color color, ENG_Color bg_color)
{
	size_t line = 0u;
	size_t column = 0u;
	size_t i = 0u;
	while(true)
	{
		if (str[i] == '\0') break;
		if (str[i] == '\n')
		{
			line++;
			column = 0u;
			i++;
			continue;
		}
		if (str[i] == '\t')
		{
			column += 8u;
			i++;
			continue;
		}
		render_debug_glyph(rt, str[i], x + column * 8u * scale,
			y + line * 12u * scale, scale, color, bg_color);
		column++;
		i++;
	}
}


static void fill_screen (ENG_RenderTarget rt, ENG_Color color)
{
	for (size_t i = 0u; i < rt.width * rt.height; i++)
	{
		set_pixel(rt.pixel_buffer + i * rt.pixel_bytes, color);
	}
}


void ENG_render (ENG_RenderTarget rt, ENG_RenderCommandBuffer *rcb)
{
	if (rt.pixel_buffer == NULL) return;
	fill_screen(rt, rcb->bg_color);
	if (rcb->commands[0].type == ENG_RENDER_TYPE_DEBUG_TEXT)
	{
		render_debug_text (rt, rcb->commands[0].text,
			(size_t)rcb->commands[0].x,
			(size_t)rcb->commands[0].y,
			(size_t)rcb->commands[0].text_size,
			rcb->commands[0].color,
			rcb->commands[0].bg_color);
		rcb->count = 0u;
	}
}
