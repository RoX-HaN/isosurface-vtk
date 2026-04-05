#include <cmath>
#include <vtkActor.h>
#include <vtkFlyingEdges3D.h>
#include <vtkImageData.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

int main() {
  int dims = 40;
  double range = 16.0;
  double spacing = range / (dims - 1);

  // 1. Create the Volumetric Data (The Grid)
  auto imageData = vtkSmartPointer<vtkImageData>::New();
  imageData->SetDimensions(dims, dims, dims);
  imageData->SetSpacing(spacing, spacing, spacing);
  imageData->SetOrigin(-8.0, -8.0, -8.0);
  imageData->AllocateScalars(VTK_DOUBLE, 1);

  // 2. Fill with your function: sin(xyz) / xyz
  for (int z = 0; z < dims; z++) {
    for (int y = 0; y < dims; y++) {
      for (int x = 0; x < dims; x++) {
        double *pixel =
            static_cast<double *>(imageData->GetScalarPointer(x, y, z));
        double posX = -8.0 + x * spacing;
        double posY = -8.0 + y * spacing;
        double posZ = -8.0 + z * spacing;

        double arg = posX * posY * posZ;
        *pixel = (arg == 0) ? 1.0 : std::sin(arg) / arg;
      }
    }
  }

  // 3. Extract the Isosurface (The "Marching Cubes" part)
  auto surface = vtkSmartPointer<vtkFlyingEdges3D>::New();
  surface->SetInputData(imageData);
  surface->SetValue(0, 0.1); // isomin
  surface->SetValue(1, 0.5); // isomax

  // 4. Setup Rendering Pipeline
  auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(surface->GetOutputPort());
  mapper->ScalarVisibilityOn();

  auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetOpacity(0.5);

  // 5. Create Window and Display
  auto colors = vtkSmartPointer<vtkNamedColors>::New();
  auto renderer = vtkSmartPointer<vtkRenderer>::New();
  auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->SetWindowName("C++ Isosurface Plot");

  auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);
  renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());

  renderWindow->Render();
  interactor->Start();

  return 0;
}
