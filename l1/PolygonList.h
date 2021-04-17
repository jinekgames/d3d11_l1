#pragma once
#include <vector>
#include <DirectXMath.h>

template<class T>
class PolygonList
{
public:
	PolygonList() = default;
	PolygonList(std::vector<T> verts_in, std::vector<unsigned short> indices_in)
		:
		vertices(std::move(verts_in)),
		indices(std::move(indices_in))
	{
		assert(vertices.size() > 2);
		assert(indices.size() % 3 == 0);
	}
	void Transform(DirectX::FXMMATRIX matrix)
	{
		for (auto& v : vertices)
		{
			const DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&v.position);
			DirectX::XMStoreFloat3(
				&v.position,
				DirectX::XMVector3Transform(position, matrix)
			);
		}
	}

public:
	std::vector<T> vertices;
	std::vector<unsigned short> indices;
};