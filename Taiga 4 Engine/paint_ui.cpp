
#include "main.h"
#include "window.h"
#include "UI.h"
#include "settings.h"
#include "visual.h"
#include "math.h"
#include "target.h"
#include "database.h"
#include "game.h"
#include "camera.h"
#include "client.h"
#include "particle.h"
#include "weather.h"

extern sf::RectangleShape brushRect;
extern sf::CircleShape brushCircle;
extern sf::Vertex vertex[4];
extern sf::VertexArray brushVertex;
extern sf::Text brushText;

void cWindow::paintUI()
{
	if (!ui.displayed) { return; }
	ui.access.lock();

	float fade;
	string text;
	sf::FloatRect floatRect;
	brushText.setCharacterSize(16);
	sf::Transform miniMatrix;
	miniMatrix.scale(sf::Vector2f(settings.sampleMod, settings.sampleMod));
	// UI Elements
	for (int y = 0; y < LIMIT_PRIORITY_UI; y++)
	{
		for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
		{
			if (ui.element[i].isValid && ui.element[i].priority == y)
			{
				fade = ui.element[i].getFade();
				// Textured body
				// Normal
				if (ui.element[i].texture != -1)
				{
					brushRect.setFillColor(sf::Color(ui.element[i].bodyColor.r, ui.element[i].bodyColor.g, ui.element[i].bodyColor.b, ui.element[i].alpha * fade));
					brushRect.setOrigin(0.00f, 0.00f);
					if (!ui.element[i].ignoreOrigin) { brushRect.setOrigin(ui.element[i].size.x / 2, ui.element[i].size.y / 2); }
					brushRect.setPosition(ui.element[i].pos.x, ui.element[i].pos.y);
					brushRect.setSize(ui.element[i].size);
					brushRect.setTexture(&visual.gameTex[ui.element[i].texture].handle);
					brushRect.setTextureRect(sf::IntRect(0, 0, visual.gameTex[ui.element[i].texture].handle.getSize().x, visual.gameTex[ui.element[i].texture].handle.getSize().y));
					window.texHandleTop.draw(brushRect, miniMatrix);
				}
				// Hover
				if (ui.element[i].hoverAlpha > 0 && ui.element[i].textureHovered != -1)
				{
					brushRect.setFillColor(sf::Color(ui.element[i].hoverColor.r, ui.element[i].hoverColor.g, ui.element[i].hoverColor.b,
						min(255.00f, (float)ui.element[i].hoverAlpha * ((float)ui.element[i].alpha / 255.00f)) * fade));
					brushRect.setOrigin(0.00f, 0.00f);
					if (!ui.element[i].ignoreOrigin) { brushRect.setOrigin(ui.element[i].size.x / 2, ui.element[i].size.y / 2); }
					brushRect.setPosition(ui.element[i].pos.x, ui.element[i].pos.y);
					brushRect.setSize(ui.element[i].size);
					brushRect.setTexture(&visual.gameTex[ui.element[i].textureHovered].handle);
					brushRect.setTextureRect(sf::IntRect(0, 0, visual.gameTex[ui.element[i].textureHovered].handle.getSize().x, visual.gameTex[ui.element[i].textureHovered].handle.getSize().y));
					window.texHandleTop.draw(brushRect, miniMatrix);
				}
				// Bar
				if (ui.element[i].bar.texture != -1)
				{
					brushRect.setFillColor(sf::Color(255, 255, 255, ui.element[i].alpha * fade));
					brushRect.setOrigin(0.00f, 0.00f);
					if (!ui.element[i].ignoreOrigin) { brushRect.setOrigin(ui.element[i].size.x / 2, ui.element[i].size.y / 2); }
					brushRect.setTexture(&visual.gameTex[ui.element[i].bar.texture].handle);

					float barValue = max(0.00f, min(1.00f, ui.element[i].bar.value / ui.element[i].bar.maxValue));
					float rawBarValue = barValue;
					barValue = barValue * (1.00f - ui.element[i].bar.maxOffset);
					barValue = barValue + ui.element[i].bar.minOffset - rawBarValue * ui.element[i].bar.minOffset;
					//barValue = barValue - barValue * ui.element[i].bar.maxOffset;
					//barValue -= ui.element[i].bar.maxOffset * barValue;
					if (ui.element[i].bar.vertical) {
						brushRect.setPosition(ui.element[i].pos.x, ui.element[i].pos.y + ui.element[i].size.y);
						brushRect.setSize(sf::Vector2f(ui.element[i].size.x, -ui.element[i].size.y * barValue));
						brushRect.setTextureRect(sf::IntRect(0,
							math.round(visual.gameTex[ui.element[i].bar.texture].handle.getSize().y * (1.00 - barValue)),
							visual.gameTex[ui.element[i].bar.texture].handle.getSize().x,
							math.round(visual.gameTex[ui.element[i].bar.texture].handle.getSize().y * barValue)));
					}
					else {
						brushRect.setPosition(ui.element[i].pos.x, ui.element[i].pos.y);
						brushRect.setSize(sf::Vector2f(ui.element[i].size.x * barValue, ui.element[i].size.y));
						brushRect.setTextureRect(sf::IntRect(0, 0,
							math.round(visual.gameTex[ui.element[i].bar.texture].handle.getSize().x * barValue),
							visual.gameTex[ui.element[i].bar.texture].handle.getSize().y));
					}
					window.texHandleTop.draw(brushRect, miniMatrix);
				}
				// Text
				if (ui.element[i].hasText())
				{
					float textAlpha = (float)ui.element[i].hoverAlpha / 255.00f;
					if (ui.element[i].textFont == FONT_DESCR) { brushText.setFont(visual.fontDescr); }
					else { brushText.setFont(visual.fontMain); }
					brushText.setCharacterSize(ui.element[i].textSize);
					brushText.setString(ui.element[i].textDisplay);
					floatRect = brushText.getGlobalBounds();
					brushText.setOrigin(0.00f, 0.00f);
					//brushText.setStyle(sf::Text::Bold);
					if (!ui.element[i].ignoreOrigin) { brushText.setOrigin(math.round(floatRect.width / 2.00f), math.round(floatRect.height / 2.00f) + 4); }
					// Shadow/outline
					float shadowThickness = ui.element[i].textOutlineThickness;
					if (shadowThickness != 0.00f)
					{
						brushText.setColor(sf::Color(
							ui.element[i].textOutlineColor.r,
							ui.element[i].textOutlineColor.g,
							ui.element[i].textOutlineColor.b,
							ui.element[i].textOutlineColor.a * fade));
						brushText.setPosition(ui.element[i].pos + ui.element[i].textOffset + vec2f(shadowThickness, -shadowThickness));
						window.texHandleTop.draw(brushText, miniMatrix);

						brushText.setPosition(ui.element[i].pos + ui.element[i].textOffset + vec2f(shadowThickness, shadowThickness));
						window.texHandleTop.draw(brushText, miniMatrix);

						brushText.setPosition(ui.element[i].pos + ui.element[i].textOffset + vec2f(-shadowThickness, -shadowThickness));
						window.texHandleTop.draw(brushText, miniMatrix);

						brushText.setPosition(ui.element[i].pos + ui.element[i].textOffset + vec2f(-shadowThickness, shadowThickness));
						window.texHandleTop.draw(brushText, miniMatrix);
					}
					// Actual object
					brushText.setColor(sf::Color(
						ui.element[i].textColor.r * (1.00f - textAlpha) + ui.element[i].textColorHover.r * textAlpha,
						ui.element[i].textColor.g * (1.00f - textAlpha) + ui.element[i].textColorHover.g * textAlpha,
						ui.element[i].textColor.b * (1.00f - textAlpha) + ui.element[i].textColorHover.b * textAlpha,
						255.00f * fade));
					brushText.setPosition(ui.element[i].pos + ui.element[i].textOffset);
					window.texHandleTop.draw(brushText, miniMatrix);
				}
				// Tooltip
				ui.element[i].tooltip.display();
			}
		}
	}
	// Targeting
	/*if (target.active && !target.activeBuild)
	{
		brushRect.setTexture(&visual.gameTex[database.texture[TEX_MOUSE_TARGET]].handle, true);
		brushRect.setSize(vec2f(40, 40));
		brushRect.setOrigin(vec2f(20, 20));
		brushRect.setPosition(window.getMousePos(false));
		brushRect.setFillColor(color(255, 255, 255));
		window.texHandleTop.draw(brushRect);
	}*/
	// Progress bar
	if (visual.progress.getCurrentAlpha() > 0.00f)
	{
		vec2i barFrame = visual.progress.getCurrentFrame();
		vec2i barFrameSize = visual.progress.getTexFrameSize();
		// Background
		brushRect.setTexture(&visual.gameTex[database.texture[TEX_MOUSE_PROGRESSBG]].handle, true);
		brushRect.setSize(vec2f(barFrameSize.x, barFrameSize.y));
		brushRect.setOrigin(vec2f(barFrameSize.x / 2, barFrameSize.y / 2));
		brushRect.setPosition(window.getMousePos(false));
		brushRect.setFillColor(color(255, 255, 255, visual.progress.getCurrentAlpha() * 255.00f));
		window.texHandleTop.draw(brushRect);
		// Bar
		brushRect.setTexture(&visual.gameTex[database.texture[TEX_MOUSE_PROGRESS]].handle, true);
		brushRect.setFillColor(color(0, 0, 0, visual.progress.getCurrentAlpha() * 255.00f));
		brushRect.setTextureRect(sf::IntRect(vec2i(barFrame.x * barFrameSize.x, barFrame.y * barFrameSize.y), barFrameSize));
		window.texHandleTop.draw(brushRect);
	}
	ui.access.unlock();
}

void cWindow::paintDebugInfo()
{
	if (!core.debugMode) { return; }
	sf::Transform miniMatrix;
	miniMatrix.scale(sf::Vector2f(settings.sampleMod, settings.sampleMod));

	brushRect.setOrigin(0, 0);
	brushRect.setPosition(0, 0);
	brushRect.setSize(sf::Vector2f(150, 70));
	brushRect.setTexture(0);
	brushRect.setFillColor(sf::Color(0, 0, 0, 150));
	// Unit orders
	if (core.advancedDebug)
	{
		float dist, angle;
		brushCircle.setFillColor(sf::Color(0, 255, 0));
		//brushCircle.setOutlineThickness(-1.00f);
		brushCircle.setOutlineColor(sf::Color(0, 0, 0));
		for (int i = 0; i < game.unitCounter; i++)
		{
			// First order
			if (game.unit[i].orderCounter > 0)
			{
				dist = math.getDistance(game.unit[i].pos, game.unit[i].order[0].targetPos);
				angle = math.getAngle(game.unit[i].pos, game.unit[i].order[0].targetPos);
				brushRect.setSize(vec2(dist, 2.00f));
				brushRect.setOrigin(0.00f, 1.00f);
				brushRect.setPosition(game.unit[i].pos);
				brushRect.setRotation(angle);
				brushRect.setFillColor(sf::Color(0, 255, 0));
				//brushRect.setOutlineThickness(-1.00f);
				brushRect.setOutlineColor(sf::Color(0, 0, 0));
				window.texHandleTop.draw(brushRect, window.matrixHandle);
			}
			// Order queue
			for (int y = 0; y < game.unit[i].orderCounter; y++)
			{
				if (game.unit[i].order[y].type == ORDER_MOVETO && (game.unit[i].order[y + 1].type == ORDER_MOVETO
					|| y == game.unit[i].orderCounter - 1))
				{
					if (y < game.unit[i].orderCounter - 1)
					{
						dist = math.getDistance(game.unit[i].order[y].targetPos, game.unit[i].order[y + 1].targetPos);
						angle = math.getAngle(game.unit[i].order[y].targetPos, game.unit[i].order[y + 1].targetPos);
						brushRect.setSize(vec2(dist, 2.00f));
						brushRect.setPosition(game.unit[i].order[y].targetPos);
						brushRect.setRotation(angle);
						window.texHandleTop.draw(brushRect, window.matrixHandle);
					}
					brushCircle.setPosition(game.unit[i].order[y].targetPos);
					brushCircle.setRadius(5.00f);
					brushCircle.setOrigin(5.00f, 5.00f);
					window.texHandleTop.draw(brushCircle, window.matrixHandle);
				}
			}
			brushCircle.setPosition(game.unit[i].pos);
			brushCircle.setRadius(game.unit[i].collisionDistance);
			brushCircle.setOrigin(game.unit[i].collisionDistance, game.unit[i].collisionDistance);
			window.texHandleTop.draw(brushCircle, window.matrixHandle);
		}
		brushRect.setRotation(0.00f);
		brushRect.setOutlineThickness(0.00f);
	}

	// Camera rect
	if (core.advancedDebug)
	{
		brushRect.setPosition(camera.pos);
		brushRect.setFillColor(sf::Color(0, 0, 0, 0));
		brushRect.setOutlineColor(sf::Color(0, 255, 0, 255));
		brushRect.setSize(vec2f(camera.res.x, camera.res.y));
		brushRect.setOutlineThickness(-10.00f);
		window.texHandleTop.draw(brushRect, window.matrixHandle);
		brushRect.setScale(vec2f(1, 1));
		brushRect.setOutlineThickness(0);
	}

	// Debug text
	string text;
	brushText.setCharacterSize(16);
	brushText.setOrigin(0, 0);
	brushText.setColor(sf::Color(255, 255, 255, 255));
	brushText.setFont(visual.fontMain);

	// Overlay
	brushRect.setOrigin(0, 0);
	brushRect.setPosition(0, 0);
	brushRect.setSize(sf::Vector2f(160, 220));
	brushRect.setTexture(0);
	brushRect.setFillColor(sf::Color(0, 0, 0, 150));
	window.texHandleTop.draw(brushRect, miniMatrix);

	// FPS (Window thread)
	brushText.setPosition(5, 0);
	text = "FPS: " + to_string(core.thread_windowTicksPerSec);
	brushText.setString(text);
	window.texHandleTop.draw(brushText, miniMatrix);

	// Timers (Server world thread)
	brushText.setPosition(5, 20);
	text = "World: " + to_string(core.thread_serverWorldTicksPerSec);
	brushText.setString(text);
	window.texHandleTop.draw(brushText, miniMatrix);

	// Timers (Animation thread)
	brushText.setPosition(5, 40);
	text = "Animation: " + to_string(core.thread_animWorldTicksPerSec);
	brushText.setString(text);
	window.texHandleTop.draw(brushText, miniMatrix);

	// Server ping
	brushText.setPosition(5, 60);
	text = "Ping: " + to_string(client.ping) + " ms";
	brushText.setString(text);
	window.texHandleTop.draw(brushText, miniMatrix);

	// Unit count
	brushText.setPosition(5, 90);
	text = "Units: " + to_string(visual.unitsPainted) + " / " + to_string(game.unitCounter);
	brushText.setString(text);
	window.texHandleTop.draw(brushText, miniMatrix);

	// Cloud count
	brushText.setPosition(5, 110);
	text = "Clouds: " + to_string(visual.cloudsPainted) + " / " + to_string((int)weather.cloud.size());
	brushText.setString(text);
	window.texHandleTop.draw(brushText, miniMatrix);

	// Particle count
	brushText.setPosition(5, 130);
	text = "Particles: " + to_string(visual.particlesPainted) + " / " + to_string(particle.unitCounter);
	brushText.setString(text);
	window.texHandleTop.draw(brushText, miniMatrix);

	// Snow power
	brushText.setPosition(5, 160);
	text = "Snow power: " + to_string(math.round(weather.power[WEATHER_SNOW]));
	brushText.setString(text);
	window.texHandleTop.draw(brushText, miniMatrix);

	// Wind power
	brushText.setPosition(5, 180);
	text = "Wind power: " + to_string(math.round(weather.windPower));
	brushText.setString(text);
	window.texHandleTop.draw(brushText, miniMatrix);

	// Cloud density
	brushText.setPosition(5, 200);
	text = "Cloud density: " + to_string(math.round(weather.cloudDensity));
	brushText.setString(text);
	window.texHandleTop.draw(brushText, miniMatrix);
}

void cWindow::paintConsole()
{
	if (!console.displayed) { return; }
	sf::Transform miniMatrix;
	miniMatrix.scale(sf::Vector2f(settings.sampleMod, settings.sampleMod));

	brushRect.setOrigin(0, 0);
	brushRect.setPosition(0, 0);
	brushRect.setSize(vec2f(camera.res.x, camera.res.y));
	brushRect.setTexture(0);
	brushRect.setFillColor(sf::Color(0, 0, 0, 235));
	if (core.serverMode) { brushRect.setFillColor(sf::Color(0, 0, 0, 255)); }
	window.texHandleTop.draw(brushRect, miniMatrix);

	brushText.setFont(visual.fontConsole);
	brushText.setCharacterSize(settings.consoleFontSize);
	brushText.setColor(sf::Color(0, 255, 0));
	brushText.setPosition(vec2f(5.00f, camera.res.y - settings.consoleFontSize - 5));
	brushText.setOrigin(vec2f(0.00f, 0.00f));
	brushText.setString("> " + console.inputDisplay + "_");
	window.texHandleTop.draw(brushText, miniMatrix);

	for (int i = max(0, (int)console.history[console.displayedPage].size() - console.getLineCount() - console.scrollOffset); i < (int)console.history[console.displayedPage].size() - max(0, console.scrollOffset); i++)
	{
		console.access.lock();
		brushText.setPosition(vec2f(5.00f, (i - max(0, (int)console.history[console.displayedPage].size() - console.getLineCount()) + console.scrollOffset) *
			(settings.consoleFontSize + settings.consoleLineSpacing)));
		brushText.setString(console.history[console.displayedPage][i]);
		window.texHandleTop.draw(brushText, miniMatrix);
		console.access.unlock();
	}
}

void cWindow::paintMousePointer()
{
	// Mouse pointer
	if (visual.mouse.type != POINTER_HARDWARE)
	{
		vec2i texSize = visual.mouse.getTexSize();
		// Alternate texture is a mouse button is pressed
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && !sf::Mouse::isButtonPressed(sf::Mouse::Right)) { brushRect.setTexture(&visual.gameTex[visual.mouse.texture].handle, true); }
		else { brushRect.setTexture(&visual.gameTex[visual.mouse.texturePress].handle, true); }
		brushRect.setSize(vec2f(texSize.x, texSize.y));
		brushRect.setOrigin(visual.mouse.getOrigin());
		brushRect.setPosition(window.getMousePos(false));
		brushRect.setFillColor(color(255, 255, 255));
		window.texHandleTop.draw(brushRect);
	}
}

void cUITooltip::display()
{
	if (text.length() == 0 || alpha <= 0.00f) { return; }

	sf::FloatRect floatRect;
	sf::Transform miniMatrix;
	miniMatrix.scale(sf::Vector2f(settings.sampleMod, settings.sampleMod));

	// Initializing the text
	//brushText.setString(sf::String(text, locale("russian")));
	brushText.setString(textDisplay);
	brushText.setPosition(math.round(pos.x + offset.x), math.round(pos.y + offset.y));
	brushText.setOrigin(0.00f, 0.00f);
	brushText.setCharacterSize(16);
	if (!ignoreOrigin) {
		brushText.setOrigin(math.round(brushText.getGlobalBounds().width / 2), 0);
	}
	brushText.setColor(sf::Color(255, 255, 255, alpha));
	// Bound to window rect
	floatRect = brushText.getGlobalBounds();
	if (floatRect.left - 3 < 0) { brushText.move(-floatRect.left + 3, 0.00f); }
	else if (floatRect.left + floatRect.width + 3 > camera.res.x) { brushText.move(camera.res.x - floatRect.left - floatRect.width - 3, 0.00f); }
	if (floatRect.top - 3 < 0) { brushText.move(0.00f, -floatRect.top + 3); }
	// Drawing text backgrund
	floatRect = brushText.getGlobalBounds();
	brushRect.setPosition(floatRect.left - 3, floatRect.top - 3);
	brushRect.setOrigin(0, 0);
	brushRect.setSize(sf::Vector2f(floatRect.width + 6, floatRect.height + 6));
	brushRect.setFillColor(sf::Color(0, 0, 0, alpha));
	brushRect.setTexture(0);
	brushRect.setOutlineColor(sf::Color(190, 190, 190, alpha));
	brushRect.setOutlineThickness(-1.00f);
	//brushRect.setTexture(&visual.gameTex[database.texture[TEX_UI_TOOLTIP]].handle);
	/*brushRect.setTextureRect(sf::IntRect(0, 0, visual.gameTex[database.texture[TEX_UI_TOOLTIP]].handle.getSize().x,
	visual.gameTex[database.texture[TEX_UI_TOOLTIP]].handle.getSize().y));*/
	window.texHandleTop.draw(brushRect, miniMatrix);

	brushRect.setOutlineThickness(0.00f);
	// Drawing the text on top
	window.texHandleTop.draw(brushText, miniMatrix);
}