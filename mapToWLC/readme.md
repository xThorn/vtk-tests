
# Purpose: comparing output data obtained by setting "SetLevel" and "SetWindow" with method of installation "vtkLookupTable"

# Test of the vtkImageMapToWindowLevelColors class

output to bmp0.bmp, bmp1.bmp

# Test of the corrected vtkXpImageMapToWindowLevelColors class

output to bmpCorr0.bmp, bmpCorr1.bmp

## For tests used dicom data from: http://deckard.mc.duke.edu/~samei/tg18_files/TG18-QC-1k-dcm.zip

# usage:
```
TestMapToWLC TG18-QC-1k-01.dcm
```

* the test "file_comp_1" comparing output files bmp0.bmp, bmp1.bmp
* the test "file_comp_2" comparing output files bmpCorr0.bmp, bmpCorr1.bmp
* must be all files are identical
* te original class vtkImageMapToWindowLevelColors failed the test

## Run test ctest.exe --force-new-ctest-process
```
Test project H:/Dev/tests/vtk-tests/mapToWLC/build
    Start 1: do_test
1/3 Test #1: do_test ..........................   Passed    2.08 sec
    Start 2: file_comp_1
2/3 Test #2: file_comp_1 ......................***Failed    0.02 sec
    Start 3: file_comp_2
3/3 Test #3: file_comp_2 ......................   Passed    0.02 sec