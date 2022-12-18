#include "Biscuit.h"
#include "Mug.h"


Biscuit::Biscuit() {

	genBiscuit();
}

void Biscuit::genBiscuit()
{
	// Create a cylinder for the body
	Cylinder cyl(30, 0.1f, 0.5f);
	cyl.genTexture("resources/textures/biscuitSide.jpg");
	cyl.genTexture("resources/textures/noSpecular.jpg");
	cylinder = cyl;

	// Create circles for top and bottom of the biscuit
	Mesh cir1;
	cir1.genCircle(30, 0.05f, 0.5f);
	cir1.genTexture("resources/textures/biscuitTop.jpg");
	cir1.genTexture("resources/textures/noSpecular.jpg");
	circleTop = cir1;

	Mesh cir2;
	cir2.genCircle(30, -0.05f, 0.5f);
	cir2.genTexture("resources/textures/biscuitTop.jpg");
	cir2.genTexture("resources/textures/noSpecular.jpg");
	circleBottom = cir2;

}

void Biscuit::Draw(Shader& shader, glm::mat4 proj, glm::mat4 view)
{

	// Multiply the transformation matrix with mug transformations
	cylinder.updateTransformationMatrix(trans);
	circleTop.updateTransformationMatrix(trans);
	circleBottom.updateTransformationMatrix(trans);


	// Draw the shapes, torus percent to draw is 0.5 as only half is needed for the handle
	cylinder.Draw(1, shader, proj, view);
	circleTop.Draw(1, shader, proj, view);
	circleBottom.Draw(1, shader, proj, view);


	// Set transformation matrix to default
	this->trans = glm::mat4(1.0f);
}
