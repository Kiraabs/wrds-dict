#pragma once
#include <stdexcept>
#include <list>
#include <string>
#include <iostream>	
#include <fstream>
#include <vector>

class WDictionary {
	bool _unsorted{};
	std::string _fileN = "wrds.txt";
	std::string _fileP;
	std::wstring* _wbuf;
	std::list<std::wstring>* _wrds;
	void _sort_if() noexcept;
	void _print_buff_err_erase(const wchar_t* lmsg, const wchar_t* rmsg) const noexcept;
	bool _try_read_from_file() const noexcept;
	bool _try_write_in_file() const noexcept;
	bool _buff_pop_back() const noexcept;
	bool _contains(const std::wstring& w) const noexcept;
	bool _put(const std::wstring& w) const noexcept;
	bool _remove(const std::wstring& w) const noexcept;

public:
	char Separator = ' ';
	bool AllowDuplicates{};
	WDictionary();
	explicit WDictionary(const char*);
	~WDictionary();
	void erase() const;
	bool print_if() noexcept;
	bool is_empty() const noexcept;
	int remove(const WDictionary&) const noexcept;
	int replace(const WDictionary&, const WDictionary&) noexcept;
	friend std::wostream& operator<<(std::wostream&, WDictionary&) noexcept;
	friend std::wistream& operator>>(std::wistream&, WDictionary&) noexcept;
};
