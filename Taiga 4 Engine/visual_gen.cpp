
#include "main.h"
#include "visual.h"
#include "database.h"
#include "math.h"

sf::RectangleShape brushRectGen;
int cVisual::genHighlighted(string filename)
{
	addTexture(filename);
}

int cVisual::genCloudTexture(vec2f size, vec2f pieceMin, vec2f pieceMax, int pieceCount)
{
	renderCloud.clear(color(0, 0, 0, 0));
	float randVal;
	vec2f tempSize;
	sf::Vector2u texSize = visual.gameTex[database.texture[TEX_CLOUD]].handle.getSize();
	for (int i = 0; i < pieceCount; i++)
	{
		vec2i frameSize = vec2i(texSize.x / 4, texSize.y / 4);
		brushRectGen.setTexture(&visual.gameTex[database.texture[TEX_CLOUD]].handle);
		brushRectGen.setTextureRect(sf::IntRect(vec2i(math.rand(0, 3) * frameSize.x, math.rand(0, 3) * frameSize.y), frameSize));
		//tempSize = vec2f(math.randf(pieceMin.x, pieceMax.x),
			//math.randf(pieceMin.y, pieceMax.y));
		randVal = math.randf(pieceMin.x, pieceMax.x);
		tempSize = vec2f(randVal, randVal);
		brushRectGen.setSize(tempSize);
		brushRectGen.setPosition(vec2f(math.randf(tempSize.x / 2.00f, size.x - tempSize.x / 2.00f),
			math.randf(tempSize.y / 2.00f, size.y - tempSize.y / 2.00f)));
		brushRectGen.setOrigin(tempSize / 2.00f);
		brushRectGen.setRotation(math.rand(0, 3) * 90.00f);
		renderCloud.draw(brushRectGen);
	}

	renderCloud.display();
	return visual.addTexture(renderCloud.getTexture());
}