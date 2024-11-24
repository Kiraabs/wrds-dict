#include "WDictionary.h"
#include <algorithm>

void WDictionary::_sort_if() noexcept {
	if (_unsorted) {
		_wrds->sort();
		_unsorted = false;
	}
}

void WDictionary::_print_buff_err_erase(const wchar_t* lmsg, const wchar_t* rmsg) const noexcept {
	std::wcerr << lmsg << *_wbuf << rmsg;
	_wbuf->clear();
}

bool WDictionary::_try_read_from_file() const noexcept {
	std::wstring ln;
	std::wifstream in(_fileP);
	if (!(in && in.is_open()))
		return false;
	while (std::getline(in, ln))
		_put(ln);
	in.close();
	return true;
}

bool WDictionary::_try_write_in_file() const noexcept {
	std::wofstream out;
	out.open(_fileP);
	if (!(out && out.is_open()))
		return false;
	for (const auto& w : *_wrds)
		out << w << '\n';
	out.close();
	return true;
}

bool WDictionary::_contains(const std::wstring& w) const noexcept {
	return std::count(_wrds->begin(), _wrds->end(), w) != 0;
}

bool WDictionary::_put(const std::wstring& w) const noexcept
{
	if (_contains(w) || AllowDuplicates)
		return false;
	_wrds->push_back(w);
	return true;
}

bool WDictionary::_remove(const std::wstring& const w) const noexcept {
	if (!_contains(w))
		return false;
	_wrds->remove(w);
	return true;
}

bool WDictionary::_buff_pop_back() const noexcept {
	if (_wbuf->empty())
		return false;
	_wbuf->pop_back();
	_wbuf->pop_back();
	return true;
}

WDictionary::WDictionary() {
	_wrds = new std::list<std::wstring>{};
	_wbuf = new std::wstring{};
}

WDictionary::WDictionary(const char* cmdArgs) {
	if (!cmdArgs)
		throw std::invalid_argument{ "WDictionary::WDictionary(const char* cmdArgs" };
	_wrds = new std::list<std::wstring>{};
	_wbuf = new std::wstring{};
	_fileP = cmdArgs;
	for (auto i = _fileP.end() - 1; *i != '\\'; i--)
		_fileP.pop_back();
	_fileP += _fileN;
	_try_read_from_file();
}

WDictionary::~WDictionary() {
	_try_write_in_file();
	delete _wrds; delete _wbuf;
}

void WDictionary::erase() const {
	if (std::remove(_fileP.data()) == _fileP.npos)
		throw std::exception{ "void WDictionary::Erase()" };
	_wrds->clear();
}

bool WDictionary::print_if() noexcept {
	if (_wrds->empty())
		return false;
	std::wcout << *this << '\n';
	return true;
}

bool WDictionary::is_empty() const noexcept { return _wrds->empty(); }

int WDictionary::remove(const WDictionary& other) const noexcept {
	int ri = 0;
	for (auto i = other._wrds->begin(); i != other._wrds->end(); i++)
		if (!_remove(*i))
			*_wbuf += *i + L", ";
		else
			ri++;
	if (_buff_pop_back())
		_print_buff_err_erase(L"\n>> Next word or words doesn't exists in dictionary: [", L"]!\n");
	return ri;
}

int WDictionary::replace(const WDictionary& od, const WDictionary& nw) noexcept {
	int ri = 0;
	auto oi = od._wrds->begin(), ni = nw._wrds->begin();
	for (; oi != od._wrds->end() && ni != nw._wrds->end(); oi++, ni++) {
		if (!_remove(*oi))
			*od._wbuf += *oi + L", ";
		else if (!_put(*ni)) {
			*nw._wbuf += *ni + L", ";
			_put(*oi);
		}
		else
			ri++;
	}
	if (od._buff_pop_back())
		od._print_buff_err_erase(L"\n>> Next word or words doesn't exists in dictionary: [", L"]!\n");
	if (nw._buff_pop_back())
		nw._print_buff_err_erase(L"\n>> Next word or words already exists in dictionary [", L"]!\n");
	if (ri > 0)
		_unsorted = true;
	return ri;
}

std::wostream& operator<<(std::wostream& os, WDictionary& d) noexcept {
	d._sort_if();
	int wi = 1;
	for (auto fu = d._wrds->begin(); fu != d._wrds->end(); fu++, wi++)
		if (*fu != d._wrds->back())
			std::wcout << wi << ". " << *fu << '\n';
		else
			std::wcout << wi << ". " << *fu;
	if (!d._wbuf->empty())
		d._print_buff_err_erase(L"\n\n>> Next word or words already exists in dictionary [", L"]!");
	return os;
}

std::wistream& operator>>(std::wistream& is, WDictionary& d) noexcept {
	wchar_t let;
	std::wstring word{};
	if (!is) {
		is.setstate(std::ios_base::failbit);
		return is;
	}
	while (is.get(let)) {
		if (let != d.Separator && let != '\n')
			word += let;
		else {
			word[0] = std::toupper(word[0]);
			if (!word.empty() && !d._put(word))
				*d._wbuf += word + L", ";
			word.clear();
			if (let == '\n')
				break;
		}
	}
	d._unsorted = true;
	d._buff_pop_back();
	return is;
}