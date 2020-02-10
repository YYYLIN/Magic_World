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

//返回文本宽度
int container_test::text_width(const litehtml::tchar_t* text, litehtml::uint_ptr hFont) { return 0; }

//此函数绘制文本字符串
void container_test::draw_text(litehtml::uint_ptr hdc, const litehtml::tchar_t* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos) {}

//将点转换为像素
int container_test::pt_to_px(int pt) {
	return (int)((double)pt * 96 / 72.0);
}

//返回默认字体大小（以像素为单位）
int container_test::get_default_font_size() const { return 16; }

//返回默认的字体名称
const litehtml::tchar_t* container_test::get_default_font_name() const { return _t("Times New Roman"); }

//绘制列表标记。使用参数标记查找标记的类型和位置
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

//您可以在此功能中预加载图像。litehtml不缓存图像，因此如果需要，您必须创建自己的图像缓存
void container_test::load_image(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, bool redraw_on_ready) {}

//用图像的宽度和高度填充sz参数
void container_test::get_image_size(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, litehtml::size& sz) {}

//在此功能中绘制背景。参数bg描述元素背景（请参阅background_paint描述）。注意<img>html元素被绘制为背景。
void container_test::draw_background(litehtml::uint_ptr hdc, const litehtml::background_paint& bg) {
	DrawSimpleGraphics::Instance()->SetColor(Magic::Color4(bg.color.red, bg.color.green, bg.color.blue, bg.color.alpha));
	DrawSimpleGraphics::Instance()->DrawLine(bg.clip_box.x, bg.clip_box.y, bg.clip_box.right(), bg.clip_box.y);
	DrawSimpleGraphics::Instance()->DrawLine(bg.clip_box.x, bg.clip_box.y, bg.clip_box.x, bg.clip_box.bottom());
	DrawSimpleGraphics::Instance()->DrawLine(bg.clip_box.right(), bg.clip_box.y, bg.clip_box.right(), bg.clip_box.bottom());
	DrawSimpleGraphics::Instance()->DrawLine(bg.clip_box.x, bg.clip_box.bottom(), bg.clip_box.right(), bg.clip_box.bottom());
}

void container_test::make_url(const litehtml::tchar_t* url, const litehtml::tchar_t* basepath, litehtml::tstring& out) { out = url; }

//在此处绘制元素边框。如果必须为根元素绘制边框，则参数root为true（通常是<html>）。
void container_test::draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root) {
	OutputDebugStringA("draw_borders\n");
}

//litehtml用<title>html标签文本调用此函数。您可以使用caption参数将窗口标题文本设置为html页面标题。
void container_test::set_caption(const litehtml::tchar_t* caption) {};    //: set_caption

//litehtml为<base>html标记调用此函数以设置基本url。保存此字符串，以备将来在获取baseurl参数的函数中使用。
void container_test::set_base_url(const litehtml::tchar_t* base_url) {};  //: set_base_url

void container_test::link(const std::shared_ptr<litehtml::document>& ptr, const litehtml::element::ptr& el) {}

//litehtml在锚元素单击时调用此函数。您可以在此处打开新网站或执行其他操作。
void container_test::on_anchor_click(const litehtml::tchar_t* url, const litehtml::element::ptr& el) {}  //: on_anchor_click

//定义此函数以处理CSS游标属性。
void container_test::set_cursor(const litehtml::tchar_t* cursor) {}                                      //: set_cursor

/*
*根据tt值转换text参数：
*
*text_transform_capitalize-使第一个字符变为大写。
*text_transform_uppercase-使所有字符变为大写。
*text_transform_lowercase-使所有字符变为小写。
*/
void container_test::transform_text(litehtml::tstring& text, litehtml::text_transform tt) {}

//litehtml调用此函数来加载样式表。您必须下载由url和baseurl参数引用的CSS文件，并将内容复制到text参数。
void container_test::import_css(litehtml::tstring& text, const litehtml::tstring& url, litehtml::tstring& baseurl) {}  //: import_css

//在此处设置绘画剪辑矩形。valid_x和valid_y被忽略。请注意，litehtml可以设置一些片段矩形。您必须保存剪辑位置并在绘制对象上应用剪辑。
void container_test::set_clip(const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius, bool valid_x, bool valid_y) {}

//删除最后一个剪辑。
void container_test::del_clip() {}

//用视口位置和大小填充参数客户端。通常，这是您要绘制html的窗口的客户矩形的大小。
void container_test::get_client_rect(litehtml::position& client) const {
	client.width = 1024;
	client.height = 768;
	char _text[64];
	sprintf_s(_text, 64, "x:%dy:%dw:%dh:%d\n", client.x, client.y, client.width, client.height);
	OutputDebugStringA(_text);
}  //: get_client_rect

//使用此功能，您可以处理自定义标签。只需创建您自己的litehtml :: element，然后从此函数返回即可。参数tag_name是HTML标记（a，p，表等）。
std::shared_ptr<litehtml::element> container_test::create_element(const litehtml::tchar_t* tag_name, const litehtml::string_map& attributes, const std::shared_ptr<litehtml::document>& doc) { return 0; }

//在呈现HTML的媒体中填充媒体参数。litehtml使用媒体功能处理样式表。更改任何媒体功能（例如，更改用户的窗口大小）后，调用document :: media_changed函数。
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