#include "guitoolkit.hpp"


void doNothing(Entity& _entity, System& _system) {};

GuiToolkit::GuiToolkit(System& _system) : 
	system(_system), 
	planeMesh(*utils::MeshLoader::get("models/plane.obj")),
	redTexture(*graphics::Texture2DManager::get("textures/red.png",
		graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
	blackTexture(*graphics::Texture2DManager::get("textures/black.png",
		graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
	greyTexture(*graphics::Texture2DManager::get("textures/lightGray.png",
		graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)))
{
	pressedButton = false;
	firstLeftClick = false;
	textFieldTextPointer = nullptr;

	pressedKey_a = pressedKey_b = pressedKey_c = pressedKey_d =	pressedKey_e = pressedKey_f = pressedKey_g = pressedKey_h = pressedKey_i = pressedKey_j = 
	pressedKey_k = pressedKey_l = pressedKey_m = pressedKey_n = pressedKey_o = pressedKey_p = pressedKey_q = pressedKey_r = pressedKey_s = pressedKey_t = 
	pressedKey_u = pressedKey_v = pressedKey_w = pressedKey_x = pressedKey_y = pressedKey_z = false;

	pressedKey_0 = pressedKey_1 = pressedKey_2 = pressedKey_3 = pressedKey_4 = pressedKey_5 = pressedKey_6 = pressedKey_7 = pressedKey_8 = pressedKey_9 = false;

	pressedKey_backspace = pressedKey_space = pressedKey_escape = pressedKey_enter = false;
}

GuiToolkit::~GuiToolkit() {
	system.registry.execute<Slider>([&](Slider& slider) {
		delete[] slider.levelEntities;
		});
}


/* ################ Button ################ */
void GuiToolkit::addButton(Entity& _entity, glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _defaultTexture, const graphics::Texture2D& _alternativeTexture, bool _swapTextureIfCursorOnIt, void (*_function)(Entity& _entity, System& _system), bool _activeFunction, std::string& _text, glm::vec4 _textColor)
{
	_position.x *= graphics::Device::getAspectRatio();
	_scale.x *= graphics::Device::getAspectRatio();

	system.addMesh(_entity, &planeMesh);
	system.addTexture(_entity, &_defaultTexture);
	system.addAlternativeTexture(_entity, &_alternativeTexture, false);
	system.addTransform(_entity, glm::mat4(1.f));
	system.addPosition(_entity, _position);
	system.addScale(_entity, _scale);
	system.addOrthogonal(_entity);
	system.addButton(_entity, _swapTextureIfCursorOnIt, _activeFunction);
	system.addFunction(_entity, _function);
	system.addMoved(_entity, false);

	float positionTextX, positionTextY, scaleY = 0.0f;
	findPositionAndScaleForAddText(_scale, _position, _text, scaleY, positionTextX, positionTextY);
	system.addText(_entity, (char*)_text.c_str(), glm::vec3(positionTextX, positionTextY, _position.z), scaleY, _textColor, 0.f, 0.f, 0.f, false);
}

void GuiToolkit::updateButton()
{
	glm::vec2 cursorPos = system.cameraOrthogonal.toWorldSpace(input::InputManager::getCursorPos());
	bool leftClick = input::InputManager::isButtonPressed(input::MouseButton::LEFT);

	system.registry.execute<Entity, Button, Position, Scale, Texture, AlternativeTexture, Moved, Text, System::Function>([&](
		Entity& entity, Button& button, Position& position, Scale& scale, Texture& texture, AlternativeTexture& alternativeTexture, Moved& moved, Text& text, System::Function& function) {

			math::Rectangle buttonBox = math::Rectangle(glm::vec2(position.position[0], position.position[1]), glm::vec2(position.position[0] + scale.scale[0], position.position[1] + scale.scale[1]));

			if (firstLeftClick == false && button.swapTextureIfCursorOnIt == true) {
				swapTextureWhenCursorOnIt(texture, alternativeTexture, buttonBox, cursorPos);
			}
			if (buttonBox.isIn(cursorPos) == true && leftClick == true && firstLeftClick == false) {
				alternativeTexture.inUse = false;
				const graphics::Texture2D& tempTexture = *texture.texture;
				texture.texture = alternativeTexture.texture;
				alternativeTexture.texture = &tempTexture;

				firstLeftClick = true;

				if (pressedButton == false) {
					pressedButton = true;
					if (button.activeFunction == true) {
						function.function(entity, system);
					}					
				}
			}
			if (leftClick == false && pressedButton == true) {
				pressedButton = false;
				firstLeftClick = false;
			}
			if (moved.moved == true) {
				float positionTextX, positionTextY, scaleY = 0.0f;
				findPositionAndScaleForAddText(scale.scale, position.position, text.text, scaleY, positionTextX, positionTextY);
				text.position.x = positionTextX;
				text.position.y = positionTextY;
				text.size = scaleY;
				moved.moved = false;
			}
		});
}


/* ################ Text-Display ################ */
void GuiToolkit::addTextDisplay(Entity& _entity, glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _texture, std::string& _text, glm::vec4 _textColor) 
{
	_position.x *= graphics::Device::getAspectRatio();
	_scale.x *= graphics::Device::getAspectRatio();

	system.addMesh(_entity, &planeMesh);
	system.addTexture(_entity, &_texture);
	system.addTransform(_entity, glm::mat4(1.f));
	system.addPosition(_entity, _position);
	system.addScale(_entity, _scale);
	system.addOrthogonal(_entity);
	system.addTextDisplay(_entity);
	system.addMoved(_entity, false);

	float positionTextX, positionTextY, scale = 0.f;
	findPositionAndScaleForAddText(_scale, _position, _text, scale, positionTextX, positionTextY);
	system.addText(_entity, (char*)_text.c_str(), glm::vec3(positionTextX, positionTextY, _position.z), scale, _textColor, 0.f, 0.f, 0.f, false);
}

void GuiToolkit::updateTextDisplay() {

	system.registry.execute<Entity, TextDisplay, Position, Scale, Moved, Text>([&](
		Entity& entity, TextDisplay& textDisplay, Position& position, Scale& scale, Moved& moved, Text& text) {
		
			if (moved.moved == true) {
				float positionTextX, positionTextY, scaleY = 0.0f;
				findPositionAndScaleForAddText(scale.scale, position.position, text.text, scaleY, positionTextX, positionTextY);
				text.position.x = positionTextX;
				text.position.y = positionTextY;
				text.size = scaleY;
				moved.moved = false;
			}
		});
}


/* ################ TextField ################ */
void GuiToolkit::addTextField(Entity& _entity, glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _defaultTexture, const graphics::Texture2D& _alternativeTexture, void (*_function)(Entity& _entity, System& _system), std::string& _text, glm::vec4 _textColor)
{
	_position.x *= graphics::Device::getAspectRatio();
	_scale.x *= graphics::Device::getAspectRatio();

	system.addMesh(_entity, &planeMesh);
	system.addTexture(_entity, &_defaultTexture);
	system.addAlternativeTexture(_entity, &_alternativeTexture, false);
	system.addTransform(_entity, glm::mat4(1.f));
	system.addPosition(_entity, _position);
	system.addScale(_entity, _scale);
	system.addOrthogonal(_entity);
	system.addTextField(_entity, false);
	system.addFunction(_entity, _function);
	system.addMoved(_entity, false);

	textFieldsDefaults[_entity.id] = _text;
	textFieldsTexts[_entity.id] = nullString;

	float positionTextX, positionTextY, scale = 0.f;
	findPositionAndScaleForAddText(_scale, _position, _text, scale, positionTextX, positionTextY);
	system.addText(_entity, (char*)_text.c_str(), glm::vec3(positionTextX, positionTextY, _position.z), scale, _textColor, 0.f, 0.f, 0.f, false);
}

void GuiToolkit::updateTextField() 
{
	glm::vec2 cursorPos = system.cameraOrthogonal.toWorldSpace(input::InputManager::getCursorPos());
	bool leftClick = input::InputManager::isButtonPressed(input::MouseButton::LEFT);

	system.registry.execute<Entity, TextField, Position, Scale, Texture, AlternativeTexture, Text, Moved, System::Function>([&](
		Entity& entity, TextField& textField, Position& position, Scale& scale, Texture& texture, AlternativeTexture& alternativeTexture, Text& text, Moved& moved, System::Function& function) {
			
			math::Rectangle buttonBox = math::Rectangle(glm::vec2(position.position[0], position.position[1]), glm::vec2(position.position[0] + scale.scale[0], position.position[1] + scale.scale[1]));

			if (textField.pressed == false && firstLeftClick == false) {
				swapTextureWhenCursorOnIt(texture, alternativeTexture, buttonBox, cursorPos);
			}
			if (buttonBox.isIn(cursorPos) == true && leftClick == true && textField.pressed == false && firstLeftClick == false) {
				textField.pressed = true;
				firstLeftClick = true;
			}
			if (buttonBox.isIn(cursorPos) == false && leftClick == true && textField.pressed == true) {
				textField.pressed = false;
			}
			if (leftClick == false) {
				firstLeftClick = false;
			}
			if (textField.pressed == true) {
				if (textFieldsTexts[entity.id] == nullString) {
					text.text = (char*)textFieldsTexts[entity.id].c_str();
				}
				std::string tempString = textFieldsTexts[entity.id];
				updateKeyInputActions(entity, function, textField);
				if (tempString != textFieldsTexts[entity.id] || pressedKey_enter == true || pressedKey_escape == true) {
					float positionTextX, positionTextY, scaleY = 0.f;
					if (findPositionAndScaleForTextFieldText(entity, scaleY, scale, position, positionTextX, positionTextY)) {
						text.text = textFieldTextPointer;
						text.position.x = positionTextX;
						text.position.y = positionTextY;
						text.size = scaleY;						
					}
				}
			}
			if (moved.moved == true) {
				float positionTextX, positionTextY, scaleY = 0.0f;
				findPositionAndScaleForAddText(scale.scale, position.position, text.text, scaleY, positionTextX, positionTextY);
				text.position.x = positionTextX;
				text.position.y = positionTextY;
				text.size = scaleY;
				moved.moved = false;
			}
		});
}


/* ################ Check-Box ################ */
void GuiToolkit::addCheckBox(Entity& _entity, glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _defaultTexture, const graphics::Texture2D& _alternativeTexture, void (*_function)(Entity& _entity, System& _system), std::string& _text, glm::vec4 _textColor, bool _status)
{
	_position.x *= graphics::Device::getAspectRatio();
	_scale.x *= graphics::Device::getAspectRatio();

	Entity buttonEntity = system.createEntity(buttonEntity);
	Entity textEntity = system.createEntity(textEntity);

	float buttonPositionX = (_position.x / graphics::Device::getAspectRatio()) + ((_scale.x / graphics::Device::getAspectRatio()) * 0.04f);
	float buttonPositionY = _position.y + (_scale.y * 0.04f);
	float buttonScaleX = (_scale.x / graphics::Device::getAspectRatio()) * 0.22f;
	float buttonScaleY = _scale.y * 0.92f;
	addButton(buttonEntity, glm::vec3(buttonPositionX, buttonPositionY, 0.f), glm::vec3(buttonScaleX, buttonScaleY, 1.f), _defaultTexture, _alternativeTexture, false, doNothing, false, off, glm::vec4(0.f));
	if (_status == true) {
		system.registry.getComponentUnsafe<Text>(buttonEntity).text = (char*)on.c_str();
	}	

	float textPositionX = buttonPositionX + buttonScaleX + ((_scale.x / graphics::Device::getAspectRatio()) * 0.04f);
	float textPositionY = buttonPositionY;
	float textScaleX = (_scale.x / graphics::Device::getAspectRatio()) * 0.66f;
	float textScaleY = _scale.y * 0.92f;
	addTextDisplay(textEntity, glm::vec3(textPositionX, textPositionY, 0.f), glm::vec3(textScaleX, textScaleY, 1.f), whiteTexture, _text, _textColor);

	system.addMesh(_entity, &planeMesh);
	system.addTexture(_entity, &lightGrayTexture);
	system.addTransform(_entity, glm::mat4(1.f));
	system.addPosition(_entity, _position);
	system.addScale(_entity, _scale);
	system.addOrthogonal(_entity);
	system.addCheckBox(_entity, buttonEntity.id, textEntity.id, false, _status);
	system.addMoved(_entity, false);
	system.addFunction(_entity, _function);
}

void GuiToolkit::updateCheckBox() 
{
	glm::vec2 cursorPos = system.cameraOrthogonal.toWorldSpace(input::InputManager::getCursorPos());
	bool leftClick = input::InputManager::isButtonPressed(input::MouseButton::LEFT);

	system.registry.execute<Entity, CheckBox, Position, Scale, Moved, System::Function>([&](
		Entity& entity, CheckBox& checkBox, Position& position, Scale& scale, Moved& moved, System::Function& function) {
			
			glm::vec3& positionButton = system.registry.getComponentUnsafe<Position>(Entity{ checkBox.buttonEntity }).position;
			glm::vec3& scaleButton = system.registry.getComponentUnsafe<Scale>(Entity{ checkBox.buttonEntity }).scale;			
			math::Rectangle checkboxButtonBox = math::Rectangle(glm::vec2(positionButton.x, positionButton.y), glm::vec2(positionButton.x + scaleButton.x, positionButton.y + scaleButton.y));

			if (checkboxButtonBox.isIn(cursorPos) == true && leftClick == true && checkBox.pressed == false && (firstLeftClick == false || pressedButton == true)) {
				checkBox.pressed = true;
				firstLeftClick = true;
				Text& text = system.registry.getComponentUnsafe<Text>(Entity{ checkBox.buttonEntity });
				if (checkBox.status == true) {
					checkBox.status = false;
					text.text = (char*)off.c_str();
					moved.moved = true;
				}
				else {
					checkBox.status = true;
					text.text = (char*)on.c_str();
					moved.moved = true;
				}
				function.function(entity, system);
			}
			if (leftClick == false) {
				checkBox.pressed = false;
				firstLeftClick = false;
			}

			if (moved.moved == true) {		
				positionButton.x = position.position.x + (scale.scale.x * 0.04f);
				positionButton.y = position.position.y + (scale.scale.y * 0.04f);
				scaleButton.x = scale.scale.x * 0.22f;
				scaleButton.y = scale.scale.y * 0.92f;
				system.registry.getComponentUnsafe<Moved>(Entity{ checkBox.buttonEntity }).moved = true;

				glm::vec3& positionText = system.registry.getComponentUnsafe<Position>(Entity{ checkBox.textEntity }).position;
				glm::vec3& scaleText = system.registry.getComponentUnsafe<Scale>(Entity{ checkBox.textEntity }).scale;
				positionText.x = positionButton.x + scaleButton.x + (scale.scale.x * 0.04f);
				positionText.y = positionButton.y;
				scaleText.x = scale.scale.x * 0.66f;
				scaleText.y = scale.scale.y * 0.92f;
				system.registry.getComponentUnsafe<Moved>(Entity{ checkBox.textEntity }).moved = true;

				moved.moved = false;
			}

		});
}

void GuiToolkit::deleteCheckBox(Entity& _entity) {
	Entity textEntity;
	textEntity.id = system.registry.getComponentUnsafe<CheckBox>(_entity).textEntity;
	system.eraseEntity(textEntity);
	Entity buttonEntity;
	buttonEntity.id = system.registry.getComponentUnsafe<CheckBox>(_entity).buttonEntity;
	system.eraseEntity(buttonEntity);
	system.eraseEntity(_entity);
}


/* ################ Slider ################ */
void GuiToolkit::addSlider(Entity _entity, glm::vec3 _position, glm::vec3 _scale, int _levels, int _selectedLevel, bool _vertical) 
{
	_position.x *= graphics::Device::getAspectRatio();
	_scale.x *= graphics::Device::getAspectRatio();
	
	Entity* levelEntities = new Entity[_levels];
	Slider slider = { _entity, levelEntities, _selectedLevel, _levels, _vertical };

	float sizeLevel;
	float sizeLevelDistance;
	if (_vertical) {
		sizeLevel = _scale.y / _levels * 0.8f;
		sizeLevelDistance = _scale.y / (_levels - 1) * 0.2f;
	}
	else {
		sizeLevel = _scale.x / _levels * 0.8;
		sizeLevelDistance = _scale.x / (_levels - 1) * 0.2;
	}
	for (int i = 0; i < _levels; i++) {
		Entity levelEntity;
		system.createEntity(levelEntity);
		system.addMesh(levelEntity, &planeMesh);
		system.addTransform(levelEntity, glm::mat4(1.f));
		if (_vertical) {
			glm::vec3 pos = glm::vec3(0.f, i * (sizeLevel + sizeLevelDistance), 0.f) + _position;
			system.addPosition(levelEntity, pos);
			glm::vec3 sca = glm::vec3(1.f, 1.f / _levels * 0.8, 1.f) * _scale;
			system.addScale(levelEntity, sca);
			system.addBox2D(levelEntity, glm::vec2(pos.x, pos.y), glm::vec2(pos.x + sca.x, pos.y + sca.y));
		}
		else {
			glm::vec3 pos = glm::vec3(i * (sizeLevel + sizeLevelDistance), 0.f, 0.f) + _position;
			system.addPosition(levelEntity, pos);
			glm::vec3 sca = glm::vec3(1.f / _levels * 0.8, 1.f, 1.f) * _scale;
			system.addScale(levelEntity, sca);
			system.addBox2D(levelEntity, glm::vec2(pos.x, pos.y), glm::vec2(pos.x + sca.x, pos.y + sca.y));
		}

		if (i <= _selectedLevel) {
			system.addTexture(levelEntity, &redTexture);
			slider.currentLevel = i;
		}
		else {
			system.addTexture(levelEntity, &blackTexture);
		}
		slider.levelEntities[i] = levelEntity;
		system.addOrthogonal(levelEntity);
	}
	system.addSlider(_entity, slider);
	system.addMesh(_entity, &planeMesh);
	system.addTexture(_entity, &greyTexture);
	system.addTransform(_entity, glm::mat4(1.f));
	system.addPosition(_entity, _position);
	system.addScale(_entity, _scale);
	system.addOrthogonal(_entity);
	system.addMoved(_entity, false);
}

void GuiToolkit::updateSlider() {	//ggf. text, move, ggf. minimaler konstruktor, gamestates auswählen
	bool isPressed = input::InputManager::isButtonPressed(input::MouseButton::LEFT);
	glm::vec2 cursorPos = system.cameraOrthogonal.toWorldSpace(input::InputManager::getCursorPos());
	system.registry.execute<Slider, Position, Scale, Moved>([&](Slider& slider, Position& position, Scale& scale, Moved& moved) {
		
		if (isPressed) {
			for (int i = 0; i < slider.numberOfLevels; i++) {
				math::Rectangle currentBox = system.registry.getComponentUnsafe<Box2D>(slider.levelEntities[i]).box;
				if (currentBox.isIn(cursorPos)) {
					if (i > slider.currentLevel) {
						for (int j = slider.currentLevel; j <= i; j++) {
							system.setTexture(slider.levelEntities[j], &redTexture);
						}
					}
					else if (i < slider.currentLevel) {
						for (int j = slider.currentLevel; j > i; j--) {
							system.setTexture(slider.levelEntities[j], &blackTexture);
						}
					}
					slider.currentLevel = i;
					break;
				}
			}
		}

		if (moved.moved == true) {

			float sizeLevel;
			float sizeLevelDistance;

			if (slider.vertical) {
				sizeLevel = scale.scale.y / slider.numberOfLevels * 0.8f;
				sizeLevelDistance = scale.scale.y / (slider.numberOfLevels - 1) * 0.2f;
			}
			else {
				sizeLevel = scale.scale.x / slider.numberOfLevels * 0.8;
				sizeLevelDistance = scale.scale.x / (slider.numberOfLevels - 1) * 0.2;
			}
			for (int i = 0; i < slider.numberOfLevels; i++) {

				if (slider.vertical) {
					glm::vec3 pos = glm::vec3(0.f, i * (sizeLevel + sizeLevelDistance), 0.f) + position.position;
					system.setPosition(slider.levelEntities[i], pos);
					glm::vec3 sca = glm::vec3(1.f, 1.f / slider.numberOfLevels * 0.8, 1.f) * scale.scale;
					system.setScale(slider.levelEntities[i], sca);
					system.registry.getComponentUnsafe<Box2D>(slider.levelEntities[i]).box = math::Rectangle(glm::vec2(pos.x, pos.y), glm::vec2(pos.x + sca.x, pos.y + sca.y));
				}
				else {
					glm::vec3 pos = glm::vec3(i * (sizeLevel + sizeLevelDistance), 0.f, 0.f) + position.position;
					system.setPosition(slider.levelEntities[i], pos);
					glm::vec3 sca = glm::vec3(1.f / slider.numberOfLevels * 0.8, 1.f, 1.f) * scale.scale;
					system.setScale(slider.levelEntities[i], sca);
					system.registry.getComponentUnsafe<Box2D>(slider.levelEntities[i]).box = math::Rectangle(glm::vec2(pos.x, pos.y), glm::vec2(pos.x + sca.x, pos.y + sca.y));
				}
			}
			moved.moved = false;
		}

	});
}

void GuiToolkit::deleteSlider(Entity& _entity) {
	Slider& slider = system.registry.getComponentUnsafe<Slider>(_entity);
	for (int i = 0; i < slider.numberOfLevels; i++) {
		system.eraseEntity(slider.levelEntities[i]);
	}
	delete[] slider.levelEntities;
	system.eraseEntity(_entity);
}


/* ################ Container ################ */
void GuiToolkit::addContainer(Entity& _entity, glm::vec3 _position, glm::vec3 _scale, int _rows, int _colums, float _gapFactor, const graphics::Texture2D& _texture, bool _background, bool _movable, std::vector<Entity> _entities)
{
	_position.x *= graphics::Device::getAspectRatio();
	_scale.x *= graphics::Device::getAspectRatio();

	if (_background == true) {
		system.addMesh(_entity, &planeMesh);
		system.addTexture(_entity, &_texture);
		system.addTransform(_entity, glm::mat4(1.f));
		system.addOrthogonal(_entity);
	}
	system.addPosition(_entity, _position);
	system.addScale(_entity, _scale);	
	system.addContainer(_entity, _rows, _colums, _gapFactor, false, glm::vec2(0.f), 0.f);
	system.addMoved(_entity, false);
	system.addMovable(_entity, _movable);	

	entitiesInContainer[_entity] = _entities;

	float gap = (_scale.y / _rows) * _gapFactor;

	float scaleX = (_scale.x - gap) / _colums;
	float scaleY = (_scale.y - gap) / _rows;

	float posX = _position.x;
	float posY = _position.y + _scale.y;

	int count = 0;

	for (int i = 0; i < _colums; i++)
	{
		for (int j = 0; j < _rows; j++)
		{
			glm::vec3& position = system.registry.getComponentUnsafe<Position>(_entities.at(count)).position;
			position.x = (posX + (scaleX * (i)) + gap);
			position.y = posY - (scaleY * (j + 1));

			glm::vec3& scale = system.registry.getComponentUnsafe<Scale>(_entities.at(count)).scale;
			scale.x = scaleX - gap;
			scale.y = scaleY - gap;

			system.registry.getComponentUnsafe<Moved>(_entities.at(count)).moved = true;
			
			count++;
			if (count >= _entities.size()) return;
		}
	}
}

void GuiToolkit::updateContainer(float _deltaTime)
{
	glm::vec2 cursorPos = system.cameraOrthogonal.toWorldSpace(input::InputManager::getCursorPos());
	bool leftClick = input::InputManager::isButtonPressed(input::MouseButton::LEFT);

	system.registry.execute<Entity, Container, Position, Scale, Moved, Movable>([&](
		Entity& entity, Container& container, Position& position, Scale& scale, Moved& moved, Movable& movable) {

			if (movable.movable == true) {				
				math::Rectangle containerBox = math::Rectangle(glm::vec2(position.position[0], position.position[1]), glm::vec2(position.position[0] + scale.scale[0], position.position[1] + scale.scale[1]));

				if (containerBox.isIn(cursorPos) == true && leftClick == true && container.pressed == false && firstLeftClick == false) {
					bool cursorOnContainerElement = false;
					math::Rectangle containerElementBoxes;
					for (int i = 0; i < entitiesInContainer[entity].size(); i++) {
						Position& pos = system.registry.getComponentUnsafe<Position>(entitiesInContainer[entity].at(i));
						Scale& sca = system.registry.getComponentUnsafe<Scale>(entitiesInContainer[entity].at(i));
						containerElementBoxes = math::Rectangle(glm::vec2(pos.position[0], pos.position[1]), glm::vec2(pos.position[0] + sca.scale[0], pos.position[1] + sca.scale[1]));
						if (containerElementBoxes.isIn(cursorPos) == true) {
							cursorOnContainerElement = true;
						}
					}
					if (cursorOnContainerElement == false) {
						container.oldCursorPosition = cursorPos;
						container.time = 0.f;
						container.pressed = true;
						firstLeftClick = true;
					}
				}
				if (container.pressed == true) {
					position.position[0] += cursorPos.x - container.oldCursorPosition.x;
					position.position[1] += cursorPos.y - container.oldCursorPosition.y;
					container.oldCursorPosition = cursorPos;
					//container.time += _deltaTime; //// only in Debug-Mode necessary
					//if (container.time > 0.05f) {
						moved.moved = true;
						//container.time = 0.f;
					//}
				}
				if (leftClick == false && container.pressed == true) {
					moved.moved = true;
					container.pressed = false;
					firstLeftClick = false;
				}
			}

			if (moved.moved == true) 
			{
				float gap = (scale.scale.y / container.rows) * container.gapFactor;

				float scaleX = (scale.scale.x - gap) / container.colums;
				float scaleY = (scale.scale.y - gap) / container.rows;

				float posX = position.position.x;
				float posY = position.position.y + scale.scale.y;

				int count = 0;

				for (int i = 0; i < container.colums; i++)
				{
					for (int j = 0; j < container.rows; j++)
					{
						glm::vec3& position = system.registry.getComponentUnsafe<Position>(entitiesInContainer[entity].at(count)).position;
						position.x = (posX + (scaleX * (i)) + gap);
						position.y = posY - (scaleY * (j + 1));

						glm::vec3& scale = system.registry.getComponentUnsafe<Scale>(entitiesInContainer[entity].at(count)).scale;
						scale.x = scaleX - gap;
						scale.y = scaleY - gap;

						system.registry.getComponentUnsafe<Moved>(entitiesInContainer[entity].at(count)).moved = true;
		
						count++;
						if (count >= entitiesInContainer[entity].size()) {
							moved.moved = false;
							return;
						}
					}
				}
				moved.moved = false;
			}
		});
}


/* ################ GUI-Update ################ */
void GuiToolkit::update(float _deltaTime)
{	
	updateContainer(_deltaTime);
	updateButton();
	updateCheckBox();
	updateTextDisplay();
	updateTextField();	
	updateTextField();
	updateSlider();
}


/* ################ GUI-Utils ################ */
void GuiToolkit::findPositionAndScaleForAddText(glm::vec3& _scale, glm::vec3& _position, std::string _text, float& scaleY, float& positionTextX, float& positionTextY)
{
	scaleY = _scale.y;
	math::Rectangle boxButton = math::Rectangle(glm::vec2(_position.x, _position.y), glm::vec2(_position.x + _scale.x, _position.y + _scale.y));
	math::Rectangle boxText = system.fontRenderer->getBoundingBox(glm::vec3(_position.x, _position.y, _position.z), _text.c_str(), scaleY, 0.f, 0.f, 0.f, false);

	auto windowSize = graphics::Device::getBufferSize();
	float scaleWindow = 0.f;
	if (windowSize.x < windowSize.y) {
		scaleWindow = static_cast<float>(windowSize.x) / 1000.f;
	}
	else scaleWindow = static_cast<float>(windowSize.y) / 1000.f;

	while (boxText.max.x > boxButton.max.x && scaleY > (0.015f / scaleWindow) ) {
		scaleY = scaleY - 0.001f;
		boxText = system.fontRenderer->getBoundingBox(glm::vec3(_position.x, _position.y, _position.z), _text.c_str(), scaleY, 0.f, 0.f, 0.f, false);
	}
	while (boxText.max.x > boxButton.max.x) {
		if (_text.size() > 0) {
			_text.pop_back();
			boxText = system.fontRenderer->getBoundingBox(glm::vec3(_position.x, _position.y, _position.z), _text.c_str(), scaleY, 0.f, 0.f, 0.f, false);
		}
	}
	float centerButtonX = (_position.x + (_position.x + _scale.x)) * 0.5f;
	positionTextX = (centerButtonX - ((boxText.max.x - boxText.min.x) * 0.5f)) + 0.005;
	float centerButtonY = (_position.y + (_position.y + _scale.y)) * 0.5f;
	positionTextY = centerButtonY - (scaleY * 0.5f);
}

bool GuiToolkit::findPositionAndScaleForTextFieldText(Entity& _entity, float& scaleY, Scale& scale, Position& position, float& positionTextX, float& positionTextY)
{
	scaleY = scale.scale[1];
	math::Rectangle boxButton = math::Rectangle(glm::vec2(position.position[0], position.position[1]), glm::vec2(position.position[0] + scale.scale[0], position.position[1] + scale.scale[1]));
	math::Rectangle boxText = system.fontRenderer->getBoundingBox(glm::vec3(position.position[0], position.position[1], position.position[2]), textFieldTextPointer, scaleY, 0.f, 0.f, 0.f, false);

	auto windowSize = graphics::Device::getBufferSize();
	float scaleWindow = 0;
	if (windowSize.x < windowSize.y) {
		scaleWindow = static_cast<float>(windowSize.x) / 1000;
	}
	else scaleWindow = static_cast<float>(windowSize.y) / 1000;

	while (boxText.max.x > boxButton.max.x && scaleY > (0.015 / scaleWindow)) {
		scaleY = scaleY - 0.001f;
		boxText = system.fontRenderer->getBoundingBox(glm::vec3(position.position[0], position.position[1], position.position[2]), textFieldTextPointer, scaleY, 0.f, 0.f, 0.f, false);
	}
	if (boxText.max.x > boxButton.max.x) {
		textFieldsTexts[_entity.id].pop_back();
		return false;
	}
	float centerButtonX = (position.position[0] + (position.position[0] + scale.scale[0])) * 0.5;
	positionTextX = centerButtonX - ((boxText.max.x - boxText.min.x) * 0.5f);
	float centerButtonY = (position.position[1] + (position.position[1] + scale.scale[1])) * 0.5;
	positionTextY = centerButtonY - (scaleY * 0.5);
	return true;
}

void GuiToolkit::swapTextureWhenCursorOnIt(Texture& texture, AlternativeTexture& alternativeTexture, math::Rectangle& buttonBox, const glm::vec2& cursorPos)
{
	if (texture.texture != alternativeTexture.texture) {
		if (buttonBox.isIn(cursorPos) == true && alternativeTexture.inUse == false) {
			alternativeTexture.inUse = true;
			const graphics::Texture2D& tempTexture = *texture.texture;
			texture.texture = alternativeTexture.texture;
			alternativeTexture.texture = &tempTexture;
		}
		if (buttonBox.isIn(cursorPos) == false && alternativeTexture.inUse == true) {
			alternativeTexture.inUse = false;
			const graphics::Texture2D& tempTexture = *texture.texture;
			texture.texture = alternativeTexture.texture;
			alternativeTexture.texture = &tempTexture;
		}
	}
}

void GuiToolkit::updateKeyInputActions(Entity& _entity, System::Function& _function, TextField& _textField)
{
	using namespace input;

	if (InputManager::isKeyPressed(Key::A) && pressedKey_a == false) {
		pressedKey_a = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("A");
		}
		else {
			textFieldsTexts[_entity.id].append("a");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::A) == false && pressedKey_a == true) {
		pressedKey_a = false;
	}

	else if (InputManager::isKeyPressed(Key::B) && pressedKey_b == false) {
		pressedKey_b = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("B");
		}
		else {
			textFieldsTexts[_entity.id].append("b");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::B) == false && pressedKey_b == true) {
		pressedKey_b = false;
	}

	else if (InputManager::isKeyPressed(Key::C) && pressedKey_c == false) {
		pressedKey_c = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("C");
		}
		else {
			textFieldsTexts[_entity.id].append("c");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::C) == false && pressedKey_c == true) {
		pressedKey_c = false;
	}

	else if (InputManager::isKeyPressed(Key::D) && pressedKey_d == false) {
		pressedKey_d = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("D");
		}
		else {
			textFieldsTexts[_entity.id].append("d");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::D) == false && pressedKey_d == true) {
		pressedKey_d = false;
	}

	else if (InputManager::isKeyPressed(Key::E) && pressedKey_e == false) {
		pressedKey_e = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("E");
		}
		else {
			textFieldsTexts[_entity.id].append("e");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::E) == false && pressedKey_e == true) {
		pressedKey_e = false;
	}

	else if (InputManager::isKeyPressed(Key::F) && pressedKey_f == false) {
		pressedKey_f = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("F");
		}
		else {
			textFieldsTexts[_entity.id].append("f");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::F) == false && pressedKey_f == true) {
		pressedKey_f = false;
	}

	else if (InputManager::isKeyPressed(Key::G) && pressedKey_g == false) {
		pressedKey_g = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("G");
		}
		else {
			textFieldsTexts[_entity.id].append("g");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::G) == false && pressedKey_g == true) {
		pressedKey_g = false;
	}

	else if (InputManager::isKeyPressed(Key::H) && pressedKey_h == false) {
		pressedKey_h = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("H");
		}
		else {
			textFieldsTexts[_entity.id].append("h");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::H) == false && pressedKey_h == true) {
		pressedKey_h = false;
	}

	else if (InputManager::isKeyPressed(Key::I) && pressedKey_i == false) {
		pressedKey_i = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("I");
		}
		else {
			textFieldsTexts[_entity.id].append("i");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::I) == false && pressedKey_i == true) {
		pressedKey_i = false;
	}

	else if (InputManager::isKeyPressed(Key::J) && pressedKey_j == false) {
		pressedKey_j = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("J");
		}
		else {
			textFieldsTexts[_entity.id].append("j");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::J) == false && pressedKey_j == true) {
		pressedKey_j = false;
	}

	else if (InputManager::isKeyPressed(Key::K) && pressedKey_k == false) {
		pressedKey_k = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("K");
		}
		else {
			textFieldsTexts[_entity.id].append("k");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::K) == false && pressedKey_k == true) {
		pressedKey_k = false;
	}

	else if (InputManager::isKeyPressed(Key::L) && pressedKey_l == false) {
		pressedKey_l = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("L");
		}
		else {
			textFieldsTexts[_entity.id].append("l");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::L) == false && pressedKey_l == true) {
		pressedKey_l = false;
	}

	else if (InputManager::isKeyPressed(Key::M) && pressedKey_m == false) {
		pressedKey_m = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("M");
		}
		else {
			textFieldsTexts[_entity.id].append("m");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::M) == false && pressedKey_m == true) {
		pressedKey_m = false;
	}

	else if (InputManager::isKeyPressed(Key::N) && pressedKey_n == false) {
		pressedKey_n = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("N");
		}
		else {
			textFieldsTexts[_entity.id].append("n");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::N) == false && pressedKey_n == true) {
		pressedKey_n = false;
	}

	else if (InputManager::isKeyPressed(Key::O) && pressedKey_o == false) {
		pressedKey_o = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("O");
		}
		else {
			textFieldsTexts[_entity.id].append("o");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::O) == false && pressedKey_o == true) {
		pressedKey_o = false;
	}

	else if (InputManager::isKeyPressed(Key::P) && pressedKey_p == false) {
		pressedKey_p = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("P");
		}
		else {
			textFieldsTexts[_entity.id].append("p");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::P) == false && pressedKey_p == true) {
		pressedKey_p = false;
	}

	else if (InputManager::isKeyPressed(Key::Q) && pressedKey_q == false) {
		pressedKey_q = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("Q");
		}
		else {
			textFieldsTexts[_entity.id].append("q");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Q) == false && pressedKey_q == true) {
		pressedKey_q = false;
	}

	else if (InputManager::isKeyPressed(Key::R) && pressedKey_r == false) {
		pressedKey_r = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("R");
		}
		else {
			textFieldsTexts[_entity.id].append("r");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::R) == false && pressedKey_r == true) {
		pressedKey_r = false;
	}

	else if (InputManager::isKeyPressed(Key::S) && pressedKey_s == false) {
		pressedKey_s = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("S");
		}
		else {
			textFieldsTexts[_entity.id].append("s");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::S) == false && pressedKey_s == true) {
		pressedKey_s = false;
	}

	else if (InputManager::isKeyPressed(Key::T) && pressedKey_t == false) {
		pressedKey_t = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("T");
		}
		else {
			textFieldsTexts[_entity.id].append("t");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::T) == false && pressedKey_t == true) {
		pressedKey_t = false;
	}

	else if (InputManager::isKeyPressed(Key::U) && pressedKey_u == false) {
		pressedKey_u = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("U");
		}
		else {
			textFieldsTexts[_entity.id].append("u");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::U) == false && pressedKey_u == true) {
		pressedKey_u = false;
	}

	else if (InputManager::isKeyPressed(Key::V) && pressedKey_v == false) {
		pressedKey_v = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("V");
		}
		else {
			textFieldsTexts[_entity.id].append("v");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::V) == false && pressedKey_v == true) {
		pressedKey_v = false;
	}

	else if (InputManager::isKeyPressed(Key::W) && pressedKey_w == false) {
		pressedKey_w = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("W");
		}
		else {
			textFieldsTexts[_entity.id].append("w");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::W) == false && pressedKey_w == true) {
		pressedKey_w = false;
	}

	else if (InputManager::isKeyPressed(Key::X) && pressedKey_x == false) {
		pressedKey_x = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("X");
		}
		else {
			textFieldsTexts[_entity.id].append("x");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::X) == false && pressedKey_x == true) {
		pressedKey_x = false;
	}

	else if (InputManager::isKeyPressed(Key::Y) && pressedKey_y == false) {
		pressedKey_y = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("Z");
		}
		else {
			textFieldsTexts[_entity.id].append("z");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Y) == false && pressedKey_y == true) {
		pressedKey_y = false;
	}

	else if (InputManager::isKeyPressed(Key::Z) && pressedKey_z == false) {
		pressedKey_z = true;
		if (InputManager::isKeyPressed(Key::LEFT_SHIFT) || InputManager::isKeyPressed(Key::RIGHT_SHIFT)) {
			textFieldsTexts[_entity.id].append("Y");
		}
		else {
			textFieldsTexts[_entity.id].append("y");
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Z) == false && pressedKey_z == true) {
		pressedKey_z = false;
	}

	else if (InputManager::isKeyPressed(Key::Num0) && pressedKey_0 == false) {
		pressedKey_0 = true;
		textFieldsTexts[_entity.id].append("0");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num0) == false && pressedKey_0 == true) {
		pressedKey_0 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num1) && pressedKey_1 == false) {
		pressedKey_1 = true;
		textFieldsTexts[_entity.id].append("1");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num1) == false && pressedKey_1 == true) {
		pressedKey_1 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num2) && pressedKey_2 == false) {
		pressedKey_2 = true;
		textFieldsTexts[_entity.id].append("2");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num2) == false && pressedKey_2 == true) {
		pressedKey_2 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num3) && pressedKey_3 == false) {
		pressedKey_3 = true;
		textFieldsTexts[_entity.id].append("3");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num3) == false && pressedKey_3 == true) {
		pressedKey_3 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num4) && pressedKey_4 == false) {
		pressedKey_4 = true;
		textFieldsTexts[_entity.id].append("4");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num4) == false && pressedKey_4 == true) {
		pressedKey_4 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num5) && pressedKey_5 == false) {
		pressedKey_5 = true;
		textFieldsTexts[_entity.id].append("5");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num5) == false && pressedKey_5 == true) {
		pressedKey_5 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num6) && pressedKey_6 == false) {
		pressedKey_6 = true;
		textFieldsTexts[_entity.id].append("6");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num6) == false && pressedKey_6 == true) {
		pressedKey_6 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num7) && pressedKey_7 == false) {
		pressedKey_7 = true;
		textFieldsTexts[_entity.id].append("7");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num7) == false && pressedKey_7 == true) {
		pressedKey_7 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num8) && pressedKey_8 == false) {
		pressedKey_8 = true;
		textFieldsTexts[_entity.id].append("8");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num8) == false && pressedKey_8 == true) {
		pressedKey_8 = false;
	}

	else if (InputManager::isKeyPressed(Key::Num9) && pressedKey_9 == false) {
		pressedKey_9 = true;
		textFieldsTexts[_entity.id].append("9");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::Num9) == false && pressedKey_9 == true) {
		pressedKey_9 = false;
	}

	else if (InputManager::isKeyPressed(Key::BACKSPACE) && pressedKey_backspace == false) {
		pressedKey_backspace = true;
		if (textFieldsTexts[_entity.id].size() > 0) {
			textFieldsTexts[_entity.id].pop_back();
		}
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::BACKSPACE) == false && pressedKey_backspace == true) {
		pressedKey_backspace = false;
	}

	else if (InputManager::isKeyPressed(Key::SPACE) && pressedKey_space == false) {
		pressedKey_space = true;
		textFieldsTexts[_entity.id].append(" ");
		textFieldTextPointer = (char*)textFieldsTexts[_entity.id].c_str();
	}
	else if (InputManager::isKeyPressed(Key::SPACE) == false && pressedKey_space == true) {
		pressedKey_space = false;
	}

	else if (InputManager::isKeyPressed(Key::ESCAPE) && pressedKey_escape == false) {
	pressedKey_escape = true;
	textFieldsTexts[_entity.id].erase();
	textFieldsTexts[_entity.id].shrink_to_fit();
	textFieldTextPointer = (char*)textFieldsDefaults[_entity.id].c_str();
	_textField.pressed = false;
	}
	else if (InputManager::isKeyPressed(Key::ESCAPE) == false && pressedKey_escape == true) {
	pressedKey_escape = false;
	}

	else if (InputManager::isKeyPressed(Key::ENTER) && pressedKey_enter == false) {
	pressedKey_enter = true;
	_function.function(_entity, system);
	textFieldsTexts[_entity.id].erase();
	textFieldsTexts[_entity.id].shrink_to_fit();
	textFieldTextPointer = (char*)textFieldsDefaults[_entity.id].c_str();
	_textField.pressed = false;;
	}
	else if (InputManager::isKeyPressed(Key::ENTER) == false && pressedKey_enter == true) {
	pressedKey_enter = false;
	}

	
}

int GuiToolkit::getLevel(Entity _entity) {
	return system.registry.getComponentUnsafe<Slider>(_entity).currentLevel;
}