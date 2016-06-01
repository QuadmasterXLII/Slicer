import unittest
import vtk

testInt = 12
testString = "test string"
testFloat = 5.4


class VTKPythonObjectCalldataInvokeEventTest(unittest.TestCase):

  @vtk.calldata_type(vtk.VTK_INT)
  def callbackInt(self, caller, event, calldata):
    self.calldata = calldata

  @vtk.calldata_type(vtk.VTK_STRING)
  def callbackString(self, caller, event, calldata):
    print calldata
    self.calldata = calldata

  @vtk.calldata_type(vtk.VTK_FLOAT)
  def callbackFloat(self, caller, event, calldata):
    self.calldata = calldata

  def setUp(self):
    self.vtkObj = vtk.vtkObject()

  def test_int(self):
    self.vtkObj.AddObserver(vtk.vtkCommand.AnyEvent, self.callbackInt)
    self.vtkObj.InvokeEvent(vtk.vtkCommand.ModifiedEvent, testInt)
    self.assertEqual(self.calldata, testInt)

  def test_string(self):
    self.vtkObj.AddObserver(vtk.vtkCommand.AnyEvent, self.callbackString)
    self.vtkObj.InvokeEvent(vtk.vtkCommand.ModifiedEvent, testString)
    self.assertEqual(self.calldata, testString)

  def test_float(self):
    self.vtkObj.AddObserver(vtk.vtkCommand.AnyEvent, self.callbackFloat)
    self.vtkObj.InvokeEvent(vtk.vtkCommand.ModifiedEvent, testFloat)
    self.assertEqual(self.calldata, testString)
