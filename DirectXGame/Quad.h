#pragma once
#include "GraphicsEngine.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "DeviceContext.h"
#include "WICTextureLoader.h"
#include <string>

class DeviceContext;

struct vec2 {
    float x, y;
};

struct vec3
{
    float x, y, z;
};

struct vertex
{
    vec3 position;
    //vec3 position1;
    //vec3 color;
    //vec3 color1;
    vec2 texcoord;
};


/*__declspec(align(16))
struct constant
{
    float m_angle;
};
*/

class Quad
{
public:
    Quad(float x, float y, float z);
    ~Quad();

    void init(ID3D11Device* device, const std::wstring& textureFilePath);
    void render();
    void release();

private:
    float position1[3];
    //float position2[3];
   // float color[3];
    VertexBuffer* m_vb;
    ConstantBuffer* m_cb;
    VertexShader* m_vs;
    PixelShader* m_ps;
    ID3D11ShaderResourceView* m_texture;
    //constant m_constant;
    ID3D11SamplerState* m_samplerState;

};