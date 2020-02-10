#include "container_test.h"
#include "Render/DrawSimpleGraphics.h"

container_test::container_test() {}
container_test::~container_test() {}

litehtml::uint_ptr container_test::create_font(const litehtml::tchar_t* faceName, int size, int weight, litehtml::font_style italic, unsigned int decoration, litehtml::font_metrics* fm) {
	if (fm) {
		fm->ascent = 10;
		fm->descent = 5;
		fm->height = 10 + 5;
		fm->x_height = 3;
	}
	return (litehtml::uint_ptr)0;
}

void container_test::delete_font(litehtml::uint_ptr hFont) {}

//�����ı����
int container_test::text_width(const litehtml::tchar_t* text, litehtml::uint_ptr hFont) { return 0; }

//�˺��������ı��ַ���
void container_test::draw_text(litehtml::uint_ptr hdc, const litehtml::tchar_t* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos) {}

//����ת��Ϊ����
int container_test::pt_to_px(int pt) {
	return (int)((double)pt * 96 / 72.0);
}

//����Ĭ�������С��������Ϊ��λ��
int container_test::get_default_font_size() const { return 16; }

//����Ĭ�ϵ���������
const litehtml::tchar_t* container_test::get_default_font_name() const { return _t("Times New Roman"); }

//�����б��ǡ�ʹ�ò�����ǲ��ұ�ǵ����ͺ�λ��
void container_test::draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker) {
	switch (marker.marker_type)
	{
	case litehtml::list_style_type_circle:
	{
		DrawSimpleGraphics::Instance()->SetColor(Magic::Color4(marker.color.red, marker.color.green, marker.color.blue, marker.color.alpha));
		DrawSimpleGraphics::Instance()->DrawLine(marker.pos.x, marker.pos.y, marker.pos.right(), marker.pos.y);
		DrawSimpleGraphics::Instance()->DrawLine(marker.pos.x, marker.pos.y, marker.pos.x, marker.pos.bottom());
		DrawSimpleGraphics::Instance()->DrawLine(marker.pos.right(), marker.pos.y, marker.pos.right(), marker.pos.bottom());
		DrawSimpleGraphics::Instance()->DrawLine(marker.pos.x, marker.pos.bottom(), marker.pos.right(), marker.pos.bottom());
	}
	break;
	case litehtml::list_style_type_disc:
	{
		DrawSimpleGraphics::Instance()->SetColor(Magic::Color4(marker.color.red, marker.color.green, marker.color.blue, marker.color.alpha));
		DrawSimpleGraphics::Instance()->DrawLine(marker.pos.x, marker.pos.y, marker.pos.right(), marker.pos.y);
		DrawSimpleGraphics::Instance()->DrawLine(marker.pos.x, marker.pos.y, marker.pos.x, marker.pos.bottom());
		DrawSimpleGraphics::Instance()->DrawLine(marker.pos.right(), marker.pos.y, marker.pos.right(), marker.pos.bottom());
		DrawSimpleGraphics::Instance()->DrawLine(marker.pos.x, marker.pos.bottom(), marker.pos.right(), marker.pos.bottom());
	}
	break;
	case litehtml::list_style_type_square:
	{
		DrawSimpleGraphics::Instance()->SetColor(Magic::Color4(marker.color.red, marker.color.green, marker.color.blue, marker.color.alpha));
		DrawSimpleGraphics::Instance()->DrawLine(marker.pos.x, marker.pos.y, marker.pos.right(), marker.pos.y);
		DrawSimpleGraphics::Instance()->DrawLine(marker.pos.x, marker.pos.y, marker.pos.x, marker.pos.bottom());
		DrawSimpleGraphics::Instance()->DrawLine(marker.pos.right(), marker.pos.y, marker.pos.right(), marker.pos.bottom());
		DrawSimpleGraphics::Instance()->DrawLine(marker.pos.x, marker.pos.bottom(), marker.pos.right(), marker.pos.bottom());
	}
	break;
	}
}

//�������ڴ˹�����Ԥ����ͼ��litehtml������ͼ����������Ҫ�������봴���Լ���ͼ�񻺴�
void container_test::load_image(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, bool redraw_on_ready) {}

//��ͼ��Ŀ�Ⱥ͸߶����sz����
void container_test::get_image_size(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, litehtml::size& sz) {}

//�ڴ˹����л��Ʊ���������bg����Ԫ�ر����������background_paint��������ע��<img>htmlԪ�ر�����Ϊ������
void container_test::draw_background(litehtml::uint_ptr hdc, const litehtml::background_paint& bg) {
	DrawSimpleGraphics::Instance()->SetColor(Magic::Color4(bg.color.red, bg.color.green, bg.color.blue, bg.color.alpha));
	DrawSimpleGraphics::Instance()->DrawLine(bg.clip_box.x, bg.clip_box.y, bg.clip_box.right(), bg.clip_box.y);
	DrawSimpleGraphics::Instance()->DrawLine(bg.clip_box.x, bg.clip_box.y, bg.clip_box.x, bg.clip_box.bottom());
	DrawSimpleGraphics::Instance()->DrawLine(bg.clip_box.right(), bg.clip_box.y, bg.clip_box.right(), bg.clip_box.bottom());
	DrawSimpleGraphics::Instance()->DrawLine(bg.clip_box.x, bg.clip_box.bottom(), bg.clip_box.right(), bg.clip_box.bottom());
}

void container_test::make_url(const litehtml::tchar_t* url, const litehtml::tchar_t* basepath, litehtml::tstring& out) { out = url; }

//�ڴ˴�����Ԫ�ر߿��������Ϊ��Ԫ�ػ��Ʊ߿������rootΪtrue��ͨ����<html>����
void container_test::draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root) {
	OutputDebugStringA("draw_borders\n");
}

//litehtml��<title>html��ǩ�ı����ô˺�����������ʹ��caption���������ڱ����ı�����Ϊhtmlҳ����⡣
void container_test::set_caption(const litehtml::tchar_t* caption) {};    //: set_caption

//litehtmlΪ<base>html��ǵ��ô˺��������û���url��������ַ������Ա������ڻ�ȡbaseurl�����ĺ�����ʹ�á�
void container_test::set_base_url(const litehtml::tchar_t* base_url) {};  //: set_base_url

void container_test::link(const std::shared_ptr<litehtml::document>& ptr, const litehtml::element::ptr& el) {}

//litehtml��êԪ�ص���ʱ���ô˺������������ڴ˴�������վ��ִ������������
void container_test::on_anchor_click(const litehtml::tchar_t* url, const litehtml::element::ptr& el) {}  //: on_anchor_click

//����˺����Դ���CSS�α����ԡ�
void container_test::set_cursor(const litehtml::tchar_t* cursor) {}                                      //: set_cursor

/*
*����ttֵת��text������
*
*text_transform_capitalize-ʹ��һ���ַ���Ϊ��д��
*text_transform_uppercase-ʹ�����ַ���Ϊ��д��
*text_transform_lowercase-ʹ�����ַ���ΪСд��
*/
void container_test::transform_text(litehtml::tstring& text, litehtml::text_transform tt) {}

//litehtml���ô˺�����������ʽ��������������url��baseurl�������õ�CSS�ļ����������ݸ��Ƶ�text������
void container_test::import_css(litehtml::tstring& text, const litehtml::tstring& url, litehtml::tstring& baseurl) {}  //: import_css

//�ڴ˴����û滭�������Ρ�valid_x��valid_y�����ԡ���ע�⣬litehtml��������һЩƬ�ξ��Ρ������뱣�����λ�ò��ڻ��ƶ�����Ӧ�ü�����
void container_test::set_clip(const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius, bool valid_x, bool valid_y) {}

//ɾ�����һ��������
void container_test::del_clip() {}

//���ӿ�λ�úʹ�С�������ͻ��ˡ�ͨ����������Ҫ����html�Ĵ��ڵĿͻ����εĴ�С��
void container_test::get_client_rect(litehtml::position& client) const {
	client.width = 1024;
	client.height = 768;
	char _text[64];
	sprintf_s(_text, 64, "x:%dy:%dw:%dh:%d\n", client.x, client.y, client.width, client.height);
	OutputDebugStringA(_text);
}  //: get_client_rect

//ʹ�ô˹��ܣ������Դ����Զ����ǩ��ֻ�贴�����Լ���litehtml :: element��Ȼ��Ӵ˺������ؼ��ɡ�����tag_name��HTML��ǣ�a��p����ȣ���
std::shared_ptr<litehtml::element> container_test::create_element(const litehtml::tchar_t* tag_name, const litehtml::string_map& attributes, const std::shared_ptr<litehtml::document>& doc) { return 0; }

//�ڳ���HTML��ý�������ý�������litehtmlʹ��ý�幦�ܴ�����ʽ�������κ�ý�幦�ܣ����磬�����û��Ĵ��ڴ�С���󣬵���document :: media_changed������
void container_test::get_media_features(litehtml::media_features& media) const {
	litehtml::position client;
	get_client_rect(client);
	media.type = litehtml::media_type_screen;
	media.width = client.width;
	media.height = client.height;
	media.device_width = 100;
	media.device_height = 100;
	media.color = 8;
	media.monochrome = 0;
	media.color_index = 256;
	media.resolution = 96;
}

void container_test::get_language(litehtml::tstring& language, litehtml::tstring& culture) const {
	language = _t("en");
	culture = _t("");
}
//: resolve_color