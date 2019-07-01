#include "delaunay.hpp"

#include "../cxxopts.hpp"
#include "../image/image.hpp"
#include "../util/util.hpp"

#define REAL double
#define VOID void

#include <cstdio>
#include <cstdlib>
#include "triangle.h"

void report(struct triangulateio* io, int markers, int reporttriangles,
            int reportneighbors, int reportsegments, int reportedges,
            int reportnorms) {
  int i, j;

  for (i = 0; i < io->numberofpoints; i++) {
    printf("Point %4d:", i);
    for (j = 0; j < 2; j++) {
      printf("  %.6g", io->pointlist[i * 2 + j]);
    }
    if (io->numberofpointattributes > 0) {
      printf("   attributes");
    }
    for (j = 0; j < io->numberofpointattributes; j++) {
      printf("  %.6g",
             io->pointattributelist[i * io->numberofpointattributes + j]);
    }
    if (markers) {
      printf("   marker %d\n", io->pointmarkerlist[i]);
    } else {
      printf("\n");
    }
  }
  printf("\n");

  if (reporttriangles || reportneighbors) {
    for (i = 0; i < io->numberoftriangles; i++) {
      if (reporttriangles) {
        printf("Triangle %4d points:", i);
        for (j = 0; j < io->numberofcorners; j++) {
          printf("  %4d", io->trianglelist[i * io->numberofcorners + j]);
        }
        if (io->numberoftriangleattributes > 0) {
          printf("   attributes");
        }
        for (j = 0; j < io->numberoftriangleattributes; j++) {
          printf("  %.6g",
                 io->triangleattributelist[i * io->numberoftriangleattributes +
                                           j]);
        }
        printf("\n");
      }
      if (reportneighbors) {
        printf("Triangle %4d neighbors:", i);
        for (j = 0; j < 3; j++) {
          printf("  %4d", io->neighborlist[i * 3 + j]);
        }
        printf("\n");
      }
    }
    printf("\n");
  }

  if (reportsegments) {
    for (i = 0; i < io->numberofsegments; i++) {
      printf("Segment %4d points:", i);
      for (j = 0; j < 2; j++) {
        printf("  %4d", io->segmentlist[i * 2 + j]);
      }
      if (markers) {
        printf("   marker %d\n", io->segmentmarkerlist[i]);
      } else {
        printf("\n");
      }
    }
    printf("\n");
  }

  if (reportedges) {
    for (i = 0; i < io->numberofedges; i++) {
      printf("Edge %4d points:", i);
      for (j = 0; j < 2; j++) {
        printf("  %4d", io->edgelist[i * 2 + j]);
      }
      if (reportnorms && (io->edgelist[i * 2 + 1] == -1)) {
        for (j = 0; j < 2; j++) {
          printf("  %.6g", io->normlist[i * 2 + j]);
        }
      }
      if (markers) {
        printf("   marker %d\n", io->edgemarkerlist[i]);
      } else {
        printf("\n");
      }
    }
    printf("\n");
  }
}

void delaunay::main(cxxopts::ParseResult args) {
  Image img(args["resolution"].as<unsigned>(),
            get_resolution(args["resolution"].as<unsigned>(),
                           args["aspect"].as<std::string>()));

  img.fill(0xffffff);
  struct triangulateio in, mid, out;
  in.numberofpoints = 4;
  in.numberofpointattributes = 1;
  in.pointlist = (REAL*)malloc(in.numberofpoints * 2 * sizeof(REAL));
  in.pointlist[0] = 0.0;
  in.pointlist[1] = 0.0;
  in.pointlist[2] = 1.0;
  in.pointlist[3] = 0.0;
  in.pointlist[4] = 1.0;
  in.pointlist[5] = 10.0;
  in.pointlist[6] = 0.0;
  in.pointlist[7] = 10.0;
  in.pointattributelist = (REAL*)malloc(
      in.numberofpoints * in.numberofpointattributes * sizeof(REAL));
  in.pointattributelist[0] = 0.0;
  in.pointattributelist[1] = 1.0;
  in.pointattributelist[2] = 11.0;
  in.pointattributelist[3] = 10.0;
  in.pointmarkerlist = (int*)malloc(in.numberofpoints * sizeof(int));
  in.pointmarkerlist[0] = 0;
  in.pointmarkerlist[1] = 2;
  in.pointmarkerlist[2] = 0;
  in.pointmarkerlist[3] = 0;

  in.numberofsegments = 0;
  in.numberofholes = 0;
  in.numberofregions = 1;
  in.regionlist = (REAL*)malloc(in.numberofregions * 4 * sizeof(REAL));
  in.regionlist[0] = 0.5;
  in.regionlist[1] = 5.0;
  in.regionlist[2] = 7.0; /* Regional attribute (for whole mesh). */
  in.regionlist[3] = 0.1; /* Area constraint that will not be used. */

  printf("Input point set:\n\n");
  report(&in, 1, 0, 0, 0, 0, 0);

  /* Make necessary initializations so that Triangle can return a */
  /*   triangulation in `mid' and a voronoi diagram in `vorout'.  */

  mid.pointlist = (REAL*)NULL; /* Not needed if -N switch used. */
  /* Not needed if -N switch used or number of point attributes is zero: */
  mid.pointattributelist = (REAL*)NULL;
  mid.pointmarkerlist = (int*)NULL; /* Not needed if -N or -B switch used. */
  mid.trianglelist = (int*)NULL;    /* Not needed if -E switch used. */
  /* Not needed if -E switch used or number of triangle attributes is zero: */
  mid.triangleattributelist = (REAL*)NULL;
  mid.neighborlist = (int*)NULL; /* Needed only if -n switch used. */
  /* Needed only if segments are output (-p or -c) and -P not used: */
  mid.segmentlist = (int*)NULL;
  /* Needed only if segments are output (-p or -c) and -P and -B not used: */
  mid.segmentmarkerlist = (int*)NULL;
  mid.edgelist = (int*)NULL;       /* Needed only if -e switch used. */
  mid.edgemarkerlist = (int*)NULL; /* Needed if -e used and -B not used. */

  /* Triangulate the points.  Switches are chosen to read and write a  */
  /*   PSLG (p), preserve the convex hull (c), number everything from  */
  /*   zero (z), assign a regional attribute to each element (A), and  */
  /*   produce an edge list (e), a Voronoi diagram (v), and a triangle */
  /*   neighbor list (n).                                              */

  triangulate("pczAen", &in, &mid, (struct triangulateio*)NULL);

  printf("Initial triangulation:\n\n");
  report(&mid, 1, 1, 1, 1, 1, 0);

  /* Attach area constraints to the triangles in preparation for */
  /*   refining the triangulation.                               */

  /* Needed only if -r and -a switches used: */
  mid.trianglearealist = (REAL*)malloc(mid.numberoftriangles * sizeof(REAL));
  mid.trianglearealist[0] = 3.0;
  mid.trianglearealist[1] = 1.0;

  /* Make necessary initializations so that Triangle can return a */
  /*   triangulation in `out'.                                    */

  out.pointlist = (REAL*)NULL; /* Not needed if -N switch used. */
  /* Not needed if -N switch used or number of attributes is zero: */
  out.pointattributelist = (REAL*)NULL;
  mid.edgelist = (int*)NULL;       /* Needed only if -e switch used. */
  mid.edgemarkerlist = (int*)NULL; /* Needed if -e used and -B not used. */
  out.trianglelist = (int*)NULL;   /* Not needed if -E switch used. */
  /* Not needed if -E switch used or number of triangle attributes is zero: */
  out.triangleattributelist = (REAL*)NULL;

  /* Refine the triangulation according to the attached */
  /*   triangle area constraints.                       */

  triangulate("prazeBP", &mid, &out, (struct triangulateio*)NULL);

  printf("Refined triangulation:\n\n");
  report(&out, 0, 1, 0, 0, 0, 0);

  for (std::size_t i = 0; i < out.numberofedges; ++i) {
    std::size_t a = out.edgelist[i * 2];
    std::size_t b = out.edgelist[i * 2 + 1];
    double x1, y1, x2, y2;
    x1 = out.pointlist[a * 2] * 10;
    y1 = out.pointlist[a * 2 + 1] * 10;
    x2 = out.pointlist[b * 2] * 10;
    y2 = out.pointlist[b * 2 + 1] * 10;
    img.line(x1, y1, x2, y2, 0x000000);
  }

  /* Free all allocated arrays, including those allocated by Triangle. */

  free(in.pointlist);
  free(in.pointattributelist);
  free(in.pointmarkerlist);
  free(in.regionlist);
  free(mid.pointlist);
  free(mid.pointattributelist);
  free(mid.pointmarkerlist);
  free(mid.trianglelist);
  free(mid.triangleattributelist);
  free(mid.trianglearealist);
  free(mid.neighborlist);
  free(mid.segmentlist);
  free(mid.segmentmarkerlist);
  free(mid.edgelist);
  free(mid.edgemarkerlist);
  free(out.pointlist);
  free(out.pointattributelist);
  free(out.trianglelist);
  free(out.triangleattributelist);

  img.save(args["output"].as<std::string>() + '.' +
           args["extension"].as<std::string>());
}
