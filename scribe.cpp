
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.
*********************************************************************************************/


/****************************************  IMPORTANT NOTE  **********************************

    Comments in this file that start with / * ! or / / ! are being used by Doxygen to
    document the software.  Dashes in these comment blocks are used to create bullet lists.
    The lack of blank lines after a block of dash preceeded comments means that the next
    block of dash preceeded comments is a new, indented bullet list.  I've tried to keep the
    Doxygen formatting to a minimum but there are some other items (like <br> and <pre>)
    that need to be left alone.  If you see a comment that starts with / * ! or / / ! and
    there is something that looks a bit weird it is probably due to some arcane Doxygen
    syntax.  Be very careful modifying blocks of Doxygen comments.

*****************************************  IMPORTANT NOTE  **********************************/



#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include "pfmGeotiff.hpp"
#include "shapefil.h"

#define         CONTOUR_POINTS          1000
#define         DEFAULT_SEGMENT_LENGTH  0.25


/***************************************************************************\
*                                                                           *
*   Module Name:        scribe                                              *
*                                                                           *
*   Programmer(s):      Jan C. Depner                                       *
*                                                                           *
*   Date Written:       December 1994                                       *
*                                                                           *
*   Purpose:            Get the contours from the contouring package and    *
*                       draw them.                                          *
*                                                                           *
*   Arguments:          ncc     -   number of columns in area               *
*                       nrr     -   number of rows in area                  *
*                       xorig   -   origin x (lower left)                   *
*                       yorig   -   origin y (lower left)                   *
*                       cntfile -   contour file pointer                    *
*                                                                           *
*   Return Value:       Number of contours                                  *
*                                                                           *
*   Calling Routines:   displaygrid                                         *
*                                                                           * 
\***************************************************************************/

int32_t scribe (int32_t num_cols, int32_t num_rows, float xorig, float yorig, float min_z, float max_z, float *ar,
                 QString shapeName, OPTIONS *options, PFM_OPEN_ARGS open_args)
{
  int32_t                 index_contour, num_points, i, num_interp, bytes, num_contours = 0;
  double                  *dcontour_x, *dcontour_y, *dcontour_m, ix[2], iy[2], dx, dy, cell_diag_length, segment_length;
  float                   level, half_gridx, half_gridy, *contour_x, *contour_y;
  char                    shape_name[512], prj_name[512];
  FILE                    *prj_fp;
  SHPHandle               shp_hnd;
  SHPObject               *shape;
  DBFHandle               dbf_hnd;  


  void contourMinMax (float, float);
  void contourEmphasis (int32_t);
  void contourMaxDensity (int32_t);
  void contourMaxPoints (int32_t);
  void contourLevels (int32_t, float *);
  int32_t initContour (float, int32_t, int32_t, float *);
  int32_t getContour (float *, int32_t *, int32_t *, float *, float *);
  void smooth_contour (int32_t, int32_t *, double *, double *);



  strcpy (shape_name, shapeName.toLatin1 ());


  if ((shp_hnd = SHPCreate (shape_name, SHPT_ARCM)) == NULL)
    {
      QMessageBox::warning (0, "pfmGeotiff",
                            pfmGeotiff::tr ("Unable to create ESRI SHP file %1\nThe error message returned was:\n\n%2\n\nContours will not be generated.").arg 
                            (QDir::toNativeSeparators (shapeName)).arg (strerror (errno)));

      return (0);
    }


  //  Making dummy DBF file so Arc won't barf.

  if ((dbf_hnd = DBFCreate (shape_name)) == NULL)
    {
      QMessageBox::warning (0, "pfmGeotiff",
                            pfmGeotiff::tr ("Unable to create ESRI DBF file %1\nThe error message returned was:\n\n%2\n\nContours will not be generated.").arg 
                            (QDir::toNativeSeparators (shapeName)).arg (strerror (errno)));

      return (0);
    }


  //  Adding a dummy field.

  if (DBFAddField (dbf_hnd, "nada", FTLogical, 1, 0) == -1)
    {
      QMessageBox::warning (0, "pfmGeotiff", pfmGeotiff::tr ("Error adding field to DBF file."));
      return (0);
    }


  //  Stupid freaking .prj file

  strcpy (prj_name, shapeName.replace (".pfm", ".prj").toLatin1 ());
  if ((prj_fp = fopen (prj_name, "w")) == NULL)
    {
      QMessageBox::warning (0, "pfmGeotiff",
                            pfmGeotiff::tr ("Unable to create ESRI PRJ file %1\nThe error message returned was:\n\n%2\n\nContours will not be generated.").arg 
                            (QDir::toNativeSeparators (QString (prj_name))).arg (strerror (errno)));

      return (0);
    }

  fprintf (prj_fp, "GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],TOWGS84[0,0,0,0,0,0,0],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.01745329251994328,AUTHORITY[\"EPSG\",\"9108\"]],AXIS[\"Lat\",NORTH],AXIS[\"Long\",EAST],AUTHORITY[\"EPSG\",\"4326\"]],VERT_CS[\"ellipsoid Z in meters\",VERT_DATUM[\"Ellipsoid\",2002],UNIT[\"metre\",1],AXIS[\"Z\",UP]]\n");


  //  Check the smoothing factor range and get the number of interpolation points per unsmoothed contour segment

  if (options->smoothing_factor < 0) options->smoothing_factor = 0;
  if (options->smoothing_factor > 10) options->smoothing_factor = 10;


  ix[0] = (xorig + (open_args.head.x_bin_size_degrees * num_cols) * 0.5) / open_args.head.x_bin_size_degrees;
  iy[0] = (yorig + (open_args.head.y_bin_size_degrees * num_rows) * 0.5) / open_args.head.y_bin_size_degrees;
  ix[1] = ix[0] + 1.0;
  iy[1] = iy[0] + 1.0;

  dx = ix[1] - ix[0];
  dy = iy[1] - iy[0];

  cell_diag_length = sqrt (pow (dx, 2) + pow (dy, 2));

  num_interp = 1;

  if (options->smoothing_factor)
    {
      segment_length = DEFAULT_SEGMENT_LENGTH / options->smoothing_factor;

      if (cell_diag_length > segment_length)
        {    
          num_interp = (int32_t) ((cell_diag_length / segment_length) + 0.5);
        }
      else
        {
          num_interp = 1;
        }
    }


  //  allocate memory for contour arrays

  if (options->smoothing_factor > 0)
    {
      bytes = (((num_interp * (CONTOUR_POINTS - 1)) + 1) * sizeof (double));
    }
  else
    {
      bytes = (CONTOUR_POINTS * sizeof (double));
    }

  if ((contour_x = (float *) malloc (bytes / 2)) == NULL)
    {
      perror ("Allocating contour_x in scribe.cpp");
      exit (-1);
    }
  if ((contour_y = (float *) malloc (bytes / 2)) == NULL)
    {
      perror ("Allocating contour_y in scribe.cpp");
      exit (-1);
    }
  if ((dcontour_x = (double *) malloc (bytes)) == NULL)
    {
      perror ("Allocating dcontour_x in scribe.cpp");
      exit (-1);
    }
  if ((dcontour_y = (double *) malloc (bytes)) == NULL)
    {
      perror ("Allocating dcontour_y in scribe.cpp");
      exit (-1);
    }
  if ((dcontour_m = (double *) malloc (bytes)) == NULL)
    {
      perror ("Allocating dcontour_m in scribe.cpp");
      exit (-1);
    }


  //  Set the min and max contours, the index contour interval, the maximum contour density, and the
  //  number of points to be returned by the package.

  contourMinMax (min_z, max_z);
  contourMaxDensity (options->maxd);
  contourMaxPoints (CONTOUR_POINTS);
  contourEmphasis (1);


  //  If the contour interval is set to 0.0 use the user defined levels.
  /*    
  if (options->cint == 0.0)
    {
      contourLevels (options->num_levels, options->contour_levels);
    }
  */


  //  Pass the grid array (arranged 1D) to the contouring package.

  initContour (options->cint, num_rows, num_cols, ar);


  //  Compute half of a grid cell in degrees.

  half_gridx = open_args.head.x_bin_size_degrees * 0.5;
  half_gridy = open_args.head.y_bin_size_degrees * 0.5;


  //  Get the contours from the package until all are saved.

  while (getContour (&level, &index_contour, &num_points, contour_x, contour_y))
    {
      if (num_points > 1)
        {
          //  Convert from grid points (returned contours) to position.  Contours are output as elevations, not depths.

          for (i = 0 ; i < num_points ; i++)
            {
              dcontour_x[i] = xorig + (contour_x[i] * open_args.head.x_bin_size_degrees) + half_gridx;
              dcontour_y[i] = yorig + (contour_y[i] * open_args.head.y_bin_size_degrees) + half_gridy;
            }


          //  smooth out the contour.

          if (options->smoothing_factor > 0) smooth_contour (num_interp, &num_points, dcontour_x, dcontour_y);


          for (i = 0 ; i < num_points ; i++) dcontour_m[i] = (double) level;


          shape = SHPCreateObject (SHPT_ARCM, -1, 0, NULL, NULL, num_points, dcontour_x, dcontour_y, NULL, dcontour_m);
          SHPWriteObject (shp_hnd, -1, shape);
          SHPDestroyObject (shape);

          DBFWriteLogicalAttribute (dbf_hnd, num_contours, 0, '0');

          num_contours++;
        }
    }


  free (contour_x);
  free (contour_y);
  free (dcontour_x);
  free (dcontour_y);
  free (dcontour_m);


  SHPClose (shp_hnd);
  DBFClose (dbf_hnd);  
  fclose (prj_fp);


  return (num_contours);
}
