# This example reads a volume dataset, extracts two isosurfaces that
# represent the skin and bone, creates three orthogonal planes
# (saggital, axial, coronal), and displays them.
from math import *
from euclid import *
from omega import *
from omegaToolkit import *
from omegaVtk import *

vtkModule = VtkModule.createAndInitialize()

import vtk

fileFormat = ofindFile("data/headsq/quarter.1")
fileFormat = fileFormat.rstrip(".1");

# The following reader is used to read a series of 2D slices (images)
# that compose the volume. The slice dimensions are set, and the
# pixel spacing. The data Endianness must also be specified. The reader
# usese the FilePrefix in combination with the slice number to construct
# filenames using the format FilePrefix.%d. (In this case the FilePrefix
# is the root name of the file: quarter.)
v16 = vtk.vtkVolume16Reader()
v16.SetDataDimensions(64, 64)
v16.SetDataByteOrderToLittleEndian()
v16.SetFilePrefix(fileFormat)
v16.SetImageRange(1, 93)
v16.SetDataSpacing(3.2, 3.2, 1.5)

# An isosurface, or contour value of 500 is known to correspond to the
# skin of the patient. Once generated, a vtkPolyDataNormals filter is
# is used to create normals for smooth surface shading during rendering.
# The triangle stripper is used to create triangle strips from the
# isosurface these render much faster on may systems.
skinExtractor = vtk.vtkContourFilter()
skinExtractor.SetInputConnection(v16.GetOutputPort())
skinExtractor.SetValue(0, 500)
skinNormals = vtk.vtkPolyDataNormals()
skinNormals.SetInputConnection(skinExtractor.GetOutputPort())
skinNormals.SetFeatureAngle(60.0)
skinStripper = vtk.vtkStripper()
skinStripper.SetInputConnection(skinNormals.GetOutputPort())
skinMapper = vtk.vtkPolyDataMapper()
skinMapper.SetInputConnection(skinStripper.GetOutputPort())
skinMapper.ScalarVisibilityOff()
skin = vtk.vtkActor()
skin.SetMapper(skinMapper)
skin.GetProperty().SetDiffuseColor(1, .49, .25)
skin.GetProperty().SetSpecular(.3)
skin.GetProperty().SetSpecularPower(20)

# An isosurface, or contour value of 1150 is known to correspond to the
# skin of the patient. Once generated, a vtkPolyDataNormals filter is
# is used to create normals for smooth surface shading during rendering.
# The triangle stripper is used to create triangle strips from the
# isosurface these render much faster on may systems.
boneExtractor = vtk.vtkContourFilter()
boneExtractor.SetInputConnection(v16.GetOutputPort())
boneExtractor.SetValue(0, 1150)
boneNormals = vtk.vtkPolyDataNormals()
boneNormals.SetInputConnection(boneExtractor.GetOutputPort())
boneNormals.SetFeatureAngle(60.0)
boneStripper = vtk.vtkStripper()
boneStripper.SetInputConnection(boneNormals.GetOutputPort())
boneMapper = vtk.vtkPolyDataMapper()
boneMapper.SetInputConnection(boneStripper.GetOutputPort())
boneMapper.ScalarVisibilityOff()
bone = vtk.vtkActor()
bone.SetMapper(boneMapper)
bone.GetProperty().SetDiffuseColor(1, 1, .9412)

# An outline provides context around the data.
outlineData = vtk.vtkOutlineFilter()
outlineData.SetInputConnection(v16.GetOutputPort())
mapOutline = vtk.vtkPolyDataMapper()
mapOutline.SetInputConnection(outlineData.GetOutputPort())
outline = vtk.vtkActor()
outline.SetMapper(mapOutline)
outline.GetProperty().SetColor(0, 0, 0)

# Now we are creating three orthogonal planes passing through the
# volume. Each plane uses a different texture map and therefore has
# diferent coloration.

# Start by creatin a black/white lookup table.
bwLut = vtk.vtkLookupTable()
bwLut.SetTableRange(0, 2000)
bwLut.SetSaturationRange(0, 0)
bwLut.SetHueRange(0, 0)
bwLut.SetValueRange(0, 1)
bwLut.Build()

# Now create a lookup table that consists of the full hue circle (from
# HSV).
hueLut = vtk.vtkLookupTable()
hueLut.SetTableRange(0, 2000)
hueLut.SetHueRange(0, 1)
hueLut.SetSaturationRange(1, 1)
hueLut.SetValueRange(1, 1)
hueLut.Build()

# Finally, create a lookup table with a single hue but having a range
# in the saturation of the hue.
satLut = vtk.vtkLookupTable()
satLut.SetTableRange(0, 2000)
satLut.SetHueRange(.6, .6)
satLut.SetSaturationRange(0, 1)
satLut.SetValueRange(1, 1)
satLut.Build()

# Create the first of the three planes. The filter vtkImageMapToColors
# maps the data through the corresponding lookup table created above.
# The vtkImageActor is a type of vtkProp and conveniently displays an
# image on a single quadrilateral plane. It does this using texture
# mapping and as a result is quite fast. (Note: the input image has to
# be unsigned char values, which the vtkImageMapToColors produces.)
# Note also that by specifying the DisplayExtent, the pipeline
# requests data of this extent and the vtkImageMapToColors only
# processes a slice of data.
saggitalColors = vtk.vtkImageMapToColors()
saggitalColors.SetInputConnection(v16.GetOutputPort())
saggitalColors.SetLookupTable(bwLut)
saggitalColors.Update()
saggital = vtk.vtkImageActor()
saggital.SetInput(saggitalColors.GetOutput())
saggital.SetDisplayExtent(32, 32, 0, 63, 0, 92)

# Create the second (axial) plane of the three planes. We use the same
# approach as before except that the extent differs.
axialColors = vtk.vtkImageMapToColors()
axialColors.SetInputConnection(v16.GetOutputPort())
axialColors.SetLookupTable(hueLut)
axialColors.Update()
axial = vtk.vtkImageActor()
axial.SetInput(axialColors.GetOutput())
axial.SetDisplayExtent(0, 63, 0, 63, 46, 46)

# Create the third (coronal) plane of the three planes. We use the same
# approach as before except that the extent differs.
coronalColors = vtk.vtkImageMapToColors()
coronalColors.SetInputConnection(v16.GetOutputPort())
coronalColors.SetLookupTable(satLut)
coronalColors.Update()
coronal = vtk.vtkImageActor()
coronal.SetInput(coronalColors.GetOutput())
coronal.SetDisplayExtent(0, 63, 32, 32, 0, 92)

# Actors are added to the renderer.
vtkNode = SceneNode.create("vtkNode")
vtkNode.setPosition(Vector3(0, 2, -2))
vtkNode.setScale(Vector3(0.02, 0.02, 0.02))

vtkAttachProp(outline, vtkNode)
vtkAttachProp(saggital, vtkNode)
vtkAttachProp(axial, vtkNode)
vtkAttachProp(coronal, vtkNode)
vtkAttachProp(skin, vtkNode)
vtkAttachProp(bone, vtkNode)

# Turn off bone for this example.
#bone.VisibilityOff()

# Set skin to semi-transparent.
skin.GetProperty().SetOpacity(0.8)
bone.GetProperty().SetOpacity(0.9)
#bone.GetProperty().BackfaceCullingOn()
#skin.GetProperty().BackfaceCullingOn()

#ovtk.addSlider("skin", 0, 1, 0.1, 'skin.GetProperty().GetOpacity()', 'skin.GetProperty().SetOpacity(${value})')
#ovtk.addSlider("bone", 0, 1, 0.1, 'bone.GetProperty().GetOpacity()', 'bone.GetProperty().SetOpacity(${value})')
#ovtk.addCheckButton("sagittal", 'saggital.GetVisibility()', 'saggital.SetVisibility(${value})')
#ovtk.addCheckButton("coronal", 'coronal.GetVisibility()', 'coronal.SetVisibility(${value})')
#ovtk.addCheckButton("axial", 'axial.GetVisibility()', 'axial.SetVisibility(${value})')
