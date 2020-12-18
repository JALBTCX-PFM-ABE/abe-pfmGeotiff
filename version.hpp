
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



#ifndef VERSION

#define     VERSION     "PFM Software - pfmGeotiff V5.41 - 09/26/17"

#endif

/*!< <pre>

    Version 1.0
    Jan C. Depner
    07/07/03

    First working version.


    Version 1.1
    Jan C. Depner
    07/23/03

    Had to switch to TOPLEFT orientation because neither CARIS nor FLEDERMAUS 
    could handle BOTLEFT.


    Version 1.2
    Jan C. Depner
    08/19/03

    Went to PIXELSCALE in addition to the geotie points since Fledermaus, etc
    can't handle real geotiepoints.  Also fixed bug in TOPLEFT implementation.


    Version 1.3
    Jan C. Depner
    08/27/03

    Added alpha channel transparency option.


    Version 2.0
    Jan C. Depner
    12/11/03

    Now able to output multiple geotiffs representing coverage for each of the
    different data types.


    Version 3.0
    Jan C. Depner
    03/02/04

    Qt Wizard version.  Also, fixed a bug that caused the minimum value to 
    always be 0.0.  More importantly, I get my new Gibson ES-135 today :D


    Version 3.13
    Jan C. Depner
    08/13/04

    Changes to deal with data topo and hydro data in the same set.  Turnover is at
    zero.  This is the Friday the 13th version ;-)


    Version 3.4
    Jan C. Depner
    09/07/04

    Now reads .ARE or .are file to only generate subset GeoTIFF.


    Version 3.51
    Jan C. Depner
    02/14/05

    Fixed a "stupid" when not building using an area file... Doh!  Happy Valentine's
    Day!


    Version 3.52
    Jan C. Depner
    02/25/05

    Added slot for modifying the output file name manually.  Switched to 
    open_existing_pfm_file from open_pfm_file.


    Version 3.53
    Jan C. Depner
    08/18/05

    Added restart colormap button so that you can optionally make the colors continuous across the
    zero boundary.


    Version 3.54
    Jan C. Depner
    10/26/05

    Now uses the PFM 4.6 handle file or list file instead of the PFM directory name.


    Version 3.6
    Jan C. Depner
    02/03/06

    Added envin and envout.  Check for null value in min bin.


    Version 3.61
    Jan C. Depner
    03/29/06

    Replaced QVox, QHBox with QVBoxLayout, QHBoxLayout to prepare for Qt 4.


    Version 3.62
    Jan C. Depner
    06/05/06

    Removed get_area_mbr.cpp.  Moved to utility.


    Version 3.7
    Jan C. Depner
    06/13/06

    Name change from pfm2geotiff to pfmGeotiff to match naming convention for PFM_ABE
    Qt programs.


    Version 3.8
    Jan C. Depner
    06/28/07

    Fixed transparency and projection data.


    Version 4.0
    Jan C. Depner
    07/02/07

    Ported to Qt4.


    Version 4.1
    Jan C. Depner
    07/04/07

    Added live update of sample data set when changing color and
    sunshading parameters.  Happy Birthday America!


    Version 4.11
    Jan C. Depner
    07/27/07

    Added background color on start and end color labels in the image parameters page.


    Version 4.12
    Jan C. Depner
    08/24/07

    Switched from setGeometry to resize and move.  See Qt4 X11 window geometry documentation.


    Version 5.00
    Jan C. Depner
    10/03/07

    Switched to GDAL instead of TIFF/GEOTIFF/JPEG.


    Version 5.01
    Jan C. Depner
    12/22/07

    Handle command line PFM file.


    Version 5.02
    Jan C. Depner
    04/07/08

    Replaced single .h and .hpp files from utility library with include of nvutility.h and nvutility.hpp


    Version 5.03
    Jan C. Depner
    04/30/08

    Added QFile->close () after reading the sample data.


    Version 5.04
    Jan C. Depner
    01/29/09

    Set checkpoint to 0 prior to calling open_existing_pfm_file.


    Version 5.05
    Jan C. Depner
    05/21/09

    Set all QFileDialogs to use List mode instead of Detail mode.


    Version 5.10
    Jan C. Depner
    06/02/09

    Ignores pfmView generated GSF files (pfmView_GSF) for coverage output.


    Version 5.20
    Jan C. Depner
    11/03/09

    Added option to output 32 bit floating point GeoTIFF instead of color coded, sunshaded.


    Version 5.21
    Jan C. Depner
    08/27/10

    Set the proper NoData value for greyscale.


    Version 5.22
    Jan C. Depner
    01/06/11

    Correct problem with the way I was instantiating the main widget.  This caused an intermittent error on Windows7/XP.


    Version 5.23
    Jan C. Depner
    06/27/11

    Save all directories used by the QFileDialogs.  Add current working directory to the sidebar for all QFileDialogs.


    Version 5.24
    Jan C. Depner
    07/22/11

    Using setSidebarUrls function from nvutility to make sure that current working directory (.) and
    last used directory are in the sidebar URL list of QFileDialogs.


    Version 5.25
    Jan C. Depner
    09/27/11

    Added NAVD88 height option.


    Version 5.26
    Jan C. Depner
    11/03/11

    Added option to include or exclude interpolated contours.  Added option to set
    smoothing level.  Fixed bug in contour generation.  Outputs empty ESRI DBF shape file
    for contours so that Arc can handle the files nicely.


    Version 5.27
    Jan C. Depner
    11/30/11

    Converted .xpm icons to .png icons.


    Version 5.28
    Jan C. Depner (PFM Software)
    12/09/13

    Switched to using .ini file in $HOME (Linux) or $USERPROFILE (Windows) in the ABE.config directory.  Now
    the applications qsettings will not end up in unknown places like ~/.config/navo.navy.mil/blah_blah_blah on
    Linux or, in the registry (shudder) on Windows.


    Version 5.29
    Jan C. Depner (PFM Software)
    01/07/14

    Replaced get_geoid03 with get_geoid12a.


    Version 5.30
    Jan C. Depner (PFM Software)
    05/07/14

    Check return from fread.


    Version 5.31
    Jan C. Depner (PFM Software)
    07/01/14

    - Replaced all of the old, borrowed icons with new, public domain icons.  Mostly from the Tango set
      but a few from flavour-extended and 32pxmania.


    Version 5.32
    Jan C. Depner (PFM Software)
    07/23/14

    - Switched from using the old NV_INT64 and NV_U_INT32 type definitions to the C99 standard stdint.h and
      inttypes.h sized data types (e.g. int64_t and uint32_t).


    Version 5.33
    Jan C. Depner (PFM Software)
    07/29/14

    - Fixed errors discovered by cppcheck.


    Version 5.34
    Jan C. Depner (PFM Software)
    02/16/15

    - To give better feedback to shelling programs in the case of errors I've added the program name to all
      output to stderr.


    Version 5.35
    Jan C. Depner (PFM Software)
    03/31/15

    - Added ability to use ESRI Polygon, PolygonZ, PolygonM, PolyLine, PolyLineZ, or PolyLineM geographic shape
      files as area files.


    Version 5.36
    Jan C. Depner (PFM Software)
    04/05/16

    - Now uses geoid12b for orthometric conversion if the files are installed in ABE_data/geoid_data.


    Version 5.37
    Jan C. Depner (PFM Software)
    05/02/16

    - Added warning if the PFM was built using LAS or LAZ (due to possible screwed up coordinate system data in
      LAS/LAZ).


    Version 5.38
    Jan C. Depner (PFM Software)
    07/12/16

    - Removed COMPD_CS from Well-known Text.


    Version 5.39
    Jan C. Depner (PFM Software)
    08/27/16

    - Now uses the same font as all other ABE GUI apps.  Font can only be changed in pfmView Preferences.


    Version 5.40
    Jan C. Depner (PFM Software)
    10/17/16

    - Put COMPD_CS back on Well-known Text (GDAL/OGR supports it but QGIS hasn't caught up yet).


    Version 5.41
    Jan C. Depner (PFM Software)
    09/26/17

    - A bunch of changes to support doing translations in the future.  There is a generic
      pfmGeotiff_xx.ts file that can be run through Qt's "linguist" to translate to another language.

</pre>*/
