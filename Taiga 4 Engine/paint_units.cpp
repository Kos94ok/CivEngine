
#include "main.h"
#include "window.h"
#include "game.h"
#include "settings.h"
#include "camera.h"
#include "visual.h"
#include "math.h"
#include "target.h"
#include "editor.h"
#include "prerender.h"

extern sf::RectangleShape brushRect;
extern sf::CircleShape brushCircle;
extern sf::Vertex vertex[4];
extern sf::VertexArray brushVertex;
extern sf::Text brushText;

void cWindow::paintUnits()
{
	bool cameraIntersection;
	sf::RenderStates renderState;

	// Random initialization
	cAnimDisplay* animDisplay;
	sf::FloatRect camRect(camera.pos.x, camera.pos.y, camera.res.x, camera.res.y);
	sf::FloatRect objRect;

	brushVertex.setPrimitiveType(sf::PrimitiveType::Quads);
	brushRect.setOutlineColor(sf::Color(0, 255, 0));

	visual.unitsPainted = 0;

	game.access.lock();
	mutex.renderUnits.lock();
	preRender.units.ready = false;
	int boundTexture = -1;

	int repeats = 1;
	if (settings.enableBetterShadows) { repeats += 1; }
	for (int u = 0; u < repeats; u++)
	{
		for (int i : preRender.units.queue)
		{
			// Unit animation
			game.unit[i].updateDisplayAnim();
			animDisplay = &game.unit[i].animDisplay;

			int anim = game.unit[i].anim.type;
			int curFrame = game.unit[i].anim.curFrame;
			int texWidth, texHeight, frameWidth;

			texWidth = visual.gameTex[animDisplay->data.tex].handle.getSize().x;
			texHeight = visual.gameTex[animDisplay->data.tex].handle.getSize().y;
			frameWidth = texWidth / animDisplay->data.frameCount;

			// Texture setup
			if (boundTexture != animDisplay->data.tex) {
				brushRect.setTexture(&visual.gameTex[animDisplay->data.tex].handle);
				boundTexture = animDisplay->data.tex;
			}
			//brushRect.setTexture(&visual.gameTex[animDisplay->data.tex].handle);
			// Frame setup
			if (animDisplay->texMod == 1) {
				brushRect.setTextureRect(sf::IntRect(curFrame * frameWidth, 0, frameWidth, texHeight));
			}
			else {
				brushRect.setTextureRect(sf::IntRect((curFrame + 1) * frameWidth, 0, -frameWidth, texHeight));
			}

			// Shadow (day)
			if (!game.unit[i].hasRef(REF_UNIT_NOSHADOW) && (u == 1 || !settings.enableBetterShadows))
			{
				float shadowAngleMod = 4.00f;
				float shadowScaleMod = 1.00f;
				float shadowScaleMin = 1.20f;
				float shadowAngle = 0.00f, shadowScale = 1.20f;

				float timeLocal = game.timeOfDay;
				float shadowBrightness = (game.ambientLight - 100.00f) / 255.00f * 210.00f;
				shadowBrightness = max(0.00f, min(255.00f, shadowBrightness));
				if (shadowBrightness > 0.00f)
				{
					shadowScale = abs(timeLocal - 12.00f) / 6.00f * shadowScaleMod + shadowScaleMin;
					shadowAngle = (timeLocal - 12.00f) * shadowAngleMod;

					visual.shadowBrightness = shadowBrightness;
					brushRect.setFillColor(sf::Color(0, 0, 0, shadowBrightness));
					if (settings.enableBetterShadows) {
						brushRect.setFillColor(sf::Color(0, 0, 0, 255.00f));
					}
					//brushRect.setPosition(game.unit[i].pos.x + game.unit[i].shadowOffset.x,
					//game.unit[i].pos.y + game.unit[i].shadowOffset.y);
					vec2 newPos = math.polar(game.unit[i].pos + game.unit[i].shadowOffset, game.unit[i].flyingHeight * shadowScale,
						shadowAngle * game.unit[i].flyingHeight / 10.00f - 90.00f);
					brushRect.setPosition(newPos);
					brushRect.setOrigin(game.unit[i].center.x, game.unit[i].center.y);
					brushRect.setSize(sf::Vector2f(game.unit[i].size.x, game.unit[i].size.y));
					brushRect.setScale(1.00f, shadowScale);
					brushRect.setRotation(shadowAngle);
					// Rotatable unit
					if (game.unit[i].hasRef(REF_UNIT_ROTATE)) {
						brushRect.setRotation(math.convertAngle(shadowAngle) - game.unit[i].facingAngle);
						brushRect.setScale(1.00f, 1.00f);
					}
					// Shadow texture override
					if (animDisplay->data.texShadow != -1) {
						brushRect.setTexture(&visual.gameTex[animDisplay->data.texShadow].handle);
						boundTexture = animDisplay->data.texShadow;
					}

					// Last-minute check
					objRect = brushRect.getGlobalBounds();
					if (camRect.intersects(objRect))
					{
						renderState.transform = window.matrixHandle;
						if (visual.shader[SHADER_SHADOW].isAvailable() && settings.shadowBlur > 1)
						{
							visual.shader[SHADER_SHADOW].setParameter("transpar", shadowBrightness / 255.00f);
							visual.shader[SHADER_SHADOW].setParameter("sampleCount", settings.shadowBlur);
							renderState.shader = &visual.shader[SHADER_SHADOW];
						}
						if (!settings.enableBetterShadows) { window.texHandle.draw(brushRect, renderState); }
						else { window.texHandleShadow.draw(brushRect, renderState); }
					}
				}

				// Shadow (night)
				//visual.updateLightLevel(game.unit[i].pos);
				shadowBrightness = -(game.ambientLight - 100.00f) / 255.00f * 210.00f * 1.50f;// - visual.maxLight / 10.00f;
				shadowBrightness = max(0.00f, min(255.00f, shadowBrightness));
				if (shadowBrightness > 0.00f && settings.enableNightShadows)
				{
					shadowAngle = 0.00f, shadowScale = 1.20f;
					if (timeLocal > 18.00f) { shadowScale = (24.00f - timeLocal) / 6.00f * shadowScaleMod + shadowScaleMin; }
					else { shadowScale = timeLocal / 6.00f * shadowScaleMod + shadowScaleMin; }

					if (timeLocal > 18.00f) { shadowAngle = -(24.00f - timeLocal) * shadowAngleMod; }
					else { shadowAngle = timeLocal * shadowAngleMod; }

					visual.shadowBrightness = shadowBrightness;
					brushRect.setFillColor(sf::Color(0, 0, 0, shadowBrightness));
					if (settings.enableBetterShadows) {
						brushRect.setFillColor(sf::Color(0, 0, 0, 255.00f));
					}
					vec2 newPos = math.polar(game.unit[i].pos + game.unit[i].shadowOffset, game.unit[i].flyingHeight * shadowScale,
						shadowAngle * game.unit[i].flyingHeight / 10.00f - 90.00f);
					brushRect.setPosition(newPos);
					brushRect.setOrigin(game.unit[i].center.x, game.unit[i].center.y);
					brushRect.setSize(sf::Vector2f(game.unit[i].size.x, game.unit[i].size.y));
					brushRect.setScale(1.00f, shadowScale);
					brushRect.setRotation(shadowAngle);
					// Rotatable unit
					if (game.unit[i].hasRef(REF_UNIT_ROTATE)) {
						brushRect.setRotation(math.convertAngle(shadowAngle) - game.unit[i].facingAngle);
						brushRect.setScale(1.00f, 1.00f);
					}
					// Shadow texture override
					if (animDisplay->data.texShadow != -1) {
						brushRect.setTexture(&visual.gameTex[animDisplay->data.texShadow].handle);
						boundTexture = animDisplay->data.texShadow;
					}
					renderState.transform = window.matrixHandle;

					// Last-minute check
					objRect = brushRect.getGlobalBounds();
					if (camRect.intersects(objRect))
					{
						if (visual.shader[SHADER_SHADOW].isAvailable() && settings.shadowBlur > 1)
						{
							visual.shader[SHADER_SHADOW].setParameter("transpar", shadowBrightness / 255.00f);
							visual.shader[SHADER_SHADOW].setParameter("sampleCount", settings.shadowBlur);
							renderState.shader = &visual.shader[SHADER_SHADOW];
						}
						if (!settings.enableBetterShadows) { window.texHandle.draw(brushRect, renderState); }
						else { window.texHandleShadow.draw(brushRect, renderState); }
					}
				}
			}

			// Display the object
			brushRect.setRotation(0.00f);
			brushRect.setScale(1.00f, 1.00f);
			brushRect.setFillColor(sf::Color(255, 255, 255));
			// Hover
			if (visual.hoveredUnit == game.unit[i].globalId && settings.enableUnitHighlightColor) {
				brushRect.setFillColor(settings.visualUnitHoverColor);
			}
			// No light
			if (!settings.enableDynamicLight) {
				brushRect.setFillColor(sf::Color(
					min(game.ambientLight, 255.0f),
					min(game.ambientLight, 255.0f),
					min(game.ambientLight, 255.0f)));
			}
			// Placeholder for building
			if (game.unit[i].hasRef(REF_UNIT_PLACEHOLDER) && target.activeBuild) {
				if (target.isBuildGood) { brushRect.setFillColor(sf::Color(0, 255, 0, 175)); }
				else { brushRect.setFillColor(sf::Color(255, 0, 0, 175)); }
			}
			brushRect.setPosition((game.unit[i].pos.x), (game.unit[i].pos.y));
			brushRect.setOrigin(game.unit[i].center.x, game.unit[i].center.y);
			brushRect.setSize(sf::Vector2f(game.unit[i].size.x, game.unit[i].size.y));
			// Editor selection
			if (editor.sel.isSelected(game.unit[i].globalId)) { brushRect.setOutlineThickness(-1.00f); }
			// Rotatable unit
			if (game.unit[i].hasRef(REF_UNIT_ROTATE)) { brushRect.setRotation(-game.unit[i].facingAngle); }
			// Shadow texture reset
			if (boundTexture != animDisplay->data.tex) {
				brushRect.setTexture(&visual.gameTex[animDisplay->data.tex].handle);
				boundTexture = animDisplay->data.tex;
			}
			// Last-minute check
			objRect = brushRect.getGlobalBounds();
			cameraIntersection = camRect.intersects(objRect);
			if (core.advancedDebug || cameraIntersection)
			{
				if (!settings.enableBetterShadows) { window.texHandle.draw(brushRect, window.matrixHandle); }
				else
				{
					if (u == 0) {
						// Unit outline
						if (game.unit[i].globalId == visual.hoveredUnit && settings.enableUnitHighlightOutline)
						{
							float offset = 2.00f;
							sf::RenderStates state;
							sf::Shader* shader = &visual.shader[SHADER_COLORBLUR];
							shader->setParameter("iSampleCount", 3.00f);
							shader->setParameter("sampleOffset", 0.003f);
							shader->setParameter("colorR", settings.visualUnitHoverOutline.r / 255.00f);
							shader->setParameter("colorG", settings.visualUnitHoverOutline.g / 255.00f);
							shader->setParameter("colorB", settings.visualUnitHoverOutline.b / 255.00f);
							state.transform = window.matrixHandle;
							state.shader = shader;

							float memOutlineThickness = brushRect.getOutlineThickness();
							brushRect.setOutlineThickness(0.00f);
							brushRect.setPosition((game.unit[i].pos.x) + offset, (game.unit[i].pos.y) + offset);
							window.texHandle.draw(brushRect, state);
							brushRect.setPosition((game.unit[i].pos.x) - offset, (game.unit[i].pos.y) + offset);
							window.texHandle.draw(brushRect, state);
							brushRect.setPosition((game.unit[i].pos.x) - offset, (game.unit[i].pos.y) - offset);
							window.texHandle.draw(brushRect, state);
							brushRect.setPosition((game.unit[i].pos.x) + offset, (game.unit[i].pos.y) - offset);
							window.texHandle.draw(brushRect, state);

							brushRect.setOutlineThickness(memOutlineThickness);
							brushRect.setFillColor(sf::Color(255, 255, 255));
							if (settings.enableUnitHighlightColor) {
								brushRect.setFillColor(settings.visualUnitHoverColor);
							}
							brushRect.setPosition((game.unit[i].pos.x), (game.unit[i].pos.y));
						}
						window.texHandle.draw(brushRect, window.matrixHandle);
					}
					else { window.texHandleShadow.draw(brushRect, window.matrixHandle); }
				}
				if (u == 0) { visual.unitsPainted += 1; }
			}
			brushRect.setRotation(0.00f);
			brushRect.setOutlineThickness(0.00f);
		}
	}
	mutex.renderUnits.unlock();
	game.access.unlock();
}