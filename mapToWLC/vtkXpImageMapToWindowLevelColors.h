/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkXpImageMapToWindowLevelColors.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkXpImageMapToWindowLevelColors
 * @brief   map the input image through a lookup table and window / level it
 *
 * The vtkXpImageMapToWindowLevelColors filter will take an input image of any
 * valid scalar type, and map the first component of the image through a
 * lookup table.  This resulting color will be modulated with value obtained
 * by a window / level operation. The result is an image of type
 * VTK_UNSIGNED_CHAR. If the lookup table is not set, or is set to nullptr, then
 * the input data will be passed through if it is already of type
 * VTK_UNSIGNED_CHAR.
 *
 * @sa
 * vtkLookupTable vtkScalarsToColors
*/

#ifndef vtkXpImageMapToWindowLevelColors_h
#define vtkXpImageMapToWindowLevelColors_h


#include "vtkImagingColorModule.h" // For export macro
#include "vtkImageMapToColors.h"

#ifdef VTK_XP_IMAGINGCOLOR_STATIC_DEFINE
#  define VTK_XP_IMAGINGCOLOR_EXPORT
#  define VTK_XP_IMAGINGCOLOR_NO_EXPORT
#else
# ifdef _WIN32

#  ifndef VTK_XP_IMAGINGCOLOR_EXPORT
#    ifdef vtkXpImagingColor_EXPORTS
/* We are building this library */
#      define VTK_XP_IMAGINGCOLOR_EXPORT __declspec(dllexport)
#    else
/* We are using this library */
#      define VTK_XP_IMAGINGCOLOR_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTK_XP_IMAGINGCOLOR_NO_EXPORT
#    define VTK_XP_IMAGINGCOLOR_NO_EXPORT 
#  endif
# else //_WIN32 -> other
#  ifndef VTK_XP_IMAGINGCOLOR_EXPORT
#    ifdef vtkXpImagingColor_EXPORTS
/* We are building this library */
#      define VTK_XP_IMAGINGCOLOR_EXPORT __attribute__((visibility("default")))
#    else
/* We are using this library */
#      define VTK_XP_IMAGINGCOLOR_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef VTK_XP_IMAGINGCOLOR_NO_EXPORT
#    define VTK_XP_IMAGINGCOLOR_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
# endif // _WIN32
#endif
class VTK_XP_IMAGINGCOLOR_EXPORT vtkXpImageMapToWindowLevelColors : public vtkImageMapToColors
{
public:
  static vtkXpImageMapToWindowLevelColors *New();
  vtkTypeMacro(vtkXpImageMapToWindowLevelColors, vtkImageMapToColors);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Set / Get the Window to use -> modulation will be performed on the
   * color based on (S - (L - W/2))/W where S is the scalar value, L is
   * the level and W is the window.
   */
  vtkSetMacro( Window, double );
  vtkGetMacro( Window, double );
  //@}

  //@{
  /**
   * Set / Get the Level to use -> modulation will be performed on the
   * color based on (S - (L - W/2))/W where S is the scalar value, L is
   * the level and W is the window.
   */
  vtkSetMacro( Level, double );
  vtkGetMacro( Level, double );
  //@}

protected:
  vtkXpImageMapToWindowLevelColors();
  ~vtkXpImageMapToWindowLevelColors() override;

  int RequestInformation (vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  void ThreadedRequestData(vtkInformation *request,
                           vtkInformationVector **inputVector,
                           vtkInformationVector *outputVector,
                           vtkImageData ***inData, vtkImageData **outData,
                           int extent[6], int id) override;
  int RequestData(vtkInformation *request,
                          vtkInformationVector **inputVector,
                          vtkInformationVector *outputVector) override;

  double Window;
  double Level;

private:
  vtkXpImageMapToWindowLevelColors(const vtkXpImageMapToWindowLevelColors&) = delete;
  void operator=(const vtkXpImageMapToWindowLevelColors&) = delete;
};

#endif







