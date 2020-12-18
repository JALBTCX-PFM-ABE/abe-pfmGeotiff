
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



#include "startPage.hpp"
#include "startPageHelp.hpp"

startPage::startPage (int32_t *argc, char **argv, OPTIONS *op, QWidget *parent):
  QWizardPage (parent)
{
  options = op;


  setPixmap (QWizard::WatermarkPixmap, QPixmap(":/icons/pfmGeotiffWatermark.png"));


  setTitle (tr ("Introduction"));

  setWhatsThis (tr ("See, it really works!"));

  QLabel *label = new QLabel (tr ("pfmGeotiff is a tool for creating a GeoTIFF file that represents one of "
                                  "the filtered surfaces of a PFM structure.  It can optionally write out "
                                  "multiple files that represent the data type coverages.  Help is available "
                                  "by clicking on the Help button and then clicking on the item for which "
                                  "you want help.  Select a PFM file below.  You may then change the default "
                                  "output file name and, optionally, select an area file to limit the extent "
                                  "of the GeoTIFF file that is created.  Click <b>Next</b> to continue or "
                                  "<b>Cancel</b> to exit."));
  label->setWordWrap (true);


  QVBoxLayout *vbox = new QVBoxLayout (this);
  vbox->addWidget (label);
  vbox->addStretch (10);


  QHBoxLayout *pfm_file_box = new QHBoxLayout (0);
  pfm_file_box->setSpacing (8);

  vbox->addLayout (pfm_file_box);


  QLabel *pfm_file_label = new QLabel (tr ("PFM File"), this);
  pfm_file_box->addWidget (pfm_file_label, 1);

  pfm_file_edit = new QLineEdit (this);
  pfm_file_edit->setReadOnly (true);
  pfm_file_box->addWidget (pfm_file_edit, 10);

  QPushButton *pfm_file_browse = new QPushButton (tr ("Browse..."), this);
  pfm_file_box->addWidget (pfm_file_browse, 1);

  pfm_file_label->setWhatsThis (pfm_fileText);
  pfm_file_edit->setWhatsThis (pfm_fileText);
  pfm_file_browse->setWhatsThis (pfm_fileBrowseText);

  connect (pfm_file_browse, SIGNAL (clicked ()), this, SLOT (slotPFMFileBrowse ()));



  QHBoxLayout *output_box = new QHBoxLayout (0);
  output_box->setSpacing (8);

  vbox->addLayout (output_box);


  QLabel *output_file_label = new QLabel (tr ("Output GeoTIFF File"), this);
  output_box->addWidget (output_file_label, 1);

  output_file_edit = new QLineEdit (this);
  output_box->addWidget (output_file_edit, 10);

  QPushButton *output_file_browse = new QPushButton (tr ("Browse..."), this);
  output_box->addWidget (output_file_browse, 1);

  output_file_label->setWhatsThis (output_fileText);
  output_file_edit->setWhatsThis (output_fileText);
  output_file_browse->setWhatsThis (output_fileBrowseText);

  connect (output_file_browse, SIGNAL (clicked ()), this, SLOT (slotOutputFileBrowse ()));


  QHBoxLayout *area_box = new QHBoxLayout (0);
  area_box->setSpacing (8);

  vbox->addLayout (area_box);


  QLabel *area_file_label = new QLabel (tr ("Optional Area File"), this);
  area_box->addWidget (area_file_label, 1);

  area_file_edit = new QLineEdit (this);
  area_file_edit->setReadOnly (true);
  area_box->addWidget (area_file_edit, 10);

  QPushButton *area_file_browse = new QPushButton (tr ("Browse..."), this);
  area_box->addWidget (area_file_browse, 1);


  area_file_label->setWhatsThis (area_fileText);
  area_file_edit->setWhatsThis (area_fileText);
  area_file_browse->setWhatsThis (area_fileBrowseText);

  connect (area_file_browse, SIGNAL (clicked ()), this, SLOT (slotAreaFileBrowse ()));


  if (*argc == 2)
    {
      PFM_OPEN_ARGS open_args;
      int32_t pfm_handle = -1;

      QString pfm_file_name = QString (argv[1]);

      strcpy (open_args.list_path, argv[1]);

      open_args.checkpoint = 0;
      pfm_handle = open_existing_pfm_file (&open_args);

      if (pfm_handle >= 0)
        {
          if (!open_args.head.proj_data.projection)
            {
              pfm_file_edit->setText (pfm_file_name);


              //  If one hasn't been set, set the output TIFF filename.

              if (output_file_edit->text ().isEmpty ())
                { 
                  QString output_file_name = pfm_file_name + ".tif";
                  output_file_edit->setText (output_file_name);
                }

              close_pfm_file (pfm_handle);
            }
        }
    }


  if (!pfm_file_edit->text ().isEmpty ())
    {
      registerField ("pfm_file_edit", pfm_file_edit);
    }
  else
    {
      registerField ("pfm_file_edit*", pfm_file_edit);
    }


  registerField ("output_file_edit", output_file_edit);
  registerField ("area_file_edit", area_file_edit);
}



void startPage::slotPFMFileBrowse ()
{
  PFM_OPEN_ARGS       open_args;
  QStringList         files, filters;
  QString             file;
  int32_t             pfm_handle = -1;


  QFileDialog *fd = new QFileDialog (this, tr ("pfmGeotiff Open PFM Structure"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, options->input_dir);


  filters << QString ("PFM (*.pfm)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::ExistingFile);
  fd->selectNameFilter (QString ("PFM (*.pfm)"));

  if (fd->exec () == QDialog::Accepted)
    {
      files = fd->selectedFiles ();

      QString pfm_file_name = files.at (0);


      if (!pfm_file_name.isEmpty())
        {
          strcpy (open_args.list_path, pfm_file_name.toLatin1 ());

          open_args.checkpoint = 0;
          pfm_handle = open_existing_pfm_file (&open_args);

          if (pfm_handle < 0)
            {
              QMessageBox::warning (this, tr ("Open PFM Structure"),
                                    tr ("The file %1 is not a PFM structure or there was an error reading the file.\nThe error message returned was:\n\n%2").arg 
                                    (QDir::toNativeSeparators (QString (open_args.list_path))).arg (pfm_error_str (pfm_error)));

              if (pfm_error == CHECKPOINT_FILE_EXISTS_ERROR)
                {
                  fprintf (stderr, "%s %s %s %d - %s - %s\n", options->progname, __FILE__, __FUNCTION__, __LINE__, open_args.list_path,
                           pfm_error_str (pfm_error));
                  exit (-1);
                }

              return;
            }
        }

      if (open_args.head.proj_data.projection)
        {
          QMessageBox::warning (this, tr ("Open PFM Structure"), tr ("Sorry, pfmGeotiff only handles geographic PFM structures."));
          close_pfm_file (pfm_handle);
        }


      pfm_file_edit->setText (pfm_file_name);


      options->input_dir = fd->directory ().absolutePath ();


      //  If one hasn't been set, set the output TIFF filename.

      if (output_file_edit->text ().isEmpty ())
        { 
          QString output_file_name = pfm_file_name + ".tif";
          output_file_edit->setText (output_file_name);
        }
    }
}



void startPage::slotOutputFileBrowse ()
{
  QFileDialog *fd = new QFileDialog (this, tr ("pfmGeotiff Output File"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, options->output_dir);


  QStringList filters;
  filters << tr ("Geotiff file (*.tif)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::AnyFile);
  fd->selectNameFilter (tr ("Geotiff file (*.tif)"));

  QStringList files;
  if (fd->exec () == QDialog::Accepted)
    {
      files = fd->selectedFiles ();

      QString output_file_name = files.at (0);

      options->output_dir = fd->directory ().absolutePath ();

      if (!output_file_name.isEmpty())
        {
          //  Add .tif to filename if not there.
            
          if (!output_file_name.endsWith (".tif")) output_file_name.append (".tif");

          output_file_edit->setText (output_file_name);
        }
    }
}


void startPage::slotAreaFileBrowse ()
{
  SHPHandle shpHandle;
  SHPObject *shape = NULL;
  int32_t type, numShapes;
  double minBounds[4], maxBounds[4];


  QFileDialog *fd = new QFileDialog (this, tr ("pfmGeotiff Area File"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, options->area_dir);


  QStringList filters;
  filters << tr ("Area file (*.ARE *.are *.afs *.shp *.SHP)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::ExistingFile);
  fd->selectNameFilter (tr ("Area file (*.ARE *.are *.afs *.shp *.SHP)"));


  QStringList files;
  if (fd->exec () == QDialog::Accepted)
    {
      files = fd->selectedFiles ();

      QString area_file_name = files.at (0);

      options->area_dir = fd->directory ().absolutePath ();

      if (!area_file_name.isEmpty())
        {
          if (area_file_name.endsWith (".shp", Qt::CaseInsensitive))
            {
              char shpname[1024];
              strcpy (shpname, area_file_name.toLatin1 ());


              //  Open shape file

              shpHandle = SHPOpen (shpname, "rb");

              if (shpHandle == NULL)
                {
                  QMessageBox::warning (this, "pfmGeotiff", tr ("Cannot open shape file %1!").arg (area_file_name));
                  return;
                }
              else
                {
                  //  Get shape file header info

                  SHPGetInfo (shpHandle, &numShapes, &type, minBounds, maxBounds);


                  if (type != SHPT_POLYGON && type != SHPT_POLYGONZ && type != SHPT_POLYGONM &&
                      type != SHPT_ARC && type != SHPT_ARCZ && type != SHPT_ARCM)
                    {
                      QMessageBox::warning (this, "pfmGeotiff", tr ("Shape file %1 is not a polygon or polyline file!").arg (area_file_name));
                      return;
                    }
                  else
                    {
                      //  Read only the first shape.

                      shape = SHPReadObject (shpHandle, 0);


                      //  Check the number of vertices.

                      if (shape->nVertices < 3)
                        {
                          SHPClose (shpHandle);
                          QMessageBox::warning (this, "pfmGeotiff", tr ("Number of vertices (%1) of shape file %2 is too few for a polygon!").arg
                                                (shape->nVertices).arg (area_file_name));
                          return;
                        }


                      //  Read the vertices to take a shot at determining that this is a geographic polygon.

                      for (int32_t j = 0 ; j < shape->nVertices ; j++)
                        {
                          if (shape->padfX[j] < -360.0 || shape->padfX[j] > 360.0 || shape->padfY[j] < -90.0 || shape->padfY[j] > 90.0)
                            {
                              SHPDestroyObject (shape);
                              SHPClose (shpHandle);
                              QMessageBox::warning (this, "pfmGeotiff", tr ("Shape file %1 does not appear to be geographic!").arg (area_file_name));
                              return;
                            }
                        }


                      SHPDestroyObject (shape);
                      SHPClose (shpHandle);
                    }
                }
            }

          area_file_edit->setText (area_file_name);
        }
    }
}
