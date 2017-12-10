



#include<vtkJPEGReader.h>
#include<vtkRenderWindow.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkRenderer.h>
#include<vtkActor.h>
#include<vtkCamera.h>
#include<vtkImageActor.h>
#include<vtkImageData.h>
#include<vtkInteractorStyleImage.h>
#include<vtkImageCast.h>
#include<vtkImageAlgorithm.h>
#include<vtkImageProperty.h>
#include<vtkImageIslandRemoval2D.h>
#include<vtkImageMedian3D.h>
#include<vtkImageMapper3D.h>
#include<vtkImageRange3D.h>	


using namespace vtk;


int main(int argc, char** argv){
	//processing code
	vtkImageData* imageData;

	//Read the image
	vtkJPEGReader* jpegReader =	vtkJPEGReader::New();
	jpegReader->SetFileName("src.jpg");
	jpegReader->Update();
	jpegReader->SetDataScalarTypeToDouble();
	imageData = jpegReader->GetOutput();


	vtkImageIslandRemoval2D* filter =	vtkImageIslandRemoval2D::New();
	filter->SetAreaThreshold(1.0f);
	filter->SetIslandValue(0.0);
	filter->SetReplaceValue(200.0);
	filter->SetInputConnection(jpegReader->GetOutputPort());
	filter->Update();


	vtkImageMedian3D* medianFilter = vtkImageMedian3D::New();
	medianFilter->SetInputConnection(filter->GetOutputPort());
	medianFilter->SetKernelSize(3, 3, 1);
	medianFilter->Update();

	vtkImageRange3D* rangeFilter =	vtkImageRange3D::New();
	rangeFilter->SetInputConnection(medianFilter->GetOutputPort());
	rangeFilter->SetKernelSize(5, 5, 5);
	rangeFilter->Update();

	vtkImageCast* castFilter = vtkImageCast::New();
	castFilter->SetInputConnection(rangeFilter->GetOutputPort());
	castFilter->SetOutputScalarTypeToUnsignedChar();
	castFilter->Update();

	
	//display  codes
	vtkImageActor* imageActor =	vtkImageActor::New(); 
	imageActor->SetInputData(jpegReader->GetOutput());

	vtkImageActor* desimageActor = vtkImageActor::New();
	desimageActor->SetInputData(filter->GetOutput());
	
	double srcviewport[4] = { 0.0, 0.0, 0.5, 1.0 };
	double desiewport[4] = { 0.5, 0.0, 1.0, 1.0 };

	// Create a renderer to display the image in the background
	vtkRenderer* renderer =  vtkRenderer::New();
	vtkRenderer* desrenderer = vtkRenderer::New();
	renderer->AddActor(imageActor);
	renderer->SetViewport(srcviewport);
	renderer->ResetCamera();
	desrenderer->AddActor(desimageActor);
	desrenderer->SetViewport(desiewport);
	desrenderer->ResetCamera();


	vtkRenderWindow* renderWindow =	vtkRenderWindow::New();
	renderWindow->SetSize(750, 680);
	renderWindow->SetPosition(175, 15);
	renderWindow->SetMultiSamples(0);
	renderWindow->AddRenderer(renderer);
	renderWindow->AddRenderer(desrenderer);

	vtkRenderWindowInteractor* renderWindowInteractor =	vtkRenderWindowInteractor::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);	
	 
	vtkInteractorStyleImage* style = vtkInteractorStyleImage::New();
	renderWindowInteractor->SetInteractorStyle(style);
	renderWindow->Render();
	
	double origin[3];
	double spacing[3];
	int extent[6];
	imageData->GetOrigin(origin);
	imageData->GetSpacing(spacing);
	imageData->GetExtent(extent);

	vtkCamera* srccamera = renderer->GetActiveCamera();
	srccamera->ParallelProjectionOn();

	double xc = origin[0] + 0.5*(extent[0] + extent[1])*spacing[0];
	double yc = origin[1] + 0.5*(extent[2] + extent[3])*spacing[1];
	
	double yd = (extent[3] - extent[2] + 1)*spacing[1];
	double d = srccamera->GetDistance();
	srccamera->SetParallelScale(0.5*yd);
	srccamera->SetFocalPoint(xc, yc, 0.0);
	srccamera->SetPosition(xc, yc, d);
	srccamera->SetRoll(0.0);

	vtkCamera* descamera = desrenderer->GetActiveCamera();
	descamera->ParallelProjectionOn();

	 xc = origin[0] + 0.5*(extent[0] + extent[1])*spacing[0];
	 yc = origin[1] + 0.5*(extent[2] + extent[3])*spacing[1];

	 yd = (extent[3] - extent[2] + 1)*spacing[1];
	 d = descamera->GetDistance();
	 descamera->SetParallelScale(0.5*yd);
	 descamera->SetFocalPoint(xc, yc, 0.0);
	 descamera->SetPosition(xc, yc, d);
	 descamera->SetRoll(0.0);;

	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
	renderWindowInteractor->TerminateApp();


	return EXIT_SUCCESS;
}