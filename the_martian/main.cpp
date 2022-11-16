#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <stdio.h>
#include <list>
#include <vector>
#include <memory>
#include <jlib/util/str_util.h>

using namespace std;
using namespace jlib;

struct Line {
	string line;
	bool eof = false;
};



Line readline(FILE* f)
{
	static int i = 0;
	Line line;
	char c;
	while ((c = fgetc(f)) != EOF) {		
		if (c == '\r') {
			continue;
		}
		if (c == '\n') {
			i++;
			while (!line.line.empty() && line.line.front() == ' ') {
				line.line = line.line.substr(1);
			}
			while (!line.line.empty() && line.line.back() == ' ') {
				line.line.pop_back();
			}
			return line;
		}
		if (c == '\t') {
			c = ' ';
		}
		if (c == ' ' && !line.line.empty() && line.line.back() == ' ') {
			continue;
		}
		line.line.push_back(c);
	}
	line.eof = true;
	return line;
}



struct Entry {
	string sol;
	list<string> lines;
};

struct Chapter {
	string title;
	vector<Entry*> entries;
};

bool is_blank(const string& line) {
	for (const auto& c : line) {
		if (c != ' ') {
			return false;
		}
	}
	return true;
}

bool is_chapter(const string& line, bool en) {
	return en ? starts_with(line, "Chapter ") : (starts_with(line, "第") && ends_with(line, "章"));
}

bool is_entry(const string& line, bool en) {
	return en ? starts_with(line, "LOG ENTRY") : starts_with(line, "日志：");
}

vector<Chapter*> read_chapters(FILE* f, bool en)
{
	vector<Chapter*> chapters;
	vector<string> lines;
	Line line;
	string title;
	Chapter* chapter = NULL;
	Entry* entry = NULL;

	while (1) {
		line = readline(f);
		if (line.eof) {
			break;
		}
		if (line.line.empty() || is_blank(line.line)) {
			continue;
		}

		if (is_chapter(line.line, en)) {
			if (chapter) {
				if (entry) {
					chapter->entries.push_back(entry);
					entry = NULL;
				}
				chapters.push_back(chapter);
			}
			chapter = new Chapter();
			chapter->title = line.line;
		} else {
			if (!chapter) {
				abort();
			}

			if (is_entry(line.line, en)) {
				if (entry) {
					chapter->entries.push_back(entry);
				}
				entry = new Entry();
				entry->sol = line.line;
			} else {
				/*if (!entry) {
					abort();
				}*/

				if (!entry) {
					entry = new Entry();
					//entry->sol = line.line;
				}
				entry->lines.push_back(line.line);
			}
		}
	}

	if (chapter) {
		if (entry) {
			chapter->entries.push_back(entry);
		}
		chapters.push_back(chapter);
	}

	return chapters;
}


const char intro[] = R"(
书名：火星救援
作者：［美国］安迪·威尔

编辑推荐：
极客宅男作家安迪威尔的逆袭，美国自出版史上的奇迹
征服科技宅男与家庭主妇，科青、文青同样爱不释手
每个人都会爱、都能懂的科幻小说，就是它了！
四大看点：
1.《异形》《银翼杀手》导演雷德利斯科特与《谍影重重》《心灵捕手》主演马特达蒙联袂打造的好莱坞年度大片《火星救援》原著小说。
2.落难宇航员火星求生，精确数据打造火星生存指南；挥汗自建蔬菜大棚，改造漫游车横穿红色沙漠。太空，探险，科技，孤独，种土豆。
3．这是一本完全由读者口碑造就的年度科幻，从个人网页连载到亚马逊kindle自出版，再到《纽约时报》畅销书榜榜首。
4.火星版《荒岛余生》+《拯救大兵瑞恩》。《荒岛余生》主演汤姆汉克斯曾对小说和电影发表如下意见：“安迪威尔的大作！真嫉妒马特达蒙能演这片！电影**人选是我！汉克斯！
2009年，安迪·威尔陆续将他的小说《火星救援》贴在自己的个人网站上，供人免费阅读。在众多读者的强烈要求下，他在亚马逊平台上发布了作品，收费0.99美金，哪知花钱买他小说的读者比免费阅读的读者更多。2013年3月，兰登书屋以六位数买下小说的版权。仅仅四天后，安迪·威尔又接到了来自20世纪福克斯电影公司的橄榄枝。2015年，由大导演雷德利·斯科特执导、马特·达蒙主演的电影《火星救援》将于10月2日上映，更是激发了这本小说的购买热潮，直接将它推向了《纽约时报》畅销书榜的榜首位置。
《火星救援》讲述了一名落难的宇航员孤身在火星上求生的故事，用精确的物理、化学和植物学数据为我们打造了一个“学好数理化，走遍宇宙都不怕”的火星生存指南。每一个氧原子，每一个卡路里，都关乎到主角马克·沃特尼的生死存亡。不过，沃特尼也没我们想象的那么悲观绝望。孤身行走在火星上的时候，他没有去想接下来该怎么办，而是等不及有孙子的那一天了：“你爷爷年轻的时候，必须在撞击坑环上走路。上坡！穿着EVA太空服！还是在火星上！你这个小兔崽子，听懂了吗？火星！”他乐观幽默地面对挑战，逐一解决难题，在“可怕至极的迪斯科音乐”的陪伴下，挥汗火星蔬菜大棚，改造漫游车横穿红色荒漠，只为回到地球那个甜蜜的家！

内容简介：

六天前，马克·沃特尼成为了第一批行走在火星上的人。如今，他也将成为第一个葬身火星的人。
一场突如其来的风暴让阿瑞斯三船员被迫放弃任务。撤离过程中，沃特尼遭遇意外，被孤身一人丢在了这片寸草不生的红色荒漠中，剩余的补给也远不够撑到救援可能抵达的那一天。
当然，沃特尼也不准备坐以待毙，凭借着他的植物学家和机械工程师背景，他决定跟火星来一场不是你死就是我活的过家家游戏。

作者简介：

安迪·威尔(AndyWeir)，从15岁起就被美国国家实验室聘为软件工程师。执着的太空宅男，沉迷于相对论物理、轨道力学和载人飞船。《火星救援》是他的处女作。
2009年，安迪·威尔陆续将他的小说《火星救援》贴在自己的个人网站上，供人免费阅读。在众多读者的强烈要求下，他在亚马逊平台上发布了作品，收费0.99美金，哪知花钱买他小说的读者比免费阅读的读者更多。2013年3月，兰登书屋以六位数买下小说的版权。仅仅四天后，安迪·威尔又接到了来自20世纪福克斯电影公司的橄榄枝。2015年，由大导演雷德利·斯科特执导、马特·达蒙主演的电影《火星救援》将于年底上映，更是激发了这本小说的购买热潮，直接将它推向了《纽约时报》畅销书榜的榜首位置。
================

)";

const  char footer[] = R"(

Copyright Info
 
"The Martian" is copyright Andy Weir, 2011. All rights reserved.
The cover art is an image from NASA and is public domain. All fonts in this document are public domain.
 
Redistribution of this e-book is permitted, so long as it is distributed for free.
)";

void test(FILE* f, bool en)
{
	auto chapters = read_chapters(f, en);
	for (const auto& chapter : chapters) {
		printf("%s\n", chapter->title.c_str());
		for (const auto& entry : chapter->entries) {
			printf("\t%s\n", entry->sol.c_str());
		}
	}

	rewind(f);
}

int main()
{
	auto en = R"(F:\电子书\The_Martian.txt)";
	//auto en = R"(F:\电子书\The_Martian_Andy_Weir.txt)";
	auto zh = R"(F:\电子书\火星救援.txt)";

	auto fen = fopen(en, "rb");
	auto fzh = fopen(zh, "rb");

	fseek(fen, 0, SEEK_END);
	auto len = ftell(fen);
	fseek(fen, 0, SEEK_SET);

	fseek(fzh, 0, SEEK_END);
	auto lzh = ftell(fzh);
	fseek(fzh, 0, SEEK_SET);

	//test(fen, 1);
	//test(fzh, 0);

	auto echapters = read_chapters(fen, 1);
	auto cchapters = read_chapters(fzh, 0);

	auto ez = "F:/电子书/The_Martian_火星救援.txt";
	auto fez = fopen(ez, "wb");

	fwrite(intro, 1, sizeof(intro) - 1, fez);
	fwrite("\n", 1, 1, fez);
	for (int i = 1; i <= 26; i++) {
		auto cc = cchapters[i - 1];
		auto ec = echapters[i - 1];
		fwrite("\n", 1, 1, fez);
		fwrite(cc->title.c_str(), 1, cc->title.size(), fez);
		fwrite("\n", 1, 1, fez);

		if (cc->entries.size() != ec->entries.size()) {
			abort();
		}

		for (size_t i = 0; i < cc->entries.size(); i++) {
			auto centry = cc->entries[i];
			auto eentry = ec->entries[i];

			if (!centry->sol.empty() && !eentry->sol.empty()) {
				fwrite("\n", 1, 1, fez);
				fwrite("\n", 1, 1, fez);
				fwrite(eentry->sol.c_str(), 1, eentry->sol.size(), fez);
				fwrite("\n", 1, 1, fez);
				fwrite(centry->sol.c_str(), 1, centry->sol.size(), fez);
				fwrite("\n", 1, 1, fez);
			}

			while (!centry->lines.empty() || !eentry->lines.empty()) {
				if (!eentry->lines.empty()) {
					auto line = eentry->lines.front();
					eentry->lines.pop_front();
					fwrite("\n", 1, 1, fez);
					fwrite(line.c_str(), 1, line.size(), fez);
				}
				if (!centry->lines.empty()) {
					auto line = centry->lines.front();
					centry->lines.pop_front();
					fwrite("\n", 1, 1, fez);
					fwrite(line.c_str(), 1, line.size(), fez);
					fwrite("\n", 1, 1, fez);
				}

			}
		}

		

	}


	fwrite("\n", 1, 1, fez);
	fwrite(footer, 1, sizeof(footer) - 1, fez);
	fwrite("\n", 1, 1, fez);
}