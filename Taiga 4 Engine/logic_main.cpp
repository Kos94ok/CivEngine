
#include "main.h"
#include "logic.h"
#include "math.h"
#include "settings.h"
#include "window.h"
#include "UI.h"
#include "chat.h"
#include "game.h"
#include "camera.h"
#include "visual.h"
#include "client.h"
#include "server.h"

void cGameLogic::updateUI(int elapsedTime)
{
	int id = -1;
	int hoverHashSum = 0;
	bool bindToMouse = math.intToBool(settings.enableDynamicTooltips);
	bool isHovered = false;
	sf::Packet data;

	float timevar = (float)elapsedTime / 1000;
	timevar *= core.timeModifier;

	//sf::Vector2i mousePos = sf::Mouse::getPosition(window.winHandle);
	sf::Vector2f mousePos = window.getMousePos();

	ui.mouseNotMoving += timevar;
	ui.updateInterface();

	// Click timer
	if (ui.clickTimer > 0.00f)
	{
		ui.clickTimer -= timevar;
		if (ui.clickTimer < 0.00f) { ui.clickTimer = 0.00f; }
	}
	
	// Elements
	for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
	{
		if (ui.element[i].isValid)
		{
			isHovered = false;
			if (!chat.inFocus && !console.displayed)
			{
				// Regular buttons
				if (ui.element[i].hasRef(REF_UI_INVENTORY_BUTTON) && sf::Keyboard::isKeyPressed(settings.hkInventory))
				{
					isHovered = true;
				}
				// Active buttons
				for (int y = 1; y < LIMIT_ACTIVEBUTTONS; y++)
				{
					if (ui.element[i].hasRef(REF_UI_ACTIVEITEM + y) && sf::Keyboard::isKeyPressed(settings.hkActiveItem[y]))
					{
						isHovered = true;
					}
				}
			}
			if (!isHovered)
			{
				if (ui.element[i].ignoreOrigin)
				{
					isHovered = math.isPointInRect(mousePos.x, mousePos.y, ui.element[i].pos.x,
						ui.element[i].pos.y, ui.element[i].size.x, ui.element[i].size.y);
				}
				else
				{
					isHovered = math.isPointInRect(mousePos.x, mousePos.y, ui.element[i].pos.x - ui.element[i].size.x / 2,
						ui.element[i].pos.y - ui.element[i].size.y / 2, ui.element[i].size.x, ui.element[i].size.y);
				}
			}

			// Fadeout timers
			if (ui.element[i].fadeTimerMax > 0.00f && ui.element[i].fadeType != FADE_STOP)
			{
				ui.element[i].fadeTimer += timevar * ui.element[i].fadeType;
				if (ui.element[i].fadeTimer >= ui.element[i].fadeTimerMax && ui.element[i].fadeType == FADE_IN) {
					ui.element[i].fadeTimer = ui.element[i].fadeTimerMax;
					ui.element[i].fadeType = FADE_STOP;
				}
				else if (ui.element[i].fadeTimer <= 0.00f && ui.element[i].fadeType == FADE_OUT) {
					ui.removeElement(ui.element[i].globalId);
				}
			}

			// Tooltips
			if (ui.element[i].tooltip.hasText())
			{
				if (isHovered && ui.mouseNotMoving > ui.element[i].tooltip.delay && ui.element[i].tooltip.alpha <= 255)
				{
					// Update position
					if (!bindToMouse)
					{
						ui.element[i].tooltip.pos = ui.element[i].pos + ui.element[i].tooltip.offset;
					}
					else if (ui.element[i].tooltip.delay <= 0.00 || ui.element[i].tooltip.alpha == 0.00f)  {
						ui.element[i].tooltip.pos = sf::Vector2f(mousePos.x, mousePos.y) + ui.element[i].tooltip.offset;
					}
					// Add alpha
					ui.element[i].tooltip.alpha += timevar * 255 * 8;
					if (ui.element[i].tooltip.alpha > 255) { ui.element[i].tooltip.alpha = 255; }
				}
				else if (ui.element[i].tooltip.alpha > 0)
				{
					// Update position
					if (bindToMouse && ui.element[i].tooltip.delay <= 0.00)  {
						ui.element[i].tooltip.pos = sf::Vector2f(mousePos.x, mousePos.y);
					}
					// Remove alpha
					ui.element[i].tooltip.alpha -= timevar * 255 * 8;
					if (ui.element[i].tooltip.alpha < 0) { ui.element[i].tooltip.alpha = 0; }
				}
			}
		}
		// Hover
		if (ui.contextMenuTarget == CONTEXTMENU_NOTHING || ui.element[i].ref[REF_UI_CONTEXTMENU])
		{
			ui.element[i].isHovered = isHovered;
			if (isHovered) { hoverHashSum += ui.element[i].globalId; }
			if (ui.element[i].textureHovered != -1)
			{
				if (isHovered) { ui.element[i].hoverAlpha = 255; }
				else if (ui.element[i].hoverAlpha > 0.00f)
				{
					ui.element[i].hoverAlpha -= timevar * 255 * 2;
					if (ui.element[i].hoverAlpha < 0.00f) { ui.element[i].hoverAlpha = 0.00f; }
				}
			}
		}
	}
	// Hover with context menu
	if (ui.contextMenuTarget != CONTEXTMENU_NOTHING)
	{
		for (int i = 0; i < LIMIT_UI_ELEMENTS; i++)
		{
			if (ui.element[i].isValid && ui.element[i].button.args[0] == to_string(ui.contextMenuTargetId))
			{
				//ui.element[i].hoverAlpha = 255;
				i = LIMIT_UI_ELEMENTS;
			}
		}
	}
	// Hover update
	if (hoverHashSum != ui.hoverHashSum)
	{
		ui.updateInterfaceItemDescr();
	}
	ui.hoverHashSum = hoverHashSum;
	// Inventory bg alpha
	if (ui.invOpened)
	{
		id = ui.findByRef(REF_UI_INVENTORY_FULL_BG);
		if (id != -1) {
			ui.element[id].alpha = 100.00f * game.ambientLight / 255.00f + 100.00f;
		}
	}
	// Camera move
	camera.moveUpdateTimer -= timevar;
	if (camera.moveUpdateTimer < 0.00f) { camera.moveUpdateTimer = 0.00f; }
	float camSpeed = 1000.00f / camera.zoomFactor;
	float camVecModIn = 5.00f;
	float camVecModOut = 5.00f;
	float finalSpeed;
	mousePos = window.getMousePos();
	//camera.moveVector = vec2(0.00f, 0.00f);
	if (GetForegroundWindow() == window.winHandle.getSystemHandle() && !console.displayed && !chat.inFocus)
	{
		bool isMoving = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(settings.hkCamMove[2])
			|| (settings.enableMouseScroll && mousePos.x < 1.00f)
			|| (sf::Mouse::isButtonPressed(sf::Mouse::Middle) && mousePos.x - visual.mouseWheelScrollAnchor.x < -5.00f))
		{
			camSpeed = value.scrollSpeed_keyboard_normal;
			if (mousePos.y > camera.res.y - 2.00f) { camSpeed = value.scrollSpeed_mouse_normal; }
			if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) { camSpeed = -value.scrollSpeed_mouseWheel_normal * (mousePos.x - visual.mouseWheelScrollAnchor.x); }

			finalSpeed = -camSpeed * timevar / core.timeModifier;
			// Check for undefined chunks
			if (camera.isCameraMoveValid(vec2f(finalSpeed, 0.00f)))
			{
				camera.move(sf::Vector2f(finalSpeed, 0.00f));
				camera.moveVector.x -= 1.00f * timevar * camVecModIn;
				if (camera.moveVector.x < -1.00f) { camera.moveVector.x = -1.00f; }
				isMoving = true;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(settings.hkCamMove[3])
			|| (settings.enableMouseScroll && mousePos.x > camera.res.x - 2.00f)
			|| (sf::Mouse::isButtonPressed(sf::Mouse::Middle) && mousePos.x - visual.mouseWheelScrollAnchor.x > 5.00f))
		{
			camSpeed = value.scrollSpeed_keyboard_normal;
			if (mousePos.y > camera.res.y - 2.00f) { camSpeed = value.scrollSpeed_mouse_normal; }
			if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) { camSpeed = value.scrollSpeed_mouseWheel_normal * (mousePos.x - visual.mouseWheelScrollAnchor.x); }

			finalSpeed = camSpeed * timevar / core.timeModifier;
			// Check for undefined chunks
			if (camera.isCameraMoveValid(vec2f(finalSpeed, 0.00f)))
			{
				camera.move(sf::Vector2f(finalSpeed, 0.00f));
				camera.moveVector.x += 1.00f * timevar * camVecModIn;
				if (camera.moveVector.x > 1.00f) { camera.moveVector.x = 1.00f; }
				isMoving = true;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(settings.hkCamMove[0])
			|| (settings.enableMouseScroll && mousePos.y < 1.00f)
			|| (sf::Mouse::isButtonPressed(sf::Mouse::Middle) && mousePos.y - visual.mouseWheelScrollAnchor.y < -5.00f))
		{
			camSpeed = value.scrollSpeed_keyboard_normal;
			if (mousePos.y > camera.res.y - 2.00f) { camSpeed = value.scrollSpeed_mouse_normal; }
			if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) { camSpeed = -value.scrollSpeed_mouseWheel_normal * (mousePos.y - visual.mouseWheelScrollAnchor.y); }

			finalSpeed = -camSpeed * timevar / core.timeModifier;
			// Check for undefined chunks
			if (camera.isCameraMoveValid(vec2f(0.00f, finalSpeed)))
			{
				camera.move(sf::Vector2f(0.00f, finalSpeed));
				camera.moveVector.y += 1.00f * timevar * camVecModIn;
				if (camera.moveVector.y > 1.00f) { camera.moveVector.y = 1.00f; }
				isMoving = true;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(settings.hkCamMove[1])
			|| (settings.enableMouseScroll && mousePos.y > camera.res.y - 2.00f)
			|| (sf::Mouse::isButtonPressed(sf::Mouse::Middle) && mousePos.y - visual.mouseWheelScrollAnchor.y > 5.00f))
		{
			camSpeed = value.scrollSpeed_keyboard_normal;
			if (mousePos.y > camera.res.y - 2.00f) { camSpeed = value.scrollSpeed_mouse_normal; }
			if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) { camSpeed = value.scrollSpeed_mouseWheel_normal * (mousePos.y - visual.mouseWheelScrollAnchor.y); }

			finalSpeed = camSpeed * timevar / core.timeModifier;
			// Check for undefined chunks
			if (camera.isCameraMoveValid(vec2f(0.00f, finalSpeed)))
			{
				camera.move(sf::Vector2f(0.00f, finalSpeed));
				camera.moveVector.y -= 1.00f * timevar * camVecModIn;
				if (camera.moveVector.y < -1.00f) { camera.moveVector.y = -1.00f; }
				isMoving = true;
			}
		}
		// 
		if (!isMoving)
		{
			if (camera.moveVector.x > 0.00f) {
				camera.moveVector.x -= 1.00f * timevar * camVecModOut;
				if (camera.moveVector.x < 0.00f) { camera.moveVector.x = 0.00f; }
			}
			if (camera.moveVector.x < 0.00f) {
				camera.moveVector.x += 1.00f * timevar * camVecModOut;
				if (camera.moveVector.x > 0.00f) { camera.moveVector.x = 0.00f; }
			}
			if (camera.moveVector.y > 0.00f) {
				camera.moveVector.y -= 1.00f * timevar * camVecModOut;
				if (camera.moveVector.y < 0.00f) { camera.moveVector.y = 0.00f; }
			}
			if (camera.moveVector.y < 0.00f) {
				camera.moveVector.y += 1.00f * timevar * camVecModOut;
				if (camera.moveVector.y > 0.00f) { camera.moveVector.y = 0.00f; }
			}
		}
	}
	// Chat auto hide
	if (chat.displayed && !chat.inFocus)
	{
		chat.noFocusTimer -= timevar;
		if (chat.noFocusTimer <= 0.00f) {
			chat.hide();
		}
	}
	// Progress bar
	if (visual.progress.enabled && !visual.progress.manualMode)
	{
		visual.progress.timeCurrent += timevar;
		if (visual.progress.timeCurrent > visual.progress.timeMaximum)
		{
			visual.progress.timeCurrent = visual.progress.timeMaximum;
			visual.disableProgressBar();
		}
	}
	// Progress bar fade
	if (visual.progress.enabled && visual.progress.fadeTimer < value.progressBarFadeInTimer) {
		visual.progress.fadeTimer += timevar;
		if (visual.progress.fadeTimer > value.progressBarFadeInTimer) { visual.progress.fadeTimer = value.progressBarFadeInTimer; }
	}
	else if (!visual.progress.enabled && visual.progress.fadeTimer > 0.00f) {
		visual.progress.fadeTimer -= timevar;
		if (visual.progress.fadeTimer < 0.00f) { visual.progress.fadeTimer = 0.00f; }
	}
	// Double click timer
	if (visual.lastMouseClickTimer > 0.00f) {
		visual.lastMouseClickTimer -= timevar;
		if (visual.lastMouseClickTimer < 0.00f) { visual.lastMouseClickTimer = 0.00f; }
	}
}

void cGameLogic::updateConnection(int elapsedTime)
{
	sf::Packet data;
	// Ping
	client.pingTimer += elapsedTime;
	server.pingTimer += elapsedTime;
	// Other timers
	server.historyTimer += elapsedTime;
	
	// Sending the mouse position
	vec2f mousePos = window.getMousePos(true);
	if (client.connected && mousePos != client.lastMousePos && client.mousePosTimer >= 10)
	{
		client.mousePosTimer = 0;
		client.lastMousePos = mousePos;
		// Sending mouse position
		data << MSG_INFO_MOUSEPOS << mousePos.x << mousePos.y;
		client.sendPacket(data);
		data.clear();
	}
	else if (client.mousePosTimer < 10) {
		client.mousePosTimer += elapsedTime;
	}

	// Mouse dragging move order
	if (client.connected && mousePos != client.lastMoveOrderPos && client.moveOrderTimer >= 50 && ui.mouseStateLMB == MOUSE_CONTROLS_MOVEMENT)
	{
		client.moveOrderTimer = 0;
		client.lastMoveOrderPos = mousePos;
		// Sending the packet
		data << MSG_CONTROLS_MOVETO << mousePos.x << mousePos.y << false;
		client.sendPacket(data);
		data.clear();
	}
	else if (client.mousePosTimer < 50) {
		client.moveOrderTimer += elapsedTime;
	}
}

void gameLogicMain()
{
	int threadId = 1;
	console << "[LOGIC] Starting the main logic thread" << " [ID: " << threadId << "]" << "\n";
	srand(time(0));
	int elapsedTime, globalTime = 0;
	while (!core.thread_shutdown[threadId])
	{
		elapsedTime = timeGetTime() - globalTime;
		globalTime = timeGetTime();
		// Ignore system time change or huge lags
		if (elapsedTime > 0 && elapsedTime < 100)
		{
			gamelogic.updateUI(elapsedTime);
			gamelogic.updateConnection(elapsedTime);
			if (!core.paused)
			{
				gamelogic.updateOrders(elapsedTime);
				gamelogic.updateUnits(elapsedTime);
				gamelogic.updateParticles(elapsedTime);
				gamelogic.updateSurvivalStats(elapsedTime);
			}

			core.thread_serverWorldTicks += 1;
			core.thread_antifreeze[threadId] = 0;
		}
		Sleep(3);
	}
	console << "[LOGIC] Cleaning up" << "\n";
}

void animationMain()
{
	int threadId = 11;
	console << "[ANIM] Starting the animation thread" << " [ID: " << threadId << "]" << "\n";
	int elapsedTime, globalTime = 0;
	while (!core.thread_shutdown[threadId])
	{
		elapsedTime = timeGetTime() - globalTime;
		globalTime = timeGetTime();
		// Ignore system time change or huge lags
		if (elapsedTime > 0 && elapsedTime < 100)
		{
			if (!core.paused)
			{
				gamelogic.updateAnim(elapsedTime);
			}

			core.thread_animWorldTicks += 1;
			core.thread_antifreeze[threadId] = 0;
		}
		Sleep(1);
	}
	console << "[LOGIC] Cleaning up" << "\n";
}