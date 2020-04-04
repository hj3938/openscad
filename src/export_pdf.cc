#include <cairo.h>
#include <cairo-pdf.h>
#include <math.h>

#include "export.h"
#include "polyset.h"
#include "polyset-utils.h"


void draw_geom(const Polygon2d &poly, cairo_t *cr){
    for(const auto &o : poly.outlines()){
        if (o.vertices.empty()) {
            continue;
        }
        const Eigen::Vector2d& p0 = o.vertices[0];
        cairo_move_to(cr,p0.x(),p0.y());
        for (unsigned int idx = 1;idx < o.vertices.size();idx++) {
            const Eigen::Vector2d& p = o.vertices[idx];
            cairo_line_to(cr,p.x(),p.y());
        }
        cairo_line_to(cr,p0.x(),p0.y());

    }
}

void draw_geom(const shared_ptr<const Geometry> &geom, cairo_t *cr){
    if (const auto geomlist = dynamic_pointer_cast<const GeometryList>(geom)) {
        for (const auto &item : geomlist->getChildren()) {
            draw_geom(item.second, cr);
        }
    }
    else if (dynamic_pointer_cast<const PolySet>(geom)) {
        assert(false && "Unsupported file format");
    }
    else if (const auto poly = dynamic_pointer_cast<const Polygon2d>(geom)) {
        draw_geom(*poly, cr);
    } else {
        assert(false && "Export as PDF for this geometry type is not supported");
    }
}

void export_pdf(const shared_ptr<const Geometry> &geom, const char *name2open){
    //A4 Size in points
    const double WPOINTS = 595;
    const double HPOINTS = 842;

    cairo_surface_t *surface = cairo_pdf_surface_create(name2open, WPOINTS, HPOINTS);
    cairo_t *cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 0.9);

    BoundingBox bbox = geom->getBoundingBox();
    int minx = (int)floor(bbox.min().x());
    int maxy = (int)floor(bbox.max().y());
    int maxx = (int)ceil(bbox.max().x());
    int miny = (int)ceil(bbox.min().y());
    //int width = maxx - minx;
    //int height = maxy - miny;

    if(minx>=0 && miny>=0 && maxx>=0 && maxy>=0){
        cairo_translate(cr, 10., HPOINTS-10.);
    }else {
        cairo_translate(cr, WPOINTS/2, HPOINTS/2);
    }
    cairo_rotate(cr,270.*(M_PI/180.));

    draw_geom(geom,cr);

    cairo_stroke(cr);
    cairo_show_page(cr);
    cairo_surface_destroy(surface);
    cairo_destroy(cr);

}
