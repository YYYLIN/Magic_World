#include "DrawSimpleGraphics.h"
#include "Render_thread.h"

DrawSimpleGraphics::DrawSimpleGraphics() {

}

DrawSimpleGraphics::~DrawSimpleGraphics() {

}

bool DrawSimpleGraphics::Initialize() {

	Magic::Management::SendMessageTo(Magic::Render_thread::Instance()->GetTHREAD_OBJECT(), 0, 0,
		[](Magic::Management::MESSAGE_TYPE _MessageType, Magic::Management::MESSAGE _Message) {
		//初始话渲染对象
	});


	return true;
}