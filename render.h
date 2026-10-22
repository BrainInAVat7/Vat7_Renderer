/* Header file defining render commands */

#ifndef vat7_RENDER_COMMANDS_H
#define vat7_RENDER_COMMANDS_H


#include <stddef.h>
#include <stdint.h>


typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
} vat7_Color;


typedef struct
{
	vat7_Color color;
	vat7_Color bg_color;
	float x;
	float y;
	char *text;
	uint8_t text_size;
	uint8_t type;  // e.g., rect, bitmap, text, etc.

} vat7_RenderCommand;


typedef struct
{
	vat7_Color bg_color;
	size_t count;
	vat7_RenderCommand commands[4096];  // TODO Revisit this hard coded size
} vat7_RenderCommandBuffer;


/* RenderCommand type constants */
// NOTE: 0 reserved in case I want error signaling or something
static const uint8_t vat7_RENDER_TYPE_DEBUG_TEXT = 1u;
static const uint8_t vat7_RENDER_TYPE_PIXEL	= 2u;
static const uint8_t vat7_RENDER_TYPE_LINE	= 3u;
static const uint8_t vat7_RENDER_TYPE_RECT       = 4u;

#endif  // header guard
