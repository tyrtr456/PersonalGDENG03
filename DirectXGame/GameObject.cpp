#include "GameObject.h"

#include <iostream>

#include "CameraManager.h"
#include "ConstantBuffer.h"

void GameObject::draw(const VertexShaderPtr& vertexShader, const GeometryShaderPtr& geometryShader, const Material& material,
                      RECT clientWindow)
{
	const DeviceContextPtr deviceContext = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext();
	Constant constants;
	Matrix4x4
		translateMatrix,
		scaleMatrix,
		xMatrix,
		yMatrix,
		zMatrix;

	translateMatrix.setTranslation(localPosition);
	scaleMatrix.setScale(localScale);

	zMatrix.setRotationZ(localRotation.z);
	yMatrix.setRotationY(localRotation.y);
	xMatrix.setRotationX(localRotation.x);

	//LogUtils::log(this, "Pos: " + CameraManager::getInstance()->activeCamera->getPosition().toString());
	constants.cameraPos = CameraManager::getInstance()->activeCamera->getPosition();
	constants.world.setIdentity();

	const Matrix4x4 rotateMatrix = xMatrix * yMatrix * zMatrix;
	constants.world = scaleMatrix * rotateMatrix * translateMatrix;

	constants.view = CameraManager::getInstance()->activeCamera->getView();
	constants.proj = CameraManager::getInstance()->activeCamera->getProjection();

	constants.time = 0;

	constants.color = Vector3D(material.color.x, material.color.y, material.color.z);
	constants.metallic = material.metallic;
	constants.smoothness = material.smoothness;
	constants.flatness = material.flatness;
	constants.tiling = material.tiling;
	constants.offset = material.offset;

	constants.hasAlbedoMap = material.albedoTexture != nullptr;
	constants.hasMetallicMap = material.metallicTexture != nullptr;
	constants.hasSmoothnessMap = material.smoothnessTexture != nullptr;
	constants.hasNormalMap = material.normalTexture != nullptr;

	constantBuffer->update(deviceContext, &constants);

	deviceContext->setConstantBuffer(constantBuffer);

	deviceContext->setVertexBuffer(vertexBuffer);
	deviceContext->setIndexBuffer(indexBuffer);

	deviceContext->setVertexShader(vertexShader);
	deviceContext->setGeometryShader(geometryShader);
	deviceContext->setPixelShader(material.getPixelShader());

	deviceContext->setTexture(material);

	deviceContext->drawIndexedTriangleList(indexBuffer->getSizeIndexList(), 0, 0);
}
