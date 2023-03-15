#pragma once

#include <string>

class Brick {
	protected:
		std::string _imgLink = "";
	public:
		Brick() = default;
		Brick(std::string link) :_imgLink(link) {}
		virtual std::string getImgLink() = 0;
};
	
class Brown_Brick3:public Brick{
	public:
		
		Brown_Brick3() :Brick("resources/image/object/block1/brown_brick3.bmp") {}
		std::string getImgLink() { return _imgLink;}
};	

class Brown_Brick5 :public Brick {
	public:
		Brown_Brick5() :Brick("resources/image/object/block1/brown_brick5.bmp") {}
		std::string getImgLink() { return _imgLink; }
};