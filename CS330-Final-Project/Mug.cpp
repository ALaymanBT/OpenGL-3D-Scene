#include "Mug.h"


Mug::Mug() {

	genMug();
}

void Mug::genMug()
{
	// Create a cylinder for the body
	Cylinder cyl(120, 2.0f, 0.7f);
	cyl.genTexture("resources/textures/mugBase.jpg");
	cyl.genTexture("resources/textures/latteSpecular.jpg");
	cyl.genTexture("resources/textures/coffee.png");
	cylinder = cyl;


	// Create a torus for the handle
	Torus tor(40, 30, 0.6, 0.1);
	tor.genTexture("resources/textures/handle.jpg");
	tor.genTexture("resources/textures/mugSpecular.jpg");
	torus = tor;

	// Create circle for top of mug
	Mesh cir;
	cir.genCircle(30, 1.0f, 0.7f);
	cir.genTexture("resources/textures/latte.jpg");
	cir.genTexture("resources/textures/latteSpecular.jpg");
	circle = cir;

}

void Mug::Draw(Shader& shader, glm::mat4 proj, glm::mat4 view)
{
	//Position torus on side of cylinder and multiply the transformation matrices
	torus.translateMesh(0.7f, 0.0f, 0.0f);
	torus.rotateMesh(glm::radians(270.0f), 0.0f, 0.0f, 1.0f);
	torus.scaleMesh(1.3f, 1.0f, 0.8f);
	torus.updateTransformationMatrix(trans);
	


	// Multiply the transformation matrix with mug transformations
	cylinder.updateTransformationMatrix(trans);
	circle.updateTransformationMatrix(trans);

	// Draw the shapes, torus percent to draw is 0.5 as only half is needed for the handle
	cylinder.Draw(1, shader, proj, view);
	torus.Draw(0.5, shader, proj, view);
	circle.Draw(1, shader, proj, view);


	// Set transformation matrix to default
	this->trans = glm::mat4(1.0f);
}


