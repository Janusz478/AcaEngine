#pragma once

#include "../entity_component_system/system.hpp"
#include "../graphics/renderer/fontrenderer.hpp"
#include "../graphics/core/device.hpp"

class GuiToolkit
{
public:
	GuiToolkit(System& _system);

	~GuiToolkit();


	/* ################ Button ################ */
	void addButton(Entity& _entity, glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _defaultTexture, const graphics::Texture2D& _alternativeTexture, bool _swapTextureIfCursorOnIt, void (*_function)(Entity& _entity, System& _system), bool _activeFunction, std::string& _text, glm::vec4 _textColor);
	
	inline void addButton(Entity& _entity, glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _defaultTexture, const graphics::Texture2D& _alternativeTexture, bool _swapTextureIfCursorOnIt, void (*_function)(Entity& _entity, System& _system), std::string& _text, glm::vec4 _textColor) {
		addButton(_entity, _position, _scale, _defaultTexture, _alternativeTexture, _swapTextureIfCursorOnIt, _function, true, _text, _textColor);
	}

	inline void addButton(Entity& _entity, void (*_function)(Entity& _entity, System& _system), std::string& _text) {
		addButton(_entity, glm::vec3(0.f), glm::vec3(1.f), darkBlueTexture, lightBlueTexture, true, _function, true, _text, glm::vec4(1.f));
	}	

	inline void addButton(Entity& _entity, void (*_function)(Entity& _entity, System& _system), std::string& _text, glm::vec4 _textColor) {
		addButton(_entity, glm::vec3(0.f), glm::vec3(1.f), darkBlueTexture, lightBlueTexture, true, _function, true, _text, _textColor);
	}

	inline void addButton(Entity& _entity, void (*_function)(Entity& _entity, System& _system), std::string& _text, glm::vec4 _textColor, glm::vec3 _position, glm::vec3 _scale ) {
		addButton(_entity, _position, _scale, darkBlueTexture, lightBlueTexture, true, _function, true, _text, _textColor);
	}

	inline void addButton(Entity& _entity, void (*_function)(Entity& _entity, System& _system), std::string& _text, const graphics::Texture2D& _defaultTexture, const graphics::Texture2D& _alternativeTexture) {
		addButton(_entity, glm::vec3(0.f), glm::vec3(1.f), _defaultTexture, _alternativeTexture, true, _function, true, _text, glm::vec4(0.f));
	}

	void updateButton();
	

	/* ################ Text-Display ################ */
	void addTextDisplay(Entity& _entity, glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _texture, std::string& _text, glm::vec4 _textColor);

	inline void addTextDisplay(Entity& _entity, std::string& _text) {
		addTextDisplay(_entity, glm::vec3(0.f), glm::vec3(1.f), whiteTexture, _text, glm::vec4(0.f));
	}

	inline void addTextDisplay(Entity& _entity, std::string& _text, const graphics::Texture2D& _texture) {
		addTextDisplay(_entity, glm::vec3(0.f), glm::vec3(1.f), _texture, _text, glm::vec4(0.f));
	}

	inline void addTextDisplay(Entity& _entity, std::string& _text, glm::vec4 _textColor) {
		addTextDisplay(_entity, glm::vec3(0.f), glm::vec3(1.f), whiteTexture, _text, _textColor);
	}

	inline void addTextDisplay(Entity& _entity, std::string& _text, glm::vec3 _position, glm::vec3 _scale) {
		addTextDisplay(_entity, _position, _scale, whiteTexture, _text, glm::vec4(0.f));
	}

	void updateTextDisplay();


	/* ################ Text-Field ################ */
	void addTextField(Entity& _entity, glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _defaultTexture, const graphics::Texture2D& _alternativeTexture, void (*_function)(Entity& _entity, System& _system), std::string& _text, glm::vec4 _textColor);
	
	inline void addTextField(Entity& _entity, void (*_function)(Entity& _entity, System& _system), std::string& _text) {
		addTextField(_entity, glm::vec3(0.f), glm::vec3(1.f), grayTexture, whiteTexture, _function, _text, glm::vec4(0.f));
	}

	void updateTextField();
	

	/* ################ Check-Box ################ */
	void addCheckBox(Entity& _entity, glm::vec3 _position, glm::vec3 _scale, const graphics::Texture2D& _defaultTexture, const graphics::Texture2D& _alternativeTexture, void (*_function)(Entity& _entity, System& _system), std::string& _text, glm::vec4 _textColor, bool _status);

	inline void addCheckBox(Entity& _entity, void (*_function)(Entity& _entity, System& _system), std::string& _text) {
		addCheckBox(_entity, glm::vec3(0.f), glm::vec3(1.f), redTexture, greenTexture, _function, _text, glm::vec4(0.f), false);
	}

	void updateCheckBox();

	void deleteCheckBox(Entity& _entity);


	/* ################ Slider ################ */
	void addSlider(Entity _entity, glm::vec3 _position, glm::vec3 _scale, int _levels, int _selectedLevel, bool _vertical);

	void updateSlider();

	void deleteSlider(Entity& _entity);


	/* ################ Container ################ */
	void addContainer(Entity& _entity, glm::vec3 _position, glm::vec3 _scale, int _rows, int _colums, float _gapFactor, const graphics::Texture2D& _texture, bool _background, bool _movable, std::vector<Entity> _entities);

	inline void addContainer(Entity& _entity, glm::vec3 _position, glm::vec3 _scale, int _rows, int _colums, float _gapFactor, bool _background, bool _movable, std::vector<Entity> _entities) {
		addContainer(_entity, _position, _scale, _rows, _colums, _gapFactor, lightGrayTexture, _background, _movable, _entities);
	}

	void updateContainer(float _deltaTime);


	/* ################ GUI-Update ################ */
	void update(float _deltaTime);


	/* ################ GUI-Utils ################ */
	void findPositionAndScaleForAddText(glm::vec3& _scale, glm::vec3& _position, std::string _text, float& scale, float& positionTextX, float& positionTextY);
	
	bool findPositionAndScaleForTextFieldText(Entity& _entity, float& scaleY, Scale& scale, Position& position, float& positionTextX, float& positionTextY);

	void swapTextureWhenCursorOnIt(Texture& texture, AlternativeTexture& alternativeTexture, math::Rectangle& buttonBox, const glm::vec2& cursorPos);

	void updateKeyInputActions(Entity& _entity, System::Function& _function, TextField& _textField);

	int getLevel(Entity _entity);

private:
	System& system;
	const graphics::Mesh planeMesh;
	
	const graphics::Texture2D& darkBlueTexture = (*graphics::Texture2DManager::get("textures/darkBlue.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)));
	const graphics::Texture2D& lightBlueTexture = (*graphics::Texture2DManager::get("textures/lightBlue.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)));
	const graphics::Texture2D& whiteTexture = (*graphics::Texture2DManager::get("textures/white.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)));
	const graphics::Texture2D& grayTexture = (*graphics::Texture2DManager::get("textures/gray.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)));
	const graphics::Texture2D& lightGrayTexture = (*graphics::Texture2DManager::get("textures/lightGray.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)));
	const graphics::Texture2D& greenTexture = (*graphics::Texture2DManager::get("textures/green.png", graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR)));
	const graphics::Texture2D& redTexture;
	const graphics::Texture2D& blackTexture;
	const graphics::Texture2D& greyTexture;

	bool pressedButton;
	bool firstLeftClick;

	std::string nullString = "";
	std::map<uint32_t, std::string> textFieldsDefaults;
	std::map<uint32_t, std::string> textFieldsTexts;
	char* textFieldTextPointer;

	std::map<Entity, std::vector<Entity>> entitiesInContainer;

	std::string off = "off";
	std::string on = "On";

	bool pressedKey_a;
	bool pressedKey_b;
	bool pressedKey_c;
	bool pressedKey_d;
	bool pressedKey_e;
	bool pressedKey_f;
	bool pressedKey_g;
	bool pressedKey_h; 
	bool pressedKey_i;
	bool pressedKey_j;
	bool pressedKey_k;
	bool pressedKey_l;
	bool pressedKey_m;
	bool pressedKey_n;
	bool pressedKey_o;
	bool pressedKey_p;
	bool pressedKey_q;
	bool pressedKey_r;
	bool pressedKey_s;
	bool pressedKey_t;
	bool pressedKey_u;
	bool pressedKey_v;
	bool pressedKey_w;
	bool pressedKey_x;
	bool pressedKey_y;
	bool pressedKey_z;

	bool pressedKey_0;
	bool pressedKey_1;
	bool pressedKey_2;
	bool pressedKey_3;
	bool pressedKey_4;
	bool pressedKey_5;
	bool pressedKey_6;
	bool pressedKey_7;
	bool pressedKey_8;
	bool pressedKey_9;

	bool pressedKey_backspace;
	bool pressedKey_space;
	bool pressedKey_escape;
	bool pressedKey_enter;
};