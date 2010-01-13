#include <cairo/cairo.h>

#include "pes.h"

#define X(stitch) ((double)((stitch)->x - pes->min_x) * outw / width)
#define Y(stitch) ((double)((stitch)->y - pes->min_y) * outh / height)

void output_cairo(struct pes *pes)
{
	int width  = pes->max_x - pes->min_x + 1;
	int height = pes->max_y - pes->min_y + 1;
	int outw = 256, outh = 256;
	struct pes_block *block;
	cairo_surface_t *surface;
	cairo_t *cr;

	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, outw, outh);
	cr = cairo_create (surface);

	block = pes->blocks;
	while (block) {
		struct color *c = block->color;
		struct stitch *stitch = block->stitch;
		int i;

		cairo_set_source_rgb(cr, c->r / 255.0, c->g / 255.0, c->b / 255.0);
		cairo_move_to(cr, X(stitch), Y(stitch));

		for (i = 1; i < block->nr_stitches; i++) {
			++stitch;
			cairo_line_to(cr, X(stitch), Y(stitch));
		}
		cairo_set_line_width (cr, 0.2);
		cairo_stroke(cr);

		block = block->next;
	}
	cairo_surface_write_to_png(surface, "out.png");
}