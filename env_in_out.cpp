
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



#include "pfmGeotiff.hpp"


double settings_version = 5.25;


/*!
  These functions store and retrieve the program settings (environment) from a .ini file.  On both Linux and Windows
  the file will be called pfmGeotiff.ini and will be stored in a directory called ABE.config.  On Linux, the
  ABE.config directory will be stored in your $HOME directory.  On Windows, it will be stored in your $USERPROFILE
  folder.  If you make a change to the way a variable is used and you want to force the defaults to be restored just
  change the settings_version to a newer number (I've been using the program version number from version.hpp - which
  you should be updating EVERY time you make a change to the program!).  You don't need to change the
  settings_version though unless you want to force the program to go back to the defaults (which can annoy your
  users).  So, the settings_version won't always match the program version.
*/

void envin (OPTIONS *options)
{
  //  We need to get the font from the global settings.

#ifdef NVWIN3X
  QString ini_file2 = QString (getenv ("USERPROFILE")) + "/ABE.config/" + "globalABE.ini";
#else
  QString ini_file2 = QString (getenv ("HOME")) + "/ABE.config/" + "globalABE.ini";
#endif

  options->font = QApplication::font ();

  QSettings settings2 (ini_file2, QSettings::IniFormat);
  settings2.beginGroup ("globalABE");


  QString defaultFont = options->font.toString ();
  QString fontString = settings2.value (QString ("ABE map GUI font"), defaultFont).toString ();
  options->font.fromString (fontString);


  settings2.endGroup ();


  double saved_version = 0.0;


  //  Get the INI file name

#ifdef NVWIN3X
  QString ini_file = QString (getenv ("USERPROFILE")) + "/ABE.config/pfmGeotiff.ini";
#else
  QString ini_file = QString (getenv ("HOME")) + "/ABE.config/pfmGeotiff.ini";
#endif

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup ("pfmGeotiff");

  saved_version = settings.value (QString ("settings version"), saved_version).toDouble ();


  //  If the settings version has changed we need to leave the values at the new defaults since they may have changed.

  if (settings_version != saved_version) return;


  options->transparent = settings.value (QString ("transparent"), options->transparent).toBool ();

  options->packbits = settings.value (QString ("packbits compression flag"), options->packbits).toBool ();

  options->grey = settings.value (QString ("32 bit floating point format"), options->grey).toBool ();

  options->restart = settings.value (QString ("restart"), options->restart).toBool ();

  options->azimuth = (float) settings.value (QString ("azimuth"), (double) options->azimuth).toDouble ();

  options->elevation = (float) settings.value (QString ("elevation"), (double) options->elevation).toDouble ();

  options->exaggeration = (float) settings.value (QString ("exaggeration"), (double) options->exaggeration).toDouble ();

  options->saturation = (float) settings.value (QString ("saturation"), (double) options->saturation).toDouble ();

  options->value = (float) settings.value (QString ("value"), (double) options->value).toDouble ();

  options->start_hsv = (float) settings.value (QString ("start_hsv"), (double) options->start_hsv).toDouble ();

  options->end_hsv = (float) settings.value (QString ("end_hsv"), (double) options->end_hsv).toDouble ();

  options->surface = settings.value (QString ("surface"), options->surface).toInt ();

  options->units = settings.value (QString ("units"), options->units).toInt ();

  options->dumb = settings.value (QString ("4800 ft per second flag"), options->dumb).toBool ();

  options->intrp = settings.value (QString ("interpolated contours"), options->intrp).toBool ();

  options->smoothing_factor = settings.value (QString ("smoothing level"), options->smoothing_factor).toBool ();

  options->navd88 = settings.value (QString ("NAVD88 flag"), options->navd88).toBool ();

  options->elev = settings.value (QString ("elevation flag"), options->elev).toBool ();

  options->cint = (float) settings.value (QString ("contour interval"), (double) options->cint).toDouble ();

  options->input_dir = settings.value (QString ("input directory"), options->input_dir).toString ();
  options->output_dir = settings.value (QString ("output directory"), options->output_dir).toString ();
  options->area_dir = settings.value (QString ("area directory"), options->area_dir).toString ();

  options->window_width = settings.value (QString ("width"), options->window_width).toInt ();
  options->window_height = settings.value (QString ("height"), options->window_height).toInt ();
  options->window_x = settings.value (QString ("x position"), options->window_x).toInt ();
  options->window_y = settings.value (QString ("y position"), options->window_y).toInt ();

  settings.endGroup ();
}


void envout (OPTIONS *options)
{
  //  Get the INI file name

#ifdef NVWIN3X
  QString ini_file = QString (getenv ("USERPROFILE")) + "/ABE.config/pfmGeotiff.ini";
#else
  QString ini_file = QString (getenv ("HOME")) + "/ABE.config/pfmGeotiff.ini";
#endif

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup ("pfmGeotiff");


  settings.setValue (QString ("settings version"), settings_version);

  settings.setValue (QString ("transparent"), options->transparent);

  settings.setValue (QString ("packbits compression flag"), options->packbits);

  settings.setValue (QString ("32 bit floating point format"), options->grey);

  settings.setValue (QString ("restart"), options->restart);

  settings.setValue (QString ("azimuth"), (double) options->azimuth);

  settings.setValue (QString ("elevation"), (double) options->elevation);

  settings.setValue (QString ("exaggeration"), (double) options->exaggeration);

  settings.setValue (QString ("saturation"), (double) options->saturation);

  settings.setValue (QString ("value"), (double) options->value);

  settings.setValue (QString ("start_hsv"), (double) options->start_hsv);

  settings.setValue (QString ("end_hsv"), (double) options->end_hsv);

  settings.setValue (QString ("surface"), options->surface);

  settings.setValue (QString ("units"), options->units);

  settings.setValue (QString ("4800 ft per second flag"), options->dumb);

  settings.setValue (QString ("interpolated contours"), options->intrp);

  settings.setValue (QString ("smoothing level"), options->smoothing_factor);

  settings.setValue (QString ("NAVD88 flag"), options->navd88);

  settings.setValue (QString ("elevation flag"), options->elev);

  settings.setValue (QString ("contour interval"), options->cint);

  settings.setValue (QString ("input directory"), options->input_dir);
  settings.setValue (QString ("output directory"), options->output_dir);
  settings.setValue (QString ("area directory"), options->area_dir);

  settings.setValue (QString ("width"), options->window_width);
  settings.setValue (QString ("height"), options->window_height);
  settings.setValue (QString ("x position"), options->window_x);
  settings.setValue (QString ("y position"), options->window_y);

  settings.endGroup ();
}
