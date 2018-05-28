/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestDICOMImageReader.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME Test of vtkDICOMImageReader
// .SECTION Description
//


#include "vtkSmartPointer.h"

#include "vtkDICOMReader.h"
#include "vtkMedicalImageProperties.h"

#include "vtkImageData.h"
#include "vtkImageViewer2.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkBMPWriter.h"

#include "vtkLookupTable.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkXpImageMapToWindowLevelColors.h"


int TestDICOMImageReader(int argc, char *argv[])
{

  if ( argc <= 1 )
  {
    cout << "Usage: " << argv[0] << " <dicom file>" << endl;
    return 1;
  }

  std::string filename = argv[1];

  vtkSmartPointer<vtkDICOMReader> DICOMReader =
      vtkSmartPointer<vtkDICOMReader>::New();


  // Check the image can be read
  if (!DICOMReader->CanReadFile(filename.c_str()))
  {
    cerr << "CanReadFile failed for " << filename.c_str() << "\n";
    exit(1);
  }

  // Read the input image
  DICOMReader->SetFileName(filename.c_str());
  DICOMReader->Update();

  // Read and display the image properties
  double * wl = nullptr;
  double range[2] = { 0 };
  double w[2] = { 0 };
  int idx = DICOMReader->GetMedicalImageProperties()->GetNumberOfWindowLevelPresets();
  if (idx > 0)
  {
      wl = DICOMReader->GetMedicalImageProperties()->GetNthWindowLevelPreset(0);
      cout << "lewel: " << wl[0] << " center: " << wl[1] << std::endl;
  }
  else {
      DICOMReader->GetOutput()->GetScalarRange(range);
      int lowVal = 0;
      int highVal = 0;
      if (range) {
          lowVal = range[0];
          highVal = range[1];
      }
      // set a limit on how tight the window can be
      if (highVal - lowVal < 20) {
          highVal = lowVal + 20;
      }
      // make sure that WindowCenter will be an integer
      if ((highVal - lowVal) % 2 != 0) {
          if (lowVal > 0) {
              lowVal--;
          }
          else {
              highVal--;
          }
      }
      range[0] = lowVal;
      range[1] = highVal;
      w[0] = 0.5*(range[1] + range[0]);
      w[1] = 1.0*(range[1] - range[0]);
      wl = w;
  }
  

  vtkSmartPointer<vtkLookupTable> colorLut
      = vtkSmartPointer<vtkLookupTable>::New();

  range[0] = wl[0] - wl[1] * 0.5;
  range[1] = wl[0] + wl[1] * 0.5;

  int norm = range[1] - range[0];
  colorLut->SetRange(range[0], range[1]);
  colorLut->SetIndexedLookup(0);
  colorLut->SetNumberOfTableValues(norm+1);
  colorLut->Build();
  double scale = 255. / norm;
  for (int i = 0; i <= norm; i++) {
      int cl = double(i) * scale;
      colorLut->SetTableValue(i, cl/255., cl/255., cl/255.);
  }

  //--------------------------------------------------------
  vtkSmartPointer<vtkImageMapToWindowLevelColors> mapOrig
      = vtkSmartPointer<vtkImageMapToWindowLevelColors>::New();

  mapOrig->SetLevel(wl[0]);
  mapOrig->SetWindow(wl[1]);
  mapOrig->SetInputConnection(DICOMReader->GetOutputPort());

  vtkSmartPointer<vtkBMPWriter> bmpW0
      = vtkSmartPointer<vtkBMPWriter>::New();

  bmpW0->SetInputConnection(mapOrig->GetOutputPort());
  bmpW0->SetFileName("bmp0.bmp");
  bmpW0->Write();


  mapOrig->SetLookupTable(colorLut);
  vtkSmartPointer<vtkBMPWriter> bmpW1
      = vtkSmartPointer<vtkBMPWriter>::New();

  bmpW1->SetInputConnection(mapOrig->GetOutputPort());
  bmpW1->SetFileName("bmp1.bmp");
  bmpW1->Write();

  //--------------------------------------------------------------------

  vtkSmartPointer<vtkXpImageMapToWindowLevelColors> mapCorrected
      = vtkSmartPointer<vtkXpImageMapToWindowLevelColors>::New();

  mapCorrected->SetLevel(wl[0]);
  mapCorrected->SetWindow(wl[1]);
  mapCorrected->SetInputConnection(DICOMReader->GetOutputPort());

  vtkSmartPointer<vtkBMPWriter> bmpW0Cor
      = vtkSmartPointer<vtkBMPWriter>::New();

  bmpW0Cor->SetInputConnection(mapCorrected->GetOutputPort());
  bmpW0Cor->SetFileName("bmpCorr0.bmp");
  bmpW0Cor->Write();


  mapCorrected->SetLookupTable(colorLut);
  vtkSmartPointer<vtkBMPWriter> bmpW1Cor
      = vtkSmartPointer<vtkBMPWriter>::New();

  bmpW1Cor->SetInputConnection(mapCorrected->GetOutputPort());
  bmpW1Cor->SetFileName("bmpCorr1.bmp");
  bmpW1Cor->Write();


  return 0;
}

int main(int argc, char *argv[])
{
	return TestDICOMImageReader(argc, argv);
}