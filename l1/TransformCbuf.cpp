
#include "TransformCbuf.h"



TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	:
	parent(parent) {

	if (!pvcbuf) {
		pvcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}

}

void TransformCbuf::Bind(Graphics& gfx) noexcept {

	pvcbuf->Update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * gfx.GetProjection()
		)
	);
	pvcbuf->Bind(gfx);

}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCbuf::pvcbuf;
