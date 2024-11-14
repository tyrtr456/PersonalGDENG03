#include "MenuScreen.h"

#include "GraphicsEngine.h"

MenuScreen::MenuScreen() : UIScreen("MenuScreen")
{
	LogUtils::logHResult(
		this,
		DirectX::CreateWICTextureFromFile(
			GraphicsEngine::get()->getRenderSystem()->getDevice(),
			L"Textures/engineLogo.png",
			nullptr,
			creditsTexture.ReleaseAndGetAddressOf()));
}

void MenuScreen::setMaterialEditor(bool* isMaterialEditorOpen)
{
	this->isMaterialEditorOpen = isMaterialEditorOpen;
}

void MenuScreen::draw()
{

	if (!LogUtils::logBool(this, ImGui::BeginMainMenuBar())) return;

	// Game Object header
	if (ImGui::BeginMenu("Game Object")) {
		if (ImGui::MenuItem("Create Cube"))
		{
			onCreateCubeClicked();
		}
		if (ImGui::MenuItem("Create Sphere"))
		{
			onCreateSphereClicked();
		}
		if (ImGui::MenuItem("Create Plane"))
		{
			onCreatePlaneClicked();
		}

		ImGui::Separator();

		if (ImGui::BeginMenu("Light"))
		{
			if (ImGui::MenuItem("Point Light"))
			{
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}

	// About header
	if (ImGui::BeginMenu("About"))
	{
		if (ImGui::MenuItem("Credits", nullptr, isCreditsOpen))
		{
			isCreditsOpen = !isCreditsOpen;
		}
		ImGui::EndMenu();
	}

	// Test header
	if (ImGui::BeginMenu("Windows"))
	{
		if(isMaterialEditorOpen != nullptr)
		if (ImGui::MenuItem("Material Editor", nullptr, *isMaterialEditorOpen))
		{
			*isMaterialEditorOpen = !(*isMaterialEditorOpen);
		}

		if (ImGui::MenuItem("Color Picker", nullptr, isColorPickerOpen))
		{
			isColorPickerOpen = !isColorPickerOpen;
		}
		ImGui::EndMenu();
	}

	// Test header
	/*if (ImGui::BeginMenu("Test"))
	{
		if (ImGui::MenuItem("Color Picker", nullptr, isColorPickerOpen))
		{
			isColorPickerOpen = !isColorPickerOpen;
		}
		ImGui::EndMenu();
	}*/

	ImGui::EndMainMenuBar();

	if (isCreditsOpen)
		showCreditsWindow();

	if (isColorPickerOpen)
		showColorPickerWindow();
}

void MenuScreen::onCreateCubeClicked()
{
}

void MenuScreen::onCreateSphereClicked()
{
}

void MenuScreen::onCreatePlaneClicked()
{
}

void MenuScreen::showCreditsWindow()
{
	if (ImGui::Begin("Credits", &isCreditsOpen))
	{
		constexpr ImVec2 imageSize = { 200, 200 };
		ImGui::Image(static_cast<ImTextureID>(reinterpret_cast<intptr_t>(creditsTexture.Get())), imageSize);
		ImGui::TextColored(ImVec4(0.3f, 0.4f, 1.0f, 1.0f), "URL_Engine v0.2 \n");

		ImGui::NewLine();
		ImGui::Text(
			"About: \n"
			"Developed by Uriel Pascual \n"
			"In partial fulfillment of the requirements for GDENG03"
			"A modified engine that has been collectively modified with the help of Group 3 and the teaching tools provided in the course."
		);

		ImGui::NewLine();
		ImGui::Text(
			"Acknowledgements: \n"
			"Dr. Neil Del Gallego - GDENG03 \n"
			"Ms. Candy Espulgar - GDPROG3 \n"
			"Mr. Raymond Cala - GDMATH/GRAP/PHYSX \n"
			"PardCode Game Engine Tutorial \n"
			"DirectXTK Tutorial - Sampler State / Texture Coordinates Implementation \n"
			"Mr. James Ursua - Cylinder / GameObjectManager (Group-Work) \n"
			"Mr. Joachim Arguelles - Material / Material Editor (Group-Work) \n"
			"Mr. Marcus Leocario - Geometry Shader / Utility Classes (Group-Work) \n"
			
		);

		ImGui::NewLine();

		ImGui::Separator();

		if (ImGui::Button("OK"))
			isCreditsOpen = false;
	}
	ImGui::End();
}

void MenuScreen::showColorPickerWindow()
{
	if (ImGui::Begin("Color Picker", &isColorPickerOpen))
	{
		static ImVec4 color(1.0f, 0.0f, 1.0f, 0.5f);
		ImGui::SameLine();
		ImGui::ColorPicker4("MyColor##4", reinterpret_cast<float*>(&color), 0);
	}
	ImGui::End();
}
