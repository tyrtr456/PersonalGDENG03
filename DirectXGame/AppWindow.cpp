#include "AppWindow.h"


#include "GameObjectManager.h"

AppWindow* AppWindow::sharedInstance = nullptr;

AppWindow* AppWindow::get()
{
	if (!sharedInstance)
		sharedInstance = new AppWindow();

	return sharedInstance;
}

AppWindow::AppWindow()
= default;

AppWindow::~AppWindow()
{
	Window::~Window();
	delete sharedInstance;
}

void AppWindow::onKeyUp(const int key)
{
	
	switch (key)
	{
	
	case VK_SPACE:
	{
		break;
	}
	default:
	{
		break;
	}
	}

}

void AppWindow::onKeyDown(const int key)
{
	if (!CameraManager::getInstance()->activeCamera)
		return;

	switch (key)
	{
	case VK_ESCAPE:
	{
		LogUtils::log(this, "Exiting application");
		this->windowIsRunning = false;
		//exit(0);
		break;
	}
	default:
	{
		break;
	}
	}
}

void AppWindow::onMouseMove(const Vector2D& mousePosition)
{

}

void AppWindow::onLeftMouseDown(const Vector2D& mousePosition)
{
}

void AppWindow::onLeftMouseUp(const Vector2D& mousePosition)
{
}

void AppWindow::onRightMouseDown(const Vector2D& mousePosition)
{
}

void AppWindow::onRightMouseUp(const Vector2D& mousePosition)
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	UIManager::initialize(this->windowHandle);
	CameraManager::initialize();

	const RECT rc = this->getClientWindowRect();
	swapChain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->windowHandle, rc.right - rc.left, rc.bottom - rc.top);

	InputSystem::get()->addListener(this);

	// camera
	CameraManager::getInstance()->activeCamera = new SceneCamera("Camera", false, rc);

	CameraManager::getInstance()->activeCamera->setPosition({ 0.0f, 0.0f, -1.0f });

	void* shaderByteCode = nullptr;
	size_t byteCodeSize = 0;

	//LogUtils::log(this, "The vertex shader is compiled");
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "main", &shaderByteCode, &byteCodeSize);
	vertexShader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shaderByteCode, byteCodeSize);

	// 2. Rotating white cube
	const std::shared_ptr<Cube> cube = std::make_shared<Cube>("cube", shaderByteCode, byteCodeSize, 1.f);
	cube->setPosition({ -1.f, 0.f, 0.f });
	GameObjectManager::get()->addObject(cube);	

	const std::shared_ptr<Cylinder> cylinder = std::make_shared<Cylinder>("cylinder", shaderByteCode, byteCodeSize, 1.f);
	cylinder->setPosition({ 1.f, 0.f, 0.f });
	GameObjectManager::get()->addObject(cylinder);

	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	//LogUtils::log(this, "The geometry shader is compiled");
	GraphicsEngine::get()->getRenderSystem()->compileGeometryShader(L"GeometryShader.hlsl", "main", &shaderByteCode, &byteCodeSize);
	geometryShader = GraphicsEngine::get()->getRenderSystem()->createGeometryShader(shaderByteCode, byteCodeSize);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	//LogUtils::log(this, "Pixel shader has been compiled");
	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "main", &shaderByteCode, &byteCodeSize);
	pixelShader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, byteCodeSize);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	//LogUtils::log(this, "Main material is completed");
	mainMaterial = std::make_shared<Material>(pixelShader);
	mainMaterial->samplerState = GraphicsEngine::get()->getRenderSystem()->createSamplerState();
	UIManager::get()->mainMaterial = mainMaterial;
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	InputSystem::get()->update();

	ticks += EngineTime::getDeltaTime() * 1.0f;

	//LogUtils::log(this, "Setting shaders");
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(vertexShader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setGeometryShader(geometryShader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(pixelShader);

	//LogUtils::log(this, "Clear render target");
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(
		this->swapChain,
		0.f,
		0.f,
		0.f,
		1.f);

	const RECT& rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	GameObjectManager::get()->updateAll(EngineTime::getDeltaTime());
	GameObjectManager::get()->drawAll(vertexShader, geometryShader, *mainMaterial, getClientWindowRect());

	UIManager::get()->draw();

	swapChain->present(true);
}



void AppWindow::onFocus()
{
	Window::onFocus();
	InputSystem::get()->setEnabled(true);
}

void AppWindow::onKillFocus()
{
	Window::onKillFocus();
	InputSystem::get()->setEnabled(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();

	//vertexBuffer->release();
	// swapChain->release();
	// vertexShader->release();
	// pixelShader->release();


	//GraphicsEngine::get()->getRenderSystem()->release();
}
