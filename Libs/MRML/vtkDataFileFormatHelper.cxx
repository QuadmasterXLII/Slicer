#include "vtkDataFileFormatHelper.h"
#include "vtkStringArray.h"
#include "itkImageIOBase.h"

#include <vector>
#include <set>
#include <string>

#define USE_TEMP_ITK_FILEFORMAT_TABLE

#ifdef USE_TEMP_ITK_FILEFORMAT_TABLE
#include "itkImageIOBase.h"

ITKImageFileFormat FileFormatTable[] =
{
  // --- Preferred extensions ---
  {"NrrdImageIO", "pixel data ?", "NRRD", ".nrrd"},
  {"NrrdImageIO", "header ?", "NRRD", ".nhdr"},
  {"MetaImageIO", "ASCII Text Header", "MetaImage", ".mhd"},
  {"MetaImageIO", "Text header followed by binary pixel data", "MetaImage", ".mha"},
  {"RawImageIO", "Uncompressed pixel data in binary", "RAW", ".raw"},
  {"VTKImageIO", "Text header followed by uncompressed pixel data in binary", "VTK", ".vtk"},
  // -----------------------------

  {"AnalyzeImageIO", "Uncompressed header in binary", "Analyze", ".hdr"},
  {"AnalyzeImageIO", "Uncompressed pixel data in binary", "Analyze", ".img"},
  {"AnalyzeImageIO", "Compressed pixel data in binary", "Analyze", ".img.gz"},
  {"BMPImageIO", "Uncompressed pixel data in binary with text header", "BMP", ".bmp"},
  {"BMPImageIO", "Uncompressed pixel data in binary with text header", "BMP", ".BMP"},
  {"BioRadImageIO", "Binary header followed by pixel data in binary", "BioRad", ".pic"},
  {"Brains2MaskImageIO", "Binary Mask Octree-encoded", "Brains2", ".mask"},
 // {"DICOMImageIO2", "Deprecated", "DICOM", "---"},


 // These are disabled for now, because we need more specifications for saving this format, 
 // and Slicer has other ways to save out dicom files.
//  {"GDCMImageIO", "DICOM 2D, 3D and 4D", "DICOM", ".dcm"},
//  {"GDCMImageIO", "DICOM 2D, 3D and 4D", "DICOM", ".DCM"},
//  {"GDCMImageIO", "DICOM 2D, 3D and 4D", "DICOM", ".dicom"},
//  {"GDCMImageIO", "DICOM 2D, 3D and 4D", "DICOM", ".DICOM"},


//  {"GE4ImageIO", "Not Available for Writing", "GE4", "---"},
//  {"GE5ImageIO", "Not Available for Writing", "GE5", "---"},
//  {"GEAwImageIO", "Not Available for Writing", "GEAw", "---"},
  {"GiplImageIO", "Uncompressed binary header followed by binary pixel data", "GIPL", ".gipl"},
  {"GiplImageIO", "Gzip Compressed binary header followed by binary pixel data", "GIPL", ".gipl.gz"},
//  {"IPLCommonImageIO", "Base class for GE4, GE5, GEAw, SiemensVision", "IPL", "---"},
  {"JPEGImageIO", "Compressed pixel data in binary", "JPEG", ".jpg"},
  {"JPEGImageIO", "Compressed pixel data in binary", "JPEG", ".jpeg"},
  {"JPEGImageIO", "Compressed pixel data in binary", "JPEG", ".JPG"},
  {"JPEGImageIO", "Compressed pixel data in binary", "JPEG", ".JPEG"},
  {"LSMImageIO", "Optional Compression. Binary pixel data", "LSM", ".lsm"},
  {"LSMImageIO", "Optional Compression. Binary pixel data", "LSM", ".LSM"},
  {"NiftiImageIO", "Header information in binary", "NifTI", ".nia"},
  {"NiftiImageIO", "Uncompressed pixel data in binary", "NifTI", ".nii"},
  {"NiftiImageIO", "Compressed pixel data in binary", "NifTI", ".nii.gz"},
  {"NiftiImageIO", "Header information in binary", "Analyze", ".hdr"},
  {"NiftiImageIO", "Uncompressed pixel data in binary", "Analyze", ".img"},
  {"NiftiImageIO", "Compressed pixel data in binary", "Analyze", ".img.gz"},
  {"PNGImageIO", "RLE compressed pixel data in binary", "PNG", ".png"},
  {"PNGImageIO", "RLE compressed pixel data in binary", "PNG", ".PNG"},
//  {"SiemensVisionImageIO", "Not available for writing", "Siemens", "---"},
  {"StimulateImageIO", "Uncompressed pixel data in binary", "Stimulate", ".spr"},
  {"TIFFImageIO", "Compressed pixel data in binary", "TIFF", ".tiff"},
  {"TIFFImageIO", "Compressed pixel data in binary", "TIFF", ".tif"},
  {"TIFFImageIO", "Compressed pixel data in binary", "TIFF", ".TIFF"},
  {"TIFFImageIO", "Compressed pixel data in binary", "TIFF", ".TIF"}
};

#endif

vtkStandardNewMacro ( vtkDataFileFormatHelper );
vtkCxxRevisionMacro ( vtkDataFileFormatHelper, "$Revision: 1.0 $" );

//----------------------------------------------------------------------------
vtkDataFileFormatHelper::vtkDataFileFormatHelper()
{
  this->SupportedWriteFileExtensions = NULL;
  this->SupportedWriteFileGenericNames = NULL;
  this->SupportedWriteFileClassNames = NULL;
  this->ITKSupportedWriteFileFormats = NULL;
}

//----------------------------------------------------------------------------
vtkDataFileFormatHelper::~vtkDataFileFormatHelper()
{
  if(this->ITKSupportedWriteFileFormats)
    {
    this->ITKSupportedWriteFileFormats->Delete();
    }
  if(this->SupportedWriteFileExtensions)
    {
    this->SupportedWriteFileExtensions->Delete();
    }
  if(this->SupportedWriteFileGenericNames)
    {
    this->SupportedWriteFileGenericNames->Delete();
    }
  if(this->SupportedWriteFileClassNames)
    {
    this->SupportedWriteFileClassNames->Delete();
    }
}

//----------------------------------------------------------------------------
vtkStringArray* vtkDataFileFormatHelper::GetITKSupportedWriteFileFormats()
{
  if(!this->ITKSupportedWriteFileFormats)
    {
    this->InitializeITKSupportedFileFormats();
    }
  return this->ITKSupportedWriteFileFormats;
}

//----------------------------------------------------------------------------
void vtkDataFileFormatHelper::PopulateITKSupportedWriteFileTypes()
{

#ifdef USE_TEMP_ITK_FILEFORMAT_TABLE
  int numFiles = sizeof(FileFormatTable)/sizeof(FileFormatTable[0]) ;
  typedef std::set<std::string>              ArrayOfITKIOClassName;
  ArrayOfITKIOClassName ITKIOClassNames;
#else
  typedef std::vector< std::string >              ArrayOfITKExtensionsType;
#endif

//  std::map< std::string, ITKImageFileFormat >    itkWriteFormatsMap;
  typedef itk::ImageIOBase                        IOBaseType;
  typedef std::list<itk::LightObject::Pointer>    ArrayOfImageIOType;
  ArrayOfImageIOType allobjects = itk::ObjectFactoryBase::CreateAllInstance("itkImageIOBase");

  ArrayOfImageIOType::iterator itr = allobjects.begin();
  while( itr != allobjects.end() )
  {
  IOBaseType * io = dynamic_cast< IOBaseType * >( itr->GetPointer() );
  if( ! io )
    {
    continue;
    }
  else
    {
#ifdef USE_TEMP_ITK_FILEFORMAT_TABLE
    std::string ioClassName = io->GetNameOfClass();
    ITKIOClassNames.insert(ioClassName);
#else
    const ArrayOfITKExtensionsType & writeExtensions = io->GetSupportedWriteExtensions();
    ArrayOfITKExtensionsType::const_iterator writeItr = writeExtensions.begin();

    while( writeItr != writeExtensions.end() )
      {
      // Once we have the GetSupportedWriteGenericNames() ready, we will used that.
      // For now, just use the class name.
      ITKImageFileFormat structFileFormat = 
        {io->GetNameOfClass(), io->GetNameOfClass(), io->GetNameOfClass(),(*writeItr).c_str()};
      this->AddSupportedWriterFileFormat(structFileFormat);      
      ++writeItr;
      }
#endif
    }
  itr++;
  }

#ifdef USE_TEMP_ITK_FILEFORMAT_TABLE
  for(int idx=0; idx<numFiles; idx++)
    {
    ITKImageFileFormat structFileFormat = FileFormatTable[idx];
    std::string ioClassName(structFileFormat.ClassName);
    if(ITKIOClassNames.find(ioClassName) != ITKIOClassNames.end())
      {
      this->AddSupportedWriterFileFormat(structFileFormat);      
      }
    }
#endif
}

//----------------------------------------------------------------------------
const char* vtkDataFileFormatHelper::GetFileExtensionFromFormatString(
  const char* format)
{
  std::string fileformat(format);
  std::string::size_type pos1 = fileformat.find("(");
  std::string::size_type pos2 = fileformat.find(")");
  if(pos1 != std::string::npos &&
    pos2 != std::string::npos )
    {
    std::string fileext = fileformat.substr(pos1+1, pos2-pos1-1);
    // remove leading and trailing space
    pos1 = fileext.find(".");
    if(pos1 != std::string::npos)
      {
      fileext = fileext.substr(pos1);
      }
    pos1 = fileext.find(' ');
    if(pos1 != std::string::npos)
      {
      fileext = fileext.substr(0, pos1);
      }
    static char buffer[20];
    sprintf(buffer, "%s", fileext.c_str());
    return buffer;
    //return fileext.c_str();
    }
  else
    {
    return NULL;
    }
}

//----------------------------------------------------------------------------
const char* vtkDataFileFormatHelper::GetClassNameFromFormatString(
  const char* format)
{
  std::string fileformat(format);
  const char* fileExt = vtkDataFileFormatHelper::GetFileExtensionFromFormatString(
    fileformat.c_str());
  // if no extension found
  if(!fileExt || !(*fileExt))
    {
    return NULL;
    }
  std::string::size_type pos1 = fileformat.find("(");
  if(pos1 != std::string::npos)
    {
    //remove trailing space too
    std::string genericName = fileformat.substr(0,pos1-1); 
  
    vtkStringArray* itkFileExtensions = this->GetITKSupportedWriteFileExtensions();

    for(int idx=0; idx<itkFileExtensions->GetNumberOfTuples(); idx++)
      {
      const char* extFormat = this->GetITKSupportedExtensionGenericNameByIndex(idx);
      std::string strExt = itkFileExtensions->GetValue(idx);
      
      if((!strExt.empty() && strcmp(strExt.c_str(), fileExt)==0)
        && (extFormat && strcmp(extFormat, genericName.c_str())==0))
        {
        std::string strClassName(
          this->GetITKSupportedExtensionClassNameByIndex(idx));
        return strClassName.c_str();
        }
      }

    }

  // if the file format is not expected.
  return NULL;
}

//----------------------------------------------------------------------------
vtkStringArray* vtkDataFileFormatHelper::GetITKSupportedWriteFileExtensions()
{
  if(!this->SupportedWriteFileExtensions || 
    !this->SupportedWriteFileGenericNames ||
    !this->SupportedWriteFileClassNames)
    {
    this->InitializeITKSupportedFileFormats();
    }

  return this->SupportedWriteFileExtensions;
}

//----------------------------------------------------------------------------
const char* vtkDataFileFormatHelper::GetITKSupportedExtensionGenericNameByIndex(
  int idx)
{
  if(!this->SupportedWriteFileExtensions || 
    !this->SupportedWriteFileGenericNames )
    {
    this->InitializeITKSupportedFileFormats();
    }

  if(idx < 0 || this->SupportedWriteFileGenericNames->GetNumberOfTuples() ==0 ||
    idx >= this->SupportedWriteFileGenericNames->GetNumberOfTuples() ||
    this->SupportedWriteFileExtensions->GetNumberOfTuples() != 
    this->SupportedWriteFileGenericNames->GetNumberOfTuples())
    {
    vtkErrorMacro(<< "Can not find the extension generic name by given index.");
    return NULL; 
    }

  return this->SupportedWriteFileGenericNames->GetValue(idx).c_str();
}

//----------------------------------------------------------------------------
const char* vtkDataFileFormatHelper::GetITKSupportedExtensionClassNameByIndex(
  int idx)
{
  if(!this->SupportedWriteFileExtensions ||
    !this->SupportedWriteFileClassNames)
    {
    this->InitializeITKSupportedFileFormats();
    }

  if(idx < 0 || this->SupportedWriteFileClassNames->GetNumberOfTuples() ==0 ||
    idx >= this->SupportedWriteFileClassNames->GetNumberOfTuples() ||
    this->SupportedWriteFileExtensions->GetNumberOfTuples() != 
    this->SupportedWriteFileClassNames->GetNumberOfTuples())
    {
    vtkErrorMacro(<< "Can not find the extension class name by given index.");
    return NULL; 
    }

  return this->SupportedWriteFileClassNames->GetValue(idx).c_str();
}

//----------------------------------------------------------------------------
void vtkDataFileFormatHelper::InitializeITKSupportedFileFormats()
{
  if(!this->SupportedWriteFileExtensions)
    {
    this->SupportedWriteFileExtensions = vtkStringArray::New();
    }  
  if(!this->SupportedWriteFileGenericNames)
    {
    this->SupportedWriteFileGenericNames = vtkStringArray::New();
    }
  if(!this->SupportedWriteFileClassNames)
    {
    this->SupportedWriteFileClassNames = vtkStringArray::New();
    }
  if(!this->ITKSupportedWriteFileFormats)
    {
    this->ITKSupportedWriteFileFormats = vtkStringArray::New();
    }

  this->ITKSupportedWriteFileFormats->Reset();
  this->ITKSupportedWriteFileFormats->SetNumberOfTuples(0);
  this->SupportedWriteFileExtensions->Reset();
  this->SupportedWriteFileExtensions->SetNumberOfTuples(0);
  this->SupportedWriteFileGenericNames->Reset();
  this->SupportedWriteFileGenericNames->SetNumberOfTuples(0);
  this->SupportedWriteFileClassNames->Reset();
  this->SupportedWriteFileClassNames->SetNumberOfTuples(0);

  this->PopulateITKSupportedWriteFileTypes();
}

//----------------------------------------------------------------------------
void vtkDataFileFormatHelper::AddSupportedWriterFileFormat(
 ITKImageFileFormat& structFileFormat)
{
  this->SupportedWriteFileExtensions->InsertNextValue(structFileFormat.Extension);
  this->SupportedWriteFileClassNames->InsertNextValue(structFileFormat.ClassName);
  this->SupportedWriteFileGenericNames->InsertNextValue(structFileFormat.GenericName);
  std::string strFormat(structFileFormat.GenericName);
  strFormat.append(" (").append(structFileFormat.Extension).append(")");
  this->ITKSupportedWriteFileFormats->InsertNextValue(strFormat);
}

//----------------------------------------------------------------------------
void vtkDataFileFormatHelper::PrintSelf(ostream& os, vtkIndent indent)
{
  Superclass::PrintSelf ( os, indent );
}
