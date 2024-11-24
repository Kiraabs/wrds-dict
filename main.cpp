#include "WDictionary.h"

#pragma region Funcs declarations
inline int menu(WDictionary& d) noexcept;
inline void menu_options() noexcept;
inline void words_input(WDictionary& d) noexcept;
inline void word_remove(WDictionary& d) noexcept;
inline void word_replace(WDictionary& d) noexcept;
inline void clear_dict(const WDictionary& d) noexcept;
inline void set_isep(WDictionary& d) noexcept;
inline bool confirm_select(const char* msg = ">> Continue (y|n): ") noexcept;
#pragma endregion

int main(int argc, char* args[]) {
	WDictionary d{ *args };
	return menu(d);
}

inline int menu(WDictionary& d) noexcept {
	char num;
	bool inpt = true;
	while (inpt) {
		menu_options();
		std::cin >> num;
		std::cout << '\n';
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		switch (num) {
		case '1':
			words_input(d);
			break;
		case '2':
			word_remove(d);
			break;
		case '3':
			word_replace(d);
			break;
		case '4':
			set_isep(d);
			break;
		case '5':
			clear_dict(d);
			break;
		case '6':
			inpt = false;
			break;
		default:
			std::cout << ">> Unrecognized input: try again or enter \'6\' to exit.\n\n";
			break;
		}
	}
	return 0;
}

inline void menu_options() noexcept {
	std::cout << "1. Words input\n2. Words removing\n3. Words replacement\n" <<
		"4. Set input separator char\n5. Clear dictionary\n6. Exit\n\n>> Number Input: ";
}

inline void words_input(WDictionary& d) noexcept {
	if (d.print_if())
		std::cout << '\n';
	while (true) {
		std::cout << ">> Words Input: ";
		std::wcin >> d;
		std::wcout << '\n' << d << "\n\n";
		if (!confirm_select())
			break;
	}
}

inline void word_replace(WDictionary& d) noexcept {
	if (d.is_empty()) {
		std::cout << ">> Dictionary is empty!\n\n";
		return;
	}
	WDictionary old{}, _new{};
	old.AllowDuplicates = true;
	_new.AllowDuplicates = true;
	old.Separator = d.Separator;
	_new.Separator = d.Separator;
	while (true) {
		d.print_if();
		std::cout << "\n>> Enter replacement word or words: ";
		std::wcin >> old;
		std::cout << "\n>> Enter new word or words: ";
		std::wcin >> _new;
		std::cout << "\n>> Count of replaced words: [" << d.replace(old, _new) << "]\n\n";
		if (!confirm_select())
			break;
	}
}

inline void word_remove(WDictionary& d) noexcept {
	if (d.is_empty()) {
		std::cout << ">> Dictionary is empty!\n\n";
		return;
	}
	WDictionary w{};
	w.Separator = d.Separator;
	while (!d.is_empty()) {
		d.print_if();
		std::cout << "\n>> Enter words to remove: ";
		std::wcin >> w;
		std::cout << "\n>> Count of removed words: [" << d.remove(w) << "]\n\n";

		if (d.is_empty()) {
			std::cout << '\n';
			break;
		}
		if (!confirm_select())
			break;
		w.erase();
	}
}

inline void set_isep(WDictionary& d) noexcept {
	std::cout << ">> Enter input separator char: ";
	std::cin >> d.Separator;
	std::cout << "\n>> Input separator char successfully set to [" << d.Separator << "]\n\n";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

inline void clear_dict(const WDictionary& d) noexcept {
	if (d.is_empty()) {
		std::cout << ">> Dictionary is already empty!\n\n";
		return;
	}
	if (confirm_select(">> Are you sure about to clear the dictionary (y|n): ")) {
		try {
			d.erase();
			std::cout << ">> Dictionary successfully cleared!\n\n";
		}
		catch (const std::exception&) {
			std::cout << ">> Failed to clear dictionary: file doesn't exists!\n\n";
		}
	}
}

inline bool confirm_select(const char* msg) noexcept {
	std::cout << msg;
	char ans = 'y';
	std::cin >> ans;
	std::cout << '\n';
	if (ans != 'y' && ans != 'Y')
		return false;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return true;
}
