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
	return en ? starts_with(line, "Chapter ") : (starts_with(line, "��") && ends_with(line, "��"));
}

bool is_entry(const string& line, bool en) {
	return en ? starts_with(line, "LOG ENTRY") : starts_with(line, "��־��");
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
���������Ǿ�Ԯ
���ߣ��������ݰ��ϡ�����

�༭�Ƽ���
����լ�����Ұ�����������Ϯ�������Գ���ʷ�ϵ��漣
�����Ƽ�լ�����ͥ���������ࡢ����ͬ����������
ÿ���˶��ᰮ�����ܶ��Ŀƻ�С˵���������ˣ�
�Ĵ󿴵㣺
1.�����Ρ�������ɱ�֡������׵���˹�����롶��Ӱ���ء������鲶�֡��������ش������Ǵ���ĺ�������ȴ�Ƭ�����Ǿ�Ԯ��ԭ��С˵��
2.�����Ա������������ȷ���ݴ����������ָ�ϣ��Ӻ��Խ��߲˴���������γ��ᴩ��ɫɳĮ��̫�գ�̽�գ��Ƽ����¶�����������
3������һ����ȫ�ɶ��߿ڱ���͵���ȿƻã��Ӹ�����ҳ���ص�����ѷkindle�Գ��棬�ٵ���ŦԼʱ�������������ס�
4.���ǰ桶�ĵ�������+�����ȴ������������ĵ�������������ķ����˹����С˵�͵�Ӱ������������������������Ĵ������漵�����ش���������Ƭ����Ӱ**��ѡ���ң�����˹��
2009�꣬���ϡ�����½��������С˵�����Ǿ�Ԯ�������Լ��ĸ�����վ�ϣ���������Ķ������ڶ���ߵ�ǿ��Ҫ���£���������ѷƽ̨�Ϸ�������Ʒ���շ�0.99������֪��Ǯ����С˵�Ķ��߱�����Ķ��Ķ��߸��ࡣ2013��3�£�������������λ������С˵�İ�Ȩ����������󣬰��ϡ������ֽӵ�������20���͸���˹��Ӱ��˾�����֦��2015�꣬�ɴ����׵�����˹����ִ�������ء��������ݵĵ�Ӱ�����Ǿ�Ԯ������10��2����ӳ�����Ǽ������ⱾС˵�Ĺ����ȳ���ֱ�ӽ��������ˡ�ŦԼʱ�����������İ���λ�á�
�����Ǿ�Ԯ��������һ�����ѵ��Ա�����ڻ����������Ĺ��£��þ�ȷ��������ѧ��ֲ��ѧ����Ϊ���Ǵ�����һ����ѧ���������߱����涼���¡��Ļ�������ָ�ϡ�ÿһ����ԭ�ӣ�ÿһ����·����غ���������ˡ������������������������������Ҳû�����������ô���۾��������������ڻ����ϵ�ʱ����û��ȥ�����������ô�죬���ǵȲ��������ӵ���һ���ˣ�����үү�����ʱ�򣬱�����ײ���ӻ�����·�����£�����EVA̫�շ��������ڻ����ϣ������С�����ӣ��������𣿻��ǣ������ֹ���Ĭ�������ս����һ������⣬�ڡ����������ĵ�˹�����֡�������£��Ӻ������߲˴���������γ��ᴩ��ɫ��Į��ֻΪ�ص������Ǹ����۵ļң�

���ݼ�飺

����ǰ����ˡ��������Ϊ�˵�һ�������ڻ����ϵ��ˡ������Ҳ����Ϊ��һ��������ǵ��ˡ�
һ��ͻ�������ķ籩�ð���˹����Ա���ȷ������񡣳�������У��������������⣬������һ�˶�������Ƭ��ݲ����ĺ�ɫ��Į�У�ʣ��Ĳ���ҲԶ�����ŵ���Ԯ���ִܵ����һ�졣
��Ȼ��������Ҳ��׼�����Դ��У�ƾ��������ֲ��ѧ�Һͻ�е����ʦ��������������������һ���������������һ�Ĺ��Ҽ���Ϸ��

���߼�飺

���ϡ�����(AndyWeir)����15����ͱ���������ʵ����ƸΪ�������ʦ��ִ�ŵ�̫��լ�У���������������������ѧ�����˷ɴ��������Ǿ�Ԯ�������Ĵ�Ů����
2009�꣬���ϡ�����½��������С˵�����Ǿ�Ԯ�������Լ��ĸ�����վ�ϣ���������Ķ������ڶ���ߵ�ǿ��Ҫ���£���������ѷƽ̨�Ϸ�������Ʒ���շ�0.99������֪��Ǯ����С˵�Ķ��߱�����Ķ��Ķ��߸��ࡣ2013��3�£�������������λ������С˵�İ�Ȩ����������󣬰��ϡ������ֽӵ�������20���͸���˹��Ӱ��˾�����֦��2015�꣬�ɴ����׵�����˹����ִ�������ء��������ݵĵ�Ӱ�����Ǿ�Ԯ�����������ӳ�����Ǽ������ⱾС˵�Ĺ����ȳ���ֱ�ӽ��������ˡ�ŦԼʱ�����������İ���λ�á�
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
	auto en = R"(F:\������\The_Martian.txt)";
	//auto en = R"(F:\������\The_Martian_Andy_Weir.txt)";
	auto zh = R"(F:\������\���Ǿ�Ԯ.txt)";

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

	auto ez = "F:/������/The_Martian_���Ǿ�Ԯ.txt";
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